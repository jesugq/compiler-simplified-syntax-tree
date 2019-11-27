// Imports
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

// Definitions
#define SYMBOL_NOT_FOUND -1

// Declarations
typedef struct symbol_item symbol_item;
typedef struct symbol_table symbol_table;
symbol_table * symbol_table_initialize();
symbol_item * symbol_items_initialize();
int hash_key(char *);
int hash_index(int);
void symbol_table_print(symbol_table *);
void symbol_table_terminate(symbol_table *);
int symbol_table_search(symbol_table *, char *);
bool symbol_table_is_full(symbol_table *);
bool symbol_table_insert(symbol_table *, char *, data_value *);
bool symbol_table_assign(symbol_table *, int, data_value *);
int symbol_table_get_key(symbol_table *, int);
char * symbol_table_get_identifier(symbol_table *, int);
data_value * symbol_table_get_data(symbol_table *, int);