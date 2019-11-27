// Definitions
#define TABLE_SIZE      20
#define TYPE_INTEGER    'I'
#define TYPE_FLOAT      'F'

// Declarations
struct symbol_table * symbol_table_initialize();
struct symbol_item * symbol_items_initialize();
int hash_key(char *);
int hash_index(int);
void symbol_table_print(struct symbol_table *);
void symbol_table_terminate(struct symbol_table *);
int symbol_table_search(struct symbol_table *, char *);
bool symbol_table_is_full(struct symbol_table *);
bool symbol_table_insert(struct symbol_table *, char *, char);
bool symbol_table_assign(struct symbol_table *, int, DATA);
int symbol_table_get_key(struct symbol_table *, int);
char * symbol_table_get_identifier(struct symbol_table *, int);
char symbol_table_get_numtype(struct symbol_table *, int);
DATA symbol_table_get_data(struct symbol_table *, int);

/**
 * Union Number stores the content of the value in either an integer type or a
 * floating point type. Which type is used rests on its type, present in an 
 * union called data.
 * @param   int_value   Integer depiction of this number.
 * @param   float_value Float depiction of this number.
 */
typedef union NUMBER {
    int int_value;
    float float_value;
} NUMBER;

/**
 * Union Data stores the value of the number that the identifier is currently
 * assigned to, and the type to know how to handle it.
 * @param   numtype     Type of the data.
 * @param   value       Value of the data.
 */
typedef union DATA {
    char numtype;
    union NUMBER value;
} DATA;


/**
 * Symbols Item is each instance of a possible symbol item. It includes its hash
 * key, the identifier string, its numtype and the data union, which can be
 * accessed depending on the numtype.
 * @param   key         Hash key of this node.
 * @param   identifier  String identifier of this node.
 * @param   numtype     Numerical type of this node.
 * @param   data        Union data of this node.
 */
typedef struct symbol_item {
    int key;
    char * identifier;
    union DATA data;
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