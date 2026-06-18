#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "Includes.hpp"

namespace IrcReply
{
	/* SUCCESS */
	inline std::string nick(const std::string &oldNick, const std::string &user, const std::string &newNick)
	{	
		return (":" + oldNick + "!" + user + "@localhost NICK " + newNick + "\r\n");
	}
	inline std::string join(const std::string &nick, const std::string &user, const std::string &channel)
	{
		return (":" + nick + "!" + user + "@localhost JOIN " + channel + "\r\n");
	}
	inline std::string quit(const std::string &nick, const std::string &user, const std::string &reason)
	{
		return (":" + nick + "!" + user + "@localhost QUIT :" + reason + "\r\n");
	}
	inline std::string kick(const std::string &nick, const std::string &user, const std::string &channel, const std::string &target, const std::string &reason)
	{
		return (":" + nick + "!" + user + "@localhost KICK " + channel + " " + target + " :" + reason + "\r\n");
	}
	inline std::string invite(const std::string &nick, const std::string &user, const std::string &target, const std::string &channel)
	{
		return (":" + nick + "!" + user + "@localhost INVITE " + target + " " + channel + "\r\n");
	}
	inline std::string topicChanged(const std::string &nick, const std::string &user, const std::string &channel, const std::string &topic)
	{
		return (":" + nick + "!" + user + "@localhost TOPIC " + channel + " :" + topic + "\r\n");
	}
	inline std::string privmsg(const std::string &nick, const std::string &user, const std::string &target, const std::string &message)
	{
		return (":" + nick + "!" + user + "@localhost PRIVMSG " + target + " :" + message + "\r\n");
	}
	inline std::string part(const std::string &nick, const std::string &user, const std::string &channel, const std::string &reason)
	{
		return (":" + nick + "!" + user + "@localhost PART " + channel + " :" + reason + "\r\n");
	}
	inline std::string dccSend(const std::string &nick, const std::string &user, const std::string &target, const std::string &fileName, const std::string &ip, const std::string &port, const std::string &fileSize)
	{
		std::string ctrl(1, '\x01');
		return (":" + nick + "!" + user + "@localhost PRIVMSG " + target + " :" + ctrl + "DCC SEND " + fileName + " " + ip + " " + port + " " + fileSize + ctrl + "\r\n");
	}
	inline std::string mode(const std::string &nick, const std::string &user, const std::string &channel, const std::string &flag, const std::string &target)
	{
		return (":" + nick + "!" + user + "@localhost MODE " + channel + " " + flag + " " + target + "\r\n");
	}
	inline std::string welcome(const std::string &nick, const std::string &user)
	{
		return (":server 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@localhost\r\n");
	}
	inline std::string noTopic(const std::string &nick, const std::string &channel)
	{
		return (":server 331 " + nick + " " + channel + " :No topic is set\r\n");
	}
	inline std::string topic(const std::string &nick, const std::string &channel, const std::string &topic)
	{
		return (":server 332 " + nick + " " + channel + " :" + topic + "\r\n");
	}
	inline std::string namesList(const std::string &nick, const std::string &channel, const std::string &users)
	{
		return (":server 353 " + nick + " = " + channel + " :" + users + "\r\n");
	}
	inline std::string endOfNames(const std::string &nick, const std::string &channel)
	{
		return (":server 366 " + nick + " " + channel + " :End of /NAMES list\r\n");
	}

	/* ERRORS */
	inline std::string noSuchNick(const std::string &nick, const std::string &target)
	{
		return (":server 401 " + nick + " " + target + " :No such nick/channel\r\n");
	}
	inline std::string noSuchChannel(const std::string &nick, const std::string &channel)
	{
		return (":server 403 " + nick + " " + channel + " :No such channel\r\n");
	}
	inline std::string topicTooLong(const std::string &nick, const std::string &channel)
	{
		return (":server 416 " + nick + " " + channel + " :Topic too long\r\n");
	}
	inline std::string unknownCommand(const std::string &nick, const std::string &command)
	{
		return (":server 421 " + nick + " " + command + " :Unknown command\r\n");
	}
	inline std::string noNick()
	{
		return (":server 431 * :No nickname given\r\n");
	}
	inline std::string badNick(const std::string &nick)
	{
		return (":server 432 * " + nick + " :Erroneous nickname\r\n");
	}
	inline std::string nickInUse(const std::string &nick)
	{
		return (":server 433 * " + nick + " :Nickname is already in use\r\n");
	}
	inline std::string notOnChannel(const std::string &nick, const std::string &channel)
	{
		return (":server 442 " + nick + " " + channel + " :You're not on that channel\r\n");
	}
	inline std::string notRegistered()
	{
		return (":server 451 * :You have not registered\r\n");
	}
	inline std::string notEnoughParams(const std::string &cmd)
	{
		return (":server 461 * " + cmd + " :Not enough parameters\r\n");
	}
	inline std::string badUser(const std::string &user)
	{
		return (":server 461 * " + user + " :Erroneous username\r\n");
	}
	inline std::string alreadyRegistered()
	{
		return (":server 462 * :You may not reregister\r\n");
	}
	inline std::string notAuthenticated()
	{
		return (":server 464 * :Password incorrect\r\n");
	}
	inline std::string channelIsFull(const std::string &nick, const std::string &channel)
	{
		return (":server 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n");
	}
	inline std::string unknownMode(const std::string &nick, const std::string &mode)
	{
		return (":server 472 " + nick + " " + mode + " :is unknown mode char to me\r\n");
	}
	inline std::string inviteOnlyChannel(const std::string &nick, const std::string &channel)
	{
		return (":server 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n");
	}
	inline std::string badChannelKey(const std::string &nick, const std::string &channel)
	{
	return (":server 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n");
	}
	inline std::string badChannelMask(const std::string &nick, const std::string &channel)
	{
		return (":server 476 " + nick + " " + channel + " :Bad Channel Mask\r\n");
	}
	inline std::string chanOpPrivsNeeded(const std::string &nick, const std::string &channel)
	{
		return (":server 482 " + nick + " " + channel + " :You're not channel operator\r\n");
	}
}
#endif