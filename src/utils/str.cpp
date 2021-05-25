#include "glass/utils/str.h"
#include "glass/utils/exceptions.h"

using namespace std;

string str::lower(string content)
{
	for(int i = 0; i < content.size(); i++)
	{
		if(content[i] >= 'A' && content[i] <= 'Z')
		{
			content[i] -= ('A'-'a');
		}
	}

	return content;
}

string str::upper(string content)
{
	for(int i = 0; i < content.size(); i++)
	{
		if(content[i] >= 'a' && content[i] <= 'z')
		{
			content[i] += ('A'-'a');
		}
	}

	return content;
}

char* str::lower(const char* content)
{
	int len_content = strlen(content);
	char* result = new char[len_content+1];
	for(int i = 0; i < len_content; i++)
	{
		if(content[i] >= 'A' && content[i] <= 'Z')
		{
			result[i] = content[i] - ('A'-'a');
		}
		else
		{
			result[i] = content[i];
		}
	}
	result[len_content] = '\0';

	return result;
}

char* str::upper(const char* content)
{
	int len_content = strlen(content);
	char* result = new char[len_content+1];
	for(int i = 0; i < len_content; i++)
	{
		if(content[i] >= 'a' && content[i] <= 'z')
		{
			result[i] = content[i] + ('A'-'a');
		}
		else
		{
			result[i] = content[i];
		}
	}
	result[len_content] = '\0';
	
	return result;
}

string str::replace(string content, const string& pattern, const string& target)
{
	int pos = 0;
	int len_pattern = pattern.size();
	int len_target = target.size();
	while(true)
	{
		pos = content.find(pattern, pos);
		if(pos == string::npos)
		{
			return content;
		}
		content.replace(pos, len_pattern, target);
		pos += len_target;
	}

	return content;
}

int str::skip_space(const string& content, int& i)
{
	if(i < 0 || i >= content.size())
	{
		return i;
	}
	while(i < content.size() && content[i] == ' ' || content[i] == '\t' || content[i] == '\n')
	{
		i++;
	}

	return i;
}

int str::skip_valid(const string& content, int& i)
{
	if(i < 0 || i >= content.size())
	{
		return i;
	}
	while(i < content.size() && content[i] != ' ' && content[i] != '\t' && content[i] != '\n')
	{
		i++;
	}

	return i;
}

string str::get_word(const string& content, int& i)
{
	if(i < 0 || i >= content.size())
	{
		return string("");
	}
	skip_space(content, i);
	int pos_start = i;
	skip_valid(content, i);
	return content.substr(pos_start, i - pos_start);
}

string str::get_var(const string& content, int& i)
{
	if(i < 0 || i >= content.size())
	{
		return string("");
	}
	skip_space(content, i);
	int pos_start = i;
	while(i < content.size() && ((content[i] >= 'a' && content[i] <= 'z') ||
		                         (content[i] >= 'A' && content[i] <= 'Z') ||
		                         (content[i] >= '0' && content[i] <= '9') ||
		                         content[i] == '_'))
	{
		i++;
	}
	return content.substr(pos_start, i - pos_start);
}

int str::skip_space_reverse(const string& content, int& i)
{
	if(i < 0 || i >= content.size())
	{
		return i;
	}
	while(i >= 0 && content[i] == ' ' || content[i] == '\t' || content[i] == '\n')
	{
		i--;
	}

	return i;
}

int str::skip_valid_reverse(const string& content, int& i)
{
	if(i < 0 || i >= content.size())
	{
		return i;
	}
	while(i >= 0 && content[i] != ' ' && content[i] != '\t' && content[i] != '\n')
	{
		i--;
	}

	return i;
}

string str::get_word_reverse(const string& content, int& i)
{
	if(i < 0 || i >= content.size())
	{
		return string("");
	}

	skip_space_reverse(content, i);
	int pos_end = i + 1;
	skip_valid_reverse(content, i);
	int pos_start = i + 1;
	
	return content.substr(pos_start, pos_end - pos_start);
}

string str::delete_C_style_comments(string code)
{
	while(true)
	{
		int start_pos = code.find("/*");
		if(start_pos == string::npos)
		{
			break;
		}
		int end_pos = code.find("*/", start_pos+2);
		if(end_pos == string::npos)
		{
			end_pos = code.size();
		}

		code.erase(start_pos, end_pos+2-start_pos);
	}

	while(true)
	{
		int start_pos = code.find("//");
		if(start_pos == string::npos)
		{
			break;
		}
		int end_pos = code.find("\n", start_pos+2);
		if(end_pos == string::npos)
		{
			end_pos = code.size();
		}

		code.erase(start_pos, end_pos-start_pos);
	}

	return code;
}

string str::strip(const string& content)
{
	if(content.size() == 0)
	{
		return content;
	}

	int pos = 0;
	int pos_start = skip_space(content, pos);
	pos = content.size()-1;
	int pos_end = skip_space_reverse(content, pos) + 1;

	return content.substr(pos_start, pos_end-pos_start);
}

vector<string> str::split(const string& content, const string& pattern)
{
	vector<string> result;
	int pos = 0, pos_start = 0;
	while(true)
	{
		pos = content.find(pattern, pos_start);
		if(pos == string::npos)
		{
			result.push_back(content.substr(pos_start));
			return result;
		}
		result.push_back(content.substr(pos_start, pos-pos_start));
		pos_start = pos + pattern.size();
	}

	return result;
}

uint str::lines(const string& content)
{
	uint n_lines = 1;
	for(int i = 0; i < content.size(); i++)
	{
		if(content[i] == '\n')
		{
			n_lines++;
		}
	}

	return n_lines;
}

string str::replace_include(string content, const set<string>& include_paths)
{
	int pos_include_start = 0;
	int pos_filename_start = 0;
	int pos_include_end = 0;
	int pos_filename_end = 0;
	while(true)
	{
		content = delete_C_style_comments(content);
		pos_include_start = content.find("#include");
		if(pos_include_start == string::npos)
		{
			return content;
		}
		pos_filename_start = pos_include_start + 8;
		skip_space(content, pos_filename_start);
		if(content[pos_filename_start] != '<' && content[pos_filename_start] != '\"')
		{
			throw glass::Exception("Error using #include: head file name must be envelop by <...> or \"...\"");
		}
		char end_char = content[pos_filename_start] == '<' ? '>' : '\"';
		pos_filename_start++;
		skip_space(content, pos_filename_start);
		pos_filename_end = content.find(end_char, pos_filename_start);
		if(pos_filename_end == string::npos)
		{
			throw glass::Exception(string("Error using #include: ") + end_char + " must be the end char.");
		}
		pos_include_end = pos_filename_end + 1;
		pos_filename_end--;
		skip_space_reverse(content, pos_filename_end);
		string filename = content.substr(pos_filename_start, pos_filename_end+1-pos_filename_start);
		bool found = false;
		for(auto it = include_paths.begin(); it != include_paths.end(); it++)
		{
			if(!path::isdir(*it))
			{
				continue;
			}
			string full_name = *it + "/" + filename;
			if(path::isfile(full_name))
			{
				content.replace(pos_include_start, pos_include_end-pos_include_start, path::cat(full_name));
				found = true;
				break;
			}
		}
		if(!found)
		{
			throw glass::Exception("Error in #include: file \"" + filename + "\" is not found!");
		}
	}
}

uint str::line_number(const string& content, uint pos)
{
	uint content_size = content.size();
	uint size = min(content_size, pos+1);
	uint n_line = 1;
	for(int i = 0; i < size; i++)
	{
		if(content[i] == '\n')
		{
			n_line++;
		}
	}

	return n_line;
}

string str::multi_space(uint n)
{
	string result;
	for(int i = 0; i < n; i++)
	{
		result += " ";
	}
	return result;
}

string str::format_space(string content)
{
	if(content.size() == 0)
	{
		return content;
	}

	content = strip(content);

	int pos = 0, pos_start = 0;
	while(pos < content.size())
	{
		if(content[pos] == ' ' || content[pos] == '\t' || content[pos] == '\n')
		{
			content[pos] = ' ';
			pos++;
			pos_start = pos;
			
			while(content[pos] == ' ' || content[pos] == '\t' || content[pos] == '\n')
			{
				pos++;
			}

			content.erase(pos_start, pos-pos_start);
			pos = pos_start;
		}
		else
		{
			pos++;
		}
	}

	return content;
}

double str::eval(const string& expression) // need re-implement
{
	stringstream ss;
	ss << expression;
	double result;
	ss >> result;
	return result;
}

string str::format_var_name(const string& name)
{
	int pos_left_brace = name.find("[");
	if(pos_left_brace == string::npos)
	{
		return strip(name);
	}
	
	int pos_right_brace = name.find("]", pos_left_brace);
	int pos = pos_left_brace-1;
	string pure_name = get_word_reverse(name, pos);
	string expression = name.substr(pos_left_brace+1, pos_right_brace-pos_left_brace-1);
	return pure_name + "[" + str(eval(expression)) + "]";
}

string str::base_type(const string& type)
{
	int pos = type.find("[");
	if(pos == string::npos)
	{
		return type;
	}
	pos--;
	return get_word_reverse(type, pos);
}

uint str::array_length(const string& type)
{
	int pos = type.find("[");
	if(pos == string::npos)
	{
		return 1;
	}
	int pos_start = pos+1;
	int pos_end = type.find("]", pos_start);
	return eval(type.substr(pos_start, pos_end-pos_start));
}