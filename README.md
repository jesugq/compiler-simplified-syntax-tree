# Assignment Specifics
## Assignment Members
Humberto Espinosa Domínguez - A01550159  
Jesús Antonio González Quevedo - A00399890

Alberto Oliart Ros - Professor for Compiler Design

## Assignment Instructions
The purpose of this assignment is to build an interpreter for the grammar given below using flex and bison. The input to the interpreter is a text file, whose name has to be given as a part of the command line in the console.

The interpreter, through the parser, must build an internal representation of the program given in the text file in the form of a simplified syntax tree, if there are no syntax errors. The parser, while constructing the simplified syntax tree, must do strong type checking and build a symbol table, which will be used during the interpretation process. If the parser finds a syntax error, it must report it and terminate the process.

Variables must be declared before use exactly once. If a variable is used and not declared, an error must be reported. Type mismatches must be reported as errors. The interpreter must do the interpretation by traversing the intermediate representation of the program.

Identifiers and numbers have to be recognized via flex using a standard definition, including negative numbers.

## Assignment Compilation
```bash
flex flex.l
bison -d bison.y
gcc lex.yy.c bison.tab.c symbol_table.c syntax_tree.c data.c -lfl -lm -o run.out
./runout file.txt

# Or the short version
flex flex.l && bison -d bison.y && gcc lex.yy.c bison.tab.c symbol_table.c syntax_tree.c data.c -lfl -lm -o run.out
./runout file.txt
```

## Assignment Nomenclature
These are the names that will be used to refer to the lexicon in both Flex and Bison. Note that the prefix R stands for Reserved Words, S stands for Symbols, and V for Values.

```
begin   R_BEGIN
end     R_END
;       S_SEMICOLON
var     R_VAR
id      V_ID
:       S_COLON
int     R_INT
float   R_FLOAT
if      R_IF
<-      S_ASSIGN
ifelse  R_IFELSE
while   R_WHILE
read    R_READ
print   R_PRINT
+       S_PLUS
-       S_MINUS
*       S_ASTERISK
/       S_SLASH
(       S_PARENTL
)       S_PARENTR
numint  V_NUMINT
numflt  V_NUMFLOAT
<       S_LESS
>       S_GREATER
=       S_EQUALS
<=      S_LTE
>=      S_GTE
~       S_NEGATIVE
```

## Assignment Grammar
```
prog
    : opt_decls opt_fun_decls R_BEGIN opt_stmts R_END
; untyped;

opt_decls
    : decls
    | %empty
; untyped;

decls
    : dec S_SEMICOLON decls
    | dec
; untyped;

dec
    : R_VAR V_ID S_COLON tipo
; untyped;

opt_fun_decls
    : fun_decls
    | %empty
; untyped;

fun_decls
    : fun_dec S_SEMICOLON fundecls
    | fun_dec
; untyped;

opt_params
    : param_lst
    | %empty
; param_list;

param_lst
    : param S_COMMA param_lst
    | param
; param_list;

param
    : V_ID S_COLON tipo
; param_list;

tipo
    : R_INT
    | R_FLOAT
; int;

opt_stmts
    : stmt_lst
    | %empty
; syntax_node;

stmt_lst
    : stmt S_SEMICOLON stmt_lst
    | stmt
; syntax_node;

stmt
    : V_ID S_ASSIGN expr
    | R_IF S_PARENTL expression S_PARENTR stmt
    | R_IFELSE S_PARENTL expression S_PARENTR stmt stmt
    | R_WHILE S_PARENTL expression S_PARENTR stmt
    | R_READ V_ID
    | R_PRINT expr
    | R_BEGIN opt_stmts R_END
    | R_RETURN expr
; syntax_node;

expression
    : expr
    | expr relop expr
; syntax_node;

expr
    : expr S_PLUS term
    | expr S_MINUS term
    | signo term
    | term
; syntax_node;

term
    : term S_ASTERISK factor
    | term S_SLASH factor
    | factor
; syntax_node;

factor
    : S_PARENTL expr S_PARENTR
    | V_ID
    | V_NUMINT
    | V_NUMFLOAT
    | V_ID S_PARENTL opt_args S_PARENTR
; syntax_node;

opt_args
    : arg_lst
    | %empty
; syntax_node;

arg_lst
    : expr S_COMMA arg_lst
    | expr
; syntax_node;

relop
    : S_LESS
    | S_GREATER
    | S_EQUALS
    | S_LTE
    | S_GTE
; char;

signo
    : S_NEGATIVE
; char;
```

# Bison Specifics
## Bison Union
Bison has to handle the values that each terminal returns inside of an union. The union value can be one of the following.
```c
%union {
    char operation;                 // Operation to do of the non terminal read.
    int instruction;                // Integer instruction of the terminal read.
    char * identifier;              // String of the idenfitier read.
    struct param_list * list;       // List of parameters.
    struct data_value value;        // Value that can either be integer or float.
    struct tree_node * node;        // Node of this expression.
    struct symbol_table * table;    // Table of the non terminals.
}
```

## Bison Terminal Types
Most terminals only return their code number, which is the Bison Union's code attribute, however, the three regular expressions have to return a value, which can be an identifier, or an integer, or a floating point.
```c
%token<code>                // All reserved terminals.
%token<identifier> V_ID     // Identifier read as a string.
%token<value> V_NUMINT      // Integer value read stored as a data_value.
%token<value> V_NUMFLOAT    // Float value read stored as a data_value.
```

## Bison Non Terminal Types
To aid the simplified tree class in the making of nodes, it is important for the grammar to be able to return sub nodes, or children nodes at will by calling them with Bison's $X operators. This implies that most of the non terminals have to be of type tree_node.

Similarly, the parameters and the 

The following are the non terminals that need a type.
```c
%type<operation> relop signo
%type<list> opt_params param_lst param
%type<value> tipo
%type<node> opt_stmts stmt_lst stmt expression expr term factor opt_args arg_lst
```

# Symbol Table Specifics
## Table Types
The Symbol Table used uses a simple hash table, implementing Java's hashCode function (https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#hashCode%28%29). Using the following structure to store the values of the hash item.

The union type used for the numeric value of the hash item are the following.
```c
typedef union data_number {
    int int_value;              // Integer depiction of a number.
    float float_value;          // Float depiction of a number.
} data_number;

typedef struct data_value {
    char numtype;               // Type of the data.
    data_number number;         // Value of the data.
} data_value;
```

Each node in the hash table has a hash key, the identifier name, its numeric type, and the value in an union of either an integer or a float, with numtype defines which to use in operations.
```c
typedef struct symbol_item {
    int key;                    // Key of the item.
    int args;                   // Argument count.
    char symtype;               // Type of the item, function or identifier.
    char * identifier;          // Name of the item.
    data_value data;            // Data of the item.
    param_list * list;          // Parameter List if the symbol is a function.
    syntax_node * node;         // Syntax node if the symbol is a function.
} hash_item;


typedef struct symbol_table {
    int size;                   // Size of the table.
    hash_item * items;          // Item array of the table.
} hash_table;
```

# Syntax Tree Specifics
## Tree Types
The Syntax Tree uses three node pointers defining what to use, its type, such as an instruction or a value, and information regarding it.
```c
typedef struct param_list {
    char * identifier;          // Identifier of this parameter.
    data_value * value;         // Value of this parameter.
    struct param_list * next;   // Next parameter.
} param_list;

typedef struct syntax_node {
    char nodetype;              // Type of this node.
    char operation;             // Operation of this node.
    bool evaluation;            // Evaluation of the node.
    char instruction;           // Instruction of this node.
    char * identifier;          // Identifier of the node
    data_value * value;         // Value of the node.
    syntax_node * nodea;        // First child node.
    syntax_node * nodeb;        // Second child node.
    syntax_node * nodec;        // Third child node.
} syntax_node;
```

## Tree Execution
The Syntax Tree nodes can be of one of many types, such as STMT, IF, ASSIGN, etc. The types and how they have to be created and executed will be shown in the following:

```c
Node of type STMT
Executes the instruction in nodea and then the instruction in nodeb if it is not NULL.
* nodetype      is INSTRUCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is STMT
* identifier    is NULL
* value         is NULL
* nodea         is INSTRUCTION of ASSIGN IF IFELSE WHILE READ PRINT or BEGINEND
* nodeb         is INSTRUCTION of STMT or NULL
* nodec         is NULL

Node of type ASSIGN
Assigns the value in nodeb to the symbol table value of the identifier in nodea.
* nodetype      is INSTRUCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is ASSIGN
* identifier    is NULL
* value         is NULL
* nodea         is IDENTIFIER
* nodeb         is INSTRUCTION OF EXPR
* nodec         is NULL

Node of type IF
Executes the instruction in nodeb if the evaluation in nodea is true.
* nodetype      is INSTRUCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is IF
* identifier    is NULL
* value         is NULL
* nodea         is INSTRUCTION of EXPRESSION
* nodeb         is INSTRUCTION of ASSIGN IF IFELSE WHILE READ PRINT or BEGINEND
* nodec         is NULL

Node of type IFELSE
Executes the instruction in nodeb if the evaluattion in nodea is true. Otherwise it executes the instruction in nodec.
* nodetype      is INSTRUCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is IFELSE
* identifier    is NULL
* value         is NULL
* nodea         is INSTRUCTION of EXPRESSION
* nodeb         is INSTRUCTION of ASSIGN IF IFELSE WHILE READ PRINT or BEGINEND
* nodec         is INSTRUCTION of ASSIGN IF IFELSE WHILE READ PRINT or BEGINEND

Node of type WHILE
Executes the instruction in nodeb while the evaluation in nodea is true.
* nodetype      is INSTRUCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is WHILE
* identifier    is NULL
* value         is NULL
* nodea         is INSTRUCTION of EXPRESSION
* nodeb         is INSTRUCTION of ASSIGN IF IFELSE WHILE READ PRINT or BEGINEND
* nodec         is NULL

Node of type READ
Executes a scanf and assigns the value read to the symbol table value of the identifier in nodea.
* nodetype      is INSTRUCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is READ
* identifier    is NULL
* value         is NULL
* nodea         is IDENTIFIER
* nodeb         is NULL
* nodec         is NULL

Node of type PRINT
Executes a printf of the value inside of the identifier in nodea.
* nodetype      is INSTRUCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is PRINT
* identifier    is NULL
* value         is NULL
* nodea         is INSTRUCTION OF EXPR
* nodeb         is NULL
* nodec         is NULL

Node of type EXPRESSION
Evaluates the value in nodea with the value in nodec using the operation in this node.
* nodetype      is INSTRUCTION
* operation     is LESS GREATER EQUALS LTE GTE or ZERO
* evaluation    is TRUE or FALSE
* instruction   is EXPRESSION
* identifier    is NULL
* value         is NULL
* nodea         is IDENTIFIER or VALUE
* nodeb         is IDENTIFIER or VALUE or NULL
* nodec         is NULL

Node of type EXPR
Operates the value in nodea and the value in nodec using the operation in this node.
* nodetype      is INSTRUCTION
* operation     is SUM SUBSTRACT NEGATIVE
* evaluation    is NULL
* instruction   is EXPR
* identifier    is NULL
* value         is INTEGER or FLOAT
* nodea         is IDENTIFIER or VALUE
* nodeb         is IDENTIFIER or VALUE
* nodec         is NULL

Node of type TERM
Operates the value in nodea and the value in nodec using the operation in this node.
* nodetype      is INSTRUCTION
* operation     is MULTIPLY or DIVIDE
* evaluation    is NULL
* instruction   is TERM
* identifier    is NULL
* value         is INTEGER or FLOAT
* nodea         is IDENTIFIER or VALUE
* nodeb         is IDENTIFIER or VALUE
* nodec         is NULL

Node of type FACTOR
Does not exist, but it can return one of three types:

Node of type FACTOR IDENTIFIER
Returns the value in this node. Since the data_value will be linked to the symbol_table, any updates e.g. during while functions should be visible.
* nodetype      is IDENTIFIER
* operation     is NULL
* evaluation    is NULL
* instruction   is NULL
* identifier    is IDENTIFIER
* value         is INTEGER or FLOAT
* nodea         is NULL
* nodeb         is NULL
* nodec         is NULL

Node of type FACTOR VALUE
Returns the value in this node.
* nodetype      is VALUE
* operation     is NULL
* evaluation    is NULL
* instruction   is NULL
* identifier    is NULL
* value         is INTEGER or FLOAT
* nodea         is NULL
* nodeb         is NULL
* nodec         is NULL

Node of type FACTOR FUNCTION
Returns the value of this node. Function is a tricky one, first it has to update the values in the
symbol table for the parameters in this function using the arguments in node a.
Afterwards it needs to execute all of its nodes, and all of those nodes will bring back a value using
the return command.
Returns the value in this node. But since all calls to FACTOR execute the node, the function node is responsible for updating its value by running the statements inside of it first.
* nodetype      is INSTRUCTION of FUNCTION
* operation     is NULL
* evaluation    is NULL
* instruction   is FUNCTION
* identifier    is IDENTIFIER
* value         is INTEGER or FLOAT
* nodea         is INSTRUCTION of ARG
* nodeb         is NULL
* nodec         is NULL

Node of type ARG
Updates the value in the nodea using the value in the nodeb and then executes nodec.
* nodetype      is INSTRUCTION of ARG
* operation     is NULL
* evaluation    is NULL
* instruction   is ARG
* identifier    is NULL
* value         is INTEGER or FLOAT
* nodea         is IDENTIFIER or VALUE
* nodeb         is INSTRUCTION of EXPR
* nodec         is INSTRUCTION of ARG or NULL
```