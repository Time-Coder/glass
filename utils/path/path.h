#ifndef __PATH_H__
#define __PATH_H__

#include <iostream>
#include <sstream>
#include <cstdio>
#include <fstream>
#include <string>
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

using namespace std;

namespace path
{
	void format(string& filename);

	int cd(string dir_name);
	string cd();
	string pwd();
	string username();
	vector<string> ls(string dir_name = ".");
	int touch(string file_name);
	int mkdir(string dir_name);
	int rm(string target_name);
	int cp(string src_name, string dest_name);
	int mv(string src_name, string dest_name);
	int rename(string src_name, string dest_name);

	bool isfile(string filename);
	bool isdir(string dirname);
	bool exists(string name);
	string cat(string filename);

	string abspath(string filename);
	string relpath(string filename, string start = ".");
	string dirname(const string& filename);
	string basename(const string& filename);
	string extname(const string& filename);
}

#endif