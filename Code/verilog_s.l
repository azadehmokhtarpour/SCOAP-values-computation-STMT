%option noyywrap

%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "verilog_p.tab.h"
#include "general.h"
%}

%%

[ \t\n]	; // ignore all whitespace

"/*"([^*]|\*+[^*/])*\*+"/"   {printf("ignore comment \n");}
"//"[^\n]*					 {printf("ignore inline comment \n");}
"--"[^\n]*					 {printf("ignore inline comment \n");}

"module"	{return T_MODULE;}
"endmodule"	{return T_ENDMODULE;}
"input"		{yylval.ival = ST_IN;   return T_SIG_TYPE;}
"output"	{yylval.ival = ST_OUT;  return T_SIG_TYPE;}
"wire"		{yylval.ival = ST_WIRE; return T_SIG_TYPE;}
"reg"		{yylval.ival = ST_REG;  return T_SIG_TYPE;}
"assign"	{return T_ASSIGN_STR;}


[a-zA-Z\\\/][a-zA-Z0-9_\\\/]*   { strcpy(yylval.str, yytext); return T_IDENTIFIER; }
[0-9]+\.[0-9]+ 			{yylval.fval = atof(yytext); return T_FLOAT;}
[0-9]+					{yylval.ival = atoi(yytext); return T_INT;}

"("		{return T_LEFT;}
")"		{return T_RIGHT;}
";"		{return T_SEMICOLON;}
","		{return T_COMMA;}
":"		{return T_RANGE;}
"."		{return T_POINT;}
"'"		{return T_SINGLEQOUT;}
"["		{return T_BRACE_O;}
"]"		{return T_BRACE_C;}
"{"		{return T_BLOCK_O;}
"}"		{return T_BLOCK_C;}

"="		{return T_ASSIGN;}


%%