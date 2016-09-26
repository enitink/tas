#include <string>
#include <unordered_map>
#include <map>
#include <boost/tokenizer.hpp>
#include <iostream>
//#include <global.h>

using namespace std;
using namespace boost;

unordered_map<string, map<int, int> > indexedmap;

void prepareIndex(vector<string>& input)
{
	for(int i=0; i<input.size(); ++i)
	{
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		tokenizer tok{input[i]};
		int wordposition = 0;
		for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
		{
			(indexedmap[*it])[i] = wordposition++;
		}
	}

	/*for (auto& x: indexedmap) {
		std::cout << x.first << ": " ;
		for(auto& y: x.second)
			cout << y.vecDocCounter << "-" << y.wordposition << ":";
		cout << endl;
	}*/
}

unordered_map<string, map<int, int> >&  getindexedmap()
{
	return indexedmap;
}
