/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 10:13:12 by humontas          #+#    #+#             */
/*   Updated: 2026/05/23 10:13:12 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// --------------------
//     constructors
// --------------------

User::User(int socketFd) :
_userFd(socketFd),
_buffer(""),
_authenticated(false),
_registered(false) {}

User::~User(void) {}

User::User(const User &other)
{
	*this = other;
}

User	&User::operator=(const User &other)
{
	if (this != &other)
	{
		this->_userFd = other._userFd;
		this->_nickName = other._nickName;
		this->_userName = other._userName;
		this->_authenticated = other._authenticated;
		this->_registered = other._registered;
		this->_buffer = other._buffer;
	}
	return (*this);
}

// --------------------
//       getters
// --------------------

int	User::getFd(void) const
{
	return (this->_userFd);
}

std::string	User::getNickname(void) const
{
	return (this->_nickName);
}

std::string	User::getUsername(void) const
{
	return (this->_userName);
}

bool	User::getAuthenticated(void) const
{
	return (this->_authenticated);
}

bool	User::getRegistered(void) const
{
	return (this->_registered);
}

std::string	User::getBuffer(void) const
{
	return (this->_buffer);
}

// --------------------
//       setters
// --------------------

void	User::setNickname(const std::string &nickname)
{
	this->_nickName = nickname;
}

void	User::setUsername(const std::string &username)
{
	this->_userName = username;
}

void	User::setAuthenticated(bool auth)
{
	this->_authenticated = auth;
}

void	User::setRegistered(bool reg)
{
	this->_registered = reg;
}

void	User::setBuffer(const std::string &buffer)
{
	this->_buffer = buffer;
}