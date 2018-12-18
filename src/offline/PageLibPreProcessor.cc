
#include "DirScanner.h"
#include "Mylogger.h"
#include "Configuration.h"
#include "PageLibPreProcessor.h"

#include <math.h>
#include <fstream>
#include <sstream>
#include <iterator>

namespace mysp
{

PageLibPreProcessor::PageLibPreProcessor()
: _stopWordsSet(new set<string>){
	readInfoFromFile();
};

void PageLibPreProcessor::doProcess(){
	cutRedundantPages();
	buildInvertIndexTable();
}

//网页去重
void PageLibPreProcessor::cutRedundantPages(){
	for (auto it1 = _pageLib.begin(); it1 != _pageLib.end(); ++it1){
		//考虑迭代器失效的问题，erase删除元素时it2后面所有元素都往前移了一位
		//所以此时it2不用++，不执行erase时it2才需要++
		for (auto it2 = it1 + 1; it2 != _pageLib.end(); ){
			if (*it1 == *it2){
				_pageLib.erase(it2);
			}else {
				++it2;
			}
		}
	}

	int docId = 1;
	for (auto & page : _pageLib){
		page.setNewDocId(docId++);
	}
}

void PageLibPreProcessor::buildInvertIndexTable(){
	unordered_map<string, size_t> wordsDf;
	//统计每个词在多少篇文档中出现(df)
	for (auto cit = _pageLib.cbegin(); cit != _pageLib.cend(); ++cit){
		for (const auto & word : cit->getWordsMap()){
			++wordsDf[word.first];
		}
	}

	for (auto it = _pageLib.begin(); it != _pageLib.end(); ++it){
		calcTFIDF(*it, wordsDf);
	}
}

//计算特征权重
void PageLibPreProcessor::calcTFIDF(const WebPage & webpage,
		unordered_map<string, size_t> & wordsDf){

	double wSum = 0.0;
	vector<double> weight;
	for (const auto & word : webpage.getWordsMap()){
		int tf = word.second;
		double idf = log10(static_cast<double>(_pageLib.size())
				/ (wordsDf[word.first] + 1));
		double w = tf * idf;
		weight.push_back(w);
		wSum += w * w;
	}

	wSum = sqrt(wSum);
	int i = 0;
	for (const auto & word : webpage.getWordsMap()){
		if (_invertIndexTable.find(word.first) == _invertIndexTable.end()){
			_invertIndexTable.emplace(word.first,
					vector<pair<int, double>>{make_pair(webpage.getDocId(), 
							weight[i] / wSum)});
		}else {
			_invertIndexTable[word.first].emplace_back(webpage.getDocId(),
					weight[i] / wSum);
		}

		++i;
	}
}

void PageLibPreProcessor::readInfoFromFile(){
	readStopWords();
	readOffsetLib();
	readPageLib();
}

void PageLibPreProcessor::readOffsetLib(){
	ifstream ifs(getConfig("offsetLibPath"));
	if (!ifs){
		LogError("create ifstream offsetLibPath error");
		return;
	}

	string line;
	int docid;
	size_t oft, length; //oft is offset
	while(getline(ifs, line)){
		if (line.empty())
			continue;
		istringstream ss(line);

		ss >> docid;
		ss >> oft;
		ss >> length;
		_offsetLib.emplace(docid, make_pair(oft, length));
	}

	ifs.close();
}

void PageLibPreProcessor::readPageLib(){
	ifstream ifs(getConfig("ripePageLibPath"));
	if (!ifs){
		LogError("create ifstream pageLibPath error");
		return;
	}

	for (const auto & item : _offsetLib){
		size_t size = item.second.second;
		shared_ptr<string> doc(new string(size, '\0'));

		ifs.seekg(item.second.first);
		ifs.read(&(*doc)[0], size);
		_pageLib.push_back(WebPage(doc, _jieba, _stopWordsSet));
	}

	ifs.close();
}

void PageLibPreProcessor::readStopWords(){
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
			_stopWordsSet->insert(word);
		}
	}
}

void PageLibPreProcessor::storeOnDisk(){
	_offsetLib.clear();

	ofstream ofs(getConfig("newRipePageLibPath"));
	if (!ofs){
		LogError("create ofstream newPageLibPath failed");
		return;
	}
	
	int docid = 1;
	for (const auto & page : _pageLib){
		_offsetLib.emplace(docid++, make_pair(ofs.tellp(), page.getDoc().size()));
		ofs << page.getDoc();
	}

	ofs.close();
	ofs.open(getConfig("newOffsetLibPath"));
	if (!ofs){
		LogError("create ofstream newOffsetLibPath failed");
		return;
	}

	for (const auto & item : _offsetLib){
		ofs << item.first << ' ' << item.second.first
			<< ' ' << item.second.second << "\n";
	}
	ofs.close();
	ofs.open(getConfig("invertIndexTablePath"));
	if (!ofs){
		LogError("create ofstream invertIndexTablePath failed");
		return;
	}

	for (const auto & item : _invertIndexTable){
		ofs << item.first << ": ";
		for (const auto & pair : item.second){
			ofs << pair.first << ' '
				<< pair.second << "  ";
		}
		ofs << "\n";
	}
}

}
