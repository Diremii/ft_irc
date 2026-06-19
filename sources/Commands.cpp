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
	switch (isValidName(nickName, MAX_NICK_LEN, " ,*?!@"))
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

	std::string oldNick = getUser(clientFd).getNickname();
	getUser(clientFd).setNickname(nickName);
	if (getUser(clientFd).getRegistered())
	{
		sendMessage(clientFd, IrcReply::nick(oldNick, getUser(clientFd).getUsername(), nickName));
		broadcastUserChannels(clientFd, IrcReply::nick(oldNick, getUser(clientFd).getUsername(), nickName), clientFd);
	}
}

void	Server::userCommand(int clientFd, const std::string &userName)
{
	User		&caller = getUser(clientFd);
	std::string	username = userName.substr(0, userName.find(' '));

	if (caller.getRegistered())
		return (sendMessage(clientFd, IrcReply::alreadyRegistered()));

	switch (isValidName(username, MAX_USER_LEN, " @\r\n"))
	{
		case 1:
			return (sendMessage(clientFd, IrcReply::notEnoughParams("USER")));
		case 2:
			return (sendMessage(clientFd, IrcReply::badUser(username)));
	}

	caller.setUsername(username);
}

void	Server::joinChannel(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	if (params.empty())
		return (sendMessage(clientFd, IrcReply::notEnoughParams("JOIN")));

	User		&caller = getUser(clientFd);
	std::string	channelName = params[0];
	std::string	password = (params.size() > 1) ? params[1] : "";

	if (channelName[0] != '#')
		return (sendMessage(clientFd, IrcReply::badChannelMask(caller.getNickname(), channelName)));

	Channel	*channel = getChannel(channelName);
	if (!channel)
	{
		_channels.push_back(Channel(channelName, getUser(clientFd)));
		channel = getChannel(channelName);
	}
	else
	{
		switch (channel->addUser(clientFd, password))
		{
			case 1:
				return (sendMessage(clientFd, IrcReply::inviteOnlyChannel(caller.getNickname(), channelName)));
			case 2:
				return (sendMessage(clientFd, IrcReply::channelIsFull(caller.getNickname(), channelName)));
			case 3:
				return (sendMessage(clientFd, IrcReply::badChannelKey(caller.getNickname(), channelName)));
		}
	}

	broadcast(channel, IrcReply::join(caller.getNickname(), caller.getUsername(), channelName));
	if (channel->viewTopic().empty())
		sendMessage(clientFd, IrcReply::noTopic(caller.getNickname(), channelName));
	else
		sendMessage(clientFd, IrcReply::topic(caller.getNickname(), channelName, channel->viewTopic()));
	sendNamesList(clientFd, channel);
}

void Server::quitCommand(int clientFd, const std::string &reason)
{
	User		&caller = getUser(clientFd);
	std::string	msg = reason.empty() ? "Client Quit" : reason;
	std::string	nick = caller.getNickname().empty() ? "unknown" : caller.getNickname();
	std::cout << "[" << getTimestamp() << "] [" << nick << "] QUIT :" << msg << std::endl;
	removeClient(clientFd, msg);
}

void	Server::kickCommand(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("KICK")));

	User		&caller = getUser(clientFd);
	std::string	targetNick = params[1];
	std::string	reason = params.size() > 2 ? params[2] : "Kicked";

	Channel *channel = checkChannelRequirements(clientFd, params[0], NEED_OPERATOR);
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
	std::vector<std::string>params	= splitArgs(args);
	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("INVITE")));

	User		&caller = getUser(clientFd);
	std::string	targetNick = params[0];

	Channel	*channel = checkChannelRequirements(clientFd, params[1], NEED_OPERATOR);
	if (!channel)
		return ;

	User	*target = getUserByNick(targetNick);
	if (!target)
		return ;

	channel->inviteClient(target->getFd());
	sendMessage(target->getFd(), IrcReply::invite(caller.getNickname(), caller.getUsername(), targetNick, params[1]));
}

void	Server::topicCommand(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	if (params.empty())
		return (sendMessage(clientFd, IrcReply::notEnoughParams("TOPIC")));

	User		&caller = getUser(clientFd);
	std::string	channelName = params[0];

	Channel *channel = checkChannelRequirements(clientFd, channelName, NO_NEED_OPERATOR);
	if (!channel)
		return ;

	if (params.size() == 1)
		return (sendMessage(clientFd, IrcReply::topic(caller.getNickname(), channelName, channel->viewTopic())));

	std::string	newTopic = params[1];
	if (newTopic.size() > MAX_TOPIC_LEN)
		return (sendMessage(clientFd, IrcReply::topicTooLong(caller.getNickname(), channelName)));
	if (!channel->changeTopic(clientFd, newTopic))
		return (sendMessage(clientFd, IrcReply::chanOpPrivsNeeded(caller.getNickname(), channelName)));

	broadcast(channel, IrcReply::topicChanged(caller.getNickname(), caller.getUsername(), channelName, newTopic));
}

void	Server::modeCommand(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	if (params.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("MODE")));

	std::string	flag = params[1];
	if (flag.size() < 2)
		return (sendMessage(clientFd, IrcReply::notEnoughParams("MODE")));

	std::string	optionalParam = params.size() > 2 ? params[2] : "";

	Channel	*channel = checkChannelRequirements(clientFd, params[0], NEED_OPERATOR);
	if (!channel)
		return ;

	User &caller = getUser(clientFd);
	bool	activate = flag[0] == '+';
	switch (flag[1])
	{
		default:
			return (sendMessage(clientFd, IrcReply::unknownMode(caller.getNickname(), std::string(1, flag[1]))));
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
			broadcast(channel, IrcReply::mode(caller.getNickname(), caller.getUsername(), params[0], flag, target->getNickname()));
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
		Channel *channel = getChannel(target);
		if (!channel)
			return (sendMessage(clientFd, IrcReply::noSuchChannel(caller.getNickname(), target)));
		if (!channel->isUserExist(clientFd))
			return (sendMessage(clientFd, IrcReply::notOnChannel(caller.getNickname(), target)));
		broadcast(channel, IrcReply::privmsg(caller.getNickname(), caller.getUsername(), target, message), clientFd);
	}
	else
	{
		User *targetUser = getUserByNick(target);
		if (!targetUser)
			return (sendMessage(clientFd, IrcReply::noSuchNick(caller.getNickname(), target)));
		sendMessage(targetUser->getFd(), IrcReply::privmsg(caller.getNickname(), caller.getUsername(), target, message));
	}
}

void	Server::partCommand(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	if (params.empty())
		return (sendMessage(clientFd, IrcReply::notEnoughParams("PART")));

	User		&caller = getUser(clientFd);
	std::string	reason = (params.size() > 1) ? params[1] : "";

	Channel	*channel = checkChannelRequirements(clientFd, params[0], NO_NEED_OPERATOR);
	if (!channel)
		return ;

	broadcast(channel, IrcReply::part(caller.getNickname(), caller.getUsername(), channel->getName(), reason));
	channel->removeUser(clientFd);
	if (channel->getUsers().empty())
		removeChannel(channel->getName());
}

void	Server::dccSend(int clientFd, const std::string &args)
{
	std::vector<std::string>	params = splitArgs(args);
	if (params.size() < 5 || params[0] != "SEND")
		return (sendMessage(clientFd, IrcReply::notEnoughParams("DCC SEND")));

	User		&caller = getUser(clientFd);
	std::string	target = params[1];
	std::string	fileName = params[2];
	std::string	port = params[3];
	std::string	fileSize = params[4];

	User	*targetUser = getUserByNick(target);
	if (!targetUser)
		return ;

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	getsockname(clientFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

	std::ostringstream	oss;
	oss << ntohl(clientAddr.sin_addr.s_addr);

	sendMessage(targetUser->getFd(), IrcReply::dccSend(caller.getNickname(), caller.getUsername(), target, fileName, oss.str(), port, fileSize));
}
