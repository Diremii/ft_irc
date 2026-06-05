/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:46:43 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 10:46:43 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"
# include "User.hpp"
# include "Channel.hpp"

class Server
{
	private:
		int							_serverSocket;
		int							_serverPort;
		std::string					_serverPassword;
		std::vector<struct pollfd>	_pollFds;
		std::vector<User>			_users;
		std::vector<Channel>		_channels;

		/* SERVER */
		void	createSocket();
		void	bindSocket();
		void	listenSocket();
		void	initPollFds();
	
		/* EVENTHANDLER */
		void	acceptClient();
		void	handleEvents();
		void	removeClient(int clientFd);
		void	handleClient(int clientFd);
		void	handleCommand(int clientFd, const std::string &command, const std::string &args);
		
		/* COMMANDS */
		void	passCommand(int clientFd, const std::string &password);
		void	nickCommand(int clientFd, const std::string &nickName);
		void	userCommand(int clientFd, const std::string &userName);
		void	quitCommand(int clientFd, const std::string &message);
		void	joinChannel(int clientFd, const std::string &channelName);
		void	kickCommand(int clientFd, int targetFd, const std::string &channelName, std::string &reason);
		void	inviteCommand(int clientFd);
		void	topicCommand(int clientFd);
		void	modeCommand(int clientFd);
		
		/* UTILS */
		std::pair<std::string, std::string>	parseMessage(const std::string &message);
		User								&getUser(int clientFd);  
		void								tryRegister(int clientFd);
		void								sendMessage(int clientFd, const std::string &message);
		void								broadcast(Channel *channel, const std::string &message);
		void								broadcastUserChannels(int clientFd, const std::string &message);
		Channel								*getChannel(const std::string &channelName);


	public:
		Server(int port, std::string password);
		~Server();

		void	run();
};

#endif