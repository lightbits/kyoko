#ifndef FILE_IO_H
#define FILE_IO_H
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

std::string get_filename(const std::string &path);
bool read_file(const std::string &path, std::string &dest);
int read_int(std::stringstream &ss);
bool read_bool(std::string &ss);
std::string read_word(std::stringstream &ss);

template <typename T>
std::vector<T> split(const std::string &str, std::size_t begin = 0, std::size_t end = std::string::npos)
{
	std::vector<T> result;
	std::size_t i = begin;
	while (i < str.length() && i < end)
	{
		while (i < str.length() && i < end && (str[i] == ' ' || str[i] == '\r' || str[i] == '\n'))
			i++;
		std::size_t j = i;
		while (j < str.length() && j < end && !(str[j] == ' ' || str[j] == '\r' || str[j] == '\n'))
			j++;
		if (i == j)
			break;
		std::stringstream ss(str.substr(i, j - i));
		T sub; ss >> sub;
		result.push_back(sub);
		i = j;
	}
	return result;
}

template <typename T>
std::vector<T> split(const std::string &str, char delimiter, std::size_t begin = 0, std::size_t end = std::string::npos)
{
	std::vector<T> result;
	std::size_t i = begin;
	while (i < str.length() && i < end)
	{
		while (i < str.length() && i < end && str[i] == delimiter)
			i++;
		std::size_t j = i;
		while (j < str.length() && j < end && str[j] != delimiter)
			j++;
		if (i == j)
			break;
		std::stringstream ss(str.substr(i, j - i));
		T sub; ss >> sub;
		result.push_back(sub);
		i = j;
	}
	return result;
}

#endif