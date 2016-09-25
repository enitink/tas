#include <string>
#include <unordered_map>
#include <boost/tokenizer.hpp>
#include <iostream>

using namespace std;
using namespace boost;

typedef struct
{
	int vecDocCounter;
	int wordposition;
}vecdata;

unordered_map<string, vector<vecdata> > indexedmap;

void prepareIndex(vector<string>& input)
{
	for(int i=0; i<input.size(); ++i)
	{
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		tokenizer tok{input[i]};
		int wordposition = 0;
		for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
		{
			vecdata temp;
			temp.vecDocCounter = i;
			temp.wordposition = wordposition++;
			indexedmap[*it].push_back(temp);
		}
	}

	/*for (auto& x: indexedmap) {
		std::cout << x.first << ": " ;
		for(auto& y: x.second)
			cout << y.vecDocCounter << "-" << y.wordposition << ":";
		cout << endl;
	}*/
}

unordered_map<string, vector<vecdata> >&  getindexedmap()
{
	return indexedmap;
}
