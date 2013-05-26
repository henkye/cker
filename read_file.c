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
#include <sys/stat.h>
#include <stdlib.h>

int read_file(const char *file_to_read, char **output)
{
	FILE *FILE_TO_READ;
	struct stat fileinfo;
	
	if( 0 != stat(file_to_read, &fileinfo)) {
		fprintf(stderr, "Cannot get file: %s informations.\n", file_to_read);
		return 1;
	}
	
	FILE_TO_READ = fopen(file_to_read, "r");
	if( FILE_TO_READ == NULL) {
		fprintf(stderr, "Cannot open file: %s.\n", file_to_read);
		return 1;
	}
	
	
	*output = malloc(fileinfo.st_size + 1);

	if( fileinfo.st_size != fread(*output, 1, fileinfo.st_size, FILE_TO_READ)) {
		fprintf(stderr, "Failed to read file.\n");
	}

	(*output)[fileinfo.st_size] = '\0';

	
	fclose(FILE_TO_READ);
	return 0;
}