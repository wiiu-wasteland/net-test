#include "app.h"
#include "console.h"
#include "network.h"

#include <curl/curl.h>

const char *url = "http://speedtestts1.telecomitalia.it:8080/speedtest/random4000x4000.jpg";

int curlEnableOptimizations(void *clientp, curl_socket_t curlfd, curlsocktype purpose)
{
	int res;
#ifdef SO_WINSCALE
#pragma message("SO_WINSCALE present!")
	int winscale = 1;
	res = setsockopt(curlfd, SOL_SOCKET, SO_WINSCALE, &winscale, sizeof(winscale));
	if(res != 0)
		console_write("error: setting SO_WINSCALE failed\n");
#else
	int winscale = 0;
#endif
#ifdef SO_TCPSACK
#pragma message("SO_TCPSACK present!")
	int tcpsack = 1;
	res = setsockopt(curlfd, SOL_SOCKET, SO_TCPSACK, &tcpsack, sizeof(tcpsack));
	if(res != 0)
		console_write("error: setting SO_TCPSACK failed\n");
#endif
#ifdef SO_RCVBUF
#pragma message("SO_RCVBUF present!")
	int rcvbuf = (winscale) ? 131072 : 65535;
	res = setsockopt(curlfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));
	if(res != 0)
		console_write("error: setting SO_RCVBUF failed\n");
#endif
	return 0;
}


size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	(void)ptr;
	(void)data;
	return (size_t)(size * nmemb);
}

void performTest(CURL *curl_handle)
{
	double val;
	CURLcode res = curl_easy_perform(curl_handle);
	
	if(res != CURLE_OK)
	{
		console_write("error: curl_easy_perform: %s\n", curl_easy_strerror(res));
		return;
	}
	
	/* check for bytes downloaded */
	res = curl_easy_getinfo(curl_handle, CURLINFO_SIZE_DOWNLOAD, &val);
	if((CURLE_OK == res) && (val > 0))
		console_write("Data downloaded: %0.0f bytes.\n", val);
	
	/* check for total download time */
	res = curl_easy_getinfo(curl_handle, CURLINFO_TOTAL_TIME, &val);
	if((CURLE_OK == res) && (val > 0))
		console_write("Total download time: %0.3f sec.\n", val);
	
	/* check for average download speed */
	res = curl_easy_getinfo(curl_handle, CURLINFO_SPEED_DOWNLOAD, &val);
	if((CURLE_OK == res) && (val > 0))
		console_write("Average download speed: %0.3f kbyte/sec.\n", val / 1024);
	
	/* check for name resolution time */
	res = curl_easy_getinfo(curl_handle, CURLINFO_NAMELOOKUP_TIME, &val);
	if((CURLE_OK == res) && (val > 0))
		console_write("Name lookup time: %0.3f sec.\n", val);
	
	/* check for connect time */
	res = curl_easy_getinfo(curl_handle, CURLINFO_CONNECT_TIME, &val);
	if((CURLE_OK == res) && (val > 0))
		console_write("Connect time: %0.3f sec.\n", val);
}

void testCurlDownloadDefault()
{
	console_write(":: 1. Performing curl download speed test without optimizations...\n");
	
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-speedchecker/1.0");	
	
	performTest(curl);
	
	curl_easy_cleanup(curl);
}

void testCurlDownloadOptimized()
{
	console_write(":: 2. Performing curl download speed test with optimizations...\n");
	
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, curlEnableOptimizations);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-speedchecker/1.0");	
	
	performTest(curl);
	
	curl_easy_cleanup(curl);
}

int main(int argc, char **argv)
{
	app_init();
	console_init();
	network_init();
	curl_global_init(CURL_GLOBAL_ALL);
   
	console_write(":: Starting networking tests!\n");
	
	testCurlDownloadDefault();
	testCurlDownloadOptimized();
	
	console_write(":: Networking tests finished!\n");
	console_write(":: Waiting user input to exit...\n");
	
	while(app_running());
	
	curl_global_cleanup();
	network_exit();
	console_exit();
	app_exit();

	return 0;
}
