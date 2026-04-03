#include <stddef.h>

typedef struct {
	size_t count;
	size_t capacity;
	char **keys;
	char **values;
} symbol_table_t;

symbol_table_t *new_symbol_table(void);
void add_symbol(symbol_table_t *st, char *key, char *value);
bool is_in_table(symbol_table_t *st, char *key);
char *get_value(symbol_table_t *st, char* key);
