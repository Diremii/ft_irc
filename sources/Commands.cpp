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
	sendMessage(clientFd, IrcReply::namesList(nick, channelName, nick));
	sendMessage(clientFd, IrcReply::endOfNames(nick, channelName));
}

void Server::quitCommand(int clientFd, const std::string &reason)
{
	std::string msg = ":" + getUser(clientFd).getNickname()
					+ " QUIT :" + (reason.empty() ? "Client Quit" : reason)
					+ "\r\n";

	broadcastUserChannels(clientFd, msg);
	removeClient(clientFd);
}

void Server::kickCommand(int clientFd, int targetFd, const std::string &channelName, std::string &reason)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
        return ;

    if (!channel->kickClient(clientFd, targetFd))
        return sendMessage(clientFd, IrcReply::chanOpPrivsNeeded(getUser(clientFd).getNickname(), channelName));

    if (reason.empty())
        reason = "Kicked";

    broadcast(channel, IrcReply::kick(getUser(clientFd).getNickname(), getUser(clientFd).getUsername(), channelName, getUser(targetFd).getNickname(),reason));
}

void	Server::inviteCommand(int clientFd)
{
	(void)clientFd;
}

void	Server::topicCommand(int clientFd)
{
	(void)clientFd;
}

void	Server::modeCommand(int clientFd)
{
	(void)clientFd;
}
