#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERR_INCORRECT_USAGE 1
#define ERR_SAME_FILE 2
#define ERR_SOURCE_OPEN 3
#define ERR_TARGET_OPEN 4
#define ERR_FSEEK_SOURCE 5
#define ERR_MEMORY_ALLOC 6
#define ERR_SOURCE_CLOSE 7
#define ERR_TARGET_WRITE 8
#define ERR_TARGET_CLOSE 9
#define ERR_SOURCE_REMOVE 10

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Incorrent number of arguments provided. Usage: ./move infile outfile\n");
		return ERR_INCORRECT_USAGE;
	}
	const char* infile = argv[1];
	const char* outfile = argv[2];

	if (!strcmp(infile, outfile)) {
		fprintf(stderr, "Same file provided as infile and outfile\n");
		return ERR_SAME_FILE;
	}
	FILE* source = fopen(infile, "rb");
	if (source == NULL) {
		fprintf(stderr, "Could not open source file\n");
		return ERR_SOURCE_OPEN;
	}
	
	if (fseek(source, 0, SEEK_END)) {
		fclose(source);
		fprintf(stderr, "Could not calculate source file size\n");
		return ERR_FSEEK_SOURCE;
	}

	int source_file_size = ftell(source);
	if (source_file_size == -1) {
		fclose(source);
		fprintf(stderr, "Could not calculate source file size\n");
		return ERR_SOURCE_OPEN;
	}
	if (fseek(source, 0, SEEK_SET)) {
		fclose(source);
		fprintf(stderr, "Could not calculate source file size\n");
		return ERR_SOURCE_OPEN;
	}
	char *buffer = (char*)malloc(source_file_size + 1);
	if (buffer == NULL && source_file_size > 0) {
		fclose(source);
		fprintf(stderr, "Could not allocate memory\n");
		return ERR_MEMORY_ALLOC;
	}
	if (source_file_size > 0) {
		size_t bytes_read = fread(buffer, 1, source_file_size, source);
		if (bytes_read != (size_t)source_file_size) {
			fclose(source);
			free(buffer);
			fprintf(stderr, "Could not read the source file fully\n");
			return ERR_SOURCE_OPEN;
		}
		buffer[source_file_size] = '\0';
	}

	if (fclose(source)) {
		free(buffer);
		fprintf(stderr, "Could not close source file\n");
		return ERR_SOURCE_CLOSE;	
	}
	
	FILE* target = fopen(outfile, "wb");
	if (target == NULL) {
		free(buffer);
		fprintf(stderr, "Could not open target file for writing\n");
		return ERR_TARGET_OPEN;
	}
	if (source_file_size > 0) {
		size_t bytes_write = fwrite(buffer, 1, source_file_size, target);
		if (bytes_write != (size_t)source_file_size) {
			//fclose(source);
			free(buffer);
			fclose(target);
			fprintf(stderr, "Could not write to the target file fully\n");
			return ERR_TARGET_WRITE;
		}
	}
	if (fclose(target)) {
		free(buffer);
		fprintf(stderr, "Could not close target file\n");
		return ERR_TARGET_CLOSE;
	}
	source = NULL;
	target = NULL;

	if (remove(infile)) {
		free(buffer);
		fprintf(stderr, "Could not remove source file\n");
		return ERR_SOURCE_REMOVE;
	}
	free(buffer);
	return 0;
}
