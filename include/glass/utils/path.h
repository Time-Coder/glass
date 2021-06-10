#ifndef __PATH_H__
#define __PATH_H__

#include <iostream>
#include <sstream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <list>
#include <stack>

#include <dirent.h>
#include <sys/stat.h>

#ifdef __linux__
	#include <unistd.h>
	#include <pwd.h>
#else
	#include <windows.h>
#endif

#ifndef TYPE_UINT
typedef unsigned int uint;
#define TYPE_UINT
#endif

namespace path
{
	void format(std::string& filename);

	int cd(std::string dir_name);
	std::string cd();
	std::string pwd();
	std::string username();
	std::vector<std::string> ls(std::string dir_name = ".");
	int touch(std::string file_name);
	int mkdir(std::string dir_name);
	int rm(std::string target_name);
	int cp(std::string src_name, std::string dest_name);
	int mv(std::string src_name, std::string dest_name);
	int rename(std::string src_name, std::string dest_name);

	bool isfile(std::string filename);
	bool isdir(std::string dirname);
	bool exists(std::string name);
	std::string cat(std::string filename);

	std::string abspath(std::string filename);
	std::string relpath(std::string filename, std::string start = ".");
	std::string dirname(const std::string& filename);
	std::string basename(const std::string& filename);
	std::string extname(const std::string& filename);
}

#endif