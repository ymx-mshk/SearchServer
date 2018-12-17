
#include "SplitToolCppJieba.h"
#include "Configuration.h"

namespace mysp
{

SplitToolCppJieba::SplitToolCppJieba()
: _jieba(getConfig("dictPath"),
		getConfig("hmmPath"),
		getConfig("userDictPath"),
		getConfig("idfPath"),
		getConfig("stopWordPath")){

}

//调用cppjieba的Cut方法
void SplitToolCppJieba::cut(vector<string> & result, const string & sentence){
	_jieba.Cut(sentence, result, true);
}

}
