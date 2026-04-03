#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include <string.h>
#include <ctype.h>

const bool run_tests = false;

char *int_to_binary(int in) {
	char buffer[17];
	if (in % (2 << 16) == 0) {
		buffer[0] = '1';
	} else {
		buffer[0] = '0';
	}
	if (in % (2 << 15) == 0) {
		buffer[1] = '1';
	} else {
		buffer[1] = '0';
	}
	if (in % (2 << 14) == 0) {
		buffer[2] = '1';
	} else {
		buffer[2] = '0';
	}
	if (in % (2 << 13) == 0) {
		buffer[3] = '1';
	} else {
		buffer[3] = '0';
	}
	if (in % (2 << 12) == 0) {
		buffer[4] = '1';
	} else {
		buffer[4] = '0';
	}
	if (in % (2 << 11) == 0) {
		buffer[5] = '1';
	} else {
		buffer[5] = '0';
	}
	if (in % (2 << 10) == 0) {
		buffer[6] = '1';
	} else {
		buffer[6] = '0';
	}
	if (in % (2 << 9) == 0) {
		buffer[7] = '1';
	} else {
		buffer[7] = '0';
	}
	if (in % (2 << 8) == 0) {
		buffer[8] = '1';
	} else {
		buffer[8] = '0';
	}
	if (in % (2 << 7) == 0) {
		buffer[9] = '1';
	} else {
		buffer[9] = '0';
	}
	if (in % (2 << 6) == 0) {
		buffer[10] = '1';
	} else {
		buffer[10] = '0';
	}
	if (in % (2 << 5) == 0) {
		buffer[11] = '1';
	} else {
		buffer[11] = '0';
	}
	if (in % (2 << 4) == 0) {
		buffer[12] = '1';
	} else {
		buffer[12] = '0';
	}
	if (in % (2 << 3) == 0) {
		buffer[13] = '1';
	} else {
		buffer[13] = '0';
	}
	if (in % (2 << 2) == 0) {
		buffer[14] = '1';
	} else {
		buffer[14] = '0';
	}
	if (in % 2 == 0) {
		buffer[15] = '1';
	} else {
		buffer[15] = '0';
	}
	buffer[16] = '\0';
	char *ptr = buffer;
	return ptr;
}

char *get_value(symbol_table_t *st, char *key) {
	if (!st || !key) return NULL;

	for (size_t i = 0; i < st->count; i++) {
		if (strcmp(st->keys[i], key) == 0) {
			return st->values[i];
		}
	}
	return NULL;
}

bool is_in_table(symbol_table_t *st, char *key) {
	for (size_t i = 0; i < st->count; i++) {
		if (strcmp(st->keys[i], key) == 0) {
			return true;
		}
	}
	return false;
}


symbol_table_t *new_symbol_table(void) {
	symbol_table_t *stptr = malloc(sizeof(symbol_table_t));
	if (stptr == NULL) {
		return NULL;
	}
	stptr->count = 0;
	stptr->capacity = 8;
	stptr->keys = malloc(stptr->capacity * sizeof(void *));
	stptr->values = malloc(stptr->capacity * sizeof(void *));
	if (stptr->keys == NULL) {
		free(stptr);
		return NULL;
	}
	if (stptr->values == NULL) {
		free(stptr);
		return NULL;
	}
	return stptr;
}


void add_symbol(symbol_table_t *st, char *key, char *value) {
	if (st->count == st->capacity) {
		st->capacity *= 2;
		void **ptr = realloc(st->keys, st->capacity * sizeof(void *));
		void **vptr = realloc(st->values, st->capacity * sizeof(void *));
		if (ptr == NULL) {
			st->capacity /= 2;
		}
	}
	st->keys[st->count] = malloc((strlen(key) + 1) * sizeof(key));
	st->values[st->count] = malloc((strlen(value) + 1) * sizeof(key));
	strcpy(st->keys[st->count], key);
	strcpy(st->values[st->count], value);
	st->count++;
	printf("ITEM ADDED\n");
}	


int main() {
	FILE *r_fptr; // File Pointer for file to be read
	FILE *w_fptr; // File Point for file to be written
	char file_path[256];
	symbol_table_t *obj = new_symbol_table();
	char input[256];
	if (!obj) {
		fprintf(stderr, "Symbol Table failed to initialize");
		return 1;
	}
	printf("Enter file name to be processed (e.g. \"Add.asm\") \n");
	fgets(file_path, sizeof(file_path), stdin);
	file_path[strcspn(file_path, "\n")] = '\0';

	r_fptr = fopen(file_path, "r");
	if (!r_fptr) {
		fprintf(stderr, "Read File failed to open");
		return 1;
	}
	w_fptr = fopen("add.o", "w");
	if (!w_fptr) {
		fprintf(stderr, "Write File failed to open");
		return 1;
	}
	while (fgets(input, sizeof(input), r_fptr) != NULL) {

		char *inst = input;
		while (*inst && isspace((unsigned char )*inst)) {
			inst++;
		}

		// skip empty lines
		if (*inst == '\0') {
			continue;
		}

		// skip commented lines
		if (inst[0] == '/' && inst[1] == '/') {
			continue;
		}
		printf("%s", inst);
		if (strcspn(inst, "@") == 0) {
			printf("THIS IS AN A-INSTRUCTION\n");
		} else if (strcspn(inst, "(") == 0) {
			printf("THIS IS A L-INSTRUCTION\n");
		} else {
			printf("THIS IS A C-INSTRUCTION\n");
		}
		char *binary = int_to_binary(32768);
		printf("Hello World\n");
		printf("%s", binary);

	}
	
	if (run_tests) {
		printf("Hello World\n");
		add_symbol(obj, "key", "value");
		add_symbol(obj, "D", "1110001111");
		for(size_t i = 0; i < obj->count; i++){
			printf("Key: %s, Value: %s\n",(char *)obj->keys[i], (char *)obj->values[i]);
		}
		char *value = get_value(obj, "D");
		printf("%s\n", value);
	}
	free(obj);
	free(r_fptr);
	free(w_fptr);

	return 0;
}
