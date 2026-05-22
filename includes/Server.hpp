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

class Server
{
    private:
        int                         _serverSocket;
        int                         _serverPort;
        std::string                 _serverPassword;
        std::vector<struct pollfd>  _pollFds;

        void        createSocket();
        void        bindSocket();
        void        listenSocket();
        void        initPollFds();
    
        void        handleEvents();
        void        acceptClient();
        void        handleClient(int clientFd);
        void        removeClient(int clientFd);

    public:
        Server(int port, std::string password);
        ~Server();

        void        run();
};

#endif