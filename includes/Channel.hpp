/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttremel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:27:57 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/03 18:37:28 by ttremel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"
# include "User.hpp"
# include "Server.hpp"

class	Channel
{
	private:
		std::stack<std::string>	_msgs;
		std::vector<User>		_users;
		
		Channel(void);
	public:
		std::string	name;

		Channel(const std::string &name);
		Channel(const Channel &other);
		~Channel(void);
	
		Channel	&operator=(const Channel &other);

		std::stack<std::string>	getMessages(void);
		std::vector<User>		getUsers(void);
};

#endif