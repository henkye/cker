#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int find_and_replace(char **input, char *find, char *replace)
{
	char *input_temp;
	char *p_input = *input;
	char *searched = NULL;
	int occurrence = 0;
	int size = 0;
	
	if( 0 == strlen(find)) {
//		printf("Nothing to be replaced, aborting.\n");
		return 1;
	}
	
	/* get size for malloc */
	while( NULL != (p_input = strstr(p_input, find))) {		// find how many needs to be replaced
		p_input += strlen(find);
		occurrence++;
	}
	
	if(occurrence == 0) {
//		printf("Found nothing to replace.\n");
		return 1;
	}
	
	size = ((strlen(replace) - strlen(find)) * occurrence) + strlen(*input);
	input_temp = malloc(sizeof(char) * size + 1);
	input_temp[0] = '\0';		// append first \0 for coming strcat
	
	/* get the job done */
	p_input = *input;		//reset
	while( NULL != (searched = strstr(p_input, find))) {		//search all occurrences
		strncat(input_temp, p_input, searched - p_input);		//append everything from start to occurrence
		strcat(input_temp, replace);		//add replacing string
		p_input = strstr(p_input, find);		//move to the right position
		p_input += strlen(find);		// ^^^
		searched += strlen(find);		// ^^^
	}
	strcat(input_temp, p_input);		//append the rest after last occurrence
	
	/* copy result and leave */
	if(NULL != *input) {
		free(*input);
	}
	
	*input = malloc(sizeof(char) * size + 1);
	strcpy(*input, input_temp);
	free(input_temp);
	return 0;
}