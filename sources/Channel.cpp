/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:34:40 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/10 12:30:49 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(void) {}

Channel::~Channel(void) {}

Channel::Channel(const std::string &name, User &channelCreator)
{
	this->_name = name;
	this->_users.push_back(channelCreator.getFd());
	this->_operators.push_back(channelCreator.getFd());
	this->_channelCreatorFd = channelCreator.getFd();
	this->_userLimit = (size_t)-1;
	this->_isInviteOnly = false;
	this->_isTopicRestricted = false;
}

Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_password = other._password;
		this->_topic = other._topic;
		this->_operators = other._operators;
		this->_users = other._users;
		this->_inviteds = other._inviteds;
		this->_msgs = other._msgs;
		this->_files = other._files;
		this->_userLimit = other._userLimit;
		this->_isInviteOnly = other._isInviteOnly;
		this->_isTopicRestricted = other._isTopicRestricted;
		this->_channelCreatorFd = other._channelCreatorFd;
	}
	return *this;
}

std::stack<std::string> Channel::getMessages(void)
{
	return _msgs;
}

const std::vector<int> &Channel::getUsers(void) const
{
    return _users;
}

void	Channel::storeFile(const File &file)
{
	_files.push_back(file);
}

File	&Channel::getFile(const std::string &fileName)
{
	for (size_t	i = 0; i < _files.size(); i++)
	{
		if (_files[i].getFileName() == fileName)
			return (_files[i]);
	}
	throw (File::FileNotFound());
}

// --------------------
// utils
// --------------------

bool	Channel::isInvited(int clientFd)
{
	for (size_t i =0; i < _inviteds.size(); i++)
	{
		if (_inviteds[i] == clientFd)
			return (true);
	}
	return (false);
}

bool Channel::isUserExist(int clientFd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i] == clientFd)
			return true;
	}
	return false;
}

bool Channel::isOperator(int clientFd)
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i] == clientFd)
			return true;
	}
	return false;
}

void Channel::removeUser(int clientFd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i] == clientFd)
		{
			_users.erase(_users.begin() + i);
			return;
		}
	}
}

// --------------------
// users
// --------------------

int	Channel::addUser(int clientFd)
{
	if (_isInviteOnly && !isInvited(clientFd))
		return (1);
	if (_users.size() >= _userLimit)
		return (2);
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
// operators
// --------------------

void	Channel::setOperator(int targetFd, bool activate)
{
	if (activate)
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

void	Channel::setInviteOnly(bool activate)
{
	_isInviteOnly = activate;
}

// --------------------
// settings
// --------------------

std::string Channel::getPassword(int clientFd)
{
	if (!isOperator(clientFd))
		return ("");
	return (_password);
}

void	Channel::changePassword(const std::string &newPassword)
{
	_password = newPassword;
}

std::string Channel::getName(void)
{
	return _name;
}

std::string Channel::viewTopic(void)
{
	return _topic;
}

bool Channel::changeTopic(int clientFd, const std::string &newTopic)
{
	if (_isTopicRestricted && !isOperator(clientFd))
		return (false);
	_topic = newTopic;
	return (true);
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

bool Channel::changeName(int clientFd, const std::string &newName)
{
	if (!isOperator(clientFd))
		return (false);
	_name = newName;
	return (true);
}

void Channel::storeMessages(const std::string &msg)
{
	_msgs.push(msg);
}