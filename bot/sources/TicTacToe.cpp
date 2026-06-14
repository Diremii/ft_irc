/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 12:26:46 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/14 19:32:40 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TicTacToe.hpp"

void    TicTacToe::initGame()
{
    for (int i = 0; i < 9; i++)
    {
        std::stringstream    ss;
        ss << (i + 1);
        _board[i] = ss.str();
    }
    srand(time(NULL));
    _botSymbol = (rand() % 2 == 0) ? "X" : "O";
    _playerSymbol = (_botSymbol == "X") ? "O" : "X";
}

void    TicTacToe::makeMove(int pos, const std::string &symbol)
{
    _board[pos] = symbol;
}

time_t  TicTacToe::getLastMoveTime()
{
    return (_lastMoveTime);
}

time_t  TicTacToe::getGameEndTime()
{
    return (_gameEndTime);
}

bool    TicTacToe::isGameOver()
{
    return (_gameOver);
}

std::string TicTacToe::getPlayerSymbol()
{
    return (_playerSymbol);
}

std::string TicTacToe::getBotSymbol()
{
    return (_botSymbol);
}

std::string TicTacToe::getCurrentPlayer()
{
    return (_currentPlayer);
}

std::string TicTacToe::getBoard(int pos)
{
    return (_board[pos]);
}

int    TicTacToe::isValidMove(int pos)
{
    if (pos < 1 || pos > 9)
        return (1);
    if (_board[pos - 1] == "X" || _board[pos - 1] == "O")
        return (2);
    return (0);
}

TicTacToe::TicTacToe(const std::string &channel, const std::string &player) : 
    _gameChannel(channel),
    _currentPlayer(player),
    _lastMoveTime(time(NULL)),
    _gameEndTime(0),
    _gameOver(false)
{
    initGame();
}

TicTacToe::~TicTacToe() {}
