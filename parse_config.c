#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int parse_config(char **config_file)
{
	char *config_file_temp;
	int config_file_counter = 0;		//here and below we will store our array indexes
	int config_file_temp_counter = 0;
	
	config_file_temp = malloc(1 + strlen(*config_file) * sizeof(char));
	
	while((1 + strlen(*config_file)) != config_file_counter) {		//read everything including terminating null
		
		/* filter commented lines */
		if((*config_file)[config_file_counter] == '#') {		//go to #
			while((*config_file)[config_file_counter] != '\n') {		//skip untill \n
				config_file_counter++;
			}
			continue;
		}

		/* leave string inside "" untouched */
		if((*config_file)[config_file_counter] == '\"') {		//go to first "
			config_file_temp[config_file_temp_counter] = (*config_file)[config_file_counter];		//copy first " and move on or our while wont start
			config_file_counter++;
			config_file_temp_counter++;
			while((*config_file)[config_file_counter] != '\"' && (*config_file)[config_file_counter] != '\0') { 	//copy everything inbetween "" untouched !!FIX second condition (in comment removing section too)
				while((*config_file)[config_file_counter] == '\\' && (*config_file)[config_file_counter+1] == '\"') {		//copy escaped "
					config_file_temp[config_file_temp_counter] = (*config_file)[config_file_counter];
					config_file_counter++; config_file_temp_counter++;
					config_file_temp[config_file_temp_counter] = (*config_file)[config_file_counter];
				}
				config_file_temp[config_file_temp_counter] = (*config_file)[config_file_counter];
				config_file_counter++;
				config_file_temp_counter++;
			}
		}
		
		/* skip whitespaces */
		if((*config_file)[config_file_counter] == ' ' || (*config_file)[config_file_counter] == '\t'/* || (*config_file)[config_file_counter] == '\n'*/) {
			config_file_counter++;
			continue;
		}
		
		/* convert reamainings to lower case */
		config_file_temp[config_file_temp_counter] = tolower((*config_file)[config_file_counter]);
		config_file_counter++;
		config_file_temp_counter++;
	}
	
	//printf("%s\n", config_file_temp);
	*config_file = realloc(*config_file, strlen(config_file_temp) + 1);
	strcpy(*config_file, config_file_temp);
	free(config_file_temp);
	return 0;
}