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
#define SYMBOL_SIZE         20
#define SYMBOL_NOT_FOUND    -1

// Declarations
typedef struct symbol_item symbol_item;
typedef struct symbol_table symbol_table;
symbol_table * symbol_initialize(int);
symbol_item * symbol_itemize();
void symbol_print(symbol_table *);
int symbol_hash_key(char *);
int symbol_hash_index(int);
int symbol_search(symbol_table *, char *);
bool symbol_exists(symbol_table *, char *);
bool symbol_is_full(symbol_table *);
bool symbol_insert(symbol_table *, char *, data_value *);
bool symbol_assign(symbol_table *, char *, data_value *);
data_value * symbol_extract(symbol_table *, char *);
int symbol_get_key(symbol_table *, int);
char * symbol_get_identifier(symbol_table *, int);
data_value * symbol_get_data(symbol_table *, int);