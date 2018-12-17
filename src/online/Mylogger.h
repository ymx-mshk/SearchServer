
#ifndef __MYSP_MYLOGGER_H__
#define __MYSP_MYLOGGER_H__

#include <log4cpp/Category.hh>
#include <string>

using std::string;
using namespace log4cpp;

namespace mysp
{

class Mylogger{
	public:
		static Mylogger * getInstance();
		static void shutdown();
	
		void info(const char * msg){
			_root.info(msg);
		}
		template<typename ...Args>
		void info(const char * msg, Args ...args){
			_root.info(msg, args...);
		}

		void warn(const char * msg){
			_root.warn(msg);
		}
		template<typename ...Args>
		void warn(const char * msg, Args ...args){
			_root.warn(msg, args...);
		}

		void error(const char * msg){
			_root.error(msg);
		}
		template<typename ...Args>
		void error(const char * msg, Args ...args){
			_root.error(msg, args...);
		}

		void debug(const char * msg){
			_root.debug(msg);
		}
		template<typename ...Args>
		void debug(const char * msg, Args ...args){
			_root.debug(msg, args...);
		}
		
	private:
		Mylogger();
		~Mylogger();
		
		Category & _root;
		static Mylogger * _pInstance; 
};


#define prefix(msg) string().append("[").append(__FILE__)\
	.append("-").append(__FUNCTION__)\
	.append("-").append(std::to_string(__LINE__))\
	.append("] ").append(msg).c_str()

#define LogInfo(msg, ...) Mylogger::getInstance()->info(prefix(msg), ##__VA_ARGS__)
#define LogWarn(msg, ...) Mylogger::getInstance()->warn(prefix(msg), ##__VA_ARGS__)
#define LogError(msg, ...) Mylogger::getInstance()->error(prefix(msg), ##__VA_ARGS__)
#define LogDebug(msg, ...) Mylogger::getInstance()->debug(prefix(msg), ##__VA_ARGS__)

}

#endif
