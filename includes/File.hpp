/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas@student.42.fr <humontas>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 15:09:42 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/08 14:51:28 by humontas@st      ###   ########.fr       */
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

		File(void);
	public:
		File(const std::string &file_path);
		~File(void);
		File(const File &other);

		File	&operator=(const File &other);

		std::string	getFileName(void);
		std::string	getData(void);
		void		createNewFile(const std::string &folderPath);

		class	FileNotFound : public std::exception
		{
			public:
				const char *what() const throw() { return ("File not found"); }
		};
		class	CantCreateFile : public std::exception
		{
			public:
				const char *what() const throw() { return ("Can't create file"); }
		};
};


#endif