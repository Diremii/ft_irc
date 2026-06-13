/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 12:26:46 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/13 15:04:46 by humontas@st      ###   ########.fr       */
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

bool    TicTacToe::checkWin(const std::string &symbol)
{
    int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    for (int i = 0; i < 8; i++)
    {
        if(_board[wins[i][0]] == symbol && _board[wins[i][1]] == symbol && _board[wins[i][2]] == symbol)
            return (true);
    }
    return (false);
}

bool    TicTacToe::checkDraw()
{
    int filled = 0;

    for (int i = 0; i < 9; i++)
    {
        if (_board[i] == "X" || _board[i] == "O")
            filled++;
    }
    return (filled == 9);
}

int TicTacToe::minimax(bool isBot)
{
    if (checkWin(_botSymbol))
        return (1);
    if (checkWin(_playerSymbol))
        return (-1);
    if (checkDraw())
        return (0);
    
    int best = isBot ? -1000 : 1000;
    for (int i = 0; i < 9; i++)
    {
        if (_board[i] != "X" && _board[i] != "O")
        {
            std::string tmp = _board[i];
            _board[i] = isBot ? _botSymbol : _playerSymbol;
            int score = minimax(!isBot);
            _board[i] = tmp;
            if (isBot)
                best = std::max(best, score);
            else
                best = std::min(best, score);
        }
    }
    return (best);
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
