/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:27:57 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/08 14:36:24 by humontas@st      ###   ########.fr       */
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

		std::stack<std::string>	_msgs;

		std::size_t				_userLimit;
		bool					_isInviteOnly;
		bool					_isTopicRestricted;
		int						_channelCreatorFd;

		Channel(void);

		void	removeUser(int clientFd);
		bool	isOperator(int clientFd);

	public:
		Channel(const std::string &name, User &channelCreator);
		Channel(const Channel &other);
		~Channel(void);

		Channel &operator=(const Channel &other);

		/* GETTERS */
		std::stack<std::string>	getMessages(void);
		const std::vector<int>	&getUsers(void) const;
		std::string				getPassword(int clientFd);
		std::string				getName(void);
		std::string				viewTopic(void);

		/* CORE */
		int		addUser(int clientFd);
		bool	kickClient(int clientFd, int targetFd);
		bool	inviteClient(int clientFd, User &target);

		/* CHANNEL SETTINGS */
		bool	changeTopic(int clientFd, const std::string &newTopic);
		bool	changePassword(int clientFd, const std::string &newPassword);
		bool	changeName(int clientFd, const std::string &newName);
		int		removeOperator(int clientFd, int targetFd);
		bool	removeUserLimit(int clientFd);
		bool	setOperator(int clientFd, int targetFd);
		bool	setTopicRestriction(int clientFd, bool isRestricted);
		int		setUserLimit(int clientFd, std::size_t newUserLimit);
		void	storeMessages(const std::string &msg);

		bool	isUserExist(int clientFd);

};

#endif