
#ifndef __MYSP_PAGELIB_H__
#define __MYSP_PAGELIB_H__

#include <string>
#include <vector>
#include <map>

#include "tinyxml2.h"

using namespace std;

namespace mysp
{

class PageLib{
	public:
		void create();
		void store();
	private:
		void getFiles();
		void parseRss(const string &);
		void addEssay(const tinyxml2::XMLElement *);
	private:
		vector<string> _files;
		vector<string> _ripePage;
		map<int, pair<size_t, size_t>> _offset;
};

}

#endif
