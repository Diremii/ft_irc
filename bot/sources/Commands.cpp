/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 01:06:27 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/15 13:06:25 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void    Bot::endGame(const std::string &channel, const std::string &message)
{
    sendMessage("PRIVMSG " + channel + message + "\r\n");
    delete _games[channel];
    _games.erase(channel);
}

void    Bot::sendBoard(const std::string &channel, TicTacToe *game)
{
    sendMessage("PRIVMSG " + channel + " : " + game->getBoard(0) + " | " + game->getBoard(1) + " | " + game->getBoard(2) + "\r\n");
    sendMessage("PRIVMSG " + channel + " :---+---+---\r\n");
    sendMessage("PRIVMSG " + channel + " : " + game->getBoard(3) + " | " + game->getBoard(4) + " | " + game->getBoard(5) + "\r\n");
    sendMessage("PRIVMSG " + channel + " :---+---+---\r\n");
    sendMessage("PRIVMSG " + channel + " : " + game->getBoard(6) + " | " + game->getBoard(7) + " | " + game->getBoard(8) + "\r\n");
}

void    Bot::handlePlay(const std::string &nick, const std::string &channel)
{
    if (_games.find(channel) != _games.end())
        return (sendMessage("PRIVMSG " + channel + " :A game is already in progress!\r\n"));

    _games[channel] = new TicTacToe(channel, nick);

    if (_games[channel]->getBotSymbol() == "X")
    {
        int botMove = _games[channel]->getBestMove();
        _games[channel]->makeMove(botMove, _games[channel]->getBotSymbol());
        sendMessage("PRIVMSG " + channel + " :I'll go first. Good luck.\r\n");
    }
    sendBoard(channel, _games[channel]);
    sendMessage("PRIVMSG " + channel + " :You are " + _games[channel]->getPlayerSymbol() + " - use !move <1-9>\r\n");
}

void    Bot::handleMove(const std::string &nick, const std::string &channel, const std::string &arg)
{
    if (_games.find(channel) == _games.end())
        return (sendMessage("PRIVMSG " + channel + BotReply::noGame()));
    if (nick != _games[channel]->getCurrentPlayer())
        return (sendMessage("PRIVMSG " + channel + BotReply::notYourTurn()));

    int position = atoi(arg.c_str());
    switch (_games[channel]->isValidMove(position))
    {
        case 1:
            return (sendMessage("PRIVMSG " + channel + BotReply::invalidPosition()));
        case 2:
            return (sendMessage("PRIVMSG " + channel + BotReply::positionTaken()));
    }

    _games[channel]->makeMove(position - 1, _games[channel]->getPlayerSymbol());
    sendMessage("PRIVMSG " + channel + BotReply::youPlayed(arg));
    sendBoard(channel, _games[channel]);

    if (_games[channel]->checkWin(_games[channel]->getPlayerSymbol()))
        return endGame(channel, BotReply::playerWins(nick));
    if (_games[channel]->checkDraw())
        return endGame(channel, BotReply::draw());

    sendMessage("PRIVMSG " + channel + BotReply::botThinks());
    int botMove = _games[channel]->getBestMove();
    _games[channel]->makeMove(botMove, _games[channel]->getBotSymbol());
    sendMessage("PRIVMSG " + channel + BotReply::botPlayed(std::string(1, '1' + botMove)));
    sendBoard(channel, _games[channel]);

    if (_games[channel]->checkWin(_games[channel]->getBotSymbol()))
        return endGame(channel, BotReply::botWins());
    if (_games[channel]->checkDraw())
        return endGame(channel, BotReply::draw());
    _games[channel]->_lastMoveTime = time(NULL);
}

void    Bot::handlePRIVMSG(const std::string &nick, const std::string &args)
{
    std::vector<std::string> params = splitArgs(args);
    if (params.size() < 2)
        return ;

    std::string channel = params[0];
    std::string message = args.substr(args.find(':') + 1);
    std::string command = message.substr(0, message.find(' '));
    std::string arg = message.find(' ') != std::string::npos ? message.substr(message.find(' ') + 1) : "";

    if (command == "!play")
        handlePlay(nick, channel);
    else if (command == "!move")
        handleMove(nick, channel, arg);
}