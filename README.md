# tas Project

All the following inside the cpp folder. Project uses some feature of c++11,boost,boost/asio,libxml sax, libcurl.

Dependecies : boost_1_61_0, asio-1.11.0, libxml, libcurl.
              cpp/client/src/Makefile.am uses libcurl
              cpp/dsalgo/src/Makefile.am uses boost, and libxml
              cpp/server/src/Makefile.am uses boost, asio in standalone mode.

Usage: Binary is in ./bin folder
        ./server 0.0.0.0 8080 docroot http://en.wikipedia.org/wiki/Filmfare_Award_for_Best_Actor

** NOT added support for case insensitivity. Search doesn't ignores case of words.

Folder descriptions :
        client : Client is used to fetch the document for the given link. It uses libCurl.
        dsalgo : The document parser, helper to prepare index and search logic part of this folder.
                         dsalgo/src/parser.cpp : implementation of the document parser. Pushes lines based on tags like <p>,<a>,<tr>,<h1..6> etc and if a Paragraph contains "." in between that is also converted to multiple line. Each line thus got is pused into a vector <string>. Uses libXml2 SAX parser.
                         dsalgo/src/prepareindex.cpp : Uses each row of vector <string> prepared previously by parser.cpp, to form a inverted index for each word. Each word, has vector row and it's position in that row, attached to it using an unordered_map.
                         dsalgo/src/searchterm.cpp : Each word from the search line is searched first in the inverted index created by prepareindex.cpp and compared with next occuring word to get the documents they belong and check if their sequence matches that of searched string or not.
        server : HTTP server code picked from - http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/examples/cpp11_examples.html
                        Modified to support search queries. In case of search query is made a JSON response is sent.
        server/src/docroot : This folder contains the index.html page. Which contains two input tags to lookup a string. Result is fetched using AJAX framework, response is in JSON format. Java script used to get data from the JSON response recieved and to display in the same page.


Limitations :
        1. Seaches are not case insensitive.

Bugs:
        1. Document parsing done based on html tags is not extensive. Many times the lines formed out of doc is not proper.
        2. SAX parser not properly written to handle errors html pages. If any document not well formed. It can cause server crash.
        3. If result contains lots of rows with long lines, the output is failing to show in browser.
