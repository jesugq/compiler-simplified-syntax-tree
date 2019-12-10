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
char * function_identifier;
int function_args = 0;

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
void bison_error_not_function(char *);
void bison_error_not_identifier(char *);
void bison_error_arg_mismatch(char *);
%}

// Bison Union
%union {
    char operation;
    int instruction;
    char * identifier;
    struct param_list * list;
    struct data_value * value;
    struct syntax_node * node;
    struct symbol_table * table;
}

// Bison Terminal Types
%token<instruction> R_BEGIN R_END R_VAR R_INT R_FLOAT
%token<instruction> R_IF R_IFELSE R_WHILE R_READ R_PRINT R_FUN R_RETURN
%token<instruction> S_SEMICOLON S_COLON S_COMMA S_ASSIGN
%token<instruction> S_PLUS S_MINUS S_ASTERISK S_SLASH
%token<instruction> S_PARENTL S_PARENTR S_NEGATIVE
%token<instruction> S_LESS S_GREATER S_EQUALS S_LTE S_GTE
%token<identifier> V_ID
%token<value> V_NUMINT
%token<value> V_NUMFLOAT

// Bison Non Terminal Types
%type<operation> relop signo
%type<list> opt_params param_lst param
%type<value> tipo
%type<node> opt_stmts stmt_lst stmt expression expr term factor opt_args arg_lst

// Grammar
%%
prog
    : opt_decls opt_fun_decls R_BEGIN opt_stmts R_END {
        // The node is given by opt_stmts.
        node = $4;
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
        if (!symbol_insert_identifier(table, $2, $4)) {
            bison_error_identifier_failed($2);
            YYERROR;
        }
    }
;

opt_fun_decls
    : fun_decls
    | %empty
;

fun_decls
    : fun_dec S_SEMICOLON fun_decls
    | fun_dec
;

fun_dec
    : R_FUN V_ID S_PARENTL opt_params S_PARENTR S_COLON tipo
      opt_decls R_BEGIN opt_stmts R_END {
        // Verify that the identifier is unique.
        if (symbol_exists(table, $2)) {
            bison_error_identifier_repeated($2);
            YYERROR;
        }

        // Verify that the identifier was inserted.
        if (!symbol_insert_function(table, $2, $7, $4, $10)) {
            bison_error_identifier_failed($2);
            YYERROR;
        }
    }
;

opt_params
    : param_lst {
        // Directly return the node.
        $$ = $1;
    }
    | %empty {
        // Return an empty node.
        $$ = NULL;
    }
;

param_lst
    : param S_COMMA param_lst {
        // Create a node and return it.
        $$ = symbol_param_join($1, $3);
    }
    | param {
        // Directly return the node.
        $$ = $1;
    }
;

param
    : V_ID S_COLON tipo {
        // Verify that the identifier is unique.
        if (symbol_exists(table, $1)) {
            bison_error_identifier_repeated($1);
            YYERROR;
        }

        // Verify that the identifier was inserted.
        if (!symbol_insert_identifier(table, $1, $3)) {
            bison_error_identifier_failed($1);
            YYERROR;
        }

        // Create a parameter using an identifier.
        $$ = symbol_param_create($1, $3);
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
    | R_RETURN expr {
        // Create a node of INSTRUCTION RETURN
        $$ = syntax_create_return($2, NULL, NULL);
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

        // Verify that the identifier is an identifier.
        if (!symbol_is_identifier(table, $1)) {
            bison_error_not_identifier($1);
            YYERROR;
        }

        // Create a node using an identifier.
        syntax_node * id_node;
        data_value * value = symbol_get_value(table, $1);
        id_node = syntax_create_value(SYNTAX_IDENTIFIER, $1, value);

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
    | V_ID S_PARENTL opt_args S_PARENTR {
        // Verify that the identifier exists.
        if (!symbol_exists(table, $1)) {
            bison_error_identifier_missing($1);
            YYERROR;
        }

        // Verify that the identifier is a function.
        if (!symbol_is_function(table, $1)) {
            bison_error_not_function($1);
            YYERROR;
        }

        // Verify that the argument count is correct.
        if (!symbol_param_equal(table, $1, function_args)) {
            bison_error_arg_mismatch($1);
            YYERROR;
        }

        // Create a node that runs a function.
        syntax_node * function_node;
        data_value * value = symbol_get_value(table, $1);
        function_node = syntax_create_function($1, value, $3);
        
        // Return the newly created node.
        function_args = 0;
        $$ = function_node;
    }
;

opt_args
    : arg_lst {
        // Directly return the parameter.
        $$ = $1;
    }
    | %empty {
        // Return nothing.
        $$ = NULL;
    }
;

arg_lst
    : expr S_COMMA arg_lst {
        // Create a node of arg.
        function_args ++;
        $$ = syntax_create_arg($1, $3, NULL);
    }
    | expr {
        function_args ++;
        $$ = syntax_create_arg($1, NULL, NULL);
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
 * Bison Error Not Identifier prints that the identifier read was found to be
 * a function instead of an identifier.
 * @param   identifier  Identifier found to be a function.
 */
void bison_error_not_identifier(char * identifier) {
    char error[1000] = "called identifier but found a function: ";
    strcat(error, identifier);
    yyerror(error);
}

/**
 * Bison Error Not Function prints that the identifier read was found to be
 * an identifier instead of a function.
 * @param   identifier  Identifier found not to be a function.
 */
void bison_error_not_function(char * identifier) {
    char error[1000] = "called a function but found an identifier: ";
    strcat(error, identifier);
    yyerror(error);
}

/**
 * Bison Error Argument Mismatch prints that the lexicon was found to be using
 * an incorrect amount of parameters for a function call.
 * @param   identifier  Identifier found to be called incorrectly.
 */
void bison_error_arg_mismatch(char * identifier) {
    char error[1000] = "called function with a wrong amount of parameters: ";
    strcat(error, identifier);
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
    int success = yyparse();
    if (success) {
        global_table = table;
        global_value = NULL;
        syntax_execute_nodetype(node);
    }
    symbol_print(table);

    // Closure of file and system.
    if (yyin != NULL) fclose(yyin);
    return 0;
}