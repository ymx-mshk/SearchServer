
#include <string>
#include <fstream>
#include <iostream>

#include <map>

using namespace std;

int main(void){

	ifstream ifs("ripepage.lib");
	if (!ifs){
		return 1;
	}

	map<int, pair<size_t, size_t>> offset;
	offset.emplace(1, make_pair(0, 10));
	offset.emplace(2, make_pair(10, 20));
	offset.emplace(3, make_pair(30, 18));

	for (const auto & item : offset){
		size_t size = item.second.second;
		string doc(size, '\0');

		ifs.read(&doc[0], size);

		cout << doc << endl;
	}
}
