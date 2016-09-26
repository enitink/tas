#include <string>
#include <map>
#include <unordered_map>
#include <boost/tokenizer.hpp>
#include <iostream>

using namespace std;
using namespace boost;

vector <int> result;

vector<int>& search(string& input, unordered_map<string, map<int,int> >& haystack)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	tokenizer tok{input};
	result.clear();
	bool isfirst = true;
	map<int,int> prev;
	for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
	{
		map<int,int>& ldata = haystack[*it];
		if (ldata.size() == 0)
			return result;
		if (isfirst)
		{
			prev = ldata;
			isfirst = false;
		}
		else
		{
			map<int,int> lprev = prev;
			prev.clear();
			for(auto& y: lprev)
			{
				map<int,int>::iterator it = ldata.find(y.first);
				if ( (it != ldata.end()) && (it->second == y.second + 1) )
					prev[y.first] = it->second;
			}
		}
		//cout << *it << " : ";
		//for(auto& xy: prev)
		//	cout << xy.first << "-" << xy.second << ":";
		//cout << endl;
	}

	for(auto& z: prev)
		result.push_back(z.first);
	return result;
}
