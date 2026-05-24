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
