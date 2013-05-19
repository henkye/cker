#include <stdio.h>
#include <stdlib.h>

void cleanup_func()
{
	if(html_data != NULL) {
		printf("%s\n", html_data);
		free(html_data);
	}
	
	if(url != NULL) {
		free(url);
	}
	
	if(config_file != NULL) {
		free(config_file);
	}
	
	if(url_identifier != NULL) {
		free(url_identifier);
	}
}