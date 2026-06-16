/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 12:12:44 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/16 01:48:14 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TICTACTOE_HPP
# define TICTACTOE_HPP

# include "Includes.hpp"

class TicTacToe
{
	private:
		std::string	_gameChannel;
		std::string	_currentPlayer;
		std::string	_botSymbol;
		std::string	_playerSymbol;
		std::string	_board[9];
		time_t		_gameEndTime;
		bool		_gameOver;

		void	initGame();
		int		minimax(bool isBot, int depth);

	public:
		time_t	_lastMoveTime;

		TicTacToe(const std::string &channel, const std::string &player);
		~TicTacToe();

		/* GETTERS */
		time_t			getGameEndTime() const;
		std::string		getCurrentPlayer() const;
		std::string		getPlayerSymbol() const;
		std::string		getBotSymbol() const;
		std::string		getBoard(int pos) const;
		bool			isGameOver() const;

		/* CORE */
		int		getBestMove();
		int		isValidMove(int pos);
		bool	checkWin(const std::string &symbol);
		bool	checkDraw();
		void	makeMove(int pos, const std::string &symbol);
};

#endif