/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttremel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:34:40 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/17 14:12:59 by ttremel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// --------------------
//     constructors
// --------------------

Channel::Channel(void) {}

Channel::~Channel(void) {}

Channel::Channel(const std::string &name, User &channelCreator)
{
	this->_name = name;
	this->_users.push_back(channelCreator.getFd());
	this->_operators.push_back(channelCreator.getFd());
	this->_userLimit = (size_t)-1;
	this->_isInviteOnly = false;
	this->_isTopicRestricted = false;
}

Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel	&Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_password = other._password;
		this->_topic = other._topic;
		this->_operators = other._operators;
		this->_users = other._users;
		this->_inviteds = other._inviteds;
		this->_userLimit = other._userLimit;
		this->_isInviteOnly = other._isInviteOnly;
		this->_isTopicRestricted = other._isTopicRestricted;
	}
	return (*this);
}

const std::vector<int>	&Channel::getUsers(void) const
{
	return (_users);
}

// --------------------
//        utils
// --------------------

bool	Channel::isInvited(int clientFd)
{
	for (size_t i = 0; i < _inviteds.size(); i++)
	{
		if (_inviteds[i] == clientFd)
			return (true);
	}
	return (false);
}

bool	Channel::isUserExist(int clientFd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i] == clientFd)
			return (true);
	}
	return (false);
}

bool	Channel::isOperator(int clientFd)
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i] == clientFd)
			return (true);
	}
	return (false);
}

void	Channel::removeUser(int clientFd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i] == clientFd)
		{
			_users.erase(_users.begin() + i);
			return ;
		}
	}
}

void	Channel::removeOperator(int clientFd)
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i] == clientFd)
		{
			_operators.erase(_operators.begin() + i);
			return ;
		}
	}
}

// --------------------
//        users
// --------------------

int	Channel::addUser(int clientFd, const std::string &password)
{
	if (_isInviteOnly && !isInvited(clientFd))
		return (1);
	if (_users.size() >= _userLimit)
		return (2);
	if (!_password.empty() && _password != password)
		return (3);
	if (!isUserExist(clientFd))
		_users.push_back(clientFd);
	return (0);
}

void	Channel::inviteClient(int targetFd)
{
	if (_isInviteOnly && !isInvited(targetFd))
		_inviteds.push_back(targetFd);
}

void	Channel::kickClient(int targetFd)
{
	removeUser(targetFd);
}

// --------------------
//      operators
// --------------------

void	Channel::setOperator(int targetFd, bool activate)
{
	if (activate && !isOperator(targetFd))
		_operators.push_back(targetFd);
	else
	{
		for (size_t i = 0; i < _operators.size(); i++)
		{
			if (_operators[i] == targetFd)
			{
				_operators.erase(_operators.begin() + i);
				return ;
			}
		}
	}
}

// --------------------
//      settings
// --------------------

std::string	Channel::getName(void)
{
	return (_name);
}

std::string	Channel::viewTopic(void)
{
	return (_topic);
}

bool	Channel::changeTopic(int clientFd, const std::string &newTopic)
{
	if (_isTopicRestricted && !isOperator(clientFd))
		return (false);
	_topic = newTopic;
	return (true);
}

void	Channel::changePassword(const std::string &newPassword)
{
	_password = newPassword;
}

void	Channel::setInviteOnly(bool activate)
{
	_isInviteOnly = activate;
}

void	Channel::setTopicRestriction(bool activate)
{
	_isTopicRestricted = activate;
}

void	Channel::setUserLimit(bool activate, size_t newLimit)
{
	if (activate)
		_userLimit = newLimit;
	else
		_userLimit = (size_t)-1;
}
