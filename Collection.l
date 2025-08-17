%{
#include "Collection.tab.h"
void yyerror (char *s);
int yylex();
%}
%%
\{                      { return '{';}
\}                      { return '}';}
\[                      { return '[';}
\]                      { return ']';}
\(                      { return '(';}
\)                      { return ')';}
str                     { return t_STR_CMD;}
int                     { return t_INT_CMD;}
collection              { return t_COLLECTION_CMD;}
set                     { return t_SET_CMD;}
output                  { return t_OUTPUT_CMD;}
input                   { return t_INPUT_CMD;}
if                      { return t_IF;}
else                    { return t_ELSE;}
\"[^\"]*\"              { return t_STRING;}
[0-9]+                  { yylval.num = atoi(yytext); return t_INT; }
[a-zA-Z_][a-zA-Z_0-9]*  { yylval.str = strdup(yytext); return t_ID; }
[-+=;,&<>*/!–]           { return yytext[0];}
"=="                    { yylval.str = strdup(yytext); return t_EQ;}
"{}"                    { yylval.str = strdup(yytext); return t_CurlB;}
&                       { yylval.str = strdup(yytext); return t_INTER;}
[ \t\n]                 {}
"//".*                  {}
.                       { yyerror("unexpected character");}

%%
int yywrap (void) {return 1;}
