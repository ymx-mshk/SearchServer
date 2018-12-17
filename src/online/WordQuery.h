
#ifndef __MYSP_WORDQUERY_H__
#define __MYSP_WORDQUERY_H__

#include "WebPage.h"
#include "SplitToolCppJieba.h"

#include <unordered_map>

namespace mysp
{

class WordQuery{
	public:
		void loadLibrary();
		string doQuery(const string &);
	private:
		vector<double> getQueryWordsWeightVector(const vector<string> &);
		bool executeQuery(const vector<string> &, vector<pair<int, vector<double>>> &);
		string createJson(const vector<int> &, const vector<string> &);
		string returnNoAnswer();

		void getQueryWords(vector<string> &, string str);
		void loadOffsetLib();
		void loadPageLib();
		void loadInvertIndexTableLib();
		void readStopWords();
	private:
		SplitToolCppJieba _jieba;
		unordered_set<string> _stopWords;
		unordered_map<int, WebPage> _pageLib;
		unordered_map<int, pair<size_t, size_t>> _offsetLib;
		unordered_map<string, vector<pair<int, double>>> _invertIndexTable;
};

double calcCos(const vector<double> &, const vector<double> &);
void preProcess(string & str);

}

#endif
