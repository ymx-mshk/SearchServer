
#include "DirScanner.h"
#include "Mylogger.h"
#include "Configuration.h"
#include "WordQuery.h"

#include <json/json.h>
#include <fstream>
#include <boost/algorithm/string.hpp>

namespace mysp
{

void WordQuery::loadLibrary(){
	loadOffsetLib();
	loadPageLib();
	loadInvertIndexTableLib();
	LogInfo("pageNum = %ld", _pageLib.size());
	LogInfo("wordsNUm = %ld",_invertIndexTable.size());
}

void WordQuery::loadOffsetLib(){
	ifstream ifs(getConfig("newOffsetLibPath"));
	if (!ifs){
		LogError("create ifstream newOffsetLibPath failed");
		exit(EXIT_FAILURE);
	}
	string line;
	int docid;
	size_t oft, length; //oft is offset
	while(getline(ifs, line)){
		if (!line.size())
			continue;
		istringstream ss(line);

		ss >> docid;
		ss >> oft;
		ss >> length;
		_offsetLib.emplace(docid, make_pair(oft, length));
	}

	ifs.close();
}

void WordQuery::loadPageLib(){
	ifstream ifs(getConfig("newRipePageLibPath"));
	if (!ifs){
		LogError("create ifstream newRipePageLibPath failed");
		exit(EXIT_FAILURE);
	}

	for (const auto & item : _offsetLib){
		size_t size = item.second.second;
		string doc(size, '\0');

		ifs.seekg(item.second.first);
		ifs.read(&doc[0], size);
		_pageLib.emplace(item.first, WebPage(doc));
	}

	ifs.close();

}

void WordQuery::loadInvertIndexTableLib(){
	ifstream ifs(getConfig("invertIndexTablePath"));
	if (!ifs){
		LogError("create ifstream invertIndexTableLib failed");
		exit(EXIT_FAILURE);
	}

	string line;
	vector<string> svec;
	int id; double w;
	while(getline(ifs, line)){
		boost::split(svec, line, boost::is_any_of(":"));
		vector<pair<int, double>> weight;
		istringstream ss(svec[1]);

		while (ss >> id){
			ss >> w;
			weight.emplace_back(id, w);
		}

		_invertIndexTable.emplace(svec[0], std::move(weight));
	}
}

void WordQuery::readStopWords(){
	vector<string> files;
	DirScanner dirscanner;
	dirscanner(getConfig("stopWordsPath"), files);

	ifstream ifs;

	for (const auto & file : files){
		ifs.open(file);
		if (!ifs){
			LogError("create ifstream stopWordsFile \"%s\" error", file.c_str());
			continue;
		}

		string word;
		while (ifs >> word){
			_stopWords.insert(word);
		}
	}
}

string WordQuery::doQuery(const string & str){
	vector<string> queryWords;
	getQueryWords(queryWords, str);
	
	vector<double> queryWordsWeight = getQueryWordsWeightVector(queryWords);
	vector<pair<int, vector<double>>> resultVec;
	if (executeQuery(queryWords, resultVec)){
		cout << resultVec.size() << " = find" << endl;
		multimap<double, int> result;
		for (const auto & item : resultVec){
			result.emplace(calcCos(queryWordsWeight, item.second), item.first);
		}

		vector<int> resultDocId;
		for (auto it = result.rbegin(); it != result.rend(); ++it){
			resultDocId.push_back(it->second);
		}
		cout << resultDocId.size() << " = process" << endl;
		return createJson(resultDocId, queryWords);
	}else {
		return returnNoAnswer();
	}
}

string WordQuery::createJson(const vector<int> & result,
		const vector<string> & queryWords){
	
	Json::Value root;
	Json::Value content;
	Json::Value item;

	for (int i : result){
		auto it = _pageLib.find(i);
		if (it == _pageLib.end())
			continue;
		item["title"] = it->second.getTitle();
		item["url"] = it->second.getUrl();
		item["summary"] = it->second.getSummary(queryWords);
		
		content.append(item);
	}
	root["files"] = content;
	return root.toStyledString(); 
}

string WordQuery::returnNoAnswer(){
	
	Json::Value root;
	Json::Value content;
	Json::Value item;

	item["title"] = "没有找到";
	item["url"] = "";
	item["summary"] = "请重新输入";
	
	content.append(item);
	root["files"] = content;
	return root.toStyledString(); 
}

void WordQuery::getQueryWords(vector<string> & queryWords, string str){
	vector<string> words;
	preProcess(str);

	string part;
	istringstream ss(str);

	while (ss >> part){
		_jieba.cut(words, part);
		for (const string & word : words){
			if (_stopWords.find(word) == _stopWords.end())
				queryWords.push_back(word);
		}
	}

}

vector<double> WordQuery::getQueryWordsWeightVector(const vector<string> & queryWords){
	vector<double> weight;
	for (const string & word : queryWords){
		weight.push_back(1 * log10(static_cast<double>(_pageLib.size()) / 
					(_invertIndexTable[word].size() + 2)));
	}

	double wSum = 0.0;
	for (double w : weight){
		wSum += w * w;
	}
	wSum = sqrt(wSum);

	for (double & w : weight){
		w = w / wSum;
	}

	return weight;
}

bool WordQuery::executeQuery(const vector<string> & queryWords,
		vector<pair<int, vector<double>>> & resultVec){
	
	resultVec.clear();

	//找出同时包含所有查询词的文档
	size_t docNum = _pageLib.size();
	char queryWordsNum = queryWords.size();
	char * pIntersection(new char[docNum + 1]());

	for (const string & word : queryWords){
		for (const auto & item : _invertIndexTable[word]){
			++pIntersection[item.first];
		}
	}

	for (size_t i = 1; i != docNum + 1; ++i){
		//如果第i篇文档内包含所有的查询词，则添加到resultVec
		if (pIntersection[i] == queryWordsNum){
			vector<double> weight;
			for (const string & word : queryWords){
				for (const auto & item : _invertIndexTable[word]){
					if (item.first == static_cast<int>(i))
						weight.push_back(item.second);
				}
			}
			resultVec.emplace_back(i, std::move(weight));
		}
	}

	delete []pIntersection;
	return resultVec.size();
}

double calcCos(const vector<double> & left, const vector<double> & right){
	if (left.size() != right.size()){
		LogWarn("calcCos input error");
		return 0;
	}
	double leftValue = 0.0, rightValue = 0.0, innerProduct = 0.0;
	for (const auto & d : left){
		leftValue += d * d;	
	}
	for (const auto & d : right){
		rightValue += d * d;	
	}
	leftValue = sqrt(leftValue);
	rightValue = sqrt(rightValue);

	for (size_t i = 0; i != left.size(); ++i){
		innerProduct += left[i] * right[i];
	}

	return innerProduct / (leftValue * rightValue);
}

void preProcess(string & str){
	for (char & ch : str){
		if (!(ch & 0x80) && !isalpha(ch)){
			ch = ' ';
		}
	}
}

}
