// Imports
#include <stdio.h>
#include <stdbool.h>

#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

#ifndef _SYMBOLH_
#define _SYMBOLH_
#include "symbol_table.h"
#endif

#ifndef _SYNTAXH_
#define _SYNTAXH_
#include "syntax_tree.h"
#endif

// Definitons
#define FUNCTION_SIZE       20
#define FUNCTION_NOT_FOUND  -1

// Declarations
typedef struct param_list {
    char * identifier;
    data_value * value;
    struct param_list * next;
} param_list;
typedef struct function_info {
    int args;
    char numtype;
    char * identifier;
    param_list * list;
    data_value * value;
    syntax_node * node;
    symbol_table * table;
} function_info;
typedef struct function_batch {
    int size;
    function_info * infos;
} function_batch;

function_batch * function_initialize();
function_info * function_itemize();
void function_print(function_batch *);

int function_search(function_batch *, char *);
bool function_exists(function_batch *, char *);
bool function_is_full(function_batch *);
bool function_insert(function_batch *, char *, data_value *, param_list *);
bool function_assign(function_batch *, char *, data_value *);
data_value * function_extract(function_batch *, char *);
param_list * function_observe(function_batch *, char *);