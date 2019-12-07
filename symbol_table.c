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
    struct data_value * value;
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

/**
 * Symbol Table Initialize returns the allocated symbol table.
 * @param   level   Recursion level this table is in.
 * @return  Initialized symbol table pointer.
 */
symbol_table * symbol_initialize(int level) {
    symbol_table * table;
    table = (symbol_table *)calloc(1, sizeof(symbol_table));
    table->size = 0;
    table->level = level;
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
bool symbol_insert(
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
    item.identifier = identifier;
    item.value = value;
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
 * Symbol Table Extract is similar to the get value function in which it returns
 * the data_value object, but using the identifier instead of the index.
 * @param   table       Symbol table.
 * @param   identifier  Identifier of the symbol.
 * @return  Data value object of the identifier.
 */
data_value * symbol_extract(symbol_table * table, char * identifier) {
    int index = symbol_search(table, identifier);
    if (index == SYMBOL_NOT_FOUND)
        return data_create_integer(0);
    else
        return table->items[index].value;
}

/**
 * Retrieves the key of the symbol table item at parameter index.
 * @param   table   Symbol table.
 * @param   index   Index of the symbol item.
 * @return  Key of the symbol item.
 */
int symbol_get_key(symbol_table * table, int index) {
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
char * symbol_get_identifier(symbol_table * table, int index) {
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
data_value * symbol_get_data(symbol_table * table, int index) {
    if (index == SYMBOL_NOT_FOUND)
        return data_create_integer(0);
    else
        return table->items[index].value;
}