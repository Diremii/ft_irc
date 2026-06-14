/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 12:12:44 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/14 19:31:58 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TICTACTOE_HPP
# define TICTACTOE_HPP

#include "Includes.hpp"

class TicTacToe
{
    private:
        std::string _gameChannel;
        std::string _currentPlayer;
        time_t      _lastMoveTime;
        time_t      _gameEndTime;
        bool        _gameOver;

        std::string _board[9];
        std::string _botSymbol;
        std::string _playerSymbol;

        void        initGame();
        int         minimax(bool isBot);
        
        public:
        TicTacToe(const std::string &channel, const std::string &player);
        ~TicTacToe();
        
        int         getBestMove();
        bool        checkWin(const std::string &symbol);
        bool        checkDraw();
        void        makeMove(int pos, const std::string &symbol);

        time_t      getLastMoveTime();
        time_t      getGameEndTime();
        std::string getCurrentPlayer();
        std::string getPlayerSymbol();
        std::string getBotSymbol();
        std::string getBoard(int pos);

        bool        isGameOver();
        int        isValidMove(int pos);
};

#endif