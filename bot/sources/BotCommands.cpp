/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommands.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 01:06:27 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/17 22:51:26 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void	Bot::helpCommand(const std::string &channel)
{
	sendMessage("PRIVMSG " + channel + " :Available commands:\r\n");
	sendMessage("PRIVMSG " + channel + " :!play - Start a new TicTacToe game\r\n");
	sendMessage("PRIVMSG " + channel + " :!move <1-9> - Play a move\r\n");
	sendMessage("PRIVMSG " + channel + " :!help - Show this help\r\n");
}

void	Bot::playCommand(const std::string &nick, const std::string &channel)
{
	if (_games.find(channel) != _games.end())
		return (sendMessage("PRIVMSG " + channel + " :A game is already in progress!\r\n"));

	_games[channel] = new TicTacToe(channel, nick);

	if (_games[channel]->getBotSymbol() == "X")
	{
		int	botMove = _games[channel]->getBestMove();
		_games[channel]->makeMove(botMove, _games[channel]->getBotSymbol());
		sendMessage("PRIVMSG " + channel + " :I'll go first. Good luck.\r\n");
	}
	sendBoard(channel, _games[channel]);
	sendMessage("PRIVMSG " + channel + " :You are " + _games[channel]->getPlayerSymbol() + " - use !move <1-9>\r\n");
}

void	Bot::moveCommand(const std::string &nick, const std::string &channel, const std::string &arg)
{
	if (_games.find(channel) == _games.end())
		return (sendMessage("PRIVMSG " + channel + BotReply::noGame()));
	if (nick != _games[channel]->getCurrentPlayer())
		return (sendMessage("PRIVMSG " + channel + BotReply::notYourTurn()));
	int	position = atoi(arg.c_str());
	switch (_games[channel]->isValidMove(position))
	{
		case 1: return (sendMessage("PRIVMSG " + channel + BotReply::invalidPosition()));
		case 2: return (sendMessage("PRIVMSG " + channel + BotReply::positionTaken()));
	}
	if (applyMove(channel, position - 1, _games[channel]->getPlayerSymbol(), BotReply::youPlayed(arg), BotReply::playerWins(nick)))
		return ;
	sendMessage("PRIVMSG " + channel + BotReply::botThinks());
	int	botMove = _games[channel]->getBestMove();
	if (applyMove(channel, botMove, _games[channel]->getBotSymbol(), BotReply::botPlayed(std::string(1, '1' + botMove)), BotReply::botWins()))
		return ;
	_games[channel]->_lastMoveTime = time(NULL);
}

void	Bot::handlePRIVMSG(const std::string &nick, const std::string &args)
{
	std::vector<std::string> params = splitArgs(args);
	if (params.size() < 2)
		return ;
	std::string target = params[0];
	std::string message = args.substr(args.find(':') + 1);
	std::string command = message.substr(0, message.find(' '));
	std::string arg = message.find(' ') != std::string::npos ? message.substr(message.find(' ') + 1) : "";

	std::string channel = (target == _nickName) ? nick : target;

	if (command == "!play")
		playCommand(nick, channel);
	else if (command == "!move")
		moveCommand(nick, channel, arg);
	else if (command == "!help")
		helpCommand(channel);
}

