//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <asio.hpp>
#include "server.hpp"
#include <unordered_map>
#include <map>
#include <set>
#include <vector>

using namespace std;
using namespace boost;

bool getPage(char*, string&); //defined in ../../client/src/curlssl.cpp
bool prepareLineDocs(string&); //defined in  ../../dsalog/src/parser.cpp
vector<string>& getParsedLineDocVector(); //defined in  ../../dsalog/src/parser.cpp
void prepareIndex(vector<string>&);
unordered_map<string, map<int,int> >&  getindexedmap();
vector<int>& search(string& input, unordered_map<string, map<int, int> >& haystack);

int main(int argc, char* argv[])
{
	string htmlpage;
	if (getPage("http://en.wikipedia.org/wiki/Filmfare_Award_for_Best_Actor", htmlpage) == false)
	{
		cerr << "**Failed to get the page : http://en.wikipedia.org/wiki/Filmfare_Award_for_Best_Actor";
		exit(0);
	}
	prepareLineDocs(htmlpage);
	vector<string>& vec = getParsedLineDocVector();

	/*for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
		cout << *it << endl;*/

	prepareIndex(vec);
	//unordered_map<string, map<int, int> >& lindex = getindexedmap();
	
	/*string sh("List of winners");
	vector<int>& res = search(sh, lindex);
	cout << "Result " << res.size() << endl;
	for(auto& x : res)
		cout << vec[x] << endl;*/
	cout << "Server is ready!!" << endl;
	try
	{
		// Check command line arguments.
		if (argc != 4)
		{
			cerr << "Usage: http_server <address> <port> <doc_root>\n";
			cerr << "  For IPv4, try:\n";
			cerr << "    receiver 0.0.0.0 80 .\n";
			cerr << "  For IPv6, try:\n";
			cerr << "    receiver 0::0 80 .\n";
			return 1;
		}

		// Initialise the server.
		http::server::server s(argv[1], argv[2], argv[3]);

		// Run the server until stopped.
		s.run();
	}
	catch (exception& e)
	{
		cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}
