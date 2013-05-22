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

/* This function searches all strings in **what separated by ',' in *where.
 *  If none is found in *where, returns 1, leaving **what untouched.
 *  If at least one is found, it puts it in **what and returns 0.
 *  If multiple are found, it puts string which is found first to **what.
 * 
 * Note: this function has built-in support for skipping "\," in **what.
 */
  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int find_nearest(char *where, char **what)
{
	char *p_what = *what;
	char *p_what_start = *what;
	char *p_where = NULL;
	char *temp_nearest = NULL;
	char *arraybaby[99];
	short int i = 0;
	short int j = 0;
	int distance = -1;
	int temp_distance = 0;

	/* check basic errors */
	if( NULL == where || NULL == *what) return 1;
	if( 0 == strlen(where) || 0 == strlen(*what)) return 1;
	if( ',' == *p_what) {
		fprintf(stderr, "Invalid input!\n");
		return 1;
	}

	while( '\0' !=*p_what ) {
		if (i > 98) { fprintf(stderr, "Too many arguments, aborting.\n"); return 1; }
		if( NULL != (p_what = strstr(p_what, ",")) ) {
			if( '\\' == *(p_what - 1) ) {
				if( '\0' == *(p_what + 1) ) {
					arraybaby[i] = malloc(strlen(p_what_start) * sizeof(char) +1);
					strcpy(arraybaby[i], p_what_start);
					i++;
					break;
				}
				else {
					p_what++;
					continue;
				}
			}
			/* skip ,,,, */
			if( 0 == p_what - p_what_start ) {
				p_what++; p_what_start++;
			}
			else {
				arraybaby[i] = malloc((p_what - p_what_start) * sizeof(char) +1);
				snprintf(arraybaby[i], (p_what - p_what_start) +1, "%s", p_what_start);
				i++;
				p_what++;
				p_what_start = p_what;
				continue;
			}
		}
		else {
			arraybaby[i] = malloc(strlen(p_what_start) * sizeof(char) +1);
			strcpy(arraybaby[i], p_what_start);
			i++;
			break;
		}
	}
	

	while( j < i ) {
		find_and_replace(&arraybaby[j], "\\,", ",");
		
		p_where = strstr(where, arraybaby[j]);
		if( NULL != p_where ) {
			temp_distance = p_where - where;
			if( -1 == distance || distance > temp_distance ) {
				distance = temp_distance;
				
				if(NULL != temp_nearest) free(temp_nearest);
				temp_nearest = malloc(sizeof(char) * strlen(arraybaby[j]) +1);
				strcpy(temp_nearest, arraybaby[j]);
			}
		}
		free(arraybaby[j]);
		j++;
	}
	
	/* if nothing ever was found */
	if( NULL == temp_nearest) {
		return 1;
	}

	strcpy(*what, temp_nearest);
	free(temp_nearest);
		
	return 0;
}