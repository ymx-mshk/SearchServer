
#ifndef __MYSP_CONFIGURATION_H__
#define __MYSP_CONFIGURATION_H__

#include <unordered_map>
#include <string>
using namespace std;

namespace mysp
{

class Configuration{
	public:
		static Configuration * getInstance(const string & file = string());
		string getConfig(const string & word) const{
			auto cit = _conf.find(word);
			if (cit == _conf.end()){
				return string();
			}
   
			return cit->second; 
		}

	private:
		Configuration(const string &);
		void init(const string &);
		static void destroy();

	private:
		unordered_map<string, string> _conf;
		static Configuration * _pInstance;
};

#define getConfig(s) Configuration::getInstance()->getConfig(s)

}

#endif
