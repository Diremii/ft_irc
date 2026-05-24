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

# include "User.hpp"

User::User(void) {}

User::~User(void) {}

User::User(int socketFd) :
    _userFd(socketFd),
    _authenticated(false),
	_registered(false) {}

User::User(const User &other)
{
    *this = other;
}

User &User::operator=(const User &other)
{
    if (this != &other)
    {
        this->_userFd = other._userFd;
        this->_nickName = other._nickName;
        this->_userName = other._userName;
        this->_hostName = other._hostName;
        this->_authenticated = other._authenticated;
		this->_registered = other._registered;
    }
    return (*this);
}

int User::getFd(void) const
{
    return (this->_userFd);
}

std::string User::getNickname(void) const
{
    return (this->_nickName);
}

std::string User::getUsername(void) const
{
    return (this->_userName);
}

std::string User::getHostname(void) const
{
    return (this->_hostName);
}

bool User::getAuthenticated(void) const
{
    return (this->_authenticated);
}

bool	User::getRegistered(void) const
{
	return (this->_registered);
}

void User::setFd(int userFd)
{
    this->_userFd = userFd;
}

void User::setNickname(std::string &nickname)
{
    this->_nickName = nickname;
}

void User::setUsername(std::string &username)
{
    this->_userName = username;
}

void User::setHostname(std::string &hostname)
{
    this->_hostName = hostname;
}

void User::setAuthenticated(bool auth)
{
    this->_authenticated = auth;
}

void	User::setRegistered(bool reg)
{
	this->_registered = reg;
}