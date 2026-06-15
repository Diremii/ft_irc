/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:43:11 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 10:43:11 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# define MAX_TOPIC_LEN 307
# define MAX_NICK_LEN 63
# define MAX_USER_LEN 10

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <vector>
# include <cstdlib>
# include <stdexcept>
# include <stack>
# include <fstream>
# include <csignal>
# include <map>
# include <netdb.h>
# include <sstream>

extern int	g_sig;

#endif