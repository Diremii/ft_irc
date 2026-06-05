/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttremel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:27:57 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/05 18:05:01 by ttremel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"
# include "User.hpp"
# include "File.hpp"

class	Channel
{
	private:
		std::string				_name;
		std::string				_password;
		std::string				_topic;
		std::vector<User>		_operators;
		std::vector<User>		_users;
		std::vector<File>		_files;
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
	
		Channel	&operator=(const Channel &other);

		std::stack<std::string>	getMessages(void);
		std::vector<User>		getUsers(void);
		std::vector<File>		getFiles(void);
		std::string				getPassword(int clientFd);
		std::string				getName(void);
		std::string				viewTopic(void);
		File					&getFile(const std::string &fileName);
		User					&getUser(int clientFd);
		void					addUser(User &newUser);
		void					kickClient(int clientFd, int targetFd);
		void					inviteClient(int clientFd, User &targetFd);
		void					changeTopic(int clientFd, const std::string &newTopic);
		void					setTopicRestriction(int clientFd, bool isRestricted);
		void					giveOperator(int clientFd, int targetFd);
		void					removeOperator(int clientFd, int targetFd);
		void					changePassword(int clientFd, const std::string &newPassword);
		void					changeName(int clientFd, const std::string &newName);
		void					setUserLimit(int clientFd, std::size_t newUserLimit);
		void					removeUserLimit(int clientFd);
		void					storeMessages(const std::string &msg);
		void					storeFile(const File &file);
		bool					isUserExist(int clientFd);

		class	NotAnOperator : public std::exception
		{
			public:
				const char	*what() const throw() { return ("Client is not a channel operator"); }
		};
		class	InvalidLimit : public std::exception
		{
			public:
				const char	*what() const throw() { return ("Invalid user limit"); }
		};
		class	UserLimitReach : public std::exception
		{
			public:
				const char	*what() const throw() { return ("User limit reached"); }
		};
		class	IsInviteOnly : public std::exception
		{
			public:
				const char	*what() const throw() { return ("Channel is invite only"); }
		};
};

#endif