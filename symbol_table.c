// Imports
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definitions
#define TABLE_SIZE      20
#define TYPE_INTEGER    'I'
#define TYPE_FLOAT      'F'

// Declarations
typedef union NUMERIC NUMERIC;
typedef struct symbol_item symbol_item;
typedef struct symbol_table symbol_table;
struct symbol_item * items_initialize();

// Variables
struct symbol_table * table;

/**
 * Numeric is an union that denotes how the numeric values of the node should be
 * handled, as an integer or as a float. The code knows how to use this value by
 * using the numtype attribute in the node.
 * @param   int_value   Integer depiction of this node's value. 
 * @param   float_value Float depiction of this node's value.
 */
typedef union NUMERIC {
    int int_value;
    float float_value;
} NUMERIC;

/**
 * Symbols Item is each instance of a possible symbol item. It includes its hash
 * key, the identifier string, its numtype and the value union, which can be
 * accessed depending on the numtype.
 * @param   key         Hash key of this node.
 * @param   identifier  String identifier of this node.
 * @param   numtype     Numerical type of this node.
 * @param   value       Union value of this node.
 */
typedef struct symbol_item {
    int key;
    char * identifier;
    char numtype;
    union NUMERIC value;
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
    struct symbol_item * items;
} symbol_table;