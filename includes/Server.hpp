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
# include "Replies.hpp"

class Server
{
	private:
		int								_serverSocket;
		int								_serverPort;
		std::string						_serverPassword;
		std::vector<struct pollfd>		_pollFds;
		std::vector<User>				_users;
		std::vector<Channel>			_channels;

		/* SERVER */
		void	createSocket();
		void	bindSocket();
		void	listenSocket();
		void	initPollFds();
	
		/* EVENTHANDLER */
		void	acceptClient();
		void	handleEvents();
		void	removeClient(int clientFd, const std::string &reason = "Connection closed");
		void	handleClient(int clientFd);
		void	handleCommand(int clientFd, const std::string &command, const std::string &args);
		
		/* COMMANDS */
		void	passCommand(int clientFd, const std::string &password);
		void	nickCommand(int clientFd, const std::string &nickName);
		void	userCommand(int clientFd, const std::string &userName);
		void	quitCommand(int clientFd, const std::string &message);
		void	joinChannel(int clientFd, const std::string &args);
		void	kickCommand(int clientFd, const std::string &args);
		void	topicCommand(int clientFd, const std::string &args);
		void	inviteCommand(int clientFd, const std::string &args);
		void	modeCommand(int clientFd, const std::string &args);
		void	privmsgCommand(int clientFd, const std::string &args);
		void	partCommand(int clientFd, const std::string &args);
		void	dccSend(int clientFd, const std::string &args);
		
		/* UTILS */
		User								&getUser(int clientFd);
		User								*getUserByFd(int clientFd);
		User								*getUserByNick(const std::string &nick);
		Channel								*getChannel(const std::string &channelName);
		Channel								*checkChannelRequirements(int clientFd, const std::string &channelName, bool checkOperator);
		void								removeChannel(const std::string &channelName);
		void								sendMessage(int clientFd, const std::string &message);
		void								broadcast(Channel *channel, const std::string &message, int excludeFd = -1);
		void								broadcastUserChannels(int clientFd, const std::string &message, int excludeFd = -1);
		void								sendNamesList(int clientFd, Channel *channel);
		std::pair<std::string, std::string>	parseMessage(const std::string &message);
		std::vector<std::string>			splitArgs(const std::string &args);
		int									isValidName(const std::string &name, size_t maxLen, const std::string &forbidden);
		void								tryRegister(int clientFd);
		void								log(int clientFd, const std::string &command, const std::string &args);
		std::string							getTimestamp();


	public:
		Server(int port, std::string password);
		~Server();

		void	run();
};

#endif