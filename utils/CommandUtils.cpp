/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 11:24:13 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/16 23:32:24 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// --------------------
//       helpers
// --------------------

int	Server::isValidName(const std::string &name, size_t maxLen, const std::string &forbidden)
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

std::vector<std::string>	Server::splitArgs(const std::string &args)
{
	std::vector<std::string>	result;
	std::string					word;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i] == ':')
		{
			result.push_back(args.substr(i + 1));
			return (result);
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
	return (result);
}

// --------------------
//      broadcast
// --------------------

void	Server::broadcast(Channel *channel, const std::string &message, int excludeFd)
{
	const std::vector<int>	&users = channel->getUsers();
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i] != excludeFd)
			sendMessage(users[i], message);
	}
}

void	Server::broadcastUserChannels(int clientFd, const std::string &message, int excludeFd)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].isUserExist(clientFd))
		{
			const std::vector<int>	&users = _channels[i].getUsers();
			for (size_t j = 0; j < users.size(); j++)
			{
				if (users[j] != excludeFd)
					sendMessage(users[j], message);
			}
		}
	}
}

// --------------------
//       getters
// --------------------

User	*Server::getUserByNick(const std::string &nick)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i].getNickname() == nick)
			return (&_users[i]);
	}
	return (NULL);
}

Channel	*Server::getOperatorChannel(int clientFd, const std::string &channelName)
{
	Channel	*channel = getChannel(channelName);
	if (!channel)
		return (NULL);
	if (!channel->isOperator(clientFd))
	{
		sendMessage(clientFd, IrcReply::chanOpPrivsNeeded(getUser(clientFd).getNickname(), channelName));
		return (NULL);
	}
	return (channel);
}

// --------------------
//     names list
// --------------------

void	Server::sendNamesList(int clientFd, Channel *channel)
{
	std::string	nick = getUser(clientFd).getNickname();
	std::string	usersList;
	const std::vector<int>	&users = channel->getUsers();
	for (size_t i = 0; i < users.size(); i++)
	{
		if (i > 0)
			usersList += " ";
		usersList += getUser(users[i]).getNickname();
	}
	sendMessage(clientFd, IrcReply::namesList(nick, channel->getName(), usersList));
	sendMessage(clientFd, IrcReply::endOfNames(nick, channel->getName()));
}