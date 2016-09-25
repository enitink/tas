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

vector <int> result;

vector<int>& search(string& input, unordered_map<string, vector<vecdata> >& haystack)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	tokenizer tok{input};
	result.clear();
	for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
	{
	}

	return result;
}
