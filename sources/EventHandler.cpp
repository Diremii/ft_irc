/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 10:03:07 by humontas          #+#    #+#             */
/*   Updated: 2026/05/26 10:03:07 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::acceptClient()
{
	int clientSocket = accept(_serverSocket, NULL, NULL);
	if (clientSocket == -1)
		throw std::runtime_error("accept() failed");

	struct pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN; // On veut être notifié des événements de lecture (données du client)
	clientPollFd.revents = 0;
	_pollFds.push_back(clientPollFd);
	_users.push_back(User(clientSocket));
}

void	Server::removeClient(int clientFd)
{
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
}

void	Server::handleCommand(int clientFd, const std::string &command, const std::string &args)
{
	User &user = getUser(clientFd);

	if (command == "QUIT")
		return (quitCommand(clientFd, args));
	if (command == "PASS")
		return (passCommand(clientFd, args));
	else if (command == "NICK")
		return (nickCommand(clientFd, args));
	else if (command == "USER")
		return (userCommand(clientFd, args));

	if (!user.getRegistered())
	{
		sendMessage(clientFd, ":server 451 * :You have not registered\r\n");
		return ;
	}

	if (command == "JOIN")
		joinChannel(clientFd, args);

	tryRegister(clientFd);
}

void	Server::handleClient(int clientFd)
{
	char buffer[1024];
	int bytes = recv(clientFd, buffer, sizeof(buffer), 0); // Lecture des données envoyées par le client
	if (bytes <= 0)
		removeClient(clientFd);
	else
	{
		std::string message(buffer, bytes);
		message.erase(message.find_last_not_of("\r\n") + 1);
		std::pair<std::string, std::string> parsed = parseMessage(message);
		std::string command = parsed.first;
		std::string args = parsed.second;
		handleCommand(clientFd, command, args);
	}
}

void	Server::handleEvents()
{
	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].revents & POLLIN)
		{
			if (_pollFds[i].fd == _serverSocket)
				acceptClient();
			else
				handleClient(_pollFds[i].fd);
		}
	}
}