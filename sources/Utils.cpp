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

std::pair<std::string, std::string> Server::parseMessage(const std::string &message)
{
    size_t space = message.find(' ');
    if (space == std::string::npos)
        return (std::make_pair(message, ""));
    std::string command = message.substr(0, space);
    std::string args = message.substr(space + 1);
    return (std::make_pair(command, args));
}

void    Server::checkRegistration(int clientFd)
{
    User    &user = getUser(clientFd);
    if (!user.getRegistered() && user.getAuthenticated() && !user.getNickname().empty() && !user.getUsername().empty())
    {
        user.setRegistered(true);
        sendMessage(clientFd, ":server 001 " + user.getNickname() + " :Welcome to the Internet Relay Network " + user.getNickname() + "!" + user.getUsername() + "@localhost\r\n");
    }
}

User    &Server::getUser(int clientFd)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].getFd() == clientFd)
            return (_users[i]);
    }
    throw std::runtime_error("User not found");
}

void    Server::sendMessage(int clientFd, const std::string &message)
{
    send(clientFd, message.c_str(), message.size(), 0);
}
