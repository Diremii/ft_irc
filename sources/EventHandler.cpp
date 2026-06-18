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

	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		close(clientSocket);
		return ;
	}
	
	struct pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;

	_pollFds.push_back(clientPollFd);
	_users.push_back(User(clientSocket));
}

// --------------------
//  Command Handling
// --------------------

const Server::s_cmd Server::_commandArray[] = {
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

void	Server::handleCommand(int clientFd, const std::string &command, const std::string &args)
{
	bool				isRegistered = getUser(clientFd).getRegistered();
	bool				found = false;
	static const size_t	arraySize = sizeof(_commandArray) / sizeof(s_cmd);
	
	for (size_t i = 0; i < arraySize; i++)
	{
		if (std::strcmp(_commandArray[i].commandName, command.c_str()) == 0)
		{
			found = true;
			if (_commandArray[i].authentification && !isRegistered)
				return sendMessage(clientFd, IrcReply::notRegistered());
			(this->*_commandArray[i].command)(clientFd, args);
			if (std::strcmp(_commandArray[i].commandName, "QUIT") == 0)
				return ;
			break ;
		}
	}
	if (!found)
		return (sendMessage(clientFd, IrcReply::unknownCommand(getUser(clientFd).getNickname(), command)));

	tryRegister(clientFd);
	log(clientFd, command, args);
}

void	Server::handleClient(int clientFd)
{
	char	buffer[RECV_BUFFER_SIZE];
	int		bytes = recv(clientFd, buffer, sizeof(buffer), 0);
	if (bytes <= 0)
	{
		removeClient(clientFd);
		return ;
	}

	User	&caller = getUser(clientFd);
	caller.setBuffer(caller.getBuffer() + std::string(buffer, bytes));

	if (caller.getBuffer().size() > MAX_BUFFER_SIZE)
	{
		removeClient(clientFd, "Buffer overflow");
		return ;
	}

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

		if (line.size() > RECV_BUFFER_SIZE)
			continue ;

		std::pair<std::string, std::string> parsed = parseMessage(line);
		handleCommand(clientFd, parsed.first, parsed.second);

		if (!getUserByFd(clientFd))
			return ;
	}
}

void	Server::handleEvents()
{
	if (_pollFds[0].revents & POLLIN)
		acceptClient();

	for (size_t i = _pollFds.size(); i-- > 1;)
	{
		if (_pollFds[i].revents & POLLIN)
			handleClient(_pollFds[i].fd);
	}
}