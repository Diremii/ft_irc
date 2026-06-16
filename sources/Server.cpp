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

// --------------------
//     constructors
// --------------------

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
	for (size_t i = 0; i < _pollFds.size(); i++)
		close(_pollFds[i].fd);
}

// --------------------
//      setup
// --------------------

void Server::createSocket()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		throw std::runtime_error("Failed to create socket");

	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(_serverSocket);
		throw std::runtime_error("setsockopt SO_REUSEADDR failed");
	}
}

void Server::bindSocket()
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_serverPort);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_serverSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		close(_serverSocket);
		throw std::runtime_error("Failed to bind socket");
	}
}

void Server::listenSocket()
{
	if (listen(_serverSocket, 10) == -1)
		throw std::runtime_error("Failed to listen on socket");
}

void Server::initPollFds()
{
	struct pollfd serverPollFd;
	serverPollFd.fd = _serverSocket;
	serverPollFd.events = POLLIN;
	serverPollFd.revents = 0;

	_pollFds.push_back(serverPollFd);
}

// --------------------
//       core
// --------------------

void Server::run()
{
	while (true)
	{
		int activity = poll(_pollFds.data(), _pollFds.size(), -1);
		if (g_sig == SIGINT || g_sig == SIGQUIT)
			return ;
		if (activity == -1)
			throw std::runtime_error("Poll error");
		handleEvents();
	}
}

// --------------------
//      getters
// --------------------

User *Server::getUserByFd(int clientFd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i].getFd() == clientFd)
			return (&_users[i]);
	}
	return (NULL);
}