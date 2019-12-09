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
    return infos;
}

/**
 * Simple print function to format and print the contents of the function batch.
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

