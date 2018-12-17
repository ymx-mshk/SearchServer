
#ifndef __MYSP_DIRSCANNER_H__
#define __MYSP_DIRSCANNER_H__

#include "Mylogger.h"

#include <dirent.h>
#include <string.h>
#include <vector>
#include <string>

using namespace std;

namespace mysp
{

class DirScanner{
	public:
	void operator()(const string & dirPath, vector<string> & files){
		getAllFiles(dirPath, files);	
	}
	static void getAllFiles(const string & dirPath, vector<string> & files){
		DIR * dir = opendir(dirPath.c_str());
		if (!dir){
			LogError("No dirctory \"%s\"", dirPath.c_str());
		}
		struct dirent * fileInfo;
		string file;

		while ((fileInfo = readdir(dir)) != NULL){
			//如果有子目录并且不是.和..，则进入该目录继续查找
			if (fileInfo->d_type == DT_DIR 
					&& strcmp(fileInfo->d_name, ".")
					&& strcmp(fileInfo->d_name, "..")){

				getAllFiles(dirPath + "/" + fileInfo->d_name, files);
			}
			if (fileInfo->d_type == DT_REG){
				file = dirPath + "/" + fileInfo->d_name;
				files.push_back(file);
				LogDebug("add \"%s\" to vector files", file.c_str());
			}
		}

	}
};

}

#endif
