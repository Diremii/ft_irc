/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttremel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:34:40 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/04 18:49:47 by ttremel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Channel.hpp"

Channel::Channel(void) {}
Channel::~Channel(void) {}

Channel::Channel(const std::string &name, const std::string &topic, User &channelCreator)
{
	this->_name = name;
	this->_topic = topic;
	this->_users.push_back(channelCreator);
	this->_operators.push_back(channelCreator);
	this->_channelCreatorFd = channelCreator.getFd();
	this->_limit = (size_t)-1;
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
		this->_users = other._users;
		this->_msgs = other._msgs;
		this->_operators = other._operators;
		this->_topic = other._topic;
		this->_isInviteOnly = other._isInviteOnly;
		this->_limit = other._limit;
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

User	&Channel::getUser(int clientFd)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].getFd() == clientFd)
            return (_users[i]);
    }
    throw std::runtime_error("User not found");
}

// ----------------//
//      Utils      //
// ----------------//

bool	Channel::_isOperator(int clientFd)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i].getFd() == clientFd)
            return (true);
    }
	return (false);
}

void	Channel::_removeUser(int clientFd)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].getFd() == clientFd)
			_users.erase(_users.begin() + i);
    }
}

bool	Channel::_isUserExist(int clientFd)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].getFd() == clientFd)
            return (true);
    }
	return (false);
}

void	Channel::storeMessages(const std::string &msg)
{
	_msgs.push(msg);
}

// ----------------//
//     Operator    //
// ----------------//

void	Channel::giveOperator(int clientFd, int clientFdToGive)
{
	if 	(!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	_operators.push_back(getUser(clientFdToGive));
}

void	Channel::removeOperator(int clientFd, int clientFdToRemove)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i].getFd() == clientFdToRemove)
			_operators.erase(_operators.begin() + i);
    }
}


// ----------------//
//   Users/Client  //
// ----------------//

void	Channel::addUser(User &newUser)
{
	if (_isInviteOnly)
		throw (Channel::IsInviteOnly());
	if (_limit == _users.size())
		throw (Channel::UserLimitReach());
	if (!_isUserExist(newUser.getFd()))
		_users.push_back(newUser);
}

void	Channel::inviteClient(int clientFd, User &userToInvite)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	if (_limit == _users.size())
		throw (Channel::UserLimitReach());
	if (!_isUserExist(userToInvite.getFd()))
		_users.push_back(userToInvite);
}

void	Channel::kickClient(int clientFd, int clientFdToKick)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	_removeUser(clientFdToKick);
}

void	Channel::removeUserLimit(int clientFd)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	_limit = -1;
}

void	Channel::setUserLimit(int clientFd, std::size_t newUserLimit)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	if (newUserLimit < 2 || newUserLimit < _users.size())
		throw (Channel::InvalidLimit());
	_limit = newUserLimit;
}

// ----------------//
//      Topic      //
// ----------------//

std::string	Channel::viewTopic(void)
{
	return (_topic);
}

void	Channel::changeTopic(int clientFd, const std::string &newTopic)
{
	if (_isTopicRestricted && !_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	_topic = newTopic;
}

void	Channel::setTopicRestriction(int clientFd, bool isRestricted)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	_isTopicRestricted = isRestricted;
}

// ----------------//
//     Password    //
// ----------------//

std::string	Channel::getPassword(int clientFd)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	return (_password);
}

void	Channel::changePassword(int clientFd, const std::string &newPassword)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	_password = newPassword;
}

// ----------------//
//       Name      //
// ----------------//

std::string	Channel::getName(void)
{
	return (_name);
}

void	Channel::changeName(int clientFd, const std::string &newName)
{
	if (!_isOperator(clientFd))
		throw (Channel::NotAnOperator());
	_name = newName;
}

