/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:46:48 by humontas          #+#    #+#             */
/*   Updated: 2026/05/22 21:46:48 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "Includes.hpp"

class User
{
    private:
        int         _userFd;
        std::string _nickName;
        std::string _userName;
        std::string _hostName;
        bool       _authenticated;

    public:
        User(int socketFd);
        ~User();
};

#endif