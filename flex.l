%{
// Imports
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "bison.tab.h"

// Declarations
int flex_left_value_code(int);
int flex_left_value_identifier(int);
int flex_left_value_integer(int);
int flex_left_value_float(int);
%}

ALPHA   [a-zA-Z]
DIGIT   [0-9]
DOT     [.]

%%
"begin"     { return flex_left_value_code(R_BEGIN); }
"end"       { return flex_left_value_code(R_END); }
"var"       { return flex_left_value_code(R_VAR); }
"int"       { return flex_left_value_code(R_INT); }
"float"     { return flex_left_value_code(R_FLOAT); }
"if"        { return flex_left_value_code(R_IF); }
"ifelse"    { return flex_left_value_code(R_IFELSE); }
"while"     { return flex_left_value_code(R_WHILE); }
"read"      { return flex_left_value_code(R_READ); }
"print"     { return flex_left_value_code(R_PRINT); }

";"     { return flex_left_value_code(S_SEMICOLON); }
":"     { return flex_left_value_code(S_COLON); }
"<-"    { return flex_left_value_code(S_ASSIGN); }
"+"     { return flex_left_value_code(S_PLUS); }
"-"     { return flex_left_value_code(S_MINUS); }
"*"     { return flex_left_value_code(S_ASTERISK); }
"/"     { return flex_left_value_code(S_SLASH); }
"("     { return flex_left_value_code(S_PARENTL); }
")"     { return flex_left_value_code(S_PARENTR); }
"~"     { return flex_left_value_code(S_NEGATIVE); }
"<"     { return flex_left_value_code(S_LESS); }
">"     { return flex_left_value_code(S_GREAT); }
"="     { return flex_left_value_code(S_EQUALS); }
"<="    { return flex_left_value_code(S_LTE); }
">="    { return flex_left_value_code(S_GTE); }
"\n"    { yylineno++; }

{DIGIT}+                    { return flex_left_value_identifier(V_ID); }
{DIGIT}*{DOT}{DIGIT}+       { return flex_left_value_integer(V_NUMINT); }
{ALPHA}({ALPHA}|{DIGIT})*   { return flex_left_value_float(V_NUMFLOAT); }
%%

/**
 * Flex Left Value Code returns the terminal code to Flex and updates Bison's
 * code attribute with the terminal code.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int flex_left_value_code(int code) {
    yylval.code = code;
    return code;
}

/**
 * Flex Left Value Identifier returns the terminal code to flex and updates 
 * Bison's identifier attribute with a copy of the current yytext read.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int flex_left_value_identifier(int code) {
    yylval.identifier = strdup(yytext);
    return code;
}

/**
 * Flex Left Value Integer returns the terminal code to flex and updates Bison's
 * int_value attribute with the integer equivalent of the yytext read.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int flex_left_value_integer(int code) {
    yylval.data = data_create_integer(atoi(yytext));
    return code;
}

/**
 * Flex Left Value Float returns the terminal code to flex and updates Bison's
 * float_value attribute with the float equivalent of the yytext read.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int flex_left_value_float(int code) {
    yylval.data = data_create_float(atof(yytext));
    return code;
}