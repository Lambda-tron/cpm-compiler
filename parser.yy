/* Skeleton and definitions for generating a LALR(1) parser in C++ */
%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

/* Required code included before the parser definition begins */
%code requires{
  #include <string>
  #include "Node.h"
  #define USE_LEX_ONLY false //change this macro to true if you want to isolate the lexer from the parser.
}

/* Code included in the parser implementation file */
%code{
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;

  Node* root;
  extern int yylineno;
}

/* Token definitions for the grammar */
/* Tokens represent the smallest units of the language, like operators and parentheses */
%token <std::string> LENGTH PLUSOP INT MINUSOP MULTOP ELSE ID LP RP LCB RCB AND OR RELOP DIV PWR LBR RBR DOR COMA QES EXM ASS INTTYPE FLOATTYPE BOOLTYPE VOID DOT VOLATILE MAIN COL SEMICOL NEWLINE IF PRINT READ RETURN FOR BREAK CONTINUE BOOL FLOAT CLASS

%token END 0 "end of file"

/* Operator precedence and associativity rules */
/* Used to resolve ambiguities in parsing expressions See https://www.gnu.org/software/bison/manual/bison.html#Precedence-Decl */ 
/* Lowest Precedence */
%nonassoc ELSE
%left OR
%left AND
%left RELOP
%left PLUSOP MINUSOP
%left MULTOP DIV
%right PWR
%right EXM            
%left DOT LBR LP RP
/* Highest Precedence */

/* Specify types for non-terminals in the grammar */
/* The type specifies the data type of the values associated with these non-terminals */
%type <Node*> type var stmtList stmtBl stmt arg root postfix expr primary forArg1 forArg3 method arg_method entry class pre_start program paramList classAttrbt values
%type <std::string> baseType
/* Grammar rules section */
/* This section defines the production rules for the language being parsed */
%%


root: program { 
                                root = $1; 
};

program:
    stmtEnd pre_start entry END {
                                $$ = new Node("program","",yylineno);
                                $$->children.push_back($2);
                                $$->children.push_back($3);
    }
    |pre_start entry END {
                                $$ = new Node("program","",yylineno);
                                $$->children.push_back($1); 
                                $$->children.push_back($2); 
    }
;

pre_start:
    %empty { 
                                $$ = new Node("GlobalList", "", yylineno); 
    }
    | pre_start var stmtEnd { 
                                $$ = $1; 
                                $$->children.push_back($2); 
    }
    | pre_start class { 
                                $$ = $1; 
                                $$->children.push_back($2); 
    }
;


class:
    CLASS ID LCB stmtEnd classAttrbt RCB stmtEnd { 
                                $$ = new Node("class", $2, yylineno); 
                                $$->children.push_back($5);
    }
;

classAttrbt:
    %empty { 
                                $$ = new Node("classAttrbt","",yylineno); 
    }
	| classAttrbt var stmtEnd { 
                                $$ = $1; $$->children.push_back($2); 
    }
	| classAttrbt method stmtEnd { 
                                $$ = $1; $$->children.push_back($2); 
    }
;

entry:
    MAIN LP RP COL INTTYPE stmtBl { 
                                $$ = new Node("method","main",yylineno); 
                                $$->children.push_back(new Node("type", "INT", yylineno));
                                $$->children.push_back($6);
    }
;

method:
    ID LP arg_method RP COL type stmtBl { 
                                $$ = new Node("method",$1,yylineno);
                                $$->children.push_back($3);
                                $$->children.push_back($6);
                                $$->children.push_back($7);          
    }
;

arg_method:
    %empty{
                                $$ = new Node("arg_method","",yylineno);
    }
    |paramList {
                                $$ = new Node("arg_method","",yylineno);
                                $$->children.push_back($1);
    }
;

paramList:
    ID COL type { 
                                $$ = new Node("param",$1,yylineno); 
                                $$->children.push_back($3);
    }
  | paramList COMA ID COL type { 
                                $$ = $1; 
                                Node* n = new Node("param",$3,yylineno); 
                                n->children.push_back($5);
                                $$->children.push_back(n);
    }
;

type:   
    baseType { 
                                $$ = new Node("type", $1, yylineno); 
    }
    |baseType LBR RBR { 
                                $$ = new Node("type", "arr_"+$1, yylineno); 
    }
    |ID { 
                                $$ = new Node("type", $1, yylineno); 
    }
    |VOID { 
                                $$ = new Node("type", "VOID", yylineno); 
    }
;

var:
	 VOLATILE ID COL type ASS expr { 
                                $$ = new Node("v_var",$2,yylineno); 
                                $$->children.push_back($4); 
                                $$->children.push_back($6);
    }
	|ID COL type { 
                                $$ = new Node("var",$1,yylineno); 
                                $$->children.push_back($3);
    }
	|VOLATILE ID COL type { 
                                $$ = new Node("v_var",$2,yylineno); 
                                $$->children.push_back($4);
    }
	|ID COL type ASS expr { 
                                $$ = new Node("var",$1,yylineno); 
                                $$->children.push_back($3); 
                                $$->children.push_back($5);
    }
;

stmt:
	 stmtBl { 
                                $$ = $1; 
     }
	|stmtBl stmtEnd { 
                                $$ = $1; 
    }
	|expr ASS expr stmtEnd { 
                                $$ = new Node("assign","",yylineno); 
                                $$->children.push_back($1); 
                                $$->children.push_back($3);
    }
	|PRINT LP expr RP stmtEnd { 
                                $$ = new Node("print","",yylineno); 
                                $$->children.push_back($3);
    }
	|READ LP expr RP stmtEnd { 
                                $$ = new Node("read","",yylineno); 
                                $$->children.push_back($3);
    }
	|RETURN expr stmtEnd { 
                                $$ = new Node("return","",yylineno); 
                                $$->children.push_back($2);
    }
	|CONTINUE stmtEnd           { $$ = new Node("continue","",yylineno); }
	|BREAK stmtEnd              { $$ = new Node("break","",yylineno); }
	|expr stmtEnd               { $$ = $1; }
	|var stmtEnd                { $$ = $1; }
    | IF LP expr RP stmtBl stmtEnd {
                                $$ = new Node("IF","IF",yylineno);
                                $$->children.push_back($3);
                                $$->children.push_back($5);
    }
	|IF LP expr RP stmtBl ELSE stmtBl stmtEnd { 
                                $$ = new Node("IF","IFELSE",yylineno); 
                                $$->children.push_back($3);
                                $$->children.push_back($5);
                                $$->children.push_back($7);
    }
    | FOR LP forArg1 COMA expr COMA forArg3 RP stmtBl stmtEnd {
                                $$ = new Node("FOR","FOR",yylineno);
                                $$->children.push_back($3);
                                $$->children.push_back($5);
                                $$->children.push_back($7);
                                $$->children.push_back($9);
    }
    | FOR LP forArg1 COMA       COMA forArg3 RP stmtBl stmtEnd {
                                $$ = new Node("FOR","FOR",yylineno);
                                $$->children.push_back($3);
                                $$->children.push_back(new Node("empty arg 2","",yylineno));
                                $$->children.push_back($6);
                                $$->children.push_back($8);
    }

;

stmtBl:
    LCB stmtEnd stmtList RCB {
                                $$ = new Node("Stmtblock","",yylineno);
                                $$->children.push_back($3);
    }
;


stmtList:
    %empty { 
                                $$ = new Node("stmtList","",yylineno); 
    }
    | stmtList stmt { 
                                $$ = $1; $$->children.push_back($2); 
    }
;


forArg1:
    %empty                      { $$ = new Node("for arg 1","",yylineno); }
    | var                       { $$ = $1; }
    | forArg3                   { $$ = $1; }
;


forArg3:
	expr ASS expr { 
                                $$ = new Node("assign","",yylineno); 
                                $$->children.push_back($1); 
                                $$->children.push_back($3);
    }
;

stmtEnd:
    NEWLINE 
  | stmtEnd NEWLINE
;


expr:
	 primary { $$ = $1; }
	|expr AND expr { 
                                $$ = new Node("and","",yylineno); 
                                $$->children.push_back($1); 
                                $$->children.push_back($3);
    }
	|expr OR expr { 
                                $$ = new Node("or","",yylineno); 
                                $$->children.push_back($1); 
                                $$->children.push_back($3);
    }
	|expr RELOP expr { 
                                $$ = new Node("relop",$2,yylineno); 
                                $$->children.push_back($1); 
                                $$->children.push_back($3);
    }
|   expr PLUSOP expr { 
                                $$ = new Node("PLUSOP", "", yylineno); 
                                $$->children.push_back($1); $$->children.push_back($3); 
    }
    |expr MINUSOP expr { 
                                $$ = new Node("MINOP", "", yylineno); 
                                $$->children.push_back($1); $$->children.push_back($3); 
    }
    |expr MULTOP expr { 
                                $$ = new Node("MULTOP", "", yylineno); 
                                $$->children.push_back($1); $$->children.push_back($3); 
    }
    |expr DIV expr { 
                                $$ = new Node("DIVOP", "", yylineno); 
                                $$->children.push_back($1); $$->children.push_back($3); 
    }
    |expr PWR expr { 
                                $$ = new Node("PWROP", "Power", yylineno); 
                                $$->children.push_back($1); 
                                $$->children.push_back($3); 
    }
	|expr postfix { 
                                $$ = new Node("postfix","",yylineno); 
                                $$->children.push_back($1); 
                                $$->children.push_back($2);
    }
	|baseType LBR values RBR { 
                                $$ = new Node("init_array","",yylineno); 
                                $$->children.push_back(new Node("type", $1, yylineno));
                                $$->children.push_back($3); 
    }
	|EXM expr { 
                                $$ = new Node("not","",yylineno); 
                                $$->children.push_back($2);
    }
	|ID LP arg RP { 
                                $$ = new Node("call",$1,yylineno); 
                                $$->children.push_back($3);
    }
;

arg:
    %empty { 
                                $$ = new Node("args", "", yylineno); 
    }
  | expr { 
                                $$ = new Node("args", "", yylineno); 
                                $$->children.push_back($1); 
    }
  | arg COMA expr { 
                                $$ = $1; 
                                $$->children.push_back($3); 
    }
;

values: 
  expr
    {
      $$ = new Node("values","",yylineno);
      $$->children.push_back($1);
    }
  | values COMA expr
    {
      $$ = $1;
      $$->children.push_back($3);
    }
  ;
baseType:
	INTTYPE                     { $$ = "INT"; }
	|FLOATTYPE                  { $$ = "FLOAT"; }
	|BOOLTYPE                   { $$ = "BOOL"; }
;



postfix:
	LBR expr RBR { 
                                $$ = new Node("arr_access","",yylineno); 
                                $$->children.push_back($2); 
    }
	|DOT LENGTH {               
                                $$ = new Node("length","",yylineno); 
    }
	|DOT ID LP arg RP { 
                                $$ = new Node("funcMethcall",$2,yylineno); 
                                $$->children.push_back($4);
    }
;	

primary:
	 INT                        { $$ = new Node("INT",$1,yylineno); }
	|FLOAT                      { $$ = new Node("FLOAT",$1,yylineno); }
	|BOOL                       { $$ = new Node("BOOL",$1,yylineno); }
	|LP expr RP                 { $$ = $2; }
	|ID                         { $$ = new Node("ID",$1,yylineno); }
;

