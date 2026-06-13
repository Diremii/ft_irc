/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttremel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 15:33:17 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/13 15:45:47 by ttremel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Includes.hpp"
#include "../includes/File.hpp"

File::File(void) : _data(""), _fileName(""), _isEmpty(true) {}
File::~File(void) {}

bool	File::open(const std::string &filePath)
{
	std::ifstream	input;
	std::string		line;
	std::size_t		fileNamePos;
	
	input.open(filePath.c_str(), std::ios::in);
	if (!input.is_open())
		return (true);
	this->_fileName = filePath;
	if ((fileNamePos = filePath.find_last_of("/")) != std::string::npos)
		this->_fileName = filePath.substr(fileNamePos + 1);
	while (getline(input, line))
		this->_data += line;
	input.close();
	return (false);
}

File::File(const File &other)
{
	*this = other;
}

File	&File::operator=(const File &other)
{
	if (this != &other)
	{
		this->_data = other._data;
		this->_fileName = other._fileName;
	}
	return (*this);
}

std::string	File::getData(void)
{
	return (this->_data);
}

std::string	File::getFileName(void)
{
	return (this->_fileName);
}

bool	File::isEmpty(void)
{
	return (_isEmpty);
}

bool	File::createNewFile(const std::string &folderPath)
{
	std::ofstream	output;
	std::string		fullPath;

	if (*folderPath.end() == '/')
		fullPath = folderPath + this->_fileName;
	else
		fullPath = folderPath + '/' + this->_fileName;
	output.open(fullPath.c_str(), std::ios::out | std::ios::trunc);
	if (!output.is_open())
		return (true);
	output << this->_data;
	output.close();
	return (false);
}
