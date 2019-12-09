%{
// Imports
#include <stdlib.h>
#include <string.h>
#include "bison.tab.h"

#ifndef _DATAH_
#define _DATAH_
#include "data.h"
#endif

// Declarations
int flex_left_value_instruction(int);
int flex_left_value_identifier(int);
int flex_left_value_integer(int);
int flex_left_value_float(int);
%}

ALPHA   [a-zA-Z]
DIGIT   [0-9]
DOT     [.]

%%
"begin"     { return flex_left_value_instruction(R_BEGIN); }
"end"       { return flex_left_value_instruction(R_END); }
"var"       { return flex_left_value_instruction(R_VAR); }
"int"       { return flex_left_value_instruction(R_INT); }
"float"     { return flex_left_value_instruction(R_FLOAT); }
"if"        { return flex_left_value_instruction(R_IF); }
"ifelse"    { return flex_left_value_instruction(R_IFELSE); }
"while"     { return flex_left_value_instruction(R_WHILE); }
"read"      { return flex_left_value_instruction(R_READ); }
"print"     { return flex_left_value_instruction(R_PRINT); }
"return"    { return flex_left_value_instruction(R_RETURN); }
"fun"       { return flex_left_value_instruction(R_FUN); }

";"     { return flex_left_value_instruction(S_SEMICOLON); }
":"     { return flex_left_value_instruction(S_COLON); }
","     { return flex_left_value_instruction(S_COMMA); }
"<-"    { return flex_left_value_instruction(S_ASSIGN); }
"+"     { return flex_left_value_instruction(S_PLUS); }
"-"     { return flex_left_value_instruction(S_MINUS); }
"*"     { return flex_left_value_instruction(S_ASTERISK); }
"/"     { return flex_left_value_instruction(S_SLASH); }
"("     { return flex_left_value_instruction(S_PARENTL); }
")"     { return flex_left_value_instruction(S_PARENTR); }
"~"     { return flex_left_value_instruction(S_NEGATIVE); }
"<"     { return flex_left_value_instruction(S_LESS); }
">"     { return flex_left_value_instruction(S_GREATER); }
"="     { return flex_left_value_instruction(S_EQUALS); }
"<="    { return flex_left_value_instruction(S_LTE); }
">="    { return flex_left_value_instruction(S_GTE); }
"\n"    { yylineno++; }

{DIGIT}+                    { return flex_left_value_integer(V_NUMINT); }
{DIGIT}*{DOT}{DIGIT}+       { return flex_left_value_float(V_NUMFLOAT); }
{ALPHA}({ALPHA}|{DIGIT})*   { return flex_left_value_identifier(V_ID); }
%%

/**
 * Flex Left Value Instruction returns the terminal instruction to Flex and
 updates Bison's instruction attribute with the terminal instruction.
 * @param   instruction    Terminal instruction.
 * @return  Terminal instruction.
 */
int flex_left_value_instruction(int instruction) {
    yylval.instruction = instruction;
    return instruction;
}

/**
 * Flex Left Value Identifier returns the terminal instruction to flex updates 
 * Bison's identifier attribute with a copy of the current yytext read.
 * @param   instruction    Terminal instruction.
 * @return  Terminal instruction.
 */
int flex_left_value_identifier(int instruction) {
    yylval.identifier = strdup(yytext);
    return instruction;
}

/**
 * Flex Left Value Integer returns the terminal instruction to flex and updates
 * Bison's int_value attribute with the integer equivalent of the yytext read.
 * @param   instruction    Terminal instruction.
 * @return  Terminal instruction.
 */
int flex_left_value_integer(int instruction) {
    yylval.value = data_create_integer(atoi(yytext));
    return instruction;
}

/**
 * Flex Left Value Float returns the terminal instruction to flex Bison's
 * float_value attribute with the float equivalent of the yytext read.
 * @param   instruction    Terminal instruction.
 * @return  Terminal instruction.
 */
int flex_left_value_float(int instruction) {
    yylval.value = data_create_float(atof(yytext));
    return instruction;
}