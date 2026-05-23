/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:46:43 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 10:46:43 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"
# include "User.hpp"

class Server
{
    private:
        int                         _serverSocket;
        int                         _serverPort;
        std::string                 _serverPassword;
        std::vector<struct pollfd>  _pollFds;
        std::vector<User>           _users;

        void        createSocket();
        void        bindSocket();
        void        listenSocket();
        void        initPollFds();
    
        void        handleEvents();
        void        acceptClient();
        void        handleClient(int clientFd);
        void        removeClient(int clientFd);
        std::pair<std::string, std::string> parseMessage(const std::string &message);
        void                                handleCommand(int clientFd, const std::string &command, const std::string &args);

        bool    passCommand(int clientFd, const std::string &password);


    public:
        Server(int port, std::string password);
        ~Server();

        void        run();
};

#endif