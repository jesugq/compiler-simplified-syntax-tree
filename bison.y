%{
// Imports
#include <stdio.h>
#include <stdbool.h>
#include "symbol_table.c"

// Definitions
#define TYPE_INTEGER    'I'
#define TYPE_FLOAT      'F'

// Flex externals
extern FILE * yyin;
extern int yyerror;
extern int yylineno;
extern char * yytext;
extern int yylex();
extern int yyerror(char const *);

// Symbol Table externals
extern typedef union NUMERIC NUMERIC;
extern typedef struct symbol_item symbol_item;
extern typedef struct symbol_table symbol_table;

// Declarations
void bison_parse_success();
bool bison_table_identifier_exists(char *);
bool bison_table_identifier_insert(char *);
void bison_error_identifier_repeated(char *);
void bison_error_identifier_failed(char *);
%}

// Bison Union
%union {
    int code;
    char * identifier;
    char numtype;
    int int_value;
    float float_value;
}

// Bison Terminal Types
%token<code> R_BEGIN R_END R_VAR R_INT R_FLOAT
%token<code> R_IF R_IFELSE R_WHILE R_READ R_PRINT
%token<code> S_SEMICOLON S_COLON S_ASSIGN
%token<code> S_PLUS S_MINUS S_ASTERISK S_SLASH
%token<code> S_PARENTL S_PARENTR S_NEGATIVE
%token<code> S_LESS S_GREATER S_EQUALS S_LTE S_GTE
%token<identifier> V_ID
%token<int_value> V_NUMINT
%token<float_value> V_NUMFLOAT

// Bison Non Terminal Types
%type<numtype> tipo expr term factor

// Grammar
%%
prog
    : opt_decls R_BEGIN opt_stmts R_END {
        bison_parse_success();
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
        // Verify that the identifier is unique, then verify that the
        // identifier was inserted. If neither returns true, YYERROR.
        if (!bison_table_identifier_exists($2)) {
            if (!bison_table_identifier_insert($2, $4)){
                bison_error_identifier_failed($2, $4);
                YYERROR;
            }
        } else {
            bison_error_identifier_repeated($2);
            YYERROR;
        }
    }
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
    | termbison_error_identifier_repeated($2);
                YYERROR;
;

term
    : termbison_error_identifier_repeated($2);
                YYERROR;
    | termbison_error_identifier_repeated($2);
                YYERROR;
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
%%

/**
 * YYError is redefined in Bison for it to include additional details regarding
 * the system's grammars and where it was found to be at fault.
 * @param   error   Error to print
 * @return  Integer code error.
 */
int yyerror(char const * error) {
    printf("%s found after reading '%s' at line %d.\n",
        error, yytext, yylineno);
    return yyerror;
}

/**
 * Bison Parse Success prints an accepted message.
 */
void bison_parse_success() {
    printf("File accepted.\n");
}

/**
 * Bison Table Identifier Exists checks if the identifier can be found in the  * symbol table, but not necessarily where it is. Useful for things like
 * prevention of duplicates.
 * @param   identifier  String of the identifier.
 * @return  True if the identifier was found on the symbol table.
 */
bool bison_table_identifier_exists(char * identifier) {
    return symbol_table_search(table, identifier) >= 0;
}

/**
 * Bison Table Identifier Insert returns true if insertion of a new node using
 * the identifier and the numtype was successful.
 * @param   identifier  String of the identifier.
 * @param   numtype     Type of the identifier.
 * @return  True if the insertion was successful.
 */
bool bison_table_identifier_insert(char * identifier, char numtype) {
    return symbol_table_insert(table, identifier, numtype);
}

/**
 * Bison Error Identifier Repeated calls the yyerror function with a message of
 * "variable declared twice: identifier".
 * @param   identifier  String of the identifier.
 */
void bison_error_identifier_repeated(char * identifier) {
    char error[] = "variable declared twice: "
    strcat(error, identifier);
    yyerror(error);
}

/**
 * Bison Error Identifier Failed calls the yyerror function with a message of
 * "variable failed to be inserted: identifier".
 * @param   identifier  String of the identifier.
 */
void bison_error_identifier_failed(char * identifier) {
    char error[] = "variable failed to be inserted: "
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
    } else if (yyin = fopen(argv[1]) == NULL) {
        printf("Failed to open file.\n");
        return 1;
    } else
        printf("File opened successfully.\n");

    // Flex and Bison parsing.
    struct symbol_table * table;
    table = symbol_table_initialize(0);
    yyparse();
    symbol_table_print(table);
    symbol_table_terminate(table);

    // Closure of file and system.
    if (yyin != NULL) fclose(yyin);
    return 0,
}