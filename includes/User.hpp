/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:46:48 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 21:46:48 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "Includes.hpp"

class User
{
    private:
        int			_userFd;
        std::string	_nickName;
        std::string	_userName;
        std::string	_hostName;
        bool		_authenticated;
		bool		_registered;

        User(void);

    public:
        User(int socketFd);
        ~User();
		User(const User &other);

		User	&operator=(const User &other);

		int			getFd(void) const;
		std::string	getNickname(void) const;
		std::string	getUsername(void) const;
		std::string	getHostname(void) const;
		bool		getAuthenticated(void) const;
		bool		getRegistered(void) const;
		
		void	setFd(int userFd);
		void	setNickname(std::string &nickName);
		void	setUsername(std::string &userName);
		void	setHostname(std::string &hostName);
		void	setAuthenticated(bool auth);
		void	setRegistered(bool reg);
		
};

#endif
