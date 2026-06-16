/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 01:50:31 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/16 02:06:04 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

// --------------------
//      parsing
// --------------------

std::vector<std::string>	Bot::splitArgs(const std::string &args)
{
	std::vector<std::string>	result;
	std::string					word;

	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i] == ':')
		{
			result.push_back(args.substr(i + 1));
			return (result);
		}

		if (args[i] == ' ')
		{
			if (!word.empty())
			{
				result.push_back(word);
				word.clear();
			}
		}
		else
		{
			word += args[i];
		}
	}

	if (!word.empty())
		result.push_back(word);

	return (result);
}

std::pair<std::string, std::string>	Bot::parseMessage(const std::string &message)
{
	std::string	line = message;

	if (line[0] == ':')
	{
		size_t space = line.find(' ');
		if (space == std::string::npos)
			return (std::make_pair(line, ""));
		line = line.substr(space + 1);
	}

	size_t	space = line.find(' ');
	if (space == std::string::npos)
		return (std::make_pair(line, ""));

	return (std::make_pair(line.substr(0, space), line.substr(space + 1)));
}

// --------------------
//       network
// --------------------

void Bot::sendMessage(const std::string &message)
{
	send(_userFd, message.c_str(), message.size(), 0);
}

// --------------------
//     nickname gen
// --------------------

std::string	Bot::generateNick()
{
	static int	suffix = 0;
	static int	underscores = 0;

	suffix++;

	if (suffix > 9)
	{
		suffix = 0;
		underscores++;
	}

	std::string	nick = "ShallowRed";

	if (suffix > 0)
	{
		std::stringstream ss;
		ss << suffix;
		nick += ss.str();
	}

	for (int i = 0; i < underscores; i++)
		nick += "_";

	return (nick);
}

// --------------------
//       helpers
// --------------------

std::string	Bot::getNickFromPrefix(const std::string &line)
{
	if (line.empty() || line[0] != ':')
		return ("");

	size_t end = line.find('!');

	if (end == std::string::npos)
		return ("");

	return (line.substr(1, end - 1));
}

// --------------------
//        game
// --------------------

void	Bot::endGame(const std::string &channel, const std::string &message)
{
	sendMessage("PRIVMSG " + channel + message + "\r\n");
	delete _games[channel];
	_games.erase(channel);
}

void	Bot::sendBoard(const std::string &channel, TicTacToe *game)
{
	sendMessage("PRIVMSG " + channel + " : " +
		game->getBoard(0) + " | " + game->getBoard(1) + " | " + game->getBoard(2) + "\r\n");

	sendMessage("PRIVMSG " + channel + " :---+---+---\r\n");

	sendMessage("PRIVMSG " + channel + " : " +
		game->getBoard(3) + " | " + game->getBoard(4) + " | " + game->getBoard(5) + "\r\n");

	sendMessage("PRIVMSG " + channel + " :---+---+---\r\n");

	sendMessage("PRIVMSG " + channel + " : " +
		game->getBoard(6) + " | " + game->getBoard(7) + " | " + game->getBoard(8) + "\r\n");
}

bool	Bot::applyMove(const std::string &channel, int position, const std::string &symbol, const std::string &display, const std::string &winMsg)
{
	_games[channel]->makeMove(position, symbol);
	sendMessage("PRIVMSG " + channel + display);
	sendBoard(channel, _games[channel]);
	if (_games[channel]->checkWin(symbol))
		return (endGame(channel, winMsg), true);
	if (_games[channel]->checkDraw())
		return (endGame(channel, BotReply::draw()), true);
	return (false);
}