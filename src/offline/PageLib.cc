
#include "Mylogger.h"
#include "Configuration.h"
#include "DirScanner.h"
#include "PageLib.h"

#include <fstream>
#include <regex>
#include <boost/algorithm/string.hpp>

using boost::replace_all;
using namespace tinyxml2;

namespace mysp
{

void PageLib::create(){
	getFiles();

	for (const string & file : _files){
		parseRss(file);
	}
}

void PageLib::store(){
	ofstream ofs(getConfig("ripePageLibPath"));
	if (!ofs){
		LogError("create ofstream ripePageLibPath failed");
		return;
	}
	
	int docid = 1;
	for (const auto & text : _ripePage){
		_offset.emplace(docid++, make_pair(ofs.tellp(), text.size()));
		ofs << text;
	}

	ofs.close();
	ofs.open(getConfig("offsetLibPath"));
	if (!ofs){
		LogError("create ofstream offsetLibPath failed");
		return;
	}

	for (const auto & item: _offset){
		ofs << item.first << ' ' << item.second.first
			<< ' ' << item.second.second << "\n";
	}

	ofs.close();
}

void PageLib::getFiles(){
	DirScanner dirScanner;
	dirScanner(getConfig("rssSourcePath"), _files);
	//DirScanner::getAllFiles(getConfig("rssSourcePath"), _files);
}

void PageLib::parseRss(const string & file){
	XMLDocument doc;
	XMLError errXml = doc.LoadFile(file.c_str());
	if (XML_SUCCESS != errXml){
		LogWarn("xml loadfile \"%s\" failed", file.c_str());
		return;
	}

	XMLElement * elmtRoot = doc.RootElement();
	XMLElement * elmtChannel = elmtRoot->FirstChildElement("channel");

	if (elmtChannel){
		XMLElement * elmtItem = elmtChannel->FirstChildElement("item");
		while (elmtItem){
			addEssay(elmtItem);
			elmtItem = elmtItem->NextSiblingElement("item");
		}
	}
}

void PageLib::addEssay(const XMLElement * elmtItem){
	string title, link, content;
	const XMLElement * elmtTitle = elmtItem->FirstChildElement("title");

	//没有内容的话GetText()会返回0，string不能用0构造
	//所以要先判断GetText()的返回值
	if (elmtTitle && elmtTitle->GetText()){
		title = elmtTitle->GetText();
	}

	const XMLElement * elmtLink = elmtItem->FirstChildElement("link");
	if (elmtLink && elmtLink->GetText()){
		link = elmtLink->GetText();
	}

	if (!link.size())
		return;

	const XMLElement * elmtContent = elmtItem->FirstChildElement("content:encoded");
	if (!elmtContent){
		elmtContent = elmtItem->FirstChildElement("description");
	}
	if (elmtContent && elmtContent->GetText()){
		string s = elmtContent->GetText();
		regex r1("<.*?>");
		s = regex_replace(s, r1, "");
		replace_all(s, "&nbsp;", " ");
		replace_all(s, "&amp;", "&");
		replace_all(s, "&lt;", "<");
		replace_all(s, "&gt;", ">");
		replace_all(s, "&apos;", ";");
		replace_all(s, "&quot;", "\"");

		//没有title的话用content的第一句作为title
		if (!title.size()){
			title = string(s, 0, s.find("\xe3\x80\x82"));//句号
		}

		content = move(s);
	}

	static int docid = 1;
	_ripePage.push_back("<doc><docid>" + to_string(docid++)
			+ "</docid><url>" + link + "</url><title>" + title 
			+ "</title><content>" + content + "</content></doc>");
}

}
