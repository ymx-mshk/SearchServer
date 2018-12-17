
#ifndef __MYSP_WEBPAGE_H__
#define __MYSP_WEBPAGE_H__

#include "SplitToolCppJieba.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

namespace mysp
{

class WebPage{
	public:
		WebPage(const shared_ptr<string> &, SplitToolCppJieba &, shared_ptr<set<string>>);
		int getDocId() const;
		void setNewDocId(int);
		const map<string, size_t> & getWordsMap() const;
		const string & getDoc() const;

		friend bool operator==(const WebPage &, const WebPage &);
		friend bool operator<(const WebPage &, const WebPage &);
	private:
		string findInDoc(const string &, const char *);
		//string getSummary(const string &, const string &);
		void calcWordsMap(SplitToolCppJieba & jieba);
		void calcTopK();
	private:
		const static int TOPK_NUMBER = 20;

		shared_ptr<string> _doc;
		int _docId;
		string _docTitle;
		string _docUrl;
		string _docContent;
		//string _docSummary;
		shared_ptr<set<string>> _stopWordsSet;
		vector<string> _topWords;
		map<string, size_t> _wordsMap;
};

bool operator==(const WebPage &, const WebPage &);
bool operator<(const WebPage &, const WebPage &);

void preProcess(string &);

}

#endif
