// Imports
#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

// Definitions

// Declarations
typedef union tree_info {
    int instruction;
    char * identifier;
    data_value value;
} tree_info;
typedef struct tree_node {
    char nodetype;
    tree_info info;
    tree_node * node_a;
    tree_node * node_b;
    tree_node * node_c;
} tree_node;

tree_node * tree_create_assign();