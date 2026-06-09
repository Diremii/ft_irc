/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 10:44:23 by humontas          #+#    #+#             */
/*   Updated: 2026/05/23 10:44:23 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static int	isValidName(const std::string &name, size_t maxLen, const std::string &forbidden)
{
	if (name.empty())
		return (1);
	else if (name.size() > maxLen)
		return (2);

	for (size_t i = 0; i < name.size(); i++)
	{
		if (forbidden.find(name[i]) != std::string::npos)
			return (2);
	}
	return (0);
}

static std::vector<std::string>	splitArgs(const std::string &args)
{
    std::vector<std::string> result;
    std::string word;

    for (size_t i = 0; i < args.size(); i++)
    {
        if (args[i] == ':')
        {
            result.push_back(args.substr(i + 1));
            return result;
        }
        if (args[i] == ' ')
        {
            if (!word.empty())
            {
                result.push_back(word);
                word.clear();
            }
        }
        else
            word += args[i];
    }
    if (!word.empty())
        result.push_back(word);
    return result;
}

void Server::passCommand(int clientFd, const std::string &password)
{
    if (getUser(clientFd).getAuthenticated())
        return sendMessage(clientFd, IrcReply::alreadyRegistered());

    if (password.empty() || password != _serverPassword)
        return sendMessage(clientFd, IrcReply::notAuthenticated());

    getUser(clientFd).setAuthenticated(true);
}

void Server::nickCommand(int clientFd, const std::string &nickName)
{
	
	switch (isValidName(nickName, 63, " ,*?!@"))
	{
		case 1:
			return sendMessage(clientFd, IrcReply::noNick());
		case 2:
			return sendMessage(clientFd, IrcReply::badNick(nickName));
	}

	for (size_t i = 0; i < _users.size(); i++)
	{
		if (nickName == _users[i].getNickname())
			return sendMessage(clientFd, IrcReply::nickInUse(nickName));
	}

	getUser(clientFd).setNickname(nickName);
}

void Server::userCommand(int clientFd, const std::string &userName)
{
	std::string username = userName.substr(0, userName.find(' '));

	if (getUser(clientFd).getRegistered())
		return sendMessage(clientFd, IrcReply::alreadyRegistered());

	switch (isValidName(username, 10, " @\r\n"))
	{
		case 1:
			return sendMessage(clientFd, IrcReply::notEnoughParams("USER"));
		case 2:
			return sendMessage(clientFd, IrcReply::badUser(username));
	}

	getUser(clientFd).setUsername(username);
}

void	Server::joinChannel(int clientFd, const std::string &channelName)
{
	Channel *channel = getChannel(channelName);
	std::string nick = getUser(clientFd).getNickname();

	if (channelName.empty() || channelName[0] != '#')
    	return sendMessage(clientFd, IrcReply::badChannelMask(getUser(clientFd).getNickname(), channelName));

	else if (!channel)
	{
		_channels.push_back(Channel(channelName, getUser(clientFd)));
		channel = getChannel(channelName);
	}

	else
	{
		switch (channel->addUser(clientFd))
		{
			case 1:
				return (sendMessage(clientFd, IrcReply::inviteOnlyChannel(nick, channelName)));
			case 2:
				return (sendMessage(clientFd, IrcReply::channelIsFull(nick, channelName)));
		}
	}

	broadcast(channel, IrcReply::join(getUser(clientFd).getNickname(),getUser(clientFd).getUsername(),channelName));
	sendMessage(clientFd, IrcReply::noTopic(nick, channelName));
	sendNamesList(clientFd, channel);
}

void Server::quitCommand(int clientFd, const std::string &reason)
{
	std::string msg = ":" + getUser(clientFd).getNickname()
					+ " QUIT :" + (reason.empty() ? "Client Quit" : reason)
					+ "\r\n";

	broadcastUserChannels(clientFd, msg);
	removeClient(clientFd);
}

void Server::kickCommand(int clientFd, const std::string &args)
{
    std::vector<std::string> params = splitArgs(args);
    if (params.size() < 2)
        return sendMessage(clientFd, IrcReply::notEnoughParams("KICK"));

    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = params.size() > 2 ? params[2] : "Kicked";

	Channel *channel = getChannel(channelName);
	if (!channel)
	    return ;

	User *target = getUserByNick(targetNick);
	if (!target)
	    return ;

	if (!channel->isOperator(clientFd))
		return (sendMessage(clientFd, IrcReply::chanOpPrivsNeeded(getUser(clientFd).getNickname(), channelName)));
	broadcast(channel, IrcReply::kick(getUser(clientFd).getNickname(), getUser(clientFd).getUsername(), channelName, targetNick, reason));
	channel->kickClient(clientFd, target->getFd());
}

void Server::inviteCommand(int clientFd, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
	if (params.size() < 2)
		return sendMessage(clientFd, IrcReply::notEnoughParams("INVITE"));

	std::string	targetNick = params[0];
	std::string channelName = params[1];

	Channel *channel = getChannel(channelName);
	if (!channel)
	    return ;

	User *target = getUserByNick(targetNick);
	if (!target)
	    return ;

	if (!channel->isOperator(clientFd))
		return (sendMessage(clientFd, IrcReply::chanOpPrivsNeeded(getUser(clientFd).getNickname(), channelName)));
	channel->inviteClient(clientFd, target->getFd());
	sendMessage(target->getFd(), IrcReply::invite(getUser(clientFd).getNickname(), getUser(clientFd).getUsername(), targetNick, channelName));
}

void	Server::topicCommand(int clientFd, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
    if (params.size() < 1)
        return sendMessage(clientFd, IrcReply::notEnoughParams("TOPIC"));

    std::string channelName = params[0];

	Channel *channel = getChannel(channelName);
	if (!channel)
	    return ;

	if (params.size() == 1)
        return sendMessage(clientFd, IrcReply::topic(getUser(clientFd).getNickname(), channelName, channel->viewTopic()));

	std::string newTopic = params[1];
	if (!channel->changeTopic(clientFd, newTopic))
		return (sendMessage(clientFd, IrcReply::chanOpPrivsNeeded(getUser(clientFd).getNickname(), channelName)));

	broadcast(channel, IrcReply::topicChanged(getUser(clientFd).getNickname(), getUser(clientFd).getUsername(), channelName, newTopic));
}

void	Server::modeCommand(int clientFd)
{
	(void)clientFd;
}
