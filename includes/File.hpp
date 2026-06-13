/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttremel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 15:09:42 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/13 15:33:23 by ttremel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# define FILE_HPP

# include "Includes.hpp"

class File
{
	private:
		std::string	_data;
		std::string	_fileName;
		bool		_isEmpty;

	public:
		File(void);
		~File(void);
		File(const File &other);

		File	&operator=(const File &other);

		std::string	getFileName(void);
		std::string	getData(void);
		bool		createNewFile(const std::string &folderPath);
		bool		open(const std::string &filePath);
		bool		isEmpty(void);
};


#endif