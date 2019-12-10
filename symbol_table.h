#ifndef _SYMBOLH_
#define _SYMBOLH_

// Imports
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data.h"
#include "syntax_tree.h"

// Definitions
#define SYMBOL_SIZE         30
#define SYMBOL_NOT_FOUND    -1
#define SYMBOL_EMPTY        'E'
#define SYMBOL_IDENTIFIER   'I'
#define SYMBOL_FUNCTION     'F'

// Declarations (Circular Header Inclusion)
// typedef struct param_list {
//     char * identifier;
//     data_value * value;
//     struct param_list * next;
// } param_list;
// typedef struct symbol_item {
//     int key;
//     int args;
//     char symtype;
//     char * identifier;
//     struct data_value * value;
//     struct param_list * list;
//     struct syntax_node * node;
// } symbol_item;
// typedef struct symbol_table {
//     int size;
//     char symtype;
//     struct symbol_item * items;
// } symbol_table;

symbol_table * symbol_initialize();
symbol_item * symbol_itemize();

int symbol_param_count(param_list *);
param_list * symbol_param_join(param_list *, param_list *);
param_list * symbol_param_create(char *, data_value *);
data_value * symbol_param_value(symbol_table *, char *, int);
bool symbol_param_equal(symbol_table *, char *, int);

void symbol_print(symbol_table *);
int symbol_hash_key(char *);
int symbol_hash_index(int);
int symbol_search(symbol_table *, char *);
bool symbol_exists(symbol_table *, char *);
bool symbol_is_full(symbol_table *);
bool symbol_is_identifier(symbol_table *, char *);
bool symbol_is_function(symbol_table *, char *);
bool symbol_insert_identifier(symbol_table *, char *, data_value *);
bool symbol_insert_function(
    symbol_table *, char *, data_value *,
    param_list *, syntax_node *
);

int symbol_get_args(symbol_table *, char *);
data_value * symbol_get_value(symbol_table *, char *);
// bool symbol_assign(symbol_table *, char *, data_value *);
// param_list * symbol_get_list(symbol_table *, char *);
// syntax_node * symbol_get_node(symbol_table *, char *);

#endif