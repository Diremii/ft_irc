/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:00:33 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 11:00:33 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Creation du socket, fonctionne comme un fd, IPv4/TCP, protocole auto
void    Server::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
}

void    Server::bindSocket()
{

}

Server::listenSocket()
{

}

Server::Server(int port, std::string password) :
    _serverPort(port),
    _serverPassword(password)
{
    createSocket();
    bindSocket();
    listenSocket();
}