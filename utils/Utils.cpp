/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 08:54:27 by humontas          #+#    #+#             */
/*   Updated: 2026/05/24 08:54:27 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// --------------------
//       getters
// --------------------

User	&Server::getUser(int clientFd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i].getFd() == clientFd)
			return (_users[i]);
	}
	throw std::runtime_error("User not found");
}

Channel	*Server::getChannel(const std::string &channelName)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == channelName)
			return (&_channels[i]);
	}
	return (NULL);
}

// --------------------
//       utils
// --------------------

void	Server::sendMessage(int clientFd, const std::string &message)
{
	send(clientFd, message.c_str(), message.size(), 0);
}

std::pair<std::string, std::string>	Server::parseMessage(const std::string &message)
{
	size_t space = message.find(' ');
	if (space == std::string::npos)
		return (std::make_pair(message, ""));

	std::string command = message.substr(0, space);
	std::string args = message.substr(space + 1);
	return (std::make_pair(command, args));
}

void	Server::tryRegister(int clientFd)
{
	User &user = getUser(clientFd);
	if (!user.getRegistered() && user.getAuthenticated() && !user.getNickname().empty() && !user.getUsername().empty())
	{
		user.setRegistered(true);
		sendMessage(clientFd, IrcReply::welcome(user.getNickname(), user.getUsername()));
	}
}

std::string	Server::getTimestamp()
{
	time_t now = time(NULL);
	char buf[20];
	strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
	return (std::string(buf));
}

void	Server::log(int clientFd, const std::string &command, const std::string &args)
{
	User		*user = getUserByFd(clientFd);
	std::string	nick = (user && !user->getNickname().empty()) ? user->getNickname() : "unknown";
	std::cout << "[" << getTimestamp() << "] [" << nick << "] " << command << " " << args << std::endl;
}

void	Server::removeClient(int clientFd, const std::string &reason)
{
	User	*user = getUserByFd(clientFd);
	if (!user)
		return ;
	broadcastUserChannels(clientFd, IrcReply::quit(user->getNickname(), user->getUsername(), reason), clientFd);
	close(clientFd);

	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (_pollFds[i].fd == clientFd)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break ;
		}
	}

	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i].getFd() == clientFd)
		{
			_users.erase(_users.begin() + i);
			break ;
		}
	}

	for (size_t i = _channels.size(); i-- > 0;)
	{
		_channels[i].removeUser(clientFd);
		_channels[i].removeOperator(clientFd);
		if (_channels[i].getUsers().empty())
			_channels.erase(_channels.begin() + i);
	}
}

void	Server::removeChannel(const std::string &channelName)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == channelName)
		{
			_channels.erase(_channels.begin() + i);
			return ;
		}
	}
}
