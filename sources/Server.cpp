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

#include "../includes/Server.hpp"

// Creation du socket, fonctionne comme un fd, IPv4/TCP, protocole auto
void    Server::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
}

void    Server::bindSocket()
{
    // Remplissage de la struct d'adresse
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;                                    // IPv4
    addr.sin_port = htons(_serverPort);                           // Port en network byte order
    addr.sin_addr.s_addr = INADDR_ANY;                            // Ecoute sur toutes les interfaces (0.0.0.0)

    // Attache le socket au port et a l'IP definis dans addr
    if (bind(_serverSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        close(_serverSocket);
        throw std::runtime_error("Failed to bind socket");
    }
}

void    Server::listenSocket()
{
    // Met le socket en attente de connexions, backlog de 10
    if (listen(_serverSocket, 10) == -1)
        throw std::runtime_error("Failed to listen on socket");
}

void    Server::initPollFds()
{
    struct pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN; // On veut être notifié des événements de lecture (nouvelles connexions)
    serverPollFd.revents = 0;
    _pollFds.push_back(serverPollFd);
}

void    Server::run()
{
    while (true)
    {
        int activity = poll(_pollFds.data(), _pollFds.size(), -1); // -1 pour attendre indéfiniment
        if (activity == -1)
            throw std::runtime_error("Poll error");
        handleEvents();
    }
}

Server::Server(int port, std::string password) :
    _serverPort(port),
    _serverPassword(password)
{
    createSocket();
    bindSocket();
    listenSocket();
    initPollFds();
}

Server::~Server()
{
    close(_serverSocket);
}