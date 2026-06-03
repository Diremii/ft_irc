/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttremel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:34:40 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/03 18:37:55 by ttremel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Channel.hpp"

Channel::Channel(void) {}
Channel::~Channel(void) {}

Channel::Channel(const std::string &name)
{
	this->name = name;
}

Channel::Channel(const Channel &other)
{
	*this = other;
}


Channel	&Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		this->name = other.name;
		this->_users = other._users;
		this->_msgs = other._msgs;
	}
	return (*this);
}

std::stack<std::string>	Channel::getMessages(void)
{
	return (this->_msgs);
}

std::vector<User>	Channel::getUsers(void)
{
	return (this->_users);
}
