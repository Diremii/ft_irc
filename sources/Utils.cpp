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

#include "../includes/Server.hpp"

std::pair<std::string, std::string>	Server::parseMessage(const std::string &message)
{
	size_t space = message.find(' ');
	if (space == std::string::npos)
		return (std::make_pair(message, ""));
	std::string command = message.substr(0, space);
	std::string args = message.substr(space + 1);
	return (std::make_pair(command, args));
}

void Server::tryRegister(int clientFd)
{
    User &user = getUser(clientFd);
    if (!user.getRegistered() && user.getAuthenticated() && !user.getNickname().empty() && !user.getUsername().empty())
    {
        user.setRegistered(true);
        sendMessage(clientFd, ":server 001 " + user.getNickname() + " :Welcome to the Internet Relay Network " + user.getNickname() + "!" + user.getUsername() + "@localhost\r\n");
    }
}

User	&Server::getUser(int clientFd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i].getFd() == clientFd)
			return (_users[i]);
	}
	throw std::runtime_error("User not found");
}

void	Server::sendMessage(int clientFd, const std::string &message)
{
	send(clientFd, message.c_str(), message.size(), 0);
}

void Server::broadcast(Channel *channel, const std::string &message)
{
    const std::vector<int> &users = channel->getUsers();

    for (size_t i = 0; i < users.size(); i++)
        sendMessage(users[i], message);
}

void	Server::broadcastUserChannels(int clientFd, const std::string &message)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].isUserExist(clientFd))
		{
			const std::vector<int> &users = _channels[i].getUsers();
			for (size_t j = 0; j < users.size(); j++)
				sendMessage(users[j], message);
		}
	}
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

User *Server::getUserByNick(const std::string &nick)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].getNickname() == nick)
            return &_users[i];
    }
    return NULL;
}

void	Server::sendNamesList(int clientFd, Channel *channel)
{
    std::string nick = getUser(clientFd).getNickname();
    std::string usersList;
    const std::vector<int> &users = channel->getUsers();
    for (size_t i = 0; i < users.size(); i++)
    {
        if (i > 0)
            usersList += " ";
        usersList += getUser(users[i]).getNickname();
    }
    sendMessage(clientFd, IrcReply::namesList(nick, channel->getName(), usersList));
    sendMessage(clientFd, IrcReply::endOfNames(nick, channel->getName()));
}