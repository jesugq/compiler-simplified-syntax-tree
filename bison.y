%{
// Imports
#include <stdio.h>
#include <stdbool.h>

#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

#ifndef _SYMBOLH_
#define _SYMBOLH_
#include "symbol_table.h"
#endif

#ifndef _SYNTAXH_
#define _SYNTAXH_
#include "syntax_tree.h"
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
    struct symbol_table * table;
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
%type<operation> relop signo
%type<value> tipo
%type<node> opt_stmts stmt_lst stmt expression expr term factor
%type<table> opt_decls decls dec

// Grammar
%%
prog
    : opt_decls R_BEGIN opt_stmts R_END {
        table = $1;

        // The parent node is the result of all the statements.
        node = $3;
    }
;

opt_decls
    : decls {
        // Directly return the table.
        $$ = $1;
    }
    | %empty {
        // Generate an empty table.
        $$ = symbol_initialize();
    }
;

decls
    : dec S_SEMICOLON decls {
        // Directly return the table.
        $$ = $3;
    }
    | dec {
        // Directly return the table.
        $$ = $1;
    }
;

dec
    : R_VAR V_ID S_COLON tipo {
        // Verify that the identifier is unique.
        if (symbol_exists(table, $2)) {
            bison_error_identifier_repeated($2);
            YYERROR;
        }

        // Verify that the identifier was inserted.
        if (!symbol_insert_identifier(table, $2, $4)) {
            bison_error_identifier_failed($2);
            YYERROR;
        }

        // Return the table.
        $$ = table;
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
        $$ = syntax_create_stmt($1, $3, NULL);
    }
    | stmt {
        // Create a node of INSTRUCTION STMT
        $$ = syntax_create_stmt($1, NULL, NULL);
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
        syntax_node * id_node;
        char * identifier = $1;
        data_value * value = symbol_get_value(table, identifier);
        id_node = syntax_create_value(SYNTAX_IDENTIFIER, identifier, value);

        // Create a node of INSTRUCTION ASSIGN
        $$ = syntax_create_assign(id_node, $3, NULL);
        
        // Verify that the types are the same.
        if (!syntax_check_types(id_node, $3)) {
            bison_error_data_mismatch(id_node->value, $3->value);
            YYERROR;
        }
    }
    | R_IF S_PARENTL expression S_PARENTR stmt {
        // Create a node of INSTRUCTION IF
        $$ = syntax_create_if($3, $5, NULL);
    }
    | R_IFELSE S_PARENTL expression S_PARENTR stmt stmt {
        // Create a node of INSTRUCTION IFELSE
        $$ = syntax_create_ifelse($3, $5, $6);
    }
    | R_WHILE S_PARENTL expression S_PARENTR stmt {
        // Create a node of INSTRUCTION WHILE
        $$ = syntax_create_while($3, $5, NULL);
    }
    | R_READ V_ID{
        // Verify that the identifier exists.
        if (!symbol_exists(table, $2)) {
            bison_error_identifier_missing($2);
            YYERROR;
        }

        // Create a node using an identifier.
        syntax_node * id_node; 
        char * identifier = $2;
        data_value * value = symbol_get_value(table, identifier);
        id_node = syntax_create_value(SYNTAX_IDENTIFIER, identifier, value);

        // Create a node of INSTRUCTION READ
        $$ = syntax_create_read(id_node, NULL, NULL);
    }
    | R_PRINT expr {
        // Create a node of INSTRUCTION PRINT
        $$ = syntax_create_print($2, NULL, NULL);
    }
    | R_BEGIN opt_stmts R_END{
        // Skip creation and go directly to OPT_STMTS, then STMT_LST, then STMT.
        $$ = $2;
    }
;

expression
    : expr {
        // Create a node of INSTRUCTION EXPRESSION
        $$ = syntax_create_expression(DATA_ZERO, $1, NULL, NULL);
    }
    | expr relop expr {
        // Create a node of INSTRUCTION EXPRESSION
        $$ = syntax_create_expression($2, $1, $3, NULL);
        
        // Verify that the types are the same.
        if (!syntax_check_types($1, $3)) {
            bison_error_data_mismatch($1->value, $3->value);
            YYERROR;
        }
    }
;

expr
    : expr S_PLUS term {
        // Create a node of INSTRUCTION EXPR
        $$ = syntax_create_expr(DATA_SUM, $1, $3, NULL);

        // TBD: Type check.
    }
    | expr S_MINUS term {
        // Create a node of INSTRUCTION EXPR
        $$ = syntax_create_expr(DATA_SUBSTRACT, $1, $3, NULL);

        // TBD: Type check.
    }
    | signo term {
        // Create a node of INSTRUCTION EXPR
        $$ = syntax_create_expr(DATA_NEGATIVE, $2, NULL, NULL);
    }
    | term {
        // Skip creation and go directly to TERM
        $$ = $1;
    }
;

term
    : term S_ASTERISK factor {
        // Create a node of INSTRUCTION TERM
        $$ = syntax_create_term(DATA_MULTIPLY, $1, $3, NULL);

        // Verify that the types are the same.
        if (!syntax_check_types($1, $3)) {
            bison_error_data_mismatch($1->value, $3->value);
            YYERROR;
        }
    }
    | term S_SLASH factor {
        // Create a node of INSTRUCTION TERM
        $$ = syntax_create_term(DATA_DIVIDE, $1, $3, NULL);

        // Verify that the types are the same.
        if (!syntax_check_types($1, $3)) {
            bison_error_data_mismatch($1->value, $3->value);
            YYERROR;
        }
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
        syntax_node * id_node;
        char * identifier = $1;
        data_value * value = symbol_get_value(table, identifier);
        id_node = syntax_create_value(SYNTAX_IDENTIFIER, identifier, value);

        // Return the newly created node.
        $$ = id_node;
    }
    | V_NUMINT{
        // Create a node using a value.
        syntax_node * int_node;
        data_value * value = $1;
        int_node = syntax_create_value(SYNTAX_VALUE, NULL, value);

        // Return the newly created node.
        $$ = int_node;
    }
    | V_NUMFLOAT{
        // Create a node using a value.
        syntax_node * float_node;
        data_value * value = $1;
        float_node = syntax_create_value(SYNTAX_VALUE, NULL, value);

        // Return the newly created node.
        $$ = float_node;
    }
;

relop
    : S_LESS {
        $$ = DATA_LESS;
    }
    | S_GREATER {
        $$ = DATA_GREATER;
    }
    | S_EQUALS {
        $$ = DATA_EQUALS;
    }
    | S_LTE {
        $$ = DATA_LTE;
    }
    | S_GTE {
        $$ = DATA_GTE;
    }
;

signo
    : S_NEGATIVE {
        $$ = DATA_NEGATIVE;
    }
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
    table = symbol_initialize();
    node = syntax_initialize();
    yyparse();
    syntax_execute_nodetype(node);
    symbol_print(table);

    // Closure of file and system.
    if (yyin != NULL) fclose(yyin);
    return 0;
}