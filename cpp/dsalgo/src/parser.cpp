#include <set>
#include <vector>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

string currline;
string currtag;
bool isClosed = true;

vector<string> parsedVector;
set<string> linelist;
void filllinelist()
{
	string list[] = {"p", "a", "tr", "h1", "h2", "h3", "h4", "h5", "h6", "title", "li"};
	linelist.insert(list, list + 11);
}

void characters_callback(void * ctx, const xmlChar * ch, int len)
{
	string c((const char*)ch, len);
	trim(c);
	if (c.size() == 0)
		return;
	if (currline.size() != 0)
		currline += " ";
	currline += c;
}

void start_element_callback(void *user_data, const xmlChar *name, const xmlChar **attrs) {
	string n((const char*)name);
	transform(n.begin(), n.end(), n.begin(), ::tolower);
	if (linelist.find(n) != linelist.end() && isClosed)
	{
		currline = "";
		currtag = n;
		isClosed = false;
	}
}

void end_element_callback(void * ctx, const xmlChar * name)
{
	string n((const char*)name);
	transform(n.begin(), n.end(), n.begin(), ::tolower);
	if ((linelist.find(n) != linelist.end()) && (currtag == n))
	{
		isClosed = true;
		std::size_t found = currline.find('.');
		if (found!=std::string::npos)
		{
			char_separator<char> sep(".");
			tokenizer<char_separator<char> > tok(currline,sep);
			for(tokenizer<char_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
				parsedVector.push_back(*beg);
			}
		}
		else
			parsedVector.push_back(currline);
	}
}

vector<string>& getParsedLineDocVector()
{
	return parsedVector;
}

bool prepareLineDocs(string& input)
{
	filllinelist();
	
	// Initialize all fields to zero
	xmlSAXHandler sh = { 0 };
	// register callback
	sh.startElement = start_element_callback;
	sh.endElement = end_element_callback;
	sh.characters = characters_callback;
	xmlParserCtxtPtr ctxt;

	if ((ctxt = xmlCreateMemoryParserCtxt(input.c_str(), input.size())) == NULL) {
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

	return true;
}
