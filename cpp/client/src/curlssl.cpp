#include <iostream>
#include <sstream>
#include <cstdio>
#include <curl/curl.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
	std::ostringstream *stream = (std::ostringstream*)userdata;
	size_t count = size * nmemb;
	stream->write(ptr, count);
	return count;
}

bool getPage(char* page, string& output)
{
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if(curl) {
	std::ostringstream stream;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
    curl_easy_setopt(curl, CURLOPT_URL, page);

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
  
	output = stream.str();
	//cout << "*******************************************************************************" << endl;
	//cout << str.c_str();
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return true;
}
