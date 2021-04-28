#ifndef __STR_UTILS_H__
#define __STR_UTILS_H__

#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <type.h>
#include <path.h>

using namespace std;

namespace str
{
	string lower(string content);
	string upper(string content);

	char* lower(const char* content);
	char* upper(const char* content);

	int skip_space(const string& content, int& i);
	int skip_valid(const string& content, int& i);
	string get_word(const string& content, int& i);

	int skip_space_reverse(const string& content, int& i);
	int skip_valid_reverse(const string& content, int& i);
	string get_word_reverse(const string& content, int& i);

	string replace_include(string content, const set<string>& include_paths);
	string delete_C_style_comments(string content);
	string strip(const string& content);
	string replace(string content, const string& pattern, const string& target);
	vector<string> split(const string& content, const string& pattern);
	uint line_number(const string& content, uint pos);
	string format_space(string content);
	string format_var_name(const string& name);
	string multi_space(uint n);
	double eval(const string& expression);
	string base_type(const string& type);
	uint array_length(const string& type);
	uint lines(const string& content);

	// ************************************************
	// Implements of all templates
	// ************************************************
	template<class DataType>
	string str(const DataType& value)
	{
		unsigned long long hash_code = typeid(DataType).hash_code();
		if(hash_code == typeid(string).hash_code()) return force_cast<string>(value);
		else if(hash_code == typeid(char*).hash_code()) return string(force_cast<char*>(value));
		else if(hash_code == typeid(char).hash_code()) return string(1, force_cast<char>(value));
		
		stringstream ss;
		ss << value;
		string result;
		ss >> result;
		return result;
	}

	template<class DataType>
	DataType str2(const string& value)
	{
		stringstream ss;
		ss << value;
		DataType result;
		ss >> result;
		return result;
	}
}

#endif