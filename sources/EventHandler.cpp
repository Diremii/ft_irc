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

#include "Server.hpp"

void	Server::acceptClient()
{
	int	clientSocket = accept(_serverSocket, NULL, NULL);
	if (clientSocket == -1)
	{
		std::cerr << "accept() failed" << std::endl;
		return ;
	}
	struct pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;

	_pollFds.push_back(clientPollFd);
	_users.push_back(User(clientSocket));
}

void	Server::removeClient(int clientFd, const std::string &reason)
{
	broadcastUserChannels(clientFd, IrcReply::quit(getUser(clientFd).getNickname(), getUser(clientFd).getUsername(), reason));
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

void	Server::handleCommand(int clientFd, const std::string &command, const std::string &args)
{
	typedef void	(Server::*CommandHandler)(int, const std::string&);
	struct			s_cmd
	{
		const char		*commandName;
		CommandHandler	command;
		bool			authentification;
	};

	static const s_cmd	commandArray[] = {
		{"PASS", &Server::passCommand, false},
		{"NICK", &Server::nickCommand, false},
		{"USER", &Server::userCommand, false},
		{"QUIT", &Server::quitCommand, false},
		{"JOIN", &Server::joinChannel, true},
		{"KICK", &Server::kickCommand, true},
		{"INVITE", &Server::inviteCommand, true},
		{"TOPIC", &Server::topicCommand, true},
		{"MODE", &Server::modeCommand, true},
		{"PRIVMSG", &Server::privmsgCommand, true},
		{"PART", &Server::partCommand, true},
		{"DCC", &Server::dccSend, true}
	};

	bool	isRegistered = getUser(clientFd).getRegistered();
	bool	found = false;
	size_t	arraySize = sizeof(commandArray) / sizeof(s_cmd);
	
	for (size_t i = 0; i < arraySize; i++)
	{
		if (std::strcmp(commandArray[i].commandName, command.c_str()) == 0)
		{
			found = true;
			if (commandArray[i].authentification && !isRegistered)
				return sendMessage(clientFd, IrcReply::notRegistered());
			(this->*commandArray[i].command)(clientFd, args);
			if (std::strcmp(commandArray[i].commandName, "QUIT") == 0)
				return ;
			break;
		}
	}
	if (!found)
		return (sendMessage(clientFd, IrcReply::unknownCommand(getUser(clientFd).getNickname(), command)));

	tryRegister(clientFd);
	log(clientFd, command, args);
}

void	Server::handleClient(int clientFd)
{
	char	buffer[1024];
	int		bytes = recv(clientFd, buffer, sizeof(buffer), 0);
	if (bytes <= 0)
	{
		removeClient(clientFd);
		return ;
	}

	User	&caller = getUser(clientFd);
	caller.setBuffer(caller.getBuffer() + std::string(buffer, bytes));
c
	while (true)
	{
		std::string	buf = caller.getBuffer();
		size_t		delimLen = 2;
		size_t		pos = buf.find("\r\n");

		if (pos == std::string::npos)
		{
			pos = buf.find("\n");
			delimLen = 1;
		}
		if (pos == std::string::npos)
			break ;

		std::string	line = buf.substr(0, pos);
		caller.setBuffer(buf.substr(pos + delimLen));

		std::pair<std::string, std::string> parsed = parseMessage(line);
		handleCommand(clientFd, parsed.first, parsed.second);

		if (!getUserByFd(clientFd))
			return ;
	}
}

void	Server::handleEvents()
{
	for (size_t i = _pollFds.size(); i-- > 0;)
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