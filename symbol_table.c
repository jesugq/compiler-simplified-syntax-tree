#include "symbol_table.h"

/**
 * Symbols Item is each instance of a possible symbol item. It includes its hash
 * key, the identifier string, its numtype and the data_value union, which can 
 * be accessed depending on the numtype.
 * @param   key         Hash key of this node.
 * @param   identifier  String identifier of this node.
 * @param   numtype     Numerical type of this node.
 * @param   value       Union value of this node.
 */
typedef struct symbol_item {
    int key;
    char * identifier;
    data_value * value;
} symbol_item;

/**
 * Symbol Table stores the array of symbol_items and the size of said array.
 * @param   size    Size of the table.
 * @param   level   Recursion level this table is in.
 * @param   items   Items of the table.    
 */
typedef struct symbol_table {
    int size;
    int level;
    symbol_item * items;
} symbol_table;

/**
 * Symbol Table Initialize returns the allocated symbol table.
 * @param   level   Recursion level this table is in.
 * @return  Initialized symbol table pointer.
 */
symbol_table * symbol_table_initialize(int level) {
    symbol_table * table;
    table = (symbol_table *)calloc(1, sizeof(symbol_table));
    table->size = 0;
    table->level = level;
    table->items = symbol_items_initialize();
    return table;
}

/**
 * Symbol Items Initialize returns the allocated symbol item array.
 * @return  Initialized symbol item array pointer.
 */
symbol_item * symbol_items_initialize() {
    symbol_item * items;
    items = (symbol_item *)calloc(
        TABLE_SIZE, sizeof(symbol_item));
    return items;
}

/**
 * Hash Key returns the hash of the identifier's string. Uses Java's hashCode()
 * equivalent, which exponentiates a constant of 31 to the length-i.
 * @param   identifier  String of the identifier.
 * @return  Hash key of the identifier.
 */
int hash_key(char * identifier) {
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
int hash_index(int key) {
    return abs(key % TABLE_SIZE);
}

/**
 * Symbol Table Terminate frees the memory used to allocate this table.
 * @param   table   Symbol table.
 */
void symbol_table_terminate(symbol_table * table) {
    int i;
    for (i=0; i<TABLE_SIZE; i++) {
        if (table->items[i].identifier != NULL)
            free(table->items[i].identifier);
    }
    free(table->items);
    free(table);
}

/**
 * Symbol Table Is Fulll tells about the occupancy of the symbol table.
 * @param   table   Symbol table.
 * @return  True if the symbol table is full.
 */
bool symbol_table_is_full(symbol_table * table) {
    return table->size == TABLE_SIZE;
}

/**
 * Symbol Table Search looks for an item inside of the symbol table array.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @return  Position of the item, -1 is not found.
 */
int symbol_table_search(symbol_table * table, char * identifier) {
    int key = hash_key(identifier);
    int index = hash_index(key);
    int curr = index;

    while (table->items[curr].key != 0) {
        if (table->items[curr].key == key) return curr;
        curr ++;
        if (curr >= TABLE_SIZE) curr = 0;
        if (curr == index) return SYMBOL_NOT_FOUND;
    }
    return SYMBOL_NOT_FOUND;
}

/**
 * Symbol Table Insert creates an item node and places it depending on its key.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @param   value       Value of the identifier.
 * @return  True if the insertion was successful.
 */
bool symbol_table_insert(
    symbol_table * table, char * identifier, data_value * value
) {
    if (symbol_table_is_full(table)) return false;
    
    int key = hash_key(identifier);
    int index = hash_index(key);
    int curr = index;

    while (table->items[curr].key != 0) {
        curr ++;
        if (curr >= TABLE_SIZE) curr = 0;
        if (curr == index) return false;
    }

    symbol_item item;
    item.key = key;
    item.identifier = identifier;
    item.value = value;
    return true;
}

/**
 * Symbol Table Assign changes the value of the table item whose identifier
 * matches, and uses either int_value or float_value depending on its type.
 * @param   table       Symbol table.
 * @param   index       Index of the symbol item.
 * @param   value       Value of the identifier.
 * @return  True if the assignment was successful.
 */
bool symbol_table_assign(
    symbol_table * table, int index, data_value * value
) {
    if (index == SYMBOL_NOT_FOUND)
        return false;
    else {
        table->items[index].value = value;
        return true;
    }
}

/**
 * Retrieves the key of the symbol table item at parameter index.
 * @param   table   Symbol table.
 * @param   index   Index of the symbol item.
 * @return  Key of the symbol item.
 */
int symbol_table_get_key(symbol_table * table, int index) {
    if (index == SYMBOL_NOT_FOUND)
        return 0;
    else
        return table->items[index].key;
}

/**
 * Retrieves the identifier of the symbol table item at parameter index.
 * @param   table       Symbol table.
 * @param   index   Index of the symbol item.
 * @return  Identifier of the symbol item.
 */
char * symbol_table_get_identifier(symbol_table * table, int index) {
    if (index == SYMBOL_NOT_FOUND)
        return NULL;
    else
        return table->items[index].identifier;
}

/**
 * Retrieves the value of the symbol table item at parameter index.
 * @param   table   Symbol table.
 * @param   index   Index of the symbol item.
 * @return  Value of the symbol item.
 */
data_value * symbol_table_get_data(
    symbol_table * table, int index
) {
    if (index == SYMBOL_NOT_FOUND) {
        return data_create_integer(0);
    }
    else
        return table->items[index].value;
}