/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 01:48:22 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/16 11:06:28 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TicTacToe.hpp"

// --------------------
//     constructors
// --------------------

TicTacToe::TicTacToe(const std::string &channel, const std::string &player)
	: _gameChannel(channel),
	  _currentPlayer(player),
	  _gameEndTime(0),
	  _gameOver(false),
	  _lastMoveTime(time(NULL))
{
	initGame();
}

TicTacToe::~TicTacToe() {}

// --------------------
//     initialization
// --------------------

void	TicTacToe::initGame()
{
	for (int i = 0; i < 9; i++)
	{
		std::stringstream	ss;
		ss << (i + 1);
		_board[i] = ss.str();
	}

	srand(time(NULL));
	_botSymbol = (rand() % 2 == 0) ? "X" : "O";
	_playerSymbol = (_botSymbol == "X") ? "O" : "X";
}

// --------------------
//        core
// --------------------

void TicTacToe::makeMove(int pos, const std::string &symbol)
{
	_board[pos] = symbol;
}

int TicTacToe::isValidMove(int pos)
{
	if (pos < 1 || pos > 9)
		return (1);
	if (_board[pos - 1] == "X" || _board[pos - 1] == "O")
		return (2);
	return (0);
}

// --------------------
//       getters
// --------------------

time_t	TicTacToe::getGameEndTime() const
{
	return (_gameEndTime);
}

bool	TicTacToe::isGameOver() const
{
	return (_gameOver);
}

std::string	TicTacToe::getPlayerSymbol() const
{
	return (_playerSymbol);
}

std::string	TicTacToe::getBotSymbol() const
{
	return (_botSymbol);
}

std::string	TicTacToe::getCurrentPlayer() const
{
	return (_currentPlayer);
}

std::string	TicTacToe::getBoard(int pos) const
{
	return (_board[pos]);
}