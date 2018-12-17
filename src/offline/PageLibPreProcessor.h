
#ifndef __MYSP_PAGELIBPREPROCESSOR_H__
#define __MYSP_PAGELIBPREPROCESSOR_H__

#include "WebPage.h"
#include "SplitToolCppJieba.h"

#include <unordered_map>

using namespace std;

namespace mysp
{

class PageLibPreProcessor{
	public:
		PageLibPreProcessor();
		void doProcess();
		void readInfoFromFile();
		void cutRedundantPages();
		void buildInvertIndexTable();
		void storeOnDisk();
	private:
		void readOffsetLib();
		void readPageLib();
		void readStopWords();
		void calcTFIDF(const WebPage &,
				unordered_map<string, size_t> &);
	private:
		shared_ptr<set<string>> _stopWordsSet;
		SplitToolCppJieba _jieba;
		vector<WebPage> _pageLib;
		map<int, pair<size_t, size_t>> _offsetLib;
		unordered_map<string, vector<pair<int, double>>> _invertIndexTable;
};

}

#endif
