/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 01:06:27 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/14 19:31:08 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

#include "Bot.hpp"

void    Bot::sendBoard(const std::string &channel, TicTacToe *game)
{
    sendMessage("PRIVMSG " + channel + " : " + game->getBoard(0) + " | " + game->getBoard(1) + " | " + game->getBoard(2) + "\r\n");
    sendMessage("PRIVMSG " + channel + " :---+---+---\r\n");
    sendMessage("PRIVMSG " + channel + " : " + game->getBoard(3) + " | " + game->getBoard(4) + " | " + game->getBoard(5) + "\r\n");
    sendMessage("PRIVMSG " + channel + " :---+---+---\r\n");
    sendMessage("PRIVMSG " + channel + " : " + game->getBoard(6) + " | " + game->getBoard(7) + " | " + game->getBoard(8) + "\r\n");
}

void    Bot::handlePRIVMSG(const std::string &nick, const std::string &args)
{
    std::vector<std::string> params = splitArgs(args);
    if (params.size() < 2)
        return ;
    
    std::string channel = params[0];
    std::string command = params[1];

    if (command == "!play")
    {
        if (_games.find(channel) != _games.end())
            return (sendMessage("PRIVMSG " + channel + " :A game is already in progress!\r\n"));
        _games[channel] = new TicTacToe(channel, nick);
        sendBoard(channel, _games[channel]);
    }
    if (command == "!move")
    {
        if (_games.find(channel) == _games.end())
            return (sendMessage("PRIVMSG " + channel + " :There is no game in progress!\r\n"));
        if (nick != _games[channel]->getCurrentPlayer())
            return (sendMessage("PRIVMSG " + channel + " :It's not your turn to play!\r\n"));
        
        std::string pos = command.substr(6);
        int position = atoi(pos.c_str());
        switch (_games[channel]->isValidMove(position))
        {
            case 1:
                return (sendMessage("PRIVMSG " + channel + " :The position can only be from 1 to 9!\r\n"));
            case 2:
                return (sendMessage("PRIVMSG " + channel + " :This position has already been played!\r\n"));
        }
        _games[channel]->makeMove(position - 1, _games[channel]->getPlayerSymbol());
        sendBoard(channel, _games[channel]);
        if (_games[channel]->checkWin(_games[channel]->getPlayerSymbol()))
        {
            sendMessage("PRIVMSG " + channel + " :Well done! " + nick + " Your brain is way too big!\r\n");
            delete _games[channel];
            _games.erase(channel);
        }
        else if (_games[channel]->checkDraw())
        {
            sendMessage("PRIVMSG " + channel + " :A draw! A rather... boring result.\r\n");
            delete _games[channel];
            _games.erase(channel);
        }
        else
        {
            int botMove = _games[channel]->getBestMove();
            _games[channel]->makeMove(botMove, _games[channel]->getBotSymbol());
            sendBoard(channel, _games[channel]);
            if (_games[channel]->checkWin(_games[channel]->getBotSymbol()))
            {
                sendMessage("PRIVMSG " + channel + " :I won, not surprising.\r\n");
                delete _games[channel];
                _games.erase(channel);
            }
            else if (_games[channel]->checkDraw())
            {
                sendMessage("PRIVMSG " + channel + " :A draw! A rather... boring result.\r\n");
                delete _games[channel];
                _games.erase(channel);
            }
        }
    }
}