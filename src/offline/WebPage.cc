
#include "WebPage.h"
#include <algorithm>
#include <queue>

namespace mysp
{

WebPage::WebPage(const shared_ptr<string> & doc, SplitToolCppJieba & jieba,
		shared_ptr<set<string>> stopWordsSet)
: _doc(doc)
, _docId(stoi(findInDoc(*doc, "docid")))
, _docTitle(findInDoc(*doc, "title"))
, _docUrl(findInDoc(*doc, "url"))
, _docContent(findInDoc(*doc, "content"))
//, _docSummary(getSummary(_docTitle, _docContent))
, _stopWordsSet(stopWordsSet){

	calcWordsMap(jieba);
	calcTopK();
}

int WebPage::getDocId() const{
	return _docId;
}

void WebPage::setNewDocId(int newId){
	_docId = newId;

	auto beg = _doc->find_first_of("0123456789");
	auto end = _doc->find_first_not_of("0123456789", beg);
	_doc->replace(beg, end - beg, to_string(newId));
}

const map<string, size_t> & WebPage::getWordsMap() const{
	return _wordsMap;
}

const string & WebPage::getDoc() const{
	return *_doc;
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

/*//把标题或者文章内容第一句作为摘要
string WebPage::getSummary(const string & title, const string & content){
	if (title.size())
		return title;
	return string(content, 0, content.find("\xe3\x80\x82") + 3);//utf-8编码的句号
}*/

void WebPage::calcWordsMap(SplitToolCppJieba & jieba){
	vector<string> words;
	//preProcess(_docTitle);
	preProcess(_docContent);

	string part;
	istringstream /*ss1(_docTitle),*/ ss2(_docContent);

	/*while (ss1 >> part){
		cout << part << endl;
		jieba.cut(words, part);
	}*/
	while (ss2 >> part){
		jieba.cut(words, part);
		for (const string & word : words){
			if (_stopWordsSet->find(word) == _stopWordsSet->end())
				_wordsMap[word]++;
		}
	}

}

void WebPage::calcTopK(){
	auto cmp = [](pair<string, size_t> left, pair<string, size_t> right) 
		{ return left.second < right.second ;};
	priority_queue<pair<string, size_t>, 
		vector<pair<string, size_t>>, decltype(cmp)> topQueue(cmp);

	for (const auto & ele : _wordsMap){
		topQueue.push(ele);
	}

	for (int i = 0; i < TOPK_NUMBER; ++i){
		if (!topQueue.empty()){
			_topWords.push_back(topQueue.top().first);
			topQueue.pop();
		}else {
			break;
		}
	}

}

bool operator==(const WebPage & left, const WebPage & right){
	vector<string> intersection;
	set_intersection(left._topWords.begin(), left._topWords.end(),
			right._topWords.begin(), right._topWords.end(),
			back_inserter(intersection));

	return intersection.size() > 12;
}

bool operator<(const WebPage & left, const WebPage & right){
	return left._docId < right._docId;
}

void preProcess(string & str){
	for (char & ch : str){
		if (!(ch & 0x80) && !isalpha(ch)){
			ch = ' ';
		}
	}
}

}
