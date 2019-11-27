%{
// Imports
#include <stdio.h>
#include <stdbool.h>
#include "symbol_table.h"

#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

// Declarations
#define OPERATION_SUM '+'
#define OPERATION_SUBSTRACT '-'
#define OPERATION_MULTIPLY '*'
#define OPERATION_DIVIDE '/'

// Global Table
symbol_table * table;

// Flex externals
extern FILE * yyin;
extern int yylineno;
extern char * yytext;
extern int yylex();
extern int yyerror(char const *);

// Declarations
void bison_parse_success();
bool bison_table_identifier_exists(char *);
bool bison_table_identifier_insert(char *, data_value *);
data_value * bison_table_identifier_data(char *);
bool bison_data_numtype_match(data_value *, data_value *);
data_value * bison_data_value_operation(data_value *, data_value *, char);
data_value * bison_data_value_negative(data_value *);
data_value * bison_data_integer_operation(data_value *, data_value *, char);
data_value * bison_data_float_operation(data_value *, data_value *, char);
void bison_error_identifier_repeated(char *);
void bison_error_identifier_failed(char *);
void bison_error_identifier_missing(char *);
void bison_error_data_mismatch(data_value *, data_value *);
void bison_error_data_misassign(char *, data_value *, data_value *);
%}

// Bison Union
%union {
    int code;
    char * identifier;
    char numtype;
    struct data_value * value;
}

// Bison Terminal Types
%token<code> R_BEGIN R_END R_VAR R_INT R_FLOAT
%token<code> R_IF R_IFELSE R_WHILE R_READ R_PRINT
%token<code> S_SEMICOLON S_COLON S_ASSIGN
%token<code> S_PLUS S_MINUS S_ASTERISK S_SLASH
%token<code> S_PARENTL S_PARENTR S_NEGATIVE
%token<code> S_LESS S_GREATER S_EQUALS S_LTE S_GTE
%token<identifier> V_ID
%token<value> V_NUMINT
%token<value> V_NUMFLOAT

// Bison Non Terminal Types
%type<value> tipo expr term factor

// Grammar
%%
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
    : R_VAR V_ID S_COLON tipo {
        // Verify the identifier is unique.
        if (bison_table_identifier_exists($2)) {
            bison_error_identifier_repeated($2);
            YYERROR;
        }

        // Verify that the identifier was inserted.
        if (!bison_table_identifier_insert($2, $4)) {
            bison_error_identifier_failed($2);
            YYERROR;
        }
    }
;

tipo
    : R_INT {
        // Return a zero initialized integer data type.
        $$ = data_create_integer(0);
    }
    | R_FLOAT {
        // Return a zero initialized float data type.
        $$ = data_create_float(0);
    }
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
    : V_ID S_ASSIGN expr {
        // Verify that the identifier exists.
        if (!bison_table_identifier_exists($1)) {
            bison_error_identifier_missing($1);
            YYERROR;
        }

        // Type check the identifier's data and the expr
        data_value * id = bison_table_identifier_data($1);
        if (!bison_data_numtype_match(id, $3)) {
            bison_error_data_misassign($1, id, $3);
            YYERROR;
        }
    }
    | R_IF S_PARENTL expression S_PARENTR stmt
    | R_IFELSE S_PARENTL expression S_PARENTR stmt stmt
    | R_WHILE S_PARENTL expression S_PARENTR stmt
    | R_READ V_ID
    | R_PRINT expr
    | R_BEGIN opt_stmts R_END
;

expression
    : expr
    | expr relop expr {
        // Type check expr and expr
        if (!bison_data_numtype_match($1, $3)) {
            bison_error_data_mismatch($1, $3);
            YYERROR;
        }
    }
;

expr
    : expr S_PLUS term {
        // Type check expr and term
        if (!bison_data_numtype_match($1, $3)) {
            bison_error_data_mismatch($1, $3);
            YYERROR;
        }

        // Return the operation of both.
        $$ = bison_data_value_operation($1, $3, OPERATION_SUM);

    }
    | expr S_MINUS term {
        // Type check expr and term
        if (!bison_data_numtype_match($1, $3)) {
            bison_error_data_mismatch($1, $3);
            YYERROR;
        }

        // Return the operation of both.
        $$ = bison_data_value_operation($1, $3, OPERATION_SUBSTRACT);
    }
    | signo term {
        // Return the operation of the single term.
        $$ = bison_data_value_negative($2);
    }
    | term
;

term
    : term S_ASTERISK factor {
        // Type check term and factor.
        if (!bison_data_numtype_match($1, $3)) {
            bison_error_data_mismatch($1, $3);
            YYERROR;
        }

        // Return the operation of both.
        $$ = bison_data_value_operation($1, $3, OPERATION_MULTIPLY);
    }
    | term S_SLASH factor {
        // Type check term and factor.
        if (!bison_data_numtype_match($1, $3)) {
            bison_error_data_mismatch($1, $3);
            YYERROR;
        }

        // Return the operation of both.
        $$ = bison_data_value_operation($1, $3, OPERATION_DIVIDE);
    }
    | factor
;

factor
    : S_PARENTL expr S_PARENTR {
        // Return the data between the parentheses.
        $$ = $2;
    }
    | V_ID {
        // Verify that the identifier exists.
        if (!bison_table_identifier_exists($1)) {
            bison_error_identifier_missing($1);
            YYERROR;
        }

        // Return the data of the identifier.
        $$ = bison_table_identifier_data($1);
    }
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
 * @param   value       Value of the identifier.
 * @return  True if the insertion was successful.
 */
bool bison_table_identifier_insert(char * identifier, data_value * value) {
    return symbol_table_insert(table, identifier, value);
}

/**
 * Bison Table Identifier Data returns the data of the identifier.
 * @param   identifier  String of the identifier.
 * @return  Data of the identifier.
 */
data_value * bison_table_identifier_data(char * identifier) {
    int index = symbol_table_search(table, identifier);
    if (index == SYMBOL_NOT_FOUND) return data_create_integer(0);
    else return symbol_table_get_data(table, index);
}

/**
 * Bison Data Numtype Match checks the types of two data unions.
 * @param   one     Data of the first element.
 * @param   two     Data of the second element.
 * @return  If both match the same numtype.
 */
bool bison_data_numtype_match(data_value * one, data_value * two) {
    return one->numtype == two->numtype;
}

/**
 * Bison Data Value Operation checks which type of operation to do, an integer
 * or a floating point operation depending on the numtype of them. It is
 * assumed that both are the same, but error handling is present.
 * @param   one         Data of the first element.
 * @param   two         Data of the second element.
 * @param   operation   Operation to do with the two datas.
 * @return  Arithmetic expression between the two->
 */
data_value * bison_data_value_operation(
    data_value * one, data_value * two, char operation
) {
    if (one->numtype != two->numtype) return data_create_integer(0);
    if (one->numtype == TYPE_INTEGER)
        return bison_data_integer_operation(one, two, operation);
    else if (one->numtype == TYPE_FLOAT)
        return bison_data_float_operation(one, two, operation);
    else return data_create_integer(0);
}

/**
 * Bison Data Value Negative simply returns a new data type with a negative
 * version of the one included as a paremter.
 * @param   one     Data of the only element.
 * @return  Negative data equivalent of the parameter.
 */
data_value * bison_data_value_negative(data_value * one) {
    switch(one->numtype) {
        case TYPE_INTEGER: return data_create_integer(one->number.int_value * -1);
        case TYPE_FLOAT: return data_create_float(one->number.float_value * -1);
        default: return data_create_float(0);
    }
}

/**
 * Bison Data Integer Operation handles an arithmethic operation between two
 * datas of type integer, and returns the new data type.
 * @param   one         Data of the first element.
 * @param   two         Data of the second element.
 * @param   operation   Operation to do with the two datas.
 * @return  Arithmetic expression between the two->
 */
data_value * bison_data_integer_operation(
    data_value * one, data_value * two, char operation
) {
    switch(operation) {
        case OPERATION_SUM:
            return data_create_integer(
                one->number.int_value + two->number.int_value
            );
        case OPERATION_SUBSTRACT:
            return data_create_integer(
                one->number.int_value - two->number.int_value
            );
        case OPERATION_MULTIPLY:
            return data_create_integer(
                one->number.int_value * two->number.int_value
            );
        case OPERATION_DIVIDE:
            return data_create_integer(
                one->number.int_value / two->number.int_value
            );
        default:
            return data_create_integer(0);
    }
}

/**
 * Bison Data Float Operation handles an arithmethic operation between two
 * datas of type integer, and returns the new data type.
 * @param   one         Data of the first element.
 * @param   two         Data of the second element.
 * @param   operation   Operation to do with the two datas.
 * @return  Arithmetic expression between the two->
 */
data_value * bison_data_float_operation(
    data_value * one, data_value * two, char operation
) {
    switch(operation) {
        case OPERATION_SUM:
            return data_create_float(
                one->number.float_value + two->number.float_value
            );
        case OPERATION_SUBSTRACT:
            return data_create_float(
                one->number.float_value - two->number.float_value
            );
        case OPERATION_MULTIPLY:
            return data_create_float(
                one->number.float_value * two->number.float_value
            );
        case OPERATION_DIVIDE:
            return data_create_float(
                one->number.float_value / two->number.float_value
            );
        default:
            return data_create_float(0);
    }
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

    if (one->numtype == TYPE_INTEGER) {
        strcat(error, erri);
        sprintf(hold, "%d", one->number.int_value);
        strcat(error, hold);
    }
    else if (one->numtype == TYPE_FLOAT) {
        strcat(error, errf);
        sprintf(hold, "%f", one->number.float_value);
        strcat(error, hold);
    }
    else strcat(error, errn);

    strcat(error, erra);

    if (two->numtype == TYPE_INTEGER) {
        strcat(error, erri);
        sprintf(hold, "%d", two->number.int_value);
        strcat(error, hold);
    }
    else if (two->numtype == TYPE_FLOAT) {
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

    if (one->numtype == TYPE_INTEGER) strcat(error, erri);
    else if (one->numtype == TYPE_FLOAT) strcat(error, errf);
    else strcat(error, errn);

    strcat(error, errc);
    strcat(error, identifier);
    strcat(error, erra);

    if (two->numtype == TYPE_INTEGER) {
        strcat(error, erri);
        sprintf(hold, "%d", two->number.int_value);
        strcat(error, hold);
    }
    else if (two->numtype == TYPE_FLOAT) {
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
    table = symbol_table_initialize(0);
    yyparse();
    symbol_table_print(table);

    // Closure of file and system.
    if (yyin != NULL) fclose(yyin);
    return 0;
}