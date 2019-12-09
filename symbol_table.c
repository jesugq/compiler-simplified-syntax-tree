#include "symbol_table.h"

/**
 * Param List has the amount of parameters and the contents of them when the
 * symbol item is of type function. Using this we can observe if calling a
 * function was correct.
 */
// typedef struct param_list {
//     char * identifier;
//     data_value * value;
//     struct param_list * next;
// } param_list;

/**
 * Symbols Item is each instance of a possible symbol item. It includes its hash
 * key, the identifier string, its numtype and the data_value union, which can 
 * be accessed depending on the numtype.
 * @param   key         Hash key of this node.
 * @param   symtype     Type of this node.
 * @param   identifier  String identifier of this node.
 * @param   numtype     Numerical type of this node.
 * @param   value       Union value of this node.
 * @param   list        List of this node.
 * @param   node        Node of this node.
 * @param   table       Table of this node.
 */
// typedef struct symbol_item {
//     int key;
//     char symtype;
//     char * identifier;
//     struct data_value * value;
//     struct param_list * list;
//     struct syntax_node * node;
//     struct symbol_table * table;
// } symbol_item;

/**
 * Symbol Table stores the array of symbol_items and the size of said array.
 * @param   size    Size of the table.
 * @param   items   Items of the table.    
 */
// typedef struct symbol_table {
//     int size;
//     struct symbol_item * items;
// } symbol_table;

/**
 * Symbol Table Initialize returns the allocated symbol table.
 * @return  Initialized symbol table pointer.
 */
symbol_table * symbol_initialize() {
    symbol_table * table;
    table = (symbol_table *)calloc(1, sizeof(symbol_table));
    table->size = 0;
    table->items = symbol_itemize();
    return table;
}

/**
 * Symbol Items Initialize returns the allocated symbol item array.
 * @return  Initialized symbol item array pointer.
 */
symbol_item * symbol_itemize() {
    symbol_item * items;
    items = (symbol_item *)calloc(
        SYMBOL_SIZE, sizeof(symbol_item));
    int i;
    for (i=0; i<SYMBOL_SIZE; i++)
        items[i].symtype = SYMBOL_EMPTY;
    return items;
}

/**
 * Simple print function to format and print the contents of the symbol table.
 * @param   table   Symbol table.
 */
void symbol_print(symbol_table * table) {
    int i;
    char typei[] = "integer";
    char typef[] = "float";
    char typen[] = "unknown";
    char headerp[] = "pos";
    char headerk[] = "key";
    char headeri[] = "name";
    char headert[] = "type";
    char headerv[] = "value";

    printf("\n");
    printf("Symbol Table:\n");
    printf("%4s%10s%10s%10s%10s", headerp, headerk, headeri, headert, headerv);
    printf("\n\n");
    
    for (i=0; i<SYMBOL_SIZE; i++) {
        if (table->items[i].value != NULL) {
        printf("[%2d]", i);
        printf("%10d", table->items[i].key);
        printf("%10s", table->items[i].identifier);
            if (table->items[i].value->numtype == DATA_INTEGER) {
                printf("%10s", typei);
                printf("%10d", table->items[i].value->number.int_value);
            } else if (table->items[i].value->numtype == DATA_FLOAT) {
                printf("%10s", typef);
                printf("%10f", table->items[i].value->number.float_value);
            } else {
                printf("%10s", typen);
                printf("%10d", 0);
            }
        printf("\n");
        }
    }
    printf("\n");
}

/**
 * Hash Key returns the hash of the identifier's string. Uses Java's hashCode()
 * equivalent, which exponentiates a constant of 31 to the length-i.
 * @param   identifier  String of the identifier.
 * @return  Hash key of the identifier.
 */
int symbol_hash_key(char * identifier) {
    int i, key, length;
    double constant, left;

    length = strlen(identifier);
    left = (double)length - 1;
    key = 0;
    constant = 31;
    for (i=0; i<length; i++){
        key += identifier[i] * (int)(floor(pow(constant, left)));
        left --;
    }
    return key;
}

/**
 * Hash Index returns the index where the item would be found in the array.
 * @param   key     Hash key of the item.
 * @return  Expected array index of the identifier.
 */
int symbol_hash_index(int key) {
    return abs(key % SYMBOL_SIZE);
}

/**
 * Symbol Table Search looks for an item inside of the symbol table array.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @return  Position of the item, -1 is not found.
 */
int symbol_search(symbol_table * table, char * identifier) {
    int key = symbol_hash_key(identifier);
    int index = symbol_hash_index(key);
    int curr = index;

    while (table->items[curr].key != 0) {
        if (table->items[curr].key == key) return curr;
        curr ++;
        if (curr >= SYMBOL_SIZE) curr = 0;
        if (curr == index) return SYMBOL_NOT_FOUND;
    }
    return SYMBOL_NOT_FOUND;
}

/**
 * Symbol Table Exists looks for an item inside of the symbol table array and
 * returns whether it has been declared or not.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @return  Whether the identifier was found on the table.
 */
bool symbol_exists(symbol_table * table, char * identifier) {
    return symbol_search(table, identifier) >= 0;
}

/**
 * Symbol Table Is Fulll tells about the occupancy of the symbol table.
 * @param   table   Symbol table.
 * @return  True if the symbol table is full.
 */
bool symbol_is_full(symbol_table * table) {
    return table->size == SYMBOL_SIZE;
}

/**
 * Symbol Table Insert creates an item node and places it depending on its key.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @param   value       Value of the identifier.
 * @return  True if the insertion was successful.
 */
bool symbol_insert_identifier(
    symbol_table * table, char * identifier, data_value * value
) {
    if (symbol_is_full(table)) return false;
    
    int key = symbol_hash_key(identifier);
    int index = symbol_hash_index(key);
    int curr = index;

    while (table->items[curr].key != 0) {
        curr ++;
        if (curr >= SYMBOL_SIZE) curr = 0;
        if (curr == index) return false;
    }

    symbol_item item;
    item.key = key;
    item.symtype = SYMBOL_IDENTIFIER;
    item.identifier = identifier;
    item.value = value;
    item.list = NULL;
    item.node = NULL;
    item.table = NULL;
    table->items[curr] = item;
    return true;
}

/**
 * Symbol Table Insert creates an item node and places it depending on its key.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @param   value       Value of the identifier.
 * @param   list        List of the identifier.
 * @param   node        Node of the identifier.
 * @param   tablein     Table inside of the identifier.
 * @return  True if the insertion was successful.
 */
bool symbol_insert_function(
    symbol_table * table, char * identifier, data_value * value,
    param_list * list, syntax_node * node, symbol_table * tablein
) {
    if (symbol_is_full(table)) return false;
    
    int key = symbol_hash_key(identifier);
    int index = symbol_hash_index(key);
    int curr = index;

    while (table->items[curr].key != 0) {
        curr ++;
        if (curr >= SYMBOL_SIZE) curr = 0;
        if (curr == index) return false;
    }

    symbol_item item;
    item.key = key;
    item.symtype = SYMBOL_IDENTIFIER;
    item.identifier = identifier;
    item.value = value;
    item.list = list;
    item.node = node;
    item.table = tablein;
    table->items[curr] = item;
    return true;
}

/**
 * Symbol Table Assign changes the value of the table item whose identifier
 * matches, and uses either int_value or float_value depending on its type.
 * @param   table       Symbol table.
 * @param   identifier  Identifier of the object.
 * @param   value       Value of the identifier.
 * @return  True if the assignment was successful.
 */
bool symbol_assign(
    symbol_table * table, char * identifier, data_value * value
) {
    int index = symbol_search(table, identifier);
    if (index == SYMBOL_NOT_FOUND)
        return false;
    else {
        table->items[index].value = value;
        return true;
    }
}

/**
 * Function Get Value gets the data_value from the Function.
 * @param   table       Function table.
 * @param   identifier  Identifier to search for.
 * @return  The value inside of the function.
 */
data_value * symbol_get_value(symbol_table * table, char * identifier) {
    int i = symbol_search(table, identifier);
    if (i == SYMBOL_NOT_FOUND) return data_create_integer(0);
    return table->items[i].value;
}

/**
 * Function Get List gets the param_list from the Function.
 * @param   table       Function table.
 * @param   identifier  Identifier to search for.
 * @return  The list inside of the function.
 */
param_list * function_observe(symbol_table * table, char * identifier) {
    int i = symbol_search(table, identifier);
    if (i == SYMBOL_NOT_FOUND) return NULL;
    return table->items[i].list;
}

/**
 * Function Get Table gets the symbol_table from the function.
 * @param   table       Function table.
 * @param   identifier  Identifier to search for.
 * @return  The table inside of the function.
 */
symbol_table * symbol_get_table(symbol_table * table, char * identifier) {
    int i = symbol_search(table, identifier);
    if (i == SYMBOL_NOT_FOUND) return symbol_initialize();
    return table->items[i].table;
}

/**
 * Function Get Node gets the syntax_node from the function.
 * @param   table       Function table.
 * @param   identifier  Identifier to search for.
 * @return  The node inside of the function.
 */
syntax_node * symbol_get_node(symbol_table * table, char * identifier) {
    int i = symbol_search(table, identifier);
    if (i == SYMBOL_NOT_FOUND) return syntax_initialize();
    return table->items[i].node;
}