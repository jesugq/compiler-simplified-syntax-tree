// Imports
#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

// Definitions
// Node Types
#define SYNTAX_INSTRUCTION  '0'
#define SYNTAX_IDENTIFIER   '1'
#define SYNTAX_VALUE        '2'

// Instructions
#define SYNTAX_STMT         'S'
#define SYNTAX_ASSIGN       'A'
#define SYNTAX_IF           'I'
#define SYNTAX_IFELSE       'E'
#define SYNTAX_WHILE        'W'
#define SYNTAX_READ         'R'
#define SYNTAX_PRINT        'P'
#define SYNTAX_BEGINEND     'B'
#define SYNTAX_EXPRESSION   'e'
#define SYNTAX_RELOP        'r'
#define SYNTAX_EXPR         'e'
#define SYNTAX_TERM         't'
#define SYNTAX_FACTOR       'f'

// Operations
#define SYNTAX_LESS         '<'
#define SYNTAX_GREATER      '>'
#define SYNTAX_EQUALS       '='
#define SYNTAX_LTE          'l'
#define SYNTAX_GTE          'g'
#define SYNTAX_ZERO         'z'
#define SYNTAX_SUM          '+'
#define SYNTAX_SUBSTRACT    '-'
#define SYNTAX_MULTIPLY     '*'
#define SYNTAX_DIVIDE       '/'
#define SYNTAX_NEGATIVE     '~'

// Declarations
typedef struct syntax_node {
    char nodetype;
    char operation;
    bool evaluation;
    char instruction;
    char * identifier;
    data_value * value;
    syntax_node * nodea;
    syntax_node * nodeb;
    syntax_node * nodec;
} syntax_node;

syntax_node * syntax_create_node(
    char, bool, char*, data_value*, syntax_node*, syntax_node*, syntax_node*
);

// syntax_node * syntax_create_stmt();
// syntax_node * syntax_create_assign();
// syntax_node * syntax_create_if();
// syntax_node * syntax_create_ifelse();
// syntax_node * syntax_create_while();
// syntax_node * syntax_create_read();
// syntax_node * syntax_create_print();
// syntax_node * syntax_create_beginend();
// syntax_node * syntax_create_expression();
// syntax_node * syntax_create_relop();
// syntax_node * syntax_create_expr();
// syntax_node * syntax_create_term();
// syntax_node * syntax_create_identifier();
// syntax_node * syntax_create_integer();
// syntax_node * syntax_create_float();