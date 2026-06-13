/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 11:10:17 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/13 12:01:33 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

std::vector<std::string> Bot::splitArgs(const std::string &args)
{
	std::vector<std::string> result;
	std::string word;
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
			word += args[i];
	}
	if (!word.empty())
		result.push_back(word);
	return (result);
}

std::pair<std::string, std::string> Bot::parseMessage(const std::string &message)
{
    std::string line = message;
    if (line[0] == ':')
    {
        size_t space = line.find(' ');
        if (space == std::string::npos)
            return std::make_pair(line, "");
        line = line.substr(space + 1);
    }
    size_t space = line.find(' ');
    if (space == std::string::npos)
        return std::make_pair(line, "");
    return std::make_pair(line.substr(0, space), line.substr(space + 1));
}

void	Bot::sendMessage(const std::string &message)
{
	send(_userFd, message.c_str(), message.size(), 0);
}

std::string Bot::generateNick()
{
    static int                 suffix = 0;
    static int                 underscores = 0;

    suffix++;
    if (suffix > 9)
    {
        suffix = 0;
        underscores++;
    }

    std::string nick = "ShallowRed";
    if (suffix > 0)
    {
        std::stringstream   ss;
        ss << suffix;
        nick += ss.str();
    }
    for (int i = 0; i < underscores; i++)
        nick+= "_";

    return (nick);
}
