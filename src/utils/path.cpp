#include "glass/utils/path.h"

using namespace std;

void path::format(string& filename)
{
	if(filename.empty())
	{
		filename = ".";
	}

	for(auto it = filename.begin(); it != filename.end();)
	{
		if(*it == '\\')
		{
			*it = '/';
		}

		if(it != filename.begin() && *(it-1) == '/' && *it == '/')
		{
			it = filename.erase(it);
		}
		else
		{
			it++;
		}
	}
	// if(filename[0] != '.' && filename[0] != '/' && filename[1] != ':')
	// {
	// 	filename = string("./") + filename;
	// }
}

vector<string> path::ls(string folder_name)
{
	path::format(folder_name);
	if(folder_name[folder_name.size()-1] == '/')
	{
		folder_name.erase(folder_name.size()-1, 1);
	}

    vector<string> filenames;

    struct stat s;
    stat(folder_name.c_str(), &s);
    if(!S_ISDIR(s.st_mode))
    {
        return filenames;
    }

    DIR* open_dir = opendir(folder_name.c_str());
    if(NULL == open_dir)
    {
        exit(EXIT_FAILURE);
    }
    dirent* p = nullptr;
    while( (p = readdir(open_dir)) != nullptr)
    {
        struct stat st;
        if(p->d_name[0] != '.')
        {
        	if(folder_name != ".")
            	filenames.push_back(folder_name + "/" + string(p->d_name));
            else
            	filenames.push_back(string(p->d_name));
        }
    }
    closedir(open_dir);
    return filenames;
}

int path::cd(string dir_name)
{
	path::format(dir_name);
	return chdir(dir_name.data());
}

string path::cd()
{
	char path[1024];
	getcwd(path, 1024);
	string str_path = string(path);
	format(str_path);

	return str_path;
}

string path::pwd()
{
	char path[1024];
	getcwd(path, 1024);
	string str_path = string(path);
	format(str_path);

	return str_path;
}

std::string path::username()
{
#ifdef __linux__
    uid_t userid;
    struct passwd* pwd;
    userid = getuid();
    pwd = getpwuid(userid);
    return pwd->pw_name;
#else
    DWORD len = 128;
    char szBuffer[len];
    GetUserName(szBuffer, &len);
    return szBuffer;
#endif
}

bool path::isfile(string filename)
{
	path::format(filename);
	if(filename[filename.size()-1] == '/')
	{
		return false;
	}

	struct stat s;
	return (stat(filename.c_str(), &s) == 0 && (s.st_mode & S_IFREG));
}

bool path::isdir(string folder_name)
{
	path::format(folder_name);
	struct stat s;
	return (stat(folder_name.c_str(), &s) == 0 && (s.st_mode & S_IFDIR));
}

bool path::exists(string name)
{
	path::format(name);
	struct stat s;
	return (stat(name.c_str(), &s) == 0);
}

int path::mkdir(string folder_name)
{
	path::format(folder_name);
	if(folder_name[folder_name.size()-1] == '/')
	{
		folder_name.erase(folder_name.size()-1, 1);
	}

	stack<string> S;
	int n = folder_name.size();
	int i = n;
	while(true)
	{
		if(i == n || folder_name[i] == '/' || folder_name[i] == '\\')
		{
			string current_name = folder_name.substr(0, i);
			if(isdir(current_name))
			{
				break;
			}
			else
			{
				S.push(current_name);
			}
		}
		i--;
		if(i == 0)
		{
			break;
		}
	}
	
	if(S.empty())
	{
		return 1;
	}

	while(!S.empty())
	{
		int flag;
		#ifdef __linux__
			flag = ::mkdir(S.top().c_str(), 0755);
		#else
			flag = ::mkdir(S.top().c_str());
		#endif

		if(flag != 0)
		{
			return -1;
		}

		S.pop();
	}

	return 0;
}

int path::rm(string name)
{
	path::format(name);

	if(isfile(name))
	{
		return remove(name.c_str());
	}

	vector<string> files = ls(name);
	if(files.empty())
	{
		return rmdir(name.c_str());
	}

	for(auto it = files.begin(); it != files.end(); it++)
	{
		rm(it->c_str());
	}
	return rmdir(name.c_str());
}

string path::abspath(string filename)
{
	path::format(filename);

	char absolute_path[MAX_PATH+1];
#ifdef __linux__
	char *ptr;
	ptr = realpath(filename.c_str(), absolute_path);
#else
	GetFullPathName(filename.c_str(), MAX_PATH, absolute_path, NULL);
#endif
	string result = absolute_path;
	format(result);

	return result;
}

string path::relpath(string filename, string basepath)
{
	filename = path::abspath(filename);
	basepath = path::abspath(basepath);

	if(filename[0] != basepath[0])
	{
		return filename;
	}

	int min_size = min(filename.size(), basepath.size());
	int pos_not_equal = min_size;
	for(int i = 0; i < min_size; i++)
	{
		if(filename[i] != basepath[i])
		{
			pos_not_equal = i;
			break;
		}
	}

	string result = "";
	int pos = basepath.size()-1;
	while(pos >= 0 && pos >= pos_not_equal)
	{
		result += "../";
		while(pos >= 0 && pos >= pos_not_equal && basepath[pos] != '/')
		{
			pos--;
		}
		pos--;
	}
	pos++;

	while(pos >= 0 && pos < filename.size() && filename[pos] != '/')
	{
		pos--;
	}

	result += filename.substr(pos+1);
	path::format(result);

	return result;
}

string path::basename(const string& filename)
{
	int name_begin = filename.find_last_of("/\\") + 1;
	if(name_begin == string::npos)
	{
		return filename;
	}
	if(name_begin == filename.size())
	{
		return "";
	}
	return filename.substr(name_begin, filename.size()-name_begin);
}

string path::dirname(const string& full_name)
{
	int end_pos = full_name.find_last_of("/\\");
	if(end_pos == string::npos)
	{
		return ".";
	}
	else
	{
		return full_name.substr(0, end_pos);
	}
}

string path::extname(const string& full_name)
{
	int pos_point = full_name.find_last_of(".");
	int pos_slash = full_name.find_last_of("/\\");
	if(pos_slash < pos_point && pos_point != full_name.size()-1)
	{
		return full_name.substr(pos_point+1, full_name.size()-pos_point-1);
	}
	else
	{
		return "";
	}
}

int copy_file_to_dir(const string& filename, const string& dir_name)
{
	if(-1 == path::mkdir(dir_name))
	{
		return -1;
	}
	if(path::isfile(dir_name + "/" + path::basename(filename)))
	{
		return 1;
	}
	
	ifstream ifile(filename.c_str(), ios::binary);
	ofstream ofile((dir_name + "/" + path::basename(filename)).c_str(), ios::binary);
	ofile << ifile.rdbuf();
	ifile.close();
	ofile.close();

	return 0;
}

int copy_file_to_file(const string& src_filename, const string& dest_filename)
{
	if(-1 == path::mkdir(path::dirname(dest_filename)))
	{
		return -1;
	}
	if(path::isfile(dest_filename))
	{
		return 1;
	}

	ifstream ifile(src_filename.c_str(), ios::binary);
	ofstream ofile(dest_filename.c_str(), ios::binary);
	ofile << ifile.rdbuf();
	ifile.close();
	ofile.close();

	return 0;
}

int in_str(const string& target, char pattern)
{
	int n = target.size();
	for(int i = 0; i < n; i++)
	{
		if(target[i] == pattern)
		{
			return i;
		}
	}
	return -1;
}

int path::cp(string src, string dest)
{
	format(src);
	format(dest);

	if(!exists(src) || isfile(dest))
	{
		return -1;
	}
	if(dest == src)
	{
		return 1;
	}

	if(src[src.size()-1] == '/')
	{
		src.erase(src.end()-1);
	}

	string name = path::basename(dest);
	if(isfile(src))
	{
		if(dest[dest.size()-1] == '/')
		{
			return copy_file_to_dir(src, dest);
		}
		else if(isdir(path::dirname(dest)) && !isdir(dest) || in_str(name, '.') > 0)
		{
			return copy_file_to_file(src, dest);
		}
		else
		{
			return copy_file_to_dir(src, dest);
		}
	}
	else
	{
		if(exists(dest))
		{
			dest = dest + "/" + path::basename(src);
		}
		vector<string> files = ls(src);
		if(-1 == mkdir(dest))
		{
			return -1;
		}
		for(auto it = files.begin(); it != files.end(); it++)
		{
			cp(*it, dest);
		}
	}

	return 0;
}

int path::mv(string src, string dest)
{
	format(src);
	format(dest);
	if(src == dest)
	{
		return 1;
	}

	int flag = cp(src, dest);
	rm(src);

	return flag;
}

int path::rename(string src, string dest)
{
	format(src);
	if(src[src.size()-1] == '/')
	{
		src.erase(src.size()-1, 1);
	}

	if(path::basename(src) == dest)
	{
		return 1;
	}

	if(!exists(src) || in_str(dest, '/') != -1 || exists(path::dirname(src) + "/" + dest))
	{
		return -1;
	}

	return ::rename(src.c_str(), (path::dirname(src) + "/" + dest).c_str());
}

int path::touch(string filename)
{
	format(filename);
	if(isfile(filename))
	{
		return 1;
	}
	if(isdir(filename))
	{
		return -1;
	}
	if(-1 == mkdir(path::dirname(filename)))
	{
		return -1;
	}
	ofstream ofile(filename, ios::binary);
	ofile.close();
	return 0;
}

string path::cat(string filename)
{
	ifstream ifile(filename.c_str());
    stringstream buffer;
    buffer << ifile.rdbuf();
    return buffer.str();
}