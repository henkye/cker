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
#include <string.h>
#include <stdlib.h>

#define HELP_MESSAGE "Usage: cker [OPTIONS]\n\n\
  -u, --url [URL]	url to download\n\
  -x, --execute		execute specified program upon completition\n\
  -d, --download	only download and print output to stdout\n\
  -v, --verbose		be verbose for some additional info\n\
  -h, --help		show this message\n\
  \n\
See man cker for more help.\n"


int parse_arguments(int argc, char *argv[], int *verbosity, int *download, int *execute_allow, char **url)
{
	int i = 1;
	int url_taken = 0;
	int verbose_taken = 0;
	int download_taken = 0;
	int execute_taken = 0;

	if (argc == 1) {
		fprintf(stderr, "No parameters.\n");
		printf("%s", HELP_MESSAGE);
		return 1;
	}
	
	while(i != argc) {		// read all arguments
		
		/* searching for url now */
		if( 0 == strcmp(argv[i], "-u") || 0 == strcmp(argv[i], "--url") ) {

			if(1 == url_taken) {
				fprintf(stderr, "Too many url arguments.\n");
				printf("%s", HELP_MESSAGE);
				return 1;
			}
			
			i++;		//skip one parameter after "-u", since url could be anything and it wouldnt pass through our checks
			
			if( i < argc) {		//if theres one more argument (url)
				*url = malloc(strlen(argv[i])+1);
				strcpy(*url, argv[i]);
				url_taken = 1;
			}
			else {
				fprintf(stderr, "No URL specified, aborting.\n");
				printf("%s", HELP_MESSAGE);
				return 1;
			}
		}
		
		/* now we search for verbosity argument */
		else if( 0 == strcmp(argv[i], "-v") || 0 == strcmp(argv[i], "--verbose") ) {
			if(1 == verbose_taken) {
				fprintf(stderr, "Too many verbose arguments.\n");
				printf("%s", HELP_MESSAGE);
				return 1;
			}
			*verbosity = 1;
			verbose_taken = 1;
		}
		
		
		/* now we search for download argument */
		else if( 0 == strcmp(argv[i], "-d") || 0 == strcmp(argv[i], "--download") ) {
			if(1 == download_taken) {
				fprintf(stderr, "Too many download arguments.\n");
				printf("%s", HELP_MESSAGE);
				return 1;
			}
			*download = 1;
			download_taken =1;
		}
		
		/* now we search for execute argument */
		else if( 0 == strcmp(argv[i], "-x") || 0 == strcmp(argv[i], "--execute") ) {
			if(1 == execute_taken) {
				fprintf(stderr, "Too many execute arguments.\n");
				printf("%s", HELP_MESSAGE);
				return 1;
			}
			*execute_allow = 1;
			execute_taken = 1;
		}
		
		/* help command */
		else if( 0 == strcmp(argv[i], "-h") || 0 == strcmp(argv[i], "--help") ) {
			printf("%s", HELP_MESSAGE);
			exit(0);
		}
		
		/* anything else should abort program */
		else {
			fprintf(stderr, "Invalid arguments.\n");
			printf("%s", HELP_MESSAGE);
			return 1;
		}
		i++;		//go for next argument
	}
	
	return 0;
}