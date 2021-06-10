#ifndef __STR_UTILS_H__
#define __STR_UTILS_H__

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>

#include "type.h"
#include "path.h"

namespace str
{
	std::string lower(std::string content);
	std::string upper(std::string content);

	char* lower(const char* content);
	char* upper(const char* content);

	int skip_space(const std::string& content, int& i);
	int skip_valid(const std::string& content, int& i);
	std::string get_word(const std::string& content, int& i);
	std::string get_var(const std::string& content, int& i);

	int skip_space_reverse(const std::string& content, int& i);
	int skip_valid_reverse(const std::string& content, int& i);
	std::string get_word_reverse(const std::string& content, int& i);

	std::string replace_include(std::string content, const std::set<std::string>& include_paths);
	std::string delete_C_style_comments(std::string content);
	std::string strip(const std::string& content);
	std::string replace(std::string content, const std::string& pattern, const std::string& target);
	std::vector<std::string> split(const std::string& content, const std::string& pattern);
	uint line_number(const std::string& content, uint pos);
	std::string format_space(std::string content);
	std::string format_var_name(const std::string& name);
	std::string multi_space(uint n);
	double eval(const std::string& expression);
	std::string base_type(const std::string& type);
	uint array_length(const std::string& type);
	uint lines(const std::string& content);

	// ************************************************
	// Implements of all templates
	// ************************************************
	template<class DataType>
	std::string str(const DataType& value)
	{
		unsigned long long hash_code = typeid(DataType).hash_code();
		if(hash_code == typeid(std::string).hash_code()) return force_cast<std::string>(value);
		else if(hash_code == typeid(char*).hash_code()) return std::string(force_cast<char*>(value));
		else if(hash_code == typeid(char).hash_code()) return std::string(1, force_cast<char>(value));
		
		std::stringstream ss;
		ss << value;
		std::string result;
		ss >> result;
		return result;
	}

	template<class DataType>
	DataType str2(const std::string& value)
	{
		std::stringstream ss;
		ss << value;
		DataType result;
		ss >> result;
		return result;
	}
}

#endif