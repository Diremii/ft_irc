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

# include "../includes/User.hpp"

User::User(void) {}
User::~User(void) {}

User::User(int socketFd) :
    _userFd(socketFd),
    _authenticated(false) {}

User::User(const User &other)
{
	*this = other;
}

User	&User::operator=(const User &other)
{
	if (this != &other)
	{
		this->_userFd = other._userFd;
		this->_hostName = other._hostName;
		this->_userName = other._userName;
		this->_authenticated = other._authenticated;
		this->_nickName = other._nickName;
	}
	return (*this);
}

// ---------------------------------------------------------------------//
/*                                                                      */
/*                               GETTER                                 */
/*                                                                      */
// ---------------------------------------------------------------------//

int	User::get_user_fd(void) const
{
	return (this->_userFd);
}

std::string	User::get_nick_name(void) const
{
	return (this->_nickName);
}

std::string	User::get_user_name(void) const
{
	return (this->_userName);
}

std::string	User::get_host_name(void) const
{
	return (this->_hostName);
}

bool	User::get_authenticated(void) const
{
	return (this->_authenticated);
}

// ---------------------------------------------------------------------//
/*                                                                      */
/*                               SETTER                                 */
/*                                                                      */
// ---------------------------------------------------------------------//

void	User::set_user_fd(int user_fd)
{
	this->_userFd = user_fd;
}

void	User::set_nick_name(std::string &nick_name)
{
	this->_nickName = nick_name;
}

void	User::set_user_name(std::string &user_name)
{
	this->_userName = user_name;
}

void	User::set_host_name(std::string &host_name)
{
	this->_hostName = host_name;
}

void	User::set_authenticated(bool auth)
{
	this->_authenticated = auth;
}
