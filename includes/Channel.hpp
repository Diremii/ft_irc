/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:27:57 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/16 23:06:37 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"
# include "User.hpp"

class Channel
{
	private:
		std::string				_name;
		std::string				_password;
		std::string				_topic;

		std::vector<int>		_operators;
		std::vector<int>		_users;
		std::vector<int>		_inviteds;

		std::size_t				_userLimit;
		bool					_isInviteOnly;
		bool					_isTopicRestricted;
		
		Channel(void);
		
	public:
		Channel(const std::string &name, User &channelCreator);
		Channel(const Channel &other);
		~Channel(void);

		Channel &operator=(const Channel &other);

		/* GETTERS */
		const std::vector<int>	&getUsers(void) const;
		std::string				getName(void);
		std::string				viewTopic(void);

		/* CORE */
		int		addUser(int clientFd, const std::string &password);
		void	kickClient(int targetFd);
		void	inviteClient(int targetFd);
		void	removeUser(int clientFd);
		void	removeOperator(int clientFd);

		/* CHANNEL SETTINGS */
		bool	changeTopic(int clientFd, const std::string &newTopic);
		void	changePassword(const std::string &newPassword);
		void	setOperator(int targetFd, bool activate);
		void	setTopicRestriction(bool activate);
		void	setInviteOnly(bool activate);
		void	setUserLimit(bool activate, size_t newLimit);
		bool	isInvited(int clientFd);
		bool	isUserExist(int clientFd);
		bool	isOperator(int clientFd);
};

#endif