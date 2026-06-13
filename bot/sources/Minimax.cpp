/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minimax.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 01:04:14 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/14 01:04:41 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TicTacToe.hpp"

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

int     TicTacToe::getBestMove()
{
    int bestScore = -1000;
    int bestMove = -1;
    for (int i = 0; i < 9; i++)
    {
        if (_board[i] != "X" && _board[i] != "O")
        {
            std::string tmp = _board[i];
            _board[i] = _botSymbol;
            int score = minimax(false);
            _board[i] = tmp;
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return (bestMove);
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