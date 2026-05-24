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

void    Server::passCommand(int clientFd, const std::string &password)
{
    if (password.empty())
    {
        sendMessage(clientFd, ":server 464 * :Password incorrect\r\n");
        return ;
    }
    if (password == _serverPassword)
        getUser(clientFd).setAuthenticated(true);
    else
        sendMessage(clientFd, ":server 464 * :Password incorrect\r\n");
}

static int    isValidName(const std::string &name, size_t maxLen, const std::string &forbidden)
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

void   Server::nickCommand(int clientFd, const std::string &nickName)
{
    switch (isValidName(nickName, 63, " ,*?!@"))
    {
        case 1: sendMessage(clientFd, ":server 431 * :No nickname given\r\n"); return ;
        case 2: sendMessage(clientFd, ":server 432 * " + nickName + " :Erroneous nickname\r\n"); return ;
    }
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (nickName == _users[i].getNickname())
        {
            sendMessage(clientFd, ":server 433 * " + nickName + " :Nickname is already in use\r\n");
            return ;
        }
    }
    getUser(clientFd).setNickname(nickName);
}

void   Server::userCommand(int clientFd, const std::string &userName)
{
    switch (isValidName(userName, 10, " @\r\n"))
    {
        case 1: sendMessage(clientFd, ":server 461 * USER :Not enough parameters\r\n"); return ;
        case 2: sendMessage(clientFd, ":server 461 * " + userName + " :Erroneous username\r\n"); return ;
    }
    getUser(clientFd).setUsername(userName);
}