/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotReplies.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 12:39:58 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/15 13:19:30 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTREPLIES_HPP
# define BOTREPLIES_HPP

# include "Includes.hpp"

namespace BotReply
{
	/* TICTACTOE */
	inline std::string gameAlreadyInProgress()
	{
		return (" :A game is already in progress!\r\n");
	}
	inline std::string noGame()
	{
		return (" :There is no game in progress!\r\n");
	}
	inline std::string notYourTurn()
	{
		return (" :It's not your turn to play!\r\n");
	}
	inline std::string invalidPosition()
	{
		return (" :The position can only be from 1 to 9!\r\n");
	}
	inline std::string positionTaken()
	{
		return (" :This position has already been played!\r\n");
	}
	inline std::string youPlayed(const std::string &pos)
	{
		return (" :You played " + pos + "!\r\n");
	}
	inline std::string botPlayed(const std::string &pos)
	{
		return (" :I played " + pos + "!\r\n");
	}
	inline std::string botThinks()
	{
		return (" :Hmm... let me think.\r\n");
	}
	inline std::string botGoesFirst()
	{
		return (" :I'll go first. Good luck.\r\n");
	}
	inline std::string playerSymbol(const std::string &symbol)
	{
		return (" :You are " + symbol + " - use !move <1-9>\r\n");
	}
	inline std::string playerWins(const std::string &nick)
	{
		return (" :Well done! " + nick + " Your brain is way too big!\r\n");
	}
	inline std::string botWins()
	{
		return (" :I won, not surprising.\r\n");
	}
	inline std::string draw()
	{
		return (" :A draw! A rather... boring result.\r\n");
	}
}

#endif