/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:00:33 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 11:00:33 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Creation du socket, fonctionne comme un fd, IPv4/TCP, protocole auto
void    Server::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
}

void    Server::bindSocket()
{
    // Remplissage de la struct d'adresse
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;                                    // IPv4
    addr.sin_port = htons(_serverPort);                           // Port en network byte order
    addr.sin_addr.s_addr = INADDR_ANY;                            // Ecoute sur toutes les interfaces (0.0.0.0)

    // Attache le socket au port et a l'IP definis dans addr
    if (bind(_serverSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw std::runtime_error("Failed to bind socket");
}

void    Server::listenSocket()
{
    // Met le socket en attente de connexions, backlog de 10
    if (listen(_serverSocket, 10) == -1)
        throw std::runtime_error("Failed to listen on socket");
}

void    Server::initPollFds()
{
    struct pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN; // On veut être notifié des événements de lecture (nouvelles connexions)
    serverPollFd.revents = 0;
    _pollFds.push_back(serverPollFd);
}

void    Server::acceptClient()
{
    int clientSocket = accept(_serverSocket, NULL, NULL);
    if (clientSocket == -1)
        throw std::runtime_error("accept() failed");

    struct pollfd clientPollFd;
    clientPollFd.fd = clientSocket;
    clientPollFd.events = POLLIN; // On veut être notifié des événements de lecture (données du client)
    clientPollFd.revents = 0;
    _pollFds.push_back(clientPollFd);
    _users.push_back(User(clientSocket));
}

void    Server::removeClient(int clientFd)
{
    close(clientFd);
    for (size_t i = 0; i < _pollFds.size(); i++)
    {
        if (_pollFds[i].fd == clientFd)
        {
            _pollFds.erase(_pollFds.begin() + i);
            break ;
        }
    }

    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].getFd() == clientFd)
        {
            _users.erase(_users.begin() + i);
            break ;
        }
    }
}

std::pair<std::string, std::string> Server::parseMessage(const std::string &message)
{
    size_t space = message.find(' ');
    if (space == std::string::npos)
        return (std::make_pair(message, ""));
    std::string command = message.substr(0, space);
    std::string args = message.substr(space + 1);
    return (std::make_pair(command, args));
}

void    Server::handleCommand(int clientFd, const std::string &command, const std::string &args)
{
    if (command == "PASS")
        passCommand(clientFd, args);
    else if (command == "NICK")
        nickCommand(clientFd, args);
    else if (command == "USER")
        userCommand(clientFd, args);

    User    &user = getUser(clientFd);
    if (!user.getRegistered() && user.getAuthenticated() && !user.getNickname().empty() && !user.getUsername().empty())
    {
        user.setRegistered(true);
        sendMessage(clientFd, ":server 001 " + user.getNickname() + " :Welcome to the Internet Relay Network " + user.getNickname() + "!" + user.getUsername() + "@localhost\r\n");
    }
}

void    Server::handleClient(int clientFd)
{
    char buffer[1024];
    int bytes = recv(clientFd, buffer, sizeof(buffer), 0); // Lecture des données envoyées par le client
    if (bytes <= 0)
        removeClient(clientFd);
    else
    {
        std::string message(buffer, bytes);
        message.erase(message.find_last_not_of("\r\n") + 1);
        std::pair<std::string, std::string> parsed = parseMessage(message);
        std::string command = parsed.first;
        std::string args = parsed.second;
        handleCommand(clientFd, command, args);
    }
}

void    Server::handleEvents()
{
    for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        if (_pollFds[i].revents & POLLIN)
        {
            if (_pollFds[i].fd == _serverSocket)
                acceptClient();
            else
                handleClient(_pollFds[i].fd);
        }
    }
}

void    Server::run()
{
    while (true)
    {
        int activity = poll(_pollFds.data(), _pollFds.size(), -1); // -1 pour attendre indéfiniment
        if (activity == -1)
            throw std::runtime_error("Poll error");
        handleEvents();
    }
}

Server::Server(int port, std::string password) :
    _serverPort(port),
    _serverPassword(password)
{
    createSocket();
    bindSocket();
    listenSocket();
    initPollFds();
}

Server::~Server()
{
    close(_serverSocket);
}