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

#include "download_url.h"
#include "parse_arguments.h"
#include "read_file.h"
#include "parse_config.h"
#include "search_and_get.h"
#include "find_and_replace.h"
#include "get_param.h"
#include "find_nearest.h"


char *html_data = NULL;
char *url = NULL;
char *config_file = NULL;
char *url_identifier = NULL;
char *jump = NULL;
char *config_file_chunk = NULL;
char *search_a = NULL;
char *search_b = NULL;
char *result = NULL;
char *preppend = NULL;
char *append = NULL;
char *cookie_temp = NULL;
char *cookies = NULL;
char *find = NULL;
char *replace = NULL;
char *execute = NULL;
char *execute_with_parameters = NULL;
char *result_with_append_preppend = NULL;
char *result_temp = NULL;

void cleanup_func()
{
	if(html_data != NULL) free(html_data);
	if(url != NULL) free(url);
	if(config_file != NULL) free(config_file);
	if(url_identifier != NULL) free(url_identifier);
	if(jump != NULL) free(jump);
	if(config_file_chunk != NULL) free(config_file_chunk);
	if(search_a != NULL) free(search_a);
	if(search_b != NULL) free(search_b);
	if(result != NULL) free(result);
	if(preppend != NULL) free(preppend);
	if(append != NULL) free(append);
	if(cookie_temp != NULL) free(cookie_temp);
	if(cookies != NULL) free(cookies);
	if(find != NULL) free(find);
	if(replace != NULL) free(replace);
	if(execute != NULL) free(execute);
	if(execute_with_parameters != NULL) free(execute_with_parameters);
	if(result_with_append_preppend != NULL) free(result_with_append_preppend);
	if(result_temp != NULL) free(result_temp);
}

int main(int argc, char *argv[])
{
	int verbosity =0;
	int download =0;
	int execute_allow = 0;
	char *user_agent="Mozilla/5.0 (X11; Linux i686; rv:19.0) Gecko/20100101 Firefox/19.0";
	char *config_file_path = "/home/henk/prace/search_funkce/clean/definitions.txt";
	char *p_config_file;
	char *p_html_data;
	char *p_html_data_get;
	

	/* parse arguments */
	if(0 != parse_arguments(argc, argv, &verbosity, &download, &execute_allow, &url)) {
		cleanup_func();
		return 1;
	}
	
	if(url == NULL) {
		fprintf(stderr, "No url specified.\n");
		cleanup_func();
		return 1;
	}
	
	
	/* config file */
	if(0 != read_file(config_file_path, &config_file)) {
		cleanup_func();
		return 1;
	}
	
	if(0 != parse_config(&config_file)) {
		cleanup_func();
		return 1;
	}
	if(verbosity == 1) printf("Using config file: \"%s\".\n", config_file_path);		/* VERBOSITY */
	
	
	/* choose valid url identifier from config */
	p_config_file = config_file;
	
	while( 0 == search_and_get(&p_config_file, "identifier=\"", "\"", &url_identifier)) { 	//scan all identifiers in config file
		find_and_replace(&url_identifier, "\\\"", "\"");
		if(NULL != strstr(url,url_identifier)) {		//if any matches url
			break;
		}
	}

	if(url_identifier == NULL) {
		fprintf(stderr, "No url identifier found in config file.\n");
		cleanup_func();
		return 1;
	}
	
	if( NULL == strstr(url,url_identifier)) {
		fprintf(stderr, "No url identifier for \"%s\" found in config file.\n", url);
		cleanup_func();
		return 1;
	}
	if(verbosity == 1) printf("Using identifier: \"%s\".\n\n");		/* VERBOSITY */

	/* now we have found identifier in config file, lets take only right section inbetween {} */
	if( 0 != search_and_get(&p_config_file, "{", "}", &config_file_chunk)) {		//NO, we cant reuse config_file because we are reading from it! :-)
		fprintf(stderr, "Error in config file (identifier: \"%s\").\n", url_identifier);
		cleanup_func();
		return 1;
	}
	
	
	/* get cookies from config */
	p_config_file = config_file_chunk;

	if( NULL != strstr(config_file_chunk, "cookie")) {		
		cookies = malloc(1);		//we'll be realloc-ing
		cookies[0] = '\0';		//we'll be strcat-ing
		
		while( 0 == search_and_get(&p_config_file, "cookie=\"","\"", &cookie_temp)) {		//read all cookies from config
			find_and_replace(&cookie_temp, "\\\"", "\"");
			if(verbosity == 1) printf("Found cookie: \"%s\".\n", cookie_temp);		/* VERBOSITY */
			cookies = realloc(cookies, strlen(cookies) + strlen(cookie_temp)+3);		// 1 for terminating null, 2 for ;<whitespace>
			strcat(cookies, cookie_temp);
			strcat(cookies, "; ");
			}
		}
		
	
	/* download url */
	if(0 != download_url(url, user_agent, cookies, &html_data)) {
		cleanup_func();
		return 1;
	}
	
	/* d switch */
	if( download == 1 && verbosity == 1) {
		printf("%s", html_data);
	}
	else if( download == 1) {
		printf("%s", html_data);
		cleanup_func();
		return 0;
	}
	
	/*********************
	* main program logic *
	*********************/
	
	/* jump handling section */
	p_config_file = config_file_chunk;
	p_html_data = html_data;
	if( NULL != strstr(config_file_chunk, "jump")) {
		while( 0 == search_and_get(&p_config_file, "jump=\"","\"", &jump)) {		//read all jumps from config
			find_and_replace(&jump, "\\\"", "\"");			
			p_html_data= strstr(p_html_data, jump);		//search jump in html data
			if(p_html_data == NULL) {
				p_html_data = html_data;
				fprintf(stderr, "Jump: \"%s\" not found in html source.\n", jump);
				continue;
			}
			if(verbosity == 1) printf("Jump: \"%s\" succesfully found in html source.\n");		/* VERBOSITY */
		}
	}
	
	
	/* prepare variables for params and search_and_get */
	result = malloc(1);		//we'll be realloc-ing
	result[0] = '\0';		//we'll be strcat-ing
	
	
	/* get get_a, get_b from config file */
	p_config_file = config_file_chunk;		//reset position (order in config could be wrong)
	if(NULL != strstr(p_config_file, "get_a=\"")) {
		while(0 == search_and_get(&p_config_file, "get_a=\"", "\"", &search_a)) {
			find_and_replace(&search_a, "\\\"", "\"");
			
			if(NULL != strstr(p_config_file, "get_b=\"")) {
				search_and_get(&p_config_file, "get_b=\"", "\"", &search_b);
				find_and_replace(&search_b, "\\\"", "\"");
				
				/* we need nearest occurence of search_b from search_a in html */
				p_html_data_get = strstr(p_html_data, search_a);		//need new variable
				if( NULL == p_html_data_get ) {
					fprintf(stderr, "\"%s\" not found in HTML source.\n", search_a);
					continue;
				}
				p_html_data_get += strlen(search_a);			//skip searched
				if( 0 != find_nearest(p_html_data_get, &search_b)) {
					fprintf(stderr, "Failed to parse: \"%s\".\n", search_b);
					cleanup_func();
					return 1;
				}
				find_and_replace(&search_b, "\\,", ",");
				
				if( 0 == get_param(p_html_data, search_a, search_b, &result_temp)) {
					result = realloc(result, (strlen(result) + strlen(result_temp)) * sizeof(char) + 1);
					strcat(result, result_temp);
					if(verbosity == 1) printf("Succesfully got text: \"%s\"\ninbetween \"%s\", \"%s\"\n",
					result_temp, search_a, search_b);		/* VERBOSITY */
				}
			}
			else {
				break;
			}
		}
	}
	
	
	/* get search_a, search_b from config file */
	p_config_file = config_file_chunk;		//reset position (order in config could be wrong)
	if(NULL != strstr(p_config_file, "search_a=\"")) {
		while(0 == search_and_get(&p_config_file, "search_a=\"", "\"", &search_a)) {
			find_and_replace(&search_a, "\\\"", "\"");
			
			if(NULL != strstr(p_config_file, "search_b=\"")) {
				search_and_get(&p_config_file, "search_b=\"", "\"", &search_b);
				find_and_replace(&search_b, "\\\"", "\"");
				
				if( 0 == search_and_get(&p_html_data, search_a, search_b, &result_temp)) {
					result = realloc(result, (strlen(result) + strlen(result_temp)) * sizeof(char) + 1);
					strcat(result, result_temp);
					if(verbosity == 1) printf("Succesfully got text: \"%s\"\ninbetween \"%s\", \"%s\"\n",
					result_temp, search_a, search_b);		/* VERBOSITY */
				}
			}
			else {
				break;
			}
		}
	}
	
	/* since none of 2 previous functions are requierd, it could happen none will execute */
	if( 0 == strlen(result)) {
		fprintf(stderr, "The result is empty, aborting.\n");
		cleanup_func();
		return 1;
	}
	

	/* filter result */
	p_config_file = config_file_chunk;		//reset position (order in config could be wrong)
	if(NULL != strstr(p_config_file, "find=\"")) {
		while(0 == search_and_get(&p_config_file, "find=\"", "\"", &find)) {
			find_and_replace(&find, "\\\"", "\"");
			
			if(NULL != strstr(p_config_file, "replace=\"")) {
				search_and_get(&p_config_file, "replace=\"", "\"", &replace);
				find_and_replace(&replace, "\\\"", "\"");
				
				if( 0 == find_and_replace(&result, find, replace)) {
					if(verbosity == 1) printf("Succesfully replaced all \"%s\" by \"%s\".", find, replace);		/* VERBOSITY */
				}
			}
			else {
				break;
			}
		}
	}

	/* preppend and append */
		/* get preppend, append from config file */
	p_config_file = config_file_chunk;		//reset position (order in config could be wrong)
	if( NULL != strstr(config_file_chunk, "preppend")) {
		if( 0 != search_and_get(&p_config_file, "preppend=\"", "\"", &preppend)) {
			fprintf(stderr, "Failed to get PREPPEND from config.\n");
			cleanup_func();
			return 1;
		}
	}
	
	p_config_file = config_file_chunk;		//reset position (order in config could be wrong)
	if( NULL != strstr(config_file_chunk, "append")) {
		if( 0 != search_and_get(&p_config_file, "append=\"", "\"", &append)) {
			fprintf(stderr, "Failed to get APPEND from config.\n");
			cleanup_func();
			return 1;
		}
	}
	
	/* geting both preppend and append */
	if( preppend != NULL && append != NULL ) {
		result_with_append_preppend = malloc(1 +(strlen(result) + strlen(append) + strlen(preppend)) * sizeof(char));
		sprintf(result_with_append_preppend, "%s%s%s", preppend, result, append);
		if(verbosity == 1) printf("Succesfully added \"%s\" and \"%s\" to result.\n", preppend, append);		/* VERBOSITY */
	}
	
	/* or only preppend */
	else if( preppend != NULL) {
		result_with_append_preppend = malloc(1 +(strlen(result) + strlen(preppend)) * sizeof(char));
		sprintf(result_with_append_preppend, "%s%s", preppend, result);
		if(verbosity == 1) printf("Succesfully added \"%s\" to result.\n", preppend);		/* VERBOSITY */
	}
	
	/* or only append */
	else if( append != NULL) {
		result_with_append_preppend = malloc(1 +(strlen(result) + strlen(append)) * sizeof(char));
		sprintf(result_with_append_preppend, "%s%s", result, append);
		if(verbosity == 1) printf("Succesfully added \"%s\" to result.\n", append);		/* VERBOSITY */
	}
	
	/* no append and preppend? */
	if(NULL == result_with_append_preppend) {
		result_with_append_preppend = malloc(1 + strlen(result));
		strcpy(result_with_append_preppend, result);
	}
	
	
	/* get execute */
	if (1 == execute_allow) {
		p_config_file = config_file_chunk;		//reset position (order in config could be wrong)
		if( NULL != strstr(config_file_chunk, "execute=\"")) {
			if( 0 != search_and_get(&p_config_file, "execute=\"", "\"", &execute)) {
				fprintf(stderr, "Failed to get EXECUTE from config.\n");
				cleanup_func();
				return 1;
			}
		}
	}
	
	/* do we want to execute and is there something to execute? */
	if( 1 == execute_allow && NULL != execute) {
		execute_with_parameters = malloc((3 + strlen(execute) + strlen(result_with_append_preppend)) * sizeof(char));
		sprintf(execute_with_parameters, "%s\"%s\"", execute, result_with_append_preppend);
		printf("Launching: %s\n", execute_with_parameters);
		system(execute_with_parameters);
		cleanup_func();
		return 0;
	}
	
	if( 1 == execute_allow ) {
		fprintf(stderr, "Nothing to execute.\n");
		cleanup_func();
		return 1;
	}
	
	printf("%s\n", result_with_append_preppend);
	
	/* cleanup */
	cleanup_func();
	return 0;
}