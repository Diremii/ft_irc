/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:53:50 by humontas@st       #+#    #+#             */
/*   Updated: 2026/06/16 22:08:36 by humontas@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int	g_sig = 0;

void	signalHandler(int sig)
{
	g_sig = sig;
}

int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <hostname> <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		Bot	bot(argv[1], atoi(argv[2]), argv[3]);
		std::signal(SIGQUIT, signalHandler);
		std::signal(SIGINT, signalHandler);
		std::signal(SIGPIPE, signalHandler);
		std::cout << "Bot started on port " << argv[2] << std::endl;
		bot.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
}