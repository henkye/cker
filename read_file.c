#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int read_file(char *file_to_read, char **output)
{
	FILE *FILE_TO_READ;
	struct stat fileinfo;
	
	if( 0 != stat(file_to_read, &fileinfo)) {
		printf("Cannot get file: %s informations.\n", file_to_read);
		return 1;
	}
	
	FILE_TO_READ = fopen(file_to_read, "r");
	if( FILE_TO_READ == NULL) {
		printf("Cannot open file: %s.\n", file_to_read);
		return 1;
	}
	
	
	*output = malloc(fileinfo.st_size + 1);

	fread(*output, 1, fileinfo.st_size, FILE_TO_READ);

	(*output)[fileinfo.st_size] = '\0';

	
	fclose(FILE_TO_READ);
	return 0;
}