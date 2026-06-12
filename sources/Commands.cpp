/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 12:10:12 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/10 12:10:12 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::passCommand(int clientFd, const std::string &password)
{
	User	&caller = getUser(clientFd);

	if (caller.getAuthenticated())
		return (sendMessage(clientFd, IrcReply::alreadyRegistered()));
	if (password.empty() || password != _serverPassword)
		return (sendMessage(clientFd, IrcReply::notAuthenticated()));

	caller.setAuthenticated(true);
}

void	Server::nickCommand(int clientFd, const std::string &nickName)
{
	switch (isValidName(nickName, 63, " ,*?!@"))
	{
		case 1:
			return (sendMessage(clientFd, IrcReply::noNick()));
		case 2:
			return (sendMessage(clientFd, IrcReply::badNick(nickName)));
	}

	for (size_t i = 0; i < _users.size(); i++)
	{
		if (nickName == _users[i].getNickname())
			return (sendMessage(clientFd, IrcReply::nickInUse(nickName)));
	}

	getUser(clientFd).setNickname(nickName);
}

void	Server::userCommand(int clientFd, const std::string &userName)
{
	User 		&caller = getUser(clientFd);
	std::string	username = userName.substr(0, userName.find(' '));

	if (caller.getRegistered())
		return (sendMessage(clientFd, IrcReply::alreadyRegistered()));

	switch (isValidName(username, 10, " @\r\n"))
	{
		case 1:
			return (sendMessage(clientFd, IrcReply::notEnoughParams("USER")));
		case 2:
			return (sendMessage(clientFd, IrcReply::badUser(username)));
	}

	caller.setUsername(username);
}

void	Server::joinChannel(int clientFd, const std::string &channelName)
{
	User	&caller = getUser(clientFd);

	if (channelName.empty() || channelName[0] != '#')
		return (sendMessage(clientFd, IrcReply::badChannelMask(caller.getNickname(), channelName)));

	Channel	*channel = getChannel(channelName);
	if (!channel)
	{
		_channels.push_back(Channel(channelName, getUser(clientFd)));
		channel = getChannel(channelName);
	}
	else
	{
		switch (channel->addUser(clientFd))
		{
			case 1:
				return (sendMessage(clientFd, IrcReply::inviteOnlyChannel(caller.getNickname(), channelName)));
			case 2:
				return (sendMessage(clientFd, IrcReply::channelIsFull(caller.getNickname(), channelName)));
		}
	}

	broadcast(channel, IrcReply::join(caller.getNickname(), caller.getUsername(), channelName));
	if (channel->viewTopic().empty())
		sendMessage(clientFd, IrcReply::noTopic(caller.getNickname(), channelName));
	else
		sendMessage(clientFd, IrcReply::topic(caller.getNickname(), channelName, channel->viewTopic()));
	sendNamesList(clientFd, channel);
}

void	Server::quitCommand(int clientFd, const std::string &reason)
{
	User	&caller = getUser(clientFd);

	broadcastUserChannels(clientFd, IrcReply::quit(caller.getNickname(), caller.getUsername(), reason.empty() ? "Client Quit" : reason));
	removeClient(clientFd);
}

void	Server::kickCommand(int clientFd, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("KICK")));

	User 		&caller = getUser(clientFd);
	std::string targetNick = params[1];
	std::string reason = params.size() > 2 ? params[2] : "Kicked";

	Channel *channel = getOperatorChannel(clientFd, params[0]);
	if (!channel)
		return ;

	User *target = getUserByNick(targetNick);
	if (!target)
		return ;

	broadcast(channel, IrcReply::kick(caller.getNickname(), caller.getUsername(), params[0], targetNick, reason));
	channel->kickClient(target->getFd());
}

void	Server::inviteCommand(int clientFd, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("INVITE")));

	User &caller = getUser(clientFd);
	std::string targetNick = params[0];

	Channel	*channel = getOperatorChannel(clientFd, params[1]);
	if (!channel)
		return ;

	User *target = getUserByNick(targetNick);
	if (!target)
		return ;

	channel->inviteClient(target->getFd());
	sendMessage(target->getFd(), IrcReply::invite(caller.getNickname(), caller.getUsername(), targetNick, params[1]));
}

void	Server::topicCommand(int clientFd, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
	if (params.size() < 1)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("TOPIC")));

	User 		&caller = getUser(clientFd);
	std::string channelName = params[0];

	Channel *channel = getChannel(channelName);
	if (!channel)
		return ;

	if (params.size() == 1)
		return (sendMessage(clientFd, IrcReply::topic(caller.getNickname(), channelName, channel->viewTopic())));

	std::string newTopic = params[1];
	if (newTopic.size() > MAX_TOPIC_LEN)
		return (sendMessage(clientFd, IrcReply::topicTooLong(caller.getNickname(), channelName)));
	if (!channel->changeTopic(clientFd, newTopic))
		return (sendMessage(clientFd, IrcReply::chanOpPrivsNeeded(caller.getNickname(), channelName)));

	broadcast(channel, IrcReply::topicChanged(caller.getNickname(), caller.getUsername(), channelName, newTopic));
}

void	Server::modeCommand(int clientFd, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("MODE")));

	std::string flag = params[1];
	if (flag.size() < 2)
    	return (sendMessage(clientFd, IrcReply::notEnoughParams("MODE")));

	std::string optionalParam = params.size() > 2 ? params[2] : "";

	Channel *channel = getOperatorChannel(clientFd, params[0]);
	if (!channel)
		return ;

	bool activate = flag[0] == '+';
	switch (flag[1])
	{
		case 'i':
		{
			channel->setInviteOnly(activate);
			break;
		}	
		case 't':
		{
			channel->setTopicRestriction(activate);
			break;
		}
		case 'k':
		{
			if (activate && optionalParam.empty())
				return (sendMessage(clientFd, IrcReply::notEnoughParams("MODE")));
			channel->changePassword(optionalParam);
			break;
		}
		case 'o':
		{
			if (optionalParam.empty())
				return (sendMessage(clientFd, IrcReply::notEnoughParams("MODE")));
			User *target = getUserByNick(optionalParam);
			if (!target)
				return ;
			channel->setOperator(target->getFd(), activate);
			break;
		}
		case 'l':
		{
			if (activate && optionalParam.empty())
				return (sendMessage(clientFd, IrcReply::notEnoughParams("MODE")));
			channel->setUserLimit(activate, (size_t)atoi(optionalParam.c_str()));
			break;
		}
	}
}

void	Server::privmsgCommand(int clientFd, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("PRIVMSG")));
	
	User		&caller = getUser(clientFd);
	std::string	target = params[0];
	std::string	message = params[1];

	if (target[0] == '#')
	{
		Channel	*channel = getChannel(target);
		if (!channel)
			return ;
		broadcast(channel, IrcReply::privmsg(caller.getNickname(), caller.getUsername(), target, message), clientFd);
	}
	else
	{
		User	*targetUser = getUserByNick(target);
		if (!targetUser)
			return ;
		sendMessage(targetUser->getFd(), IrcReply::privmsg(caller.getNickname(), caller.getUsername(), target, message));
	}
}

void	Server::uploadCommand(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	std::string					target;
	std::string					filePath;
	std::string					message;
	Channel						*channel;
	User						&caller = getUser(clientFd);

	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("UPLOAD")));
	target = params[0];
	filePath = params[1];
	channel = getChannel(target);
	if (!channel)
		return ;
	try 
	{
		channel->storeFile(File(filePath));
		message = "upload a file: " + filePath.substr(filePath.find_last_of('/') + 1);
		broadcast(channel, IrcReply::privmsg(caller.getNickname(), caller.getUsername(), target, message), clientFd);
	}
	catch (const std::exception &e)
	{
		message = "Failed to upload file cause: " + std::string(e.what());
		sendMessage(clientFd, IrcReply::privmsg(caller.getNickname(), caller.getUsername(), target, message));
	}
}

void	Server::downloadCommand(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	std::string					target;
	std::string					fileName;
	std::string					folderPath;
	Channel						*channel;
	User						&caller = getUser(clientFd);

	if (params.size() < 3)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("DOWNLOAD")));
	target = params[0];
	fileName = params[1];
	folderPath = params[2];
	channel = getChannel(target);
	if (!channel)
		return ;
	try
	{
		channel->getFile(fileName).createNewFile(folderPath);
	}
	catch (const std::exception &e)
	{
		sendMessage(clientFd, IrcReply::privmsg(caller.getNickname(), caller.getUsername(), target,
			"Failed to download file cause: " + std::string(e.what())));
	}
}
