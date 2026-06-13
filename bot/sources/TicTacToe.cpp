/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 12:26:46 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/14 01:04:32 by humontas@st      ###   ########.fr       */
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

std::string    TicTacToe::printBoard()
{
    std::string result = "";
                result += " " + _board[0] + " | " + _board[1] + " | " + _board[2] + "\n";
                result += "---+---+---\n";
                result += " " + _board[3] + " | " + _board[4] + " | " + _board[5] + "\n";
                result += "---+---+---\n";
                result += " " + _board[6] + " | " + _board[7] + " | " + _board[8] + "\n";
    return (result);      
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

TicTacToe::TicTacToe(const std::string &channel, const std::string &player) : 
    _gameChannel(channel),
    _currentPlayer(player),
    _gameOver(false),
    _lastMoveTime(time(NULL)),
    _gameEndTime(0)
{
    initGame();
}

TicTacToe::~TicTacToe() {}
