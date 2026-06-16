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
	typedef void (Server::*CommandHandler)(int, const std::string&);
	struct  s_cmd
	{
		const char      *str;
		CommandHandler  cmd;
		bool            auth;
	};
	static s_cmd cmd[11] = {
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
		{"DCC", &Server::dccSend, true}
	};
	bool    is_registered = getUser(clientFd).getRegistered();

	for (size_t i = 0; i < 11; i++)
	{
		if (std::strcmp(cmd[i].str, command.c_str()) == 0)
		{
			if (cmd[i].auth && !is_registered)
				return sendMessage(clientFd, IrcReply::notRegistered());
			(this->*cmd[i].cmd)(clientFd, args);
			if (i == 3) // QUIT index
				return ;
			break ;
		}
	}
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

	getUser(clientFd).setBuffer(getUser(clientFd).getBuffer() + std::string(buffer, bytes));

	while (true)
	{
		std::string	buf = getUser(clientFd).getBuffer();
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
		getUser(clientFd).setBuffer(buf.substr(pos + delimLen));

		std::pair<std::string, std::string>	parsed = parseMessage(line);
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