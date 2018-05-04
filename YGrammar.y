%{
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include "IOMngr.h"
  #include "Scanner.h"
  #include "YSemantics.h"

  void yyerror(char *s);
  %}

/* Union structure of data that can be attached to non-terminals   */
%union     {
// real-type union-field-name
  int Integer;
  char * Text;
  struct IdList * IdList;
  enum BaseTypes BaseType;
  struct InstrSeq * InstrSeq;
}

/* Type declaration for data attached to non-terminals. Allows     */
/* $# notation to be correctly type casted for function arguments. */
//    <union-field-name> non-terminal
%type <Text> Id
%type <IdList> DeclList
%type <IdList> DeclItem
%type <InstrSeq> DeclImpls
%type <BaseType> Type
%type <InstrSeq> FuncBody
%type <InstrSeq> FuncStmts
%type <InstrSeq> Stmt
%type <InstrSeq> AssignStmt

/* List of token name and corresponding numbers */
/* y.tab.h will be generated from these */
%token IDENT_TOK  	1
%token DECL_TOK   	2
%token IMPL_TOK   	3
%token INT_TOK   	4
%token CHR_TOK   	5
%token INTLIT_TOK
// can't go past 32 without conflicting with single char tokens
// could use larger token numbers


%%

Module        : DeclImpls                                       { Finish(); };

DeclImpls     : Decl DeclImpls                                  { };
DeclImpls     : Impl DeclImpls                                  { };
DeclImpls     :                                                 { };

Decl          : DECL_TOK DeclList ':' Type  ';'                 { ProcDecls($2,$4); };
DeclList      : DeclItem ',' DeclList                           { $$ = AppendIdList($1,$3); };
DeclList      : DeclItem                                        { $$ = $1; };

DeclItem      : Id                                              { $$ = ProcName($1,PrimType); };
DeclItem      : Id FuncArgTypes                                 { $$ = ProcName($1,FuncType); };

Id            : IDENT_TOK                                       { $$ = strdup(yytext); };
FuncArgTypes  : '(' ')'                                         {  };

Type          : INT_TOK                                         { $$ = IntBaseType; };
Type          : CHR_TOK                                         { $$ = ChrBaseType; };

Impl          : IMPL_TOK Id FuncArgNames FuncBody ';'           { ProcFunc($2,$4); };
FuncArgNames  : '(' ')'                                         {  };
FuncBody      : '{' FuncStmts '}'                               {  };

FuncStmts     : Stmt ';' FuncStmts                              {  };
FuncStmts     :                                                 {  };

Stmt          : AssignStmt                                      {  };

AssignStmt    : Id '=' Expr                                     {  };
AssignStmt    :                                                 {  };

Expr          :  Term '+' Expr                                    { $$ = $1 + $3; } ;
Expr          :  Term '-' Expr                                    { $$ = $1 - $3; } ;
Expr    :  Term                                             { $$ = $1; } ;
Term    :  Factor '*' Term                                  { $$ = $1 * $3; } ;
Term    :  Factor '/' Term                                  { $$ = $1 / $3; } ;
Term    :  Factor                                           { $$ = $1; } ;
Factor  :  '(' Expr ')'                                     { $$ = $2; } ;
Factor  :  '-' Factor                                       { $$ = - $2; } ;
Factor  :  INTLIT_TOK                                       { $$ = atoi(yytext); } ;

%%

void
yyerror( char *s)     {
  char msg[MAXLINE];
  sprintf(msg,"ERROR \"%s\" token: \"%s\"",s,yytext);
  PostMessage(GetCurrentColumn(),strlen(yytext),msg);
}
