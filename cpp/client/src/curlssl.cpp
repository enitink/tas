#include <iostream>
#include <sstream>
#include <cstdio>
#include <curl/curl.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <set>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/erase.hpp>

//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
using namespace std;
using namespace boost::algorithm;

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
	std::ostringstream *stream = (std::ostringstream*)userdata;
	size_t count = size * nmemb;
	stream->write(ptr, count);
	return count;
}

bool ignore = false;
int i = 0;
string prevtag;
string currtag;
string currline;

set<string> linelist;
void filllinelist()
{
	string list[] = {"p", "tr", "h1", "h2", "h3", "h4", "h5", "h6", "title", "li"};
	linelist.insert(list, list + 10);
	set<string>::iterator it;
	it = linelist.find(string("p"));
	if (it != linelist.end())
		printf("found p\n");
}

void characters_callback(void * ctx, const xmlChar * ch, int len)
{
	//if (ignore)
	//	return;
	string c((const char*)ch, len);
	trim(c);
	if (c.size() == 0)
		return;
	if (currline.size() != 0)
		currline += " ";
	currline += c;
	//printf("<%s>", currtag.c_str());
	//printf("%s\n", c.c_str());
	//printf("</%s>\n", currtag.c_str());
}

set<string> ignorelist;// (string("p"),string("a"));
void filluseset()
{
	string un[] = {"script"};
	ignorelist.insert(un,un+1);
}
void start_element_callback(void *user_data, const xmlChar *name, const xmlChar **attrs) {
	/*++i;
	for (int j = 0; j < i; ++j)
		printf("\t");
	printf("start < %s > \n", name);*/
	string n((const char*)name);
	transform(n.begin(), n.end(), n.begin(), ::tolower);
	/*if (ignorelist.find(n) != ignorelist.end())
	{
		ignore = true;
		prevtag = n;
	}
	currtag = n;*/
	if (linelist.find(n) != linelist.end())
	{
		currline = "";
	//	printf("start < %s > \n", n.c_str());
	}
}

void end_element_callback(void * ctx, const xmlChar * name)
{
	/*for (int j = 0; j < i; ++j)
		printf("\t");
	printf("end < %s >\n", name);
	ignore = true;
	--i;*/
	string n((const char*)name);
	/*if (n == prevtag)
	{
		ignore = false;
		prevtag="";
	}*/
	if (linelist.find(n) != linelist.end())
	{
		printf("%s\n", currline.c_str());
	//	printf("end < %s > \n", n.c_str());
	}
}

int main(void)
{
  filluseset();
  filllinelist();
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if(curl) {
	std::ostringstream stream;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
    curl_easy_setopt(curl, CURLOPT_URL, "http://en.wikipedia.org/wiki/Filmfare_Award_for_Best_Actor");

#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
  
	// Initialize all fields to zero
	xmlSAXHandler sh = { 0 };
	// register callback
	sh.startElement = start_element_callback;
	sh.endElement = end_element_callback;
	sh.characters = characters_callback;
	xmlParserCtxtPtr ctxt;
	string str = stream.str();
	//erase_all(str,"<br>");
	//erase_all(str,"</br>");

	if ((ctxt = xmlCreateMemoryParserCtxt(str.c_str(), str.size())) == NULL) {
		fprintf(stderr, "Erreur lors de la cration du contexte\n");
		return EXIT_FAILURE;
	}
	// register sax handler with the context
	ctxt->sax = &sh;

	// parse the doc
	xmlParseDocument(ctxt);
	// well-formed document?
	if (ctxt->wellFormed) {
		printf("XML Document is well formed\n");
	} else {
		fprintf(stderr, "XML Document isn't well formed\n");
		//xmlFreeParserCtxt(ctxt);
		return EXIT_FAILURE;
	}
	//cout << "*******************************************************************************" << endl;
	//cout << str.c_str();
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return 0;
}
