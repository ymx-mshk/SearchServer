
#include "Configuration.h"
#include "Mylogger.h"

#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>
#include <fstream>

namespace mysp
{

Configuration* Configuration::_pInstance;

Configuration * Configuration::getInstance(const string & file){
	if (!_pInstance){
		_pInstance = new Configuration(file);
		atexit(destroy);
	}

	return _pInstance;
}

void Configuration::destroy(){
	if (_pInstance){
		delete _pInstance;
	}
}

Configuration::Configuration(const string & file){
	init(file);
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
		boost::split(vec, line, boost::is_any_of(" \t,:"), boost::token_compress_on);
		_conf.emplace(vec[0], vec[1]);
	}

	input.close();
}

}
