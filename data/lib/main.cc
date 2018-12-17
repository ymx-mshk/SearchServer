
#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

int main(void){
	vector<int> ivec{1, 3, 3, 3, 3, 7, 1, 2, 4, 3, 7, 8, 2, 0, 0, 5, 1, 8, 9, 5};

	for (auto it1 = ivec.begin(); it1 != ivec.end(); ++it1){
		cout << "size = " << ivec.end() - ivec.begin() << endl;

		for (auto it2 = it1 + 1; it2 != ivec.end();){
			if (*it1 == *it2){
				ivec.erase(it2);
			}else 
				++it2;
		}
	}

	copy(ivec.begin(), ivec.end(), ostream_iterator<int>(cout, "  "));

	cout << endl;
}
