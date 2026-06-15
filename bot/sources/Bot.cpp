/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 02:16:22 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/15 13:03:55 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void    Bot::createSocket()
{
    _userFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_userFd == -1)
        throw std::runtime_error("Failed to create socket");
}

void    Bot::connectToServer(const std::string &hostname, int port)
{
    struct hostent  *host = gethostbyname(hostname.c_str());
    if (!host)
        throw std::runtime_error("Failed to resolve hostname");
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr, host->h_addr, host->h_length);

    if (connect(_userFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw std::runtime_error("Failed to connect to server");
}

void    Bot::registerBot(const std::string &password)
{
    std::string out = "PASS " + password + "\r\n";
                out+= "NICK " + _nickName + "\r\n";
                out+= "USER " + _userName + " 0 * :" + _userName + "\r\n";
    if (send(_userFd, out.c_str(), out.length(), 0) == -1)
        throw std::runtime_error("Failed to register bot");
}

void    Bot::handleCommand(const std::string &line, const std::string &command, const std::string &args)
{
    if (command == "433")
    {
        _nickName = generateNick();
        sendMessage("NICK " + _nickName + "\r\n");
    }
    else if (command == "001")
        sendMessage("JOIN #general\r\n");
    else if (command == "INVITE")
    {
        std::vector<std::string> params = splitArgs(args);
        if (params.size() >= 2)
            sendMessage("JOIN " + params[1] + "\r\n");
    }
    else if (command == "PRIVMSG")
        handlePRIVMSG(getNickFromPrefix(line), args);
}

void    Bot::handleBot()
{
    size_t pos = _buffer.find("\r\n");
    while (pos != std::string::npos)
    {
        std::string line = _buffer.substr(0, pos);
        _buffer = _buffer.substr(pos + 2);
        pos = _buffer.find("\r\n");
        std::pair<std::string, std::string> parsed = parseMessage(line);
        std::string command = parsed.first;
        std::string args = parsed.second;
        handleCommand(line, command, args);
        std::cout << "Command: " << command << " | Args: " << args << std::endl;
    }
}

void    Bot::checkTimers()
{
    std::map<std::string, TicTacToe*>::iterator it;

    for (it = _games.begin(); it != _games.end();)
    {
        if (time(NULL) - it->second->_lastMoveTime > 60)
        {
            sendMessage("PRIVMSG " + it->first + " :Game over! You took too long to play.\r\n");
            delete it->second;
            _games.erase(it++);
        }
        else
            it++;
    }
}

void    Bot::run()
{
    struct pollfd pfd;
    pfd.fd = _userFd;
    pfd.events = POLLIN;
    
    char buffer[1024];
    while (true)
    {
        pfd.revents = 0;
        int activity = poll(&pfd, 1, 1000);
        if (activity > 0 && pfd.revents & POLLIN)
        {
            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(_userFd, buffer, sizeof(buffer), 0);
            if (bytes <= 0)
                break;
            _buffer += std::string(buffer, bytes);
            handleBot();
        }
        checkTimers();
    }
}

Bot::Bot(const std::string &hostname, int port, const std::string &password) : 
    _nickName("ShallowRed"), 
    _userName("ShallowRed")
{
    createSocket();
    connectToServer(hostname, port);
    registerBot(password);
}

Bot::~Bot()
{
    close(_userFd);
}