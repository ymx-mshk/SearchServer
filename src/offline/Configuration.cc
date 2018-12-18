
#include "Configuration.h"
#include "Mylogger.h"
#include "DirScanner.h"

#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>
#include <fstream>

namespace mysp
{

Configuration * Configuration::_pInstance;

Configuration::Configuration(const string & file){
	init(file);
}

Configuration * Configuration::getInstance(const string & path){
	if (!_pInstance){
		_pInstance = new Configuration(path);
		atexit(destroy);
	}
	return _pInstance;
}

void Configuration::destroy(){
	if (_pInstance)
		delete _pInstance;
}

void Configuration::init(const string & file){
	std::ifstream input(file);

	if (!input){
		LogError("Open config file \"%s\" failed", file.c_str());
		exit(EXIT_FAILURE);
	}else{
		LogInfo("Open config file \"%s\" success", file.c_str());
	}

	std::string line;
	std::vector<string> vec;
	while (getline(input, line)){
		if (line.empty())
			continue;
		//使用boost库的字符串分割函数
		boost::split(vec, line, boost::is_any_of(" \t,:"), boost::token_compress_on);
		//因为配置文件都是键值对,而且每项一行，所以vec里只有两个元素
		_conf.emplace(vec[0], vec[1]);
	}

	input.close();
}

}
