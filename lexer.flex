%top{
    #include "parser.tab.hh"
    #define YY_DECL yy::parser::symbol_type yylex()
    #include "Node.h"
    int lexical_errors = 0;
}

%option yylineno noyywrap nounput batch noinput stack 
%%

"/"			            {if(USE_LEX_ONLY) {printf("DIV ");} else {return yy::parser::make_DIV(yytext);}}
"+"                     {if(USE_LEX_ONLY) {printf("PLUSOP ");} else {return yy::parser::make_PLUSOP(yytext);}}
"-"                     {if(USE_LEX_ONLY) {printf("SUBOP ");} else {return yy::parser::make_MINUSOP(yytext);}}
"*"                     {if(USE_LEX_ONLY) {printf("MULTOP ");} else {return yy::parser::make_MULTOP(yytext);}}

"("                     {if(USE_LEX_ONLY) {printf("LP  ");} else {return yy::parser::make_LP(yytext);}}
")"                     {if(USE_LEX_ONLY) {printf("RP  ");} else {return yy::parser::make_RP(yytext);}}
"{"			            {if(USE_LEX_ONLY) {printf("LCB ");} else {return yy::parser::make_LCB(yytext);}}
"}"			            {if(USE_LEX_ONLY) {printf("RCB ");} else {return yy::parser::make_RCB(yytext);}}

"&"			            {if(USE_LEX_ONLY) {printf("AND ");} else {return yy::parser::make_AND(yytext);}}
"|"			            {if(USE_LEX_ONLY) {printf("OR ");} else {return yy::parser::make_OR(yytext);}}
"^"			            {if(USE_LEX_ONLY) {printf("PWR ");} else {return yy::parser::make_PWR(yytext);}}

"<="|">="|"<"|">"|"="|"!=" {if(USE_LEX_ONLY) {printf("RELOP ");} else {return yy::parser::make_RELOP(yytext);}}


"["			            {if(USE_LEX_ONLY) {printf("LBR ");} else {return yy::parser::make_LBR(yytext);}}
"]"			            {if(USE_LEX_ONLY) {printf("RBR ");} else {return yy::parser::make_RBR(yytext);}}
"."			            {if(USE_LEX_ONLY) {printf("DOT ");} else {return yy::parser::make_DOT(yytext);}}
","			            {if(USE_LEX_ONLY) {printf("COMA ");} else {return yy::parser::make_COMA(yytext);}}
"?"			            {if(USE_LEX_ONLY) {printf("QES ");} else {return yy::parser::make_QES(yytext);}}
"!"			            {if(USE_LEX_ONLY) {printf("EXM ");} else {return yy::parser::make_EXM(yytext);}}
":="			        {if(USE_LEX_ONLY) {printf("ASS ");} else {return yy::parser::make_ASS(yytext);}}
"int"			        {if(USE_LEX_ONLY) {printf("INTTYPE ");} else {return yy::parser::make_INTTYPE(yytext);}}
"float"			        {if(USE_LEX_ONLY) {printf("FLOATTYPE ");} else {return yy::parser::make_FLOATTYPE(yytext);}}
"boolean"		        {if(USE_LEX_ONLY) {printf("BOOLTYPE ");} else {return yy::parser::make_BOOLTYPE(yytext);}}
"void"			        {if(USE_LEX_ONLY) {printf("VOID ");} else {return yy::parser::make_VOID(yytext);}}
"volatile"		        {if(USE_LEX_ONLY) {printf("VOLATILE ");} else {return yy::parser::make_VOLATILE(yytext);}}
"main"			        {if(USE_LEX_ONLY) {printf("MAIN ");} else {return yy::parser::make_MAIN(yytext);}}
":"			            {if(USE_LEX_ONLY) {printf("COL ");} else {return yy::parser::make_COL(yytext);}}
";"			            {if(USE_LEX_ONLY) {printf("SEMICOL ");} else {return yy::parser::make_SEMICOL(yytext);}}
"if"			        {if(USE_LEX_ONLY) {printf("IF ");} else {return yy::parser::make_IF(yytext);}}
"else"			        {if(USE_LEX_ONLY) {printf("ELSE ");} else {return yy::parser::make_ELSE(yytext);}}
"length"			    {if(USE_LEX_ONLY) {printf("LENGTH ");} else {return yy::parser::make_LENGTH(yytext);}}
"print"			        {if(USE_LEX_ONLY) {printf("PRINT ");} else {return yy::parser::make_PRINT(yytext);}}
"read"			        {if(USE_LEX_ONLY) {printf("READ ");} else {return yy::parser::make_READ(yytext);}}
"return"		        {if(USE_LEX_ONLY) {printf("RETURN ");} else {return yy::parser::make_RETURN(yytext);}}
"for"			        {if(USE_LEX_ONLY) {printf("FOR ");} else {return yy::parser::make_FOR(yytext);}}
"break"			        {if(USE_LEX_ONLY) {printf("BREAK ");} else {return yy::parser::make_BREAK(yytext);}}
"continue"		        {if(USE_LEX_ONLY) {printf("CONTINUE ");} else {return yy::parser::make_CONTINUE(yytext);}}
"class"		            {if(USE_LEX_ONLY) {printf("CLASS ");} else {return yy::parser::make_CLASS(yytext);}}

"true"		            {if(USE_LEX_ONLY) {printf("TRUE ");} else {return yy::parser::make_BOOL(yytext);}}
"false"		            {if(USE_LEX_ONLY) {printf("false ");} else {return yy::parser::make_BOOL(yytext);}}

\n+		                {if(USE_LEX_ONLY) {printf("NEWLINE ");} else {return yy::parser::make_NEWLINE(yytext);}}

0|[1-9][0-9]*           {if(USE_LEX_ONLY) {printf("INT ");} else {return yy::parser::make_INT(yytext);}}

[0-9]+\.[0-9]+   	    { if(USE_LEX_ONLY) { printf("FLOAT "); } else { return yy::parser::make_FLOAT(yytext);}}


[A-Za-z_][A-Za-z0-9_]*  { if (USE_LEX_ONLY) { printf("ID "); } else {return yy::parser::make_ID(yytext);}}

[ \t\r]+                {}
"//".*                  {}
.                       { if(!lexical_errors) fprintf(stderr, "Lexical errors found! See the logs below: \n"); fprintf(stderr,"\t@error at line %d. Character %s is not recognized\n", yylineno, yytext); lexical_errors = 1;}


<<EOF>>                  {return yy::parser::make_END();}
%%

