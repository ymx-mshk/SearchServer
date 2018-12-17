
#ifndef __MYSP_SPLITTOOLCPPJIEBA_H__
#define __MYSP_SPLITTOOLCPPJIEBA_H__

#include "../../libs/cppjieba/include/cppjieba/Jieba.hpp"

#include <string>
#include <vector>

using namespace std;

namespace mysp
{

class SplitToolCppJieba{
	public:
		SplitToolCppJieba();
		void cut(vector<string> &, const string &);
	private:
		cppjieba::Jieba _jieba;
};

}

#endif
