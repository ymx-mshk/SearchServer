
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
		WebPage(const string &);
		int getDocId() const;
		string getTitle() const;
		string getUrl() const;
		void processDoc(const string &);
		string getSummary(const vector<string> &);

	private:
		string getSentenceByWord(const string &, const string &);
		string findInDoc(const string &, const char *);
	private:
		const static int TOPK_NUMBER = 20;

		int _docId;
		string _docTitle;
		string _docUrl;
		string _docContent;
		string _docSummary;
};

string::size_type getPreCharPos(string::size_type, const string &);
string::size_type getNextCharPos(string::size_type, const string &);

}

#endif
