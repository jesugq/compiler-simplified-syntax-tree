%{
// Imports
#include <stdlib.h>
#include <string.h>
#include "bison.tab.h"

// Declarations
int left_value_code(int);
int left_value_identifier(int);
int left_value_integer(int);
int left_value_float(int);
%}

ALPHA   [a-zA-Z]
DIGIT   [0-9]
DOT     [.]

%%
"begin"     { return left_value_code(R_BEGIN); }
"end"       { return left_value_code(R_END); }
"var"       { return left_value_code(R_VAR); }
"int"       { return left_value_code(R_INT); }
"float"     { return left_value_code(R_FLOAT); }
"if"        { return left_value_code(R_IF); }
"ifelse"    { return left_value_code(R_IFELSE); }
"while"     { return left_value_code(R_WHILE); }
"read"      { return left_value_code(R_READ); }
"print"     { return left_value_code(R_PRINT); }

";"     { return left_value_code(S_SEMICOLON); }
":"     { return left_value_code(S_COLON); }
"<-"    { return left_value_code(S_ASSIGN); }
"+"     { return left_value_code(S_PLUS); }
"-"     { return left_value_code(S_MINUS); }
"*"     { return left_value_code(S_ASTERISK); }
"/"     { return left_value_code(S_SLASH); }
"("     { return left_value_code(S_PARENTL); }
")"     { return left_value_code(S_PARENTR); }
"~"     { return left_value_code(S_NEGATIVE); }
"<"     { return left_value_code(S_LESS); }
">"     { return left_value_code(S_GREAT); }
"="     { return left_value_code(S_EQUALS); }
"<="    { return left_value_code(S_LTE); }
">="    { return left_value_code(S_GTE); }
"\n"    { yylineno++; }

{DIGIT}+                    { return left_value_identifier(V_ID); }
{DIGIT}*{DOT}{DIGIT}+       { return left_value_integer(V_NUMINT); }
{ALPHA}({ALPHA}|{DIGIT})*   { return left_value_float(V_NUMFLOAT); }
%%

/**
 * Left Value Code returns the terminal code to Flex and updates Bison's
 * code attribute with the terminal code.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int left_value_code(int code) {
    yylval.code = code;
    return code;
}

/**
 * Left Value Identifier returns the terminal code to flex and updates Bison's
 * identifier attribute with a copy of the current yytext read.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int left_value_identifier(int code) {
    yylval.identifier = strdup(yytext);
    return code;
}

/**
 * Left Value Integer returns the terminal code to flex and updates Bison's
 * int_value attribute with the integer equivalent of the yytext read.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int left_value_integer(int code) {
    yylval.int_value = atoi(yytext);
    return code;
}

/**
 * Left Value Float returns the terminal code to flex and updates Bison's
 * float_value attribute with the float equivalent of the yytext read.
 * @param   code    Terminal code.
 * @return  Terminal code.
 */
int left_value_float(int code) {
    yylval.float_value = atof(yytext);
    return code;
}