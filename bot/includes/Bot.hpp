/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 01:53:59 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/14 19:30:57 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "Includes.hpp"
# include "TicTacToe.hpp"

class Bot
{
    private:
        int                         _userFd;
        std::string                 _nickName;
        std::string                 _userName;
        std::string                 _buffer;
        std::vector<std::string>    _channels;

        std::map<std::string, TicTacToe*> _games;

        void    createSocket();
        void    connectToServer(const std::string &hostname, int port);
        void    registerBot(const std::string &password);

        void    handleCommand(const std::string &line, const std::string &command, const std::string &args);
        void    handleBot();
        void    checkTimers();
        void    sendBoard(const std::string &channel, TicTacToe *game);

        std::pair<std::string, std::string> parseMessage(const std::string &message);
        std::vector<std::string>            splitArgs(const std::string &args);
        std::string                         generateNick();
        std::string                         getNickFromPrefix(const std::string &line);
        void                                sendMessage(const std::string &message);
        void                                handlePRIVMSG(const std::string &nick, const std::string &args);
    
    public:
        Bot(const std::string &hostname, int port, const std::string &password);
        ~Bot();

        void    run();
};

#endif