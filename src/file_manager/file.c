#include "file.h"

char* ffullread(const char* file_directory) {
	FILE* file = fopen(file_directory, "r");

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	char* bytes = (char*)malloc(sizeof(char) * (size + 1));
	if (fread(bytes, 1, size, file) == 0) {
		fprintf(stderr, "Error while reading file\n");
		exit(1);
	}

	fclose(file);
	bytes[size + 1] = 0;
	return bytes;
}