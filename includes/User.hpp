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

# include "./Includes.hpp"

class User
{
    private:
        int			_userFd;
        std::string	_nickName;
        std::string	_userName;
        std::string	_hostName;
        bool		_authenticated;

        User(void);
    public:
        User(int socketFd);
        ~User();
		User(const User &other);

		User	&operator=(const User &other);

		int			get_user_fd(void) const;
		std::string	get_nick_name(void) const;
		std::string	get_user_name(void) const;
		std::string	get_host_name(void) const;
		bool		get_authenticated(void) const;
		
		void	set_user_fd(int user_fd);
		void	set_nick_name(std::string &nick_name);
		void	set_user_name(std::string &user_name);
		void	set_host_name(std::string &host_name);
		void	set_authenticated(bool auth);
		
};

#endif