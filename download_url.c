/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>
 
typedef struct {
	char *text_data;
	int counter;
}d_html_data;

int
curl_data_colector(char *curl_data, size_t size, size_t nmemb, d_html_data *structure)
{ 
	/* nmemb is supposed to be strlen of curl_data but, but in last pass its lesser!!! */
	
	structure->counter += strlen(curl_data);
	
	structure->text_data = realloc(structure->text_data, structure->counter + 1);
	
	if(structure->text_data == NULL) {
		fprintf(stderr, "Not enough memory for storing html data.\n");
		return 0;
	}
	
	strcpy(&structure->text_data[(structure->counter-strlen(curl_data))], curl_data);
	
	return size * nmemb;		/* forced by curl specs */
}
 
 
int download_url(char *url, char *user_agent, char *cookies, char **result)
{
	CURL *curl_handle;
	CURLcode res;
	d_html_data s_html_data;
	
	s_html_data.text_data = malloc(1); /* we'll be realloc-ing */
	s_html_data.counter = 0;
	
	
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();		/* init the curl session */ 
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);		/* specify URL to get */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_data_colector);		/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &s_html_data);			/* output of data collector function */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);			/* set user agent */
	
	if(NULL != cookies) {
		curl_easy_setopt(curl_handle, CURLOPT_COOKIE, cookies);
	}
	
	res = curl_easy_perform(curl_handle);		/* get it */
	
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));
		return -1;
	}

  curl_easy_cleanup(curl_handle);		/* cleanup curl stuff */ 
  curl_global_cleanup();		/* we're done with libcurl, so clean it up */ 
  
  
	/* to get actuall data out */
	
	*result = malloc(1 + s_html_data.counter);
	strcpy(*result, s_html_data.text_data);

	free(s_html_data.text_data);

	return 0;
}