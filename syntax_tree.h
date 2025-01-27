#ifndef _SYNTAXH_
#define _SYNTAXH_

// Imports
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data.h"

// Definitions
// Node Types
#define SYNTAX_INSTRUCTION  '0'
#define SYNTAX_IDENTIFIER   '1'
#define SYNTAX_VALUE        '2'

// NULL Types
#define SYNTAX_NULLV        NULL
#define SYNTAX_NULLT        '\0'
#define SYNTAX_NULLB        false

// Instructions
#define SYNTAX_STMT         'S'
#define SYNTAX_ASSIGN       'A'
#define SYNTAX_IF           'I'
#define SYNTAX_IFELSE       'F'
#define SYNTAX_WHILE        'W'
#define SYNTAX_READ         'R'
#define SYNTAX_PRINT        'P'
#define SYNTAX_EXPRESSION   'E'
#define SYNTAX_EXPR         'e'
#define SYNTAX_TERM         't'
#define SYNTAX_FUNCTION     'f'
#define SYNTAX_RETURN       'r'
#define SYNTAX_ARG          'A'

// Declarations
typedef struct param_list {
    char * identifier;
    data_value * value;
    struct param_list * next;
} param_list;
typedef struct symbol_item {
    int key;
    int args;
    char symtype;
    char * identifier;
    struct data_value * value;
    struct param_list * list;
    struct syntax_node * node;
} symbol_item;
typedef struct symbol_table {
    int size;
    char symtype;
    struct symbol_item * items;
} symbol_table;
typedef struct syntax_node {
    char nodetype;
    char operation;
    bool evaluation;
    char instruction;
    char * identifier;
    struct data_value * value;
    struct syntax_node * nodea;
    struct syntax_node * nodeb;
    struct syntax_node * nodec;
} syntax_node;

// Global Values.
data_value * global_value;
symbol_table * global_table;

void syntax_print_node(syntax_node*);
bool syntax_check_types(syntax_node*, syntax_node*);
syntax_node * syntax_initialize();
syntax_node * syntax_create_node(
    char, char, bool, char, char*, data_value*,
    syntax_node*, syntax_node*, syntax_node*
);
syntax_node * syntax_create_expression(
    char, syntax_node*, syntax_node*, syntax_node*
);
syntax_node * syntax_create_expr(
    char, syntax_node*, syntax_node*, syntax_node*
);
syntax_node * syntax_create_term(
    char, syntax_node*, syntax_node*, syntax_node*
);
syntax_node * syntax_create_value(
    char, char*, data_value*
);
syntax_node * syntax_create_function(
    char *, data_value *, syntax_node *
);
syntax_node * syntax_create_stmt(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_assign(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_if(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_ifelse(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_while(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_read(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_print(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_return(syntax_node*, syntax_node*, syntax_node*);
syntax_node * syntax_create_arg(syntax_node*, syntax_node*, syntax_node*);

void syntax_execute_nodetype(syntax_node*);
void syntax_execute_instruction(syntax_node*);
void syntax_execute_stmt(syntax_node*);
void syntax_execute_assign(syntax_node*);
void syntax_execute_if(syntax_node*);
void syntax_execute_ifelse(syntax_node*);
void syntax_execute_while(syntax_node*);
void syntax_execute_read(syntax_node*);
void syntax_execute_print(syntax_node*);
void syntax_evaluate_expression(syntax_node*);
void syntax_operate_expr(syntax_node*);
void syntax_operate_term(syntax_node*);
void syntax_operate_identifier(syntax_node *);
void syntax_execute_function(syntax_node*);
void syntax_execute_return(syntax_node*);
void syntax_update_args(syntax_node*, param_list*);

bool symbol_assign(symbol_table *, char *, data_value *);
data_value * symbol_get_value(symbol_table *, char *);
param_list * symbol_get_list(symbol_table *, char *);
syntax_node * symbol_get_node(symbol_table *, char *);

#endif