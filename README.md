# Assignment Six - Simplified Syntax Tree
## Members
Humberto Espinosa Domínguez - A01550159  
Jesús Antonio González Quevedo - A00399890

Alberto Oliart Ros - Professor for Compiler Design

## Instructions
The purpose of this assignment is to build an interpreter for the grammar given below using flex and bison. The input to the interpreter is a text file, whose name has to be given as a part of the command line in the console.

The interpreter, through the parser, must build an internal representation of the program given in the text file in the form of a simplified syntax tree, if there are no syntax errors. The parser, while constructing the simplified syntax tree, must do strong type checking and build a symbol table, which will be used during the interpretation process. If the parser finds a syntax error, it must report it and terminate the process.

Variables must be declared before use exactly once. If a variable is used and not declared, an error must be reported. Type mismatches must be reported as errors. The interpreter must do the interpretation by traversing the intermediate representation of the program.

Identifiers and numbers have to be recognized via flex using a standard definition, including negative numbers.

## Terminal Nomenclature
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

## Grammar
```
prog
    : opt_decls R_BEGIN opt_stmts R_END
;

opt_decls
    : decls
    | %empty
;

decls
    : dec S_SEMICOLON decls
    | dec
;

dec
    : R_VAR V_ID S_COLON tipo
;

tipo
    : R_INT
    | R_FLOAT
;

opt_stmts
    : stmt_lst
    | %empty
;

stmt_lst
    : stmt S_SEMICOLON stmt_lst
    | stmt
;

stmt
    : V_ID S_ASSIGN expr
    | R_IF S_PARENTL expression S_PARENTR stmt
    | R_IFELSE S_PARENTL expression S_PARENTR stmt stmt
    | R_WHILE S_PARENTL expression S_PARENTR stmt
    | R_READ V_ID
    | R_PRINT expr
    | R_BEGIN opt_stmts R_END
;

expression
    : expr
    | expr relop expr
;

expr
    : expr S_PLUS term
    | expr S_MINUS term
    | signo term
    | term
;

term
    : term S_ASTERISK factor
    | term S_SLASH factor
    | factor
;

factor
    : S_PARENTL expr S_PARENTR
    | V_ID
    | V_NUMINT
    | V_NUMFLOAT
;

relop
    : S_LESS
    | S_GREATER
    | S_EQUALS
    | S_LTE
    | S_GTE
;

signo
    : S_NEGATIVE
;
```

## Bison Union
Bison has to handle the values that each terminal returns inside of an union. The union value can be one of the following.
```c
%union {
    int code;           // Integer code of the terminal read.
    char * identifier;  // String of the idenfitier read.
    NUMERIC value       // Value that can either be integer or float.
}
```

## Terminal Types
Most terminals only return their code number, which is the Bison Union's code attribute, however, the three regular expressions have to return a value, which can be an identifier, or an integer, or a floating point.
```c
%token<code>                    // All reserved terminals.
%token<identifier> V_ID         // Identifier read as a string.
%token<NUMERIC> V_NUMINT        // Integer value read stored as a NUMERIC of type int.
```

## Non Terminal Types
In order to place functions regarding strong type checking inside of the Bison grammar, it is important to define the types that the non terminals will return, mainly the ones that inquire the integer or floating point values.

As such, the tipo, expr, term and factor non terminals return a character which defines if the value to strong type check is an integer or a floating point.
```c
%type<numtype> tipo     // Returns the type of the reserved word read.
%type<numtype> expr     // Returns the type of the term non terminal.
%type<numtype> term     // Returns the type of the factor non terminal.
%type<numtype> factor   // Returns the type of the identifier, integer or float read.
```

## Symbol Table Types
The Symbol Table used uses a simple hash table, implementing Java's hashCode function (https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#hashCode%28%29). Using the following structure to store the values of the hash item.

The union type used for the numeric value of the hash item are the following.
```c
typedef union NUMBER {
    int int_value;      // Integer depiction of a number.
    float float_value;  // Float depiction of a number.
} NUMBER;

typedef union DATA {
    char numtype;       // Type of the data.
    union NUMBER value; // Value of the data.
} DATA;
```

Each node in the hash table has a hash key, the identifier name, its numeric type, and the value in an union of either an integer or a float, with numtype defines which to use in operations.
```c
typedef struct symbol_item {
    int key;
    char * identifier;      // Name of the item.
    DATA value;             // Data of the item.
} hash_item;
```

These nodes are stored in a hash table, which stores the integer for its size, the array of hash items, and an integer denoting the recursion level this table is currently in.
```c
typedef struct symbol_table {
    int size;                   // Size of the table.
    int level;                  // Stack level of the table.
    struct hash_item * items;   // Item array of the table.
} hash_table;
```

## Syntax Tree Types
The Syntax Tree uses three node pointers defining what to use, its type, such as an instruction or a value, and an union value regarding the contents of this node, with the type defining how to use them.
```c
typedef struct syntax_node {
    char nodetype;          // Type of this node, used to handle the data.
    union data {            // Data of this node.
        int instruction;    // Instruction to execute of this node.
        char * identifier;  // Identifier to read of this node.
        DATA value;         // Data of the item.
    }
} node;
```

## Functions Information
The one recommended by the professor is the following.
Número
Tipo
Número de Parámetros
Lista de Parámetros
