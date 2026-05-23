/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 10:44:23 by humontas          #+#    #+#             */
/*   Updated: 2026/05/23 10:44:23 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Bool    Server::passCommand(int clientFd, const std::string &password)
{
    return (password == _serverPassword);
}