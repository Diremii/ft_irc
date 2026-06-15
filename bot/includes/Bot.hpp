/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 01:53:59 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/15 13:20:41 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Includes.hpp"
# include "BotReplies.hpp"
# include "TicTacToe.hpp"

class Bot
{
	private:
		int									_userFd;
		std::string							_nickName;
		std::string							_userName;
		std::string							_buffer;
		std::vector<std::string>			_channels;
		std::map<std::string, TicTacToe*>	_games;

		/* SERVER */
		void	createSocket();
		void	connectToServer(const std::string &hostname, int port);
		void	registerBot(const std::string &password);

		/* EVENTS */
		void	handleBot();
		void	checkTimers();
		void	handleCommand(const std::string &line, const std::string &command, const std::string &args);
		void	handlePRIVMSG(const std::string &nick, const std::string &args);

		/* COMMANDS */
		void	handlePlay(const std::string &nick, const std::string &channel);
		void	handleMove(const std::string &nick, const std::string &channel, const std::string &arg);
		void	endGame(const std::string &channel, const std::string &message);
		void	sendBoard(const std::string &channel, TicTacToe *game);

		/* UTILS */
		std::pair<std::string, std::string>	parseMessage(const std::string &message);
		std::vector<std::string>			splitArgs(const std::string &args);
		std::string							generateNick();
		std::string							getNickFromPrefix(const std::string &line);
		void								sendMessage(const std::string &message);

	public:
		Bot(const std::string &hostname, int port, const std::string &password);
		~Bot();

		void	run();
};

#endif