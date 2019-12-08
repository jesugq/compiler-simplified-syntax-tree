%{
// Imports
#include <stdio.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "syntax_tree.h"

#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

// Global Table
symbol_table * table;
syntax_node * node;

// Flex externals
extern FILE * yyin;
extern int yylineno;
extern char * yytext;
extern int yylex();
extern int yyerror(char const *);

// Declarations
void bison_parse_success();
void bison_error_identifier_repeated(char *);
void bison_error_identifier_failed(char *);
void bison_error_identifier_missing(char *);
void bison_error_data_mismatch(data_value *, data_value *);
void bison_error_data_misassign(char *, data_value *, data_value *);
%}

// Bison Union
%union {
    char operation;
    int instruction;
    char * identifier;
    struct data_value * value;
    struct syntax_node * node;
}

// Bison Terminal Types
%token<instruction> R_BEGIN R_END R_VAR R_INT R_FLOAT
%token<instruction> R_IF R_IFELSE R_WHILE R_READ R_PRINT
%token<instruction> S_SEMICOLON S_COLON S_ASSIGN
%token<instruction> S_PLUS S_MINUS S_ASTERISK S_SLASH
%token<instruction> S_PARENTL S_PARENTR S_NEGATIVE
%token<instruction> S_LESS S_GREATER S_EQUALS S_LTE S_GTE
%token<identifier> V_ID
%token<value> V_NUMINT
%token<value> V_NUMFLOAT

// Bison Non Terminal Types
%type<operation> relop
%type<value> tipo
%type<node> opt_stmts stmt_lst stmt expression expr term factor

// Grammar
%%
prog
    : opt_decls R_BEGIN opt_stmts R_END {
        // The parent node is the result of all the statements.
        node = $3;
    }
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
    : R_VAR V_ID S_COLON tipo {
        // Verify that the identifier is unique.
        if (symbol_exists(table, $2)) {
            bison_error_identifier_repeated($2);
            YYERROR;
        }

        // Verify that the identifier was inserted.
        if (!symbol_insert(table, $2, $4)) {
            bison_error_identifier_failed($2);
            YYERROR;
        }
    }
;

tipo
    : R_INT {
        // Return a zero initialized integer data value.
        $$ = data_create_integer(0);
    }
    | R_FLOAT {
        // Return a zero initialized float data value.
        $$ = data_create_float(0.0);
    }
;

opt_stmts
    : stmt_lst {
        // Skip creation and go directly to STMT_LST, then STMT.
        $$ = $1;
    }
    | %empty {
        $$ = syntax_initialize();
    }
;

stmt_lst
    : stmt S_SEMICOLON stmt_lst {
        // Create a node of INSTRUCTION STMT.
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_STMT,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, $3, SYNTAX_DNULL
        );
    }
    | stmt {
        // Create a node of INSTRUCTION STMT
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_STMT,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, SYNTAX_DNULL, SYNTAX_DNULL
        );
    }
;

stmt
    : V_ID S_ASSIGN expr {
        // Verify that the identifier exists.
        if (!symbol_exists(table, $1)) {
            bison_error_identifier_missing($1);
            YYERROR;
        }

        // Create a node using an identifier.
        char * identifier = $1;
        data_value * value = symbol_extract(table, identifier);
        syntax_node * id_node = syntax_create_node(
            SYNTAX_IDENTIFIER, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_CNULL,
            identifier, value, SYNTAX_DNULL, SYNTAX_DNULL, SYNTAX_DNULL
        );

        // Create a node of INSTRUCTION ASSIGN
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_ASSIGN,
            SYNTAX_DNULL, SYNTAX_DNULL, id_node, $3, SYNTAX_DNULL
        );
        
        // Type Check TBD.
    }
    | R_IF S_PARENTL expression S_PARENTR stmt{
        // Create a node of INSTRUCTION IF
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_IF, SYNTAX_DNULL, SYNTAX_DNULL, $3, $5, SYNTAX_DNULL
        );
    }
    | R_IFELSE S_PARENTL expression S_PARENTR stmt stmt{
        // Create a node of INSTRUCTION IFELSE
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_IFELSE, SYNTAX_DNULL, SYNTAX_DNULL, $3, $5, $6
        );
    }
    | R_WHILE S_PARENTL expression S_PARENTR stmt{
        // Create a node of INSTRUCTION WHILE
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_WHILE, SYNTAX_DNULL, SYNTAX_DNULL, $3, $5, SYNTAX_DNULL
        );
    }
    | R_READ V_ID{
        // Verify that the identifier exists.
        if (!symbol_exists(table, $2)) {
            bison_error_identifier_missing($2);
            YYERROR;
        }

        // Create a node using an identifier.
        char * identifier = $2;
        data_value * value = symbol_extract(table, identifier);
        syntax_node * id_node = syntax_create_node(
            SYNTAX_IDENTIFIER, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_CNULL,
            SYNTAX_DNULL, SYNTAX_DNULL, id_node, SYNTAX_DNULL, SYNTAX_DNULL
        );

        // Create a node of INSTRUCTION READ
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_READ,
            SYNTAX_DNULL, SYNTAX_DNULL, id_node, SYNTAX_DNULL, SYNTAX_DNULL
        );
    }
    | R_PRINT expr {
        // Create a node of INSTRUCTION PRINT
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_PRINT, SYNTAX_DNULL, SYNTAX_DNULL, $2, SYNTAX_DNULL, SYNTAX_DNULL
        );
    }
    | R_BEGIN opt_stmts R_END{
        // Skip creation and go directly to OPT_STMTS, then STMT_LST, then STMT.
        $$ = $2;
    }
;

expression
    : expr {
        // Create a node of INSTRUCTION EXPRESSION
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_ZERO, SYNTAX_BNULL, SYNTAX_EXPRESSION,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, SYNTAX_DNULL, SYNTAX_DNULL
        );
    }
    | expr relop expr {
        // Create a node of INSTRUCTION EXPRESSION
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, $2, SYNTAX_BNULL, SYNTAX_EXPRESSION,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, $3, SYNTAX_DNULL
        );

        // Type Check TBD.
    }
;

expr
    : expr S_PLUS term {
        // Create a node of INSTRUCTION EXPR
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_SUM, SYNTAX_BNULL, SYNTAX_EXPR,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, $3, SYNTAX_DNULL
        );

        // Type Check TBD.
    }
    | expr S_MINUS term {
        // Create a node of INSTRUCTION EXPR
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_SUBSTRACT, SYNTAX_BNULL, SYNTAX_EXPR,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, $3, SYNTAX_DNULL
        );

        // Type Check TBD.
    }
    | signo term {
        // Create a node of INSTRUCTION EXPR
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_NEGATIVE, SYNTAX_BNULL, SYNTAX_EXPR,
            SYNTAX_DNULL, SYNTAX_DNULL, $2, SYNTAX_DNULL, SYNTAX_DNULL
        );

        // Type Check TBD.
    }
    | term {
        // Skip creation and go directly to TERM
        $$ = $1;
    }
;

term
    : term S_ASTERISK factor {
        // Create a node of INSTRUCTION TERM
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_MULTIPLY, SYNTAX_BNULL, SYNTAX_TERM,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, $3, SYNTAX_DNULL
        );

        // Type Check TBD.
    }
    | term S_SLASH factor {
        // Create a node of INSTRUCTION TERM
        $$ = syntax_create_node(
            SYNTAX_INSTRUCTION, SYNTAX_DIVIDE, SYNTAX_BNULL, SYNTAX_TERM,
            SYNTAX_DNULL, SYNTAX_DNULL, $1, $3, SYNTAX_DNULL
        );

        // Type Check TBD.
    }
    | factor {
        // Skip creation and go directly to FACTOR
        $$ = $1;
    }
;

factor
    : S_PARENTL expr S_PARENTR{
        // Skip creation and go directly to EXPR.
        $$ = $2;
    }
    | V_ID {
        // Verify that the identifier exists.
        if (!symbol_exists(table, $1)) {
            bison_error_identifier_missing($1);
            YYERROR;
        }

        // Create a node using an identifier.
        char * identifier = $1;
        data_value * value = symbol_extract(table, identifier);
        syntax_node * id_node = syntax_create_node(
            SYNTAX_IDENTIFIER, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_CNULL,
            identifier, value, SYNTAX_DNULL, SYNTAX_DNULL, SYNTAX_DNULL
        );

        // Return the newly created node.
        $$ = id_node;
    }
    | V_NUMINT{
        // Create a node using a value.
        data_value * value = data_create_integer(0);
        syntax_node * int_node = syntax_create_node(
            SYNTAX_VALUE, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_CNULL,
            SYNTAX_DNULL, value, SYNTAX_DNULL, SYNTAX_DNULL, SYNTAX_DNULL
        );

        // Return the newly created node.
        $$ = int_node;
    }
    | V_NUMFLOAT{
        // Create a node using a value.
        data_value * value = data_create_float(0.0);
        syntax_node * float_node = syntax_create_node(
            SYNTAX_VALUE, SYNTAX_CNULL, SYNTAX_BNULL, SYNTAX_CNULL,
            SYNTAX_DNULL, value, SYNTAX_DNULL, SYNTAX_DNULL, SYNTAX_DNULL
        );

        // Return the newly created node.
        $$ = float_node;
    }
;

relop
    : S_LESS {
        $$ = SYNTAX_LESS;
    }
    | S_GREATER {
        $$ = SYNTAX_GREATER;
    }
    | S_EQUALS {
        $$ = SYNTAX_EQUALS;
    }
    | S_LTE {
        $$ = SYNTAX_LTE;
    }
    | S_GTE {
        $$ = SYNTAX_GTE;
    }
;

signo
    : S_NEGATIVE
;
%%

/**
 * YYError is redefined in Bison for it to include additional details regarding
 * the system's grammars and where it was found to be at fault.
 * @param   error   Error to print
 * @return  Integer code error.
 */
int yyerror(char const * error) {
    printf("\n%s found after reading '%s' at line %d.\n",
        error, yytext, yylineno);
}

/**
 * Bison Parse Success prints an accepted message.
 */
void bison_parse_success() {
    printf("File accepted.\n");
}

/**
 * Bison Error Identifier Repeated calls the yyerror function with a message of
 * "variable declared twice: identifier".
 * @param   identifier  String of the identifier.
 */
void bison_error_identifier_repeated(char * identifier) {
    char error[1000] = "variable declared twice: ";
    strcat(error, identifier);
    yyerror(error);
}

/**
 * Bison Error Identifier Failed calls the yyerror function with a message of
 * "variable failed to be inserted: identifier".
 * @param   identifier  String of the identifier.
 */
void bison_error_identifier_failed(char * identifier) {
    char error[1000] = "variable failed to be inserted: ";
    strcat(error, identifier);
    yyerror(error);
}

/**
 * Bison Error Identifier Missing calls the yyerror function with a message of
 * "variable not found: identifier".
 * @param   identifier  String of the identifier.
 */
void bison_error_identifier_missing(char * identifier) {
    char error[1000] = "variable not found: ";
    strcat(error, identifier);
    yyerror(error);
}

/**
 * Bison Error Data Mismatch calls the yyerror function with a message of
 * "illegal operation of: one and two".
 * @param   one     First data type.
 * @param   two     Second data type.
 */
void bison_error_data_mismatch(data_value * one, data_value * two) {
    char error[1000] = "illegal operation of ";
    char hold[1000];
    char erra[] = " and ";
    char erri[] = "int:";
    char errf[] = "float:";
    char errn[] = "unknown";

    if (one->numtype == DATA_INTEGER) {
        strcat(error, erri);
        sprintf(hold, "%d", one->number.int_value);
        strcat(error, hold);
    }
    else if (one->numtype == DATA_FLOAT) {
        strcat(error, errf);
        sprintf(hold, "%f", one->number.float_value);
        strcat(error, hold);
    }
    else strcat(error, errn);

    strcat(error, erra);

    if (two->numtype == DATA_INTEGER) {
        strcat(error, erri);
        sprintf(hold, "%d", two->number.int_value);
        strcat(error, hold);
    }
    else if (two->numtype == DATA_FLOAT) {
        strcat(error, errf);
        sprintf(hold, "%f", two->number.float_value);
        strcat(error, hold);
    }
    else strcat(error, errn);

    yyerror(error);
}

/**
 * Bison Error Identifier Missing calls the yyerror function with a message of
 * "illegal assignment of one <- two".
 * @param   identifier  Identifier of the first data type.
 * @param   one         First data type.
 * @param   two         Second data type.
 */
void bison_error_data_misassign(
    char * identifier, data_value * one, data_value * two
) {
    char error[1000] = "illegal assignment of ";
    char hold[1000];
    char errc[] = ":";
    char erra[] = " <- ";
    char erri[] = "int";
    char errf[] = "float:";
    char errn[] = "unknown";

    if (one->numtype == DATA_INTEGER) strcat(error, erri);
    else if (one->numtype == DATA_FLOAT) strcat(error, errf);
    else strcat(error, errn);

    strcat(error, errc);
    strcat(error, identifier);
    strcat(error, erra);

    if (two->numtype == DATA_INTEGER) {
        strcat(error, erri);
        sprintf(hold, "%d", two->number.int_value);
        strcat(error, hold);
    }
    else if (two->numtype == DATA_FLOAT) {
        strcat(error, errf);
        sprintf(hold, "%f", two->number.float_value);
        strcat(error, hold);
    }
    else strcat(error, errn);

    yyerror(error);
}

/**
 * Main function couples the yyparse, hash table initialize and syntax tree
 * initialize functions along for this assignment.
 * @param   argc    Argument count.
 * @param   argv    Argument values.
 * @return  Runtime code, zero for OK and one for ERR.
 */
int main(int argc, char * argv[]) {
    // Argument and file verification
    if (argc < 2) {
        printf("No file argument provided.\n");
        return 1;
    } else if ((yyin = fopen(argv[1], "r")) == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    // Flex and Bison parsing.
    table = symbol_initialize(0);
    node = syntax_initialize();
    yyparse();
    symbol_print(table);

    // Closure of file and system.
    if (yyin != NULL) fclose(yyin);
    return 0;
}