//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>

using namespace std;
using namespace boost;

unordered_map<string, map<int,int> >&  getindexedmap();
vector<int>& search(string& input, unordered_map<string, map<int, int> >& haystack);
vector<string>& getParsedLineDocVector(); //defined in  ../../dsalog/src/parser.cpp


namespace http {
namespace server {

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

void request_handler::handle_request(const request& req, reply& rep)
{
	// Decode url to path.
	std::string request_path;
	if (!url_decode(req.uri, request_path))
	{
		rep = reply::stock_reply(reply::bad_request);
		return;
	}

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
			|| request_path.find("..") != std::string::npos)
	{
		rep = reply::stock_reply(reply::bad_request);
		return;
	}

	// If path ends in slash (i.e. is a directory) then add "index.html".

	if (request_path[request_path.size() - 1] == '/')
	{
		request_path += "index.html";

		// Determine the file extension.
		std::size_t last_slash_pos = request_path.find_last_of("/");
		std::size_t last_dot_pos = request_path.find_last_of(".");
		std::string extension;
		if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
		{
			extension = request_path.substr(last_dot_pos + 1);
		}

		// Open the file to send back.
		std::string full_path = doc_root_ + request_path;
		std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
		if (!is)
		{
			rep = reply::stock_reply(reply::not_found);
			return;
		}

		// Fill out the reply to be sent to the client.
		rep.status = reply::ok;
		char buf[512];
		while (is.read(buf, sizeof(buf)).gcount() > 0)
			rep.content.append(buf, is.gcount());
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = std::to_string(rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type(extension);
	} 
	else if( strncmp(request_path.c_str(), "/search", 7) == 0)
	{
		//rep.content = /*"[ { \"display\": \"Nitin\", \"url\": \"localhost:8080/default.asp\" }, { \"display\": \"Kumar\", \"url\": \"http://www.w3schools.com/html/default.asp\" }, { \"display\": \"Sharma\", \"url\": \"http://www.w3schools.com/css/default.asp\" } ]";*/
		//cout << "requsted path : " << request_path.c_str() << " uri : " << req.uri << endl;
		string searchstr =(request_path.c_str()+10); 
		cout << "searched: " << searchstr.c_str() << endl;

		unordered_map<string, map<int, int> >& lindex = getindexedmap();
		vector<int>& res = search(searchstr, lindex);
		cout << "result with : " << res.size() << "outputs" << endl;
		
		rep.content = "";
		bool first = true;
		vector<string>& vec = getParsedLineDocVector();
		for(auto& x: res)
		{
			if (rep.content.empty() == false)
				rep.content += ",";
			rep.content += string("{ \"display\": ") + string("\"") + vec[x] + string("\" }");
		}

		rep.content = "[" + rep.content + "]";

		cout << "result : " << rep.content.c_str() << endl;
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = std::to_string(rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = "json"; //mime_types::extension_to_type();
	}
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace http
