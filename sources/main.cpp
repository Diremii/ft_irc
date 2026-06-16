/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 12:04:59 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 12:04:59 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"
#include "Server.hpp"

int	g_sig = 0;

void	signalHandler(int sig)
{
	g_sig = sig;
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		Server server(atoi(argv[1]), argv[2]);
		std::signal(SIGQUIT, signalHandler);
		std::signal(SIGINT, signalHandler);
		std::signal(SIGPIPE, signalHandler);
		std::cout << "Server started on port " << argv[1] << std::endl;
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}