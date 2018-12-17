
#include "WebPage.h"
#include <algorithm>
#include <queue>

namespace mysp
{

WebPage::WebPage(const string & doc){
	processDoc(doc);
}

void WebPage::processDoc(const string & doc){
	string docid = findInDoc(doc, "docid");
	_docId = docid.size() ? stoi(docid) : 0;

	_docTitle = findInDoc(doc, "title");
	_docUrl = findInDoc(doc, "url");
	_docContent = findInDoc(doc, "content");

}

int WebPage::getDocId() const{
	return _docId;
}

string WebPage::getTitle() const{
	return _docTitle;
}

string WebPage::getUrl() const{
	return _docUrl;
}

string WebPage::getSummary(const vector<string> & queryWords){
	string summary;
	bool includeTitle = 0;
	for (const auto & word : queryWords){
		if (!includeTitle){
			summary += getSentenceByWord(word, _docTitle);
		}
		summary += getSentenceByWord(word, _docContent);
	}
	return summary;
}

string WebPage::getSentenceByWord(const string & word, const string & str){
	auto pos = str.find(word);
	if (pos == string::npos)
		return string();

	decltype(pos) beg = str.rfind("\xe3\x80\x82", pos),
				  end = str.find("\xe3\x80\x82", pos);
	if (beg == string::npos){
		beg = 0;
	}else {
		beg += 3;
	}
	if (end == string::npos){
		end = str.size();
	}else {
		end += 3;
	}
	if (abs(static_cast<int>(beg - pos)) > 120)
		beg = getPreCharPos(pos - 120, str);
	if (abs(static_cast<int>(end - pos)) > 120)
		end = getNextCharPos(pos + 120, str);

	return string(str, beg, end - beg);
}

string WebPage::findInDoc(const string & doc, const char * str){
	string::size_type beg, end;
	string begWord = string() + '<' + str + '>',
		   endWord = string() + "</" + str + '>';
	if ((beg = doc.find(begWord)) == string::npos ||
			(end = doc.find(endWord, beg)) == string::npos){
		return string();
	}

	return string(doc.begin() + beg + begWord.size(), doc.begin() + end);
}

string::size_type getPreCharPos(string::size_type pos, const string & str){
	while ((str[pos] & 0xC0) == 0x80)
		pos--;
	return pos;
}
string::size_type getNextCharPos(string::size_type pos, const string & str){
	while ((str[pos] & 0xC0) == 0x80)
		pos++;
	return pos;
}

}
