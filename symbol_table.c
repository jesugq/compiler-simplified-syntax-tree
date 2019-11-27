// Imports
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"

/**
 * Symbol Table Initialize returns the allocated symbol table.
 * @param   level   Recursion level this table is in.
 * @return  Initialized symbol table pointer.
 */
struct symbol_table * symbol_table_initialize(int level) {
    struct symbol_table * table;
    table = (struct symbol_table *)calloc(1, sizeof(struct symbol_table));
    table->size = 0;
    table->level = level;
    table->items = symbol_items_initialize();
    return table;
}

/**
 * Symbol Items Initialize returns the allocated symbol item array.
 * @return  Initialized symbol item array pointer.
 */
struct symbol_item * symbol_items_initialize() {
    struct symbol_item * items;
    items = (struct symbol_item *)calloc(
        TABLE_SIZE, sizeof(struct symbol_item));
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
 * Simple print function to format and print the contents of the symbol table.
 * @param   table   Symbol table.
 */
void symbol_table_print(struct symbol_table * table) {
    int i;
    printf("%4s|%12s|%12s|%12s|\n", "-", "-", "-", "-");
    for (i=0; i<TABLE_SIZE; i++) {
        printf(
            "[%2d]|%12d|%12s|",
            i,
            table->items[i].key,
            table->items[i].identifier
        );
        if (table->items[i].data.numtype == TYPE_INTEGER)
            printf(
                "%5s|%12d|\n", "int",
                table->items[i].data.value.int_value
            );
        else if (table->items[i].data.numtype == TYPE_FLOAT)
            printf(
                "%5s|%12f|\n", "float",
                table->items[i].data.value.float_value
            );
        else
            printf("%5s|%12d|\n", "none", 0);
    }
    printf("%4s|%12s|%12s|%12s|\n", "-", "-", "-", "-");
}

/**
 * Symbol Table Terminate frees the memory used to allocate this table.
 * @param   table   Symbol table.
 */
void symbol_table_terminate(struct symbol_table * table) {
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
bool symbol_table_is_full(struct symbol_table * table) {
    return table->size == TABLE_SIZE;
}

/**
 * Symbol Table Search looks for an item inside of the symbol table array.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @return  Position of the item, -1 is not found.
 */
int symbol_table_search(struct symbol_table * table, char * identifier) {
    int key = hash_key(identifier);
    int index = hash_index(key);
    int curr = index;

    while (table->items[curr].key != 0) {
        if (table->items[curr].key == key) return curr;
        curr ++;
        if (curr >= TABLE_SIZE) curr = 0;
        if (curr == index) return -1;
    }
    return -1;
}

/**
 * Symbol Table Insert creates an item node and places it depending on its key.
 * @param   table       Symbol table.
 * @param   identifier  String of the identifier.
 * @param   numtype     Type of the identifier.
 * @return  True if the insertion was successful.
 */
bool symbol_table_insert(
    struct symbol_table * table, char * identifier, char numtype
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

    struct symbol_item item;
    item.key = key;
    item.identifier = identifier;
    item.data.numtype = numtype;
    if (item.data.numtype == TYPE_INTEGER)
        item.data.value.int_value = 0;
    else if (item.data.numtype == TYPE_FLOAT)
        item.data.value.float_value = 0;
    else
        item.data.value.int_value = 0;
    return true;
}

/**
 * Symbol Table Assign changes the data of the table item whose identifier
 * matches, and uses either int_value or float_value depending on its type.
 * @param   table       Symbol table.
 * @param   index       Index of the symbol item.
 * @param   data        Data of the identifier.
 * @return  True if the assignment was successful.
 */
bool symbol_table_assign(
    struct symbol_table * table, int index, DATA data
) {
    if (index == -1)
        return false;
    else {
        table->items[index].data = data;
        return true;
    }
}

/**
 * Retrieves the key of the symbol table item at parameter index.
 * @param   table   Symbol table.
 * @param   index   Index of the symbol item.
 * @return  Key of the symbol item.
 */
int symbol_table_get_key(struct symbol_table * table, int index) {
    if (index == -1)
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
char * symbol_table_get_identifier(struct symbol_table * table, int index) {
    if (index == -1)
        return NULL;
    else
        return table->items[index].identifier;
}

/**
 * Retrieves the numtype of the symbol table item at parameter index.
 * @param   table   Symbol table.
 * @param   index   Index of the symbol item.
 * @return  Numtype of the symbol item.
 */
char symbol_table_get_numtype(struct symbol_table * table, int index) {
    if (index == -1)
        return '\0';
    else
        return table->items[index].data.numtype;
}

/**
 * Retrieves the data of the symbol table item at parameter index.
 * @param   table   Symbol table.
 * @param   index   Index of the symbol item.
 * @return  Data of the symbol item.
 */
DATA symbol_table_get_data(struct symbol_table * table, int index) {
    if (index == -1) {
        union DATA placeholder;
        placeholder.numtype = TYPE_INTEGER;
        placeholder.value.int_value = 0;
        return placeholder;
    }
    else
        return table->items[index].data;
}