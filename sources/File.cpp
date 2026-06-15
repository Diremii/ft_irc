/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: humontas <humontas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 15:33:17 by ttremel           #+#    #+#             */
/*   Updated: 2026/06/15 14:44:05 by humontas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"
#include "File.hpp"

File::File(void) {}
File::~File(void) {}

File::File(const std::string &filePath)
{
	std::ifstream	input;
	std::string		line;
	std::size_t		fileNamePos;
	
	input.open(filePath.c_str(), std::ios::in);
	if (!input.is_open())
		throw (File::FileNotFound());
	this->_fileName = filePath;
	if ((fileNamePos = filePath.find_last_of("/")) != std::string::npos)
		this->_fileName = filePath.substr(fileNamePos + 1);
	while (getline(input, line))
		this->_data += line;
	input.close();
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

void	File::createNewFile(const std::string &folderPath)
{
	std::ofstream	output;
	std::string		fullPath;

	if (*folderPath.end() == '/')
		fullPath = folderPath + this->_fileName;
	else
		fullPath = folderPath + '/' + this->_fileName;
	output.open(fullPath.c_str(), std::ios::out | std::ios::trunc);
	if (!output.is_open())
		throw (File::CantCreateFile());
	output << this->_data;
	output.close();
}
