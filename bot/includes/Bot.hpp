/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 01:53:59 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/18 09:27:16 by humontas@st      ###   ########.fr       */
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
		std::map<std::string, TicTacToe*>	_games;

		/* SERVER */
		void	createSocket();
		void	connectToServer(const std::string &hostname, int port);
		void	registerBot(const std::string &password);

		/* EVENTS */
		void	handleBot();
		void	checkTimers();
		void	handleCommand(const std::string &line, const std::string &command, const std::string &args);
		void	handlePrefixCommands(const std::string &nick, const std::string &args);

		/* COMMANDS */
		void	helpCommand(const std::string &channel);
		void	playCommand(const std::string &nick, const std::string &channel);
		void	moveCommand(const std::string &nick, const std::string &channel, const std::string &arg);

		/*GAME UTILS*/
		void	endGame(const std::string &channel, const std::string &message);
		void	sendBoard(const std::string &channel, TicTacToe *game);
		bool	applyMove(const std::string &channel, int position, const std::string &symbol, const std::string &display, const std::string &winMsg);

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
