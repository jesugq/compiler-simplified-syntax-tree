#include "function_batch.h"

/**
 * Param List contains a list of the arguments needed for this function to be
 * properly called. It is a simple linked list where the contents are shown, and
 * the next parameter is a simple structure of the same type to the next.
 * @param   identifier  Identifier of this parameter.
 * @param   value       Value of this parameter. Defines the type.
 * @param   next        Next parameter to read.
 */
// typedef struct param_list {
//     char * identifier;
//     data_value * value;
//     struct param_list * next;
// } param_list;

/**
 * Function Info has everything denoting a function, the number of arguments to
 * check that they match, the type that the function returns and its value, with
 * standalone symbol tables and syntax node trees.
 * @param   args        Argument count of the function.
 * @param   numtype     Type of the function.
 * @param   identifier  Identifier of the function.
 * @param   list        Parameter list of the function.
 * @param   value       Value of the function.
 * @param   node        Syntax Node of the function.
 * @param   table       Symbol Table of the function.
 */
// typedef struct function_info {
//     int args;
//     int index;
//     char numtype;
//     char * identifier;
//     param_list * list;
//     data_value * value;
//     syntax_node * node;
//     symbol_table * table;
// } function_info;

/**
 * Function Table simply holds every function that has been created in an
 * array of function infos, and keeps track of its size.
 * @param   size        Size of the table array.
 * @param   infos       Contents of the table array.
 */
// typedef struct function_batch {
//     int size;
//     function_info * infos;
// } function_batch;

/**
 * Function Initialize creates a new table with FUNCTION_SIZE items.
 * @return  Initialized function table pointer.
 */
function_batch * function_initialize() {
    function_batch * batch;
    batch = (function_batch *)calloc(1, sizeof(function_batch));
    batch->size = 0;
    batch->infos = function_itemize();
}

/**
 * Function Itemize creates an allocated array of function infos.
 * @return  Allocated array of function infos.
 */
function_info * function_itemize() {
    function_info * infos;
    infos = (function_info *)calloc(FUNCTION_SIZE, sizeof(function_info));

    int i;
    for (i=0; i<FUNCTION_SIZE; i++)
        infos[i].index = -1;
    return infos;
}

/**
 * Function Paramize adds a parameter to the parameter list and returns the
 * header pointer.
 * @param   list        Parameter list.
 * @param   identifier  Identifier of the list to add.
 * @param   value       Value of the list to add.
 * @return  Header of the parameter list.
 */
param_list * function_paramize(
    param_list * list, char * identifier, data_value * value
) {
    param_list * current = list;
    param_list instance;
    while (current->next != NULL)
        current = current->next;

    current->next = &instance;
    instance.identifier = identifier;
    instance.value = value;
    instance.next = NULL;
    return list;
}

/**
 * Simple print function to format and print the contents of the function batch.
 * @param   batch   Function batch.
 */
void function_print(function_batch * batch) {
    int i;
    char typei[] = "integer";
    char typef[] = "float";
    char typen[] = "unknown";
    char headerp[] = "pos";
    char headera[] = "args";
    char headert[] = "type";
    char headeri[] = "name";
    char headerv[] = "value";

    printf("\n");
    printf("Function Table\n");
    printf("%4s%4s%10s%10s%10s\n", headerp, headera, headert, headeri, headerv);
    printf("\n");

    for (i=0; i<FUNCTION_SIZE; i++) {
        if (batch->infos[i].value != NULL) {
            printf("[%2d]", i);
            printf("%4d", batch->infos[i].args);
            if (batch->infos[i].value->numtype == DATA_INTEGER) {
                printf("%10s", typei);
                printf("%10s", batch->infos[i].value->number.int_value);
            } else if (batch->infos[i].value->numtype == DATA_FLOAT) {
                printf("%10s", typef);
                printf("%10s", batch->infos[i].value->number.float_value);
            } else {
                printf("%10s", typen);
                printf("%10d", 0);
            }
        if (batch->infos[i].table != NULL)
            symbol_print(batch->infos[i].table);
        printf("\n");
        }
    }
    printf("\n");
}

/**
 * Function Search returns the position where the function with name identifier
 * is found.
 * @param   batch       Function batch.
 * @param   identifier  Identifier to search.
 * @return  Index of the function.
 */
int function_search(function_batch * batch, char * identifier) {
    int i;
    for (i=0; i<FUNCTION_SIZE; i++) {
        if (batch->infos[i].index == i) return i;
    }
    return FUNCTION_NOT_FOUND;
}

/**
 * Function Exists returns whether the function is inside of this batch.
 * @param   batch       Function batch.
 * @param   identifier  Identifier to search.
 * @return  Whether the function exists.
 */
bool function_exists(function_batch * batch, char * identifier) {
    return function_search(batch, identifier) >= 0;
}

/**
 * Function Is Full returns the state of the batch.
 * @param   batch   Function batch.
 * @return  Whether the batch is full.
 */
bool function_is_full(function_batch * batch) {
    return batch->size == FUNCTION_SIZE;
}

/**
 * Function Insert returns whether the function was inserted.
 * @param   batch       Function batch. 
 * @param   identifier  Identifier to add.
 * @param   value       Value to add.
 * @param   list        List to add.
 * @param   args        Argument count to add.
 * @return Whether the function was inserted
 */
bool function_insert(
    function_batch * batch, char * identifier,
    data_value * value, param_list * list, int args
) {
    if (function_is_full(batch)) return false;

    int i;
    for (i=0; i<FUNCTION_SIZE; i++) {
        if (batch->infos[i].index == -1)
            break;
    }
    if (i == FUNCTION_SIZE) return false;

    function_info info;
    info.args = args;
    info.index = i;
    info.numtype = value->numtype;
    info.identifier = identifier;
    info.list = list;
    info.value = value;
    info.node = syntax_initialize();
    info.table = symbol_initialize();
    batch->infos[i] = info;
    return true;
}

/**
 * Function Assign changes the value in the value attribute of the function.
 * @param   batch       Function batch. 
 * @param   identifier  Identifier to add.
 * @param   value       Value to add.
 * @return true if the function value attribute is changed, false if the function was not found
 */
bool function_assign(
    function_batch * batch, char * identifier, data_value * value
) {
    int i = function_search(batch, identifier);
    if (i == FUNCTION_NOT_FOUND) return false,
    batch->infos[i].value = value;
    return true;
}

/**
 * Function Get Value gets the data_value from the Function.
 * @param   batch       Function batch.
 * @param   identifier  Identifier to search for.
 * @return  The value inside of the function.
 */
data_value * function_get_value(function_batch * batch, char * identifier) {
    int i = function_search(batch, identifier);
    if (i == FUNCTION_NOT_FOUND) return data_create_integer(0);
    return batch->infos[i].value;
}

/**
 * Function Get List gets the param_list from the Function.
 * @param   batch       Function batch.
 * @param   identifier  Identifier to search for.
 * @return  The list inside of the function.
 */
param_list * function_observe(function_batch * batch, char * identifier) {
    int i = function_search(batch, identifier);
    if (i == FUNCTION_NOT_FOUND) return NULL;
    return batch->infos[i].list;
}

/**
 * Function Get Table gets the symbol_table from the function.
 * @param   batch       Function batch.
 * @param   identifier  Identifier to search for.
 * @return  The table inside of the function.
 */
symbol_table * function_get_table(function_batch * batch, char * identifier) {
    int i = function_search(batch, identifier);
    if (i == FUNCTION_NOT_FOUND) return symbol_initialize();
    return batch->infos[i].table;
}

/**
 * Function Get Node gets the syntax_node from the function.
 * @param   batch       Function batch.
 * @param   identifier  Identifier to search for.
 * @return  The node inside of the function.
 */
syntax_node * function_get_node(function_batch * batch, char * identifier) {
    int i = function_search(batch, identifier);
    if (i == FUNCTION_NOT_FOUND) return syntax_initialize();
    return batch->infos[i].node;
}