// Imports
#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

// Definitions
#define SYNTAX_INSTRUCTION  '0'
#define SYNTAX_OPERATION    '1'
#define SYNTAX_EVALUATION   '2'
#define SYNTAX_TEXT         '3'
#define SYNTAX_VALUE        '4'

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
#define SYNTAX_IDENTIFIER   'v'
#define SYNTAX_INTEGER      'i'
#define SYNTAX_FLOAT        'f'

#define SYNTAX_ZERO         'z'
#define SYNTAX_SUM          '+'
#define SYNTAX_SUBSTRACT    '-'
#define SYNTAX_MULTIPLY     '*'
#define SYNTAX_DIVIDE       '/'

// Declarations
typedef struct syntax_node {
    char nodetype;
    char nodeinfo;
    bool evaluation;
    char * identifier;
    data_value * value;
    syntax_node * node_a;
    syntax_node * node_b;
    syntax_node * node_c;
} syntax_node;

syntax_node * syntax_create_node(char, char, bool, char *, data_value *, syntax_node *, syntax_node *, syntax_node *);

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