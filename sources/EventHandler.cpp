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
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	_pollFds.push_back(clientPollFd);
	_users.push_back(User(clientSocket));
}

void	Server::removeClient(int clientFd)
{
	broadcastUserChannels(clientFd, IrcReply::quit(getUser(clientFd).getNickname(), getUser(clientFd).getUsername(), "Connection closed"));
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

	for (size_t i = 0; i < _channels.size(); i++)
    	_channels[i].removeUser(clientFd);
}

void	Server::handleCommand(int clientFd, const std::string &command, const std::string &args)
{
    if (command == "PASS")
        passCommand(clientFd, args);
    else if (command == "NICK")
        nickCommand(clientFd, args);
    else if (command == "USER")
        userCommand(clientFd, args);
    else if (command == "QUIT")
        return quitCommand(clientFd, args);
    else if (!getUser(clientFd).getRegistered())
        return sendMessage(clientFd, IrcReply::notRegistered());
    else if (command == "JOIN")
        joinChannel(clientFd, args);
    else if (command == "KICK")
        kickCommand(clientFd, args);
    else if (command == "INVITE")
        inviteCommand(clientFd, args);
    else if (command == "TOPIC")
        topicCommand(clientFd, args);
    else if (command == "MODE")
        modeCommand(clientFd, args);
	else if (command == "PRIVMSG")
		privmsgCommand(clientFd, args);
    else if (command == "DCCSEND")
        dccSendCommand(clientFd, args);
    else
        return ;
    tryRegister(clientFd);
}

void Server::handleClient(int clientFd)
{
    char buffer[1024];
    int bytes = recv(clientFd, buffer, sizeof(buffer), 0);
    if (bytes <= 0)
    {
        removeClient(clientFd);
        return ;
    }
    User &user = getUser(clientFd);
    user.setBuffer(user.getBuffer() + std::string(buffer, bytes));
    
	size_t pos;
	while (true)
	{
	    size_t delimLen = 2;
	    pos = user.getBuffer().find("\r\n");
	    if (pos == std::string::npos)
	    {
	        pos = user.getBuffer().find("\n");
	        delimLen = 1;
	    }
	    if (pos == std::string::npos)
	        break ;
	
	    std::string line = user.getBuffer().substr(0, pos);
	    user.setBuffer(user.getBuffer().substr(pos + delimLen));
	    std::pair<std::string, std::string> parsed = parseMessage(line);
	    handleCommand(clientFd, parsed.first, parsed.second);
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