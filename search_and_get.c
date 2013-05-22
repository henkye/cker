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

/*
 * This function gets text between <searchA> and <searchB> from <where>
 * into dynamically allocated <result>, leaving <where> after last search (<searchB>).
 * Returns 0 at success, 1 at failure.
 *
 * Remember, <where> and <result> needs to be ?pointer address?, ie: "&where".
 */

int search_and_get(char **where, char *searchA, char *searchB,char **result)

{
	char *p_f_searchA = NULL;
	char *p_f_searchB = NULL;
	int string_lenght=0;
	
	if(0 == strlen(searchA)) {
		fprintf(stderr, "searchA is empty, aborting.\n");
		return 1;
	}
	if(0 == strlen(searchB)) {
		fprintf(stderr, "searchB is empty, aborting.\n");
		return 1;
	}

	/* search first word */
	p_f_searchA = strstr(*where, searchA);
	if (p_f_searchA == NULL)		// is it found?
		{
			return 1;
		}
	
	p_f_searchA+=(strlen(searchA));		// move to the actuall data

	/* search second word */
	if(0 == strcmp(searchB, "\"")) {
		p_f_searchB = p_f_searchA;		// we'll start search here
		while( NULL != (p_f_searchB = strstr(p_f_searchB,searchB))) {		// loop intil found
			--p_f_searchB;		// we need to get before " to search for backslash
			if( 0 != strncmp(p_f_searchB, "\\", 1)) {		//is there backlash
				++p_f_searchB;		// nice to return things as we took it
				break;
			}
			p_f_searchB += 2;		//nobody breaked loop - there is \". skip " and backslash
		}
	}
	else {
		p_f_searchB = strstr(p_f_searchA, searchB);
	}
	
	if (p_f_searchB == NULL)		// is it found?
		{
			return 1;
		}
	
	
	/* count string lenght */
	string_lenght = p_f_searchB - p_f_searchA;
	
	/* it is neccesary to free before another malloc (valgrind) */
	if(NULL != *result) {
		free(*result);
	}

	/* get the actual result */
	*result = malloc(sizeof(char) * string_lenght+1);		// malloc
	snprintf(*result, string_lenght+1, "%s",p_f_searchA);		// copy number of chars

	/* move after last search */
	*where = p_f_searchB + strlen(searchB);
	return 0;
}