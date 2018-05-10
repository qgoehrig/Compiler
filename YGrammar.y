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
  enum Operators Operator;
  enum CondOps CondOp;
  struct InstrSeq * InstrSeq;
  struct ExprResult * ExprResult;
  struct CondResult * CondResult;
}

/* Type declaration for data attached to non-terminals. Allows     */
/* $# notation to be correctly type casted for function arguments. */
//    <union-field-name> non-terminal
%type <Text> Id
%type <IdList> DeclList
%type <IdList> DeclItem
%type <InstrSeq> DeclImpls
%type <BaseType> Type
%type <InstrSeq> AssignStmt
%type <InstrSeq> PutStmt
%type <InstrSeq> IfStmt
%type <InstrSeq> WhileStmt
%type <InstrSeq> FuncBody
%type <InstrSeq> FuncStmts
%type <InstrSeq> Stmt
%type <ExprResult> Expr
%type <ExprResult> Term
%type <ExprResult> Factor
%type <Text> ChrLit
%type <Operator> Operator
%type <CondOp> CondOp
%type <CondResult> Cond
%type <Text> StringLit

/* List of token name and corresponding numbers */
/* y.tab.h will be generated from these */
%token IDENT_TOK  	1
%token DECL_TOK   	2
%token IMPL_TOK   	3
%token INT_TOK   	4
%token CHR_TOK   	5
%token INTLIT_TOK   6
%token CHRLIT_TOK   7
%token GET_TOK      8
%token PUT_TOK      9
%token IF_TOK       10
%token ELSE_TOK     11
%token COND_TOK     12
%token NOTEQL_TOK   13
%token EQL_TOK      14
%token LESS_TOK     15
%token GRTR_TOK     16
%token GRTR_EQL_TOK 17
%token LESS_EQL_TOK 18
%token WHILE_TOK    19
%token STRLIT_TOK   20

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
FuncBody      : '{' FuncStmts '}'                               { $$ = $2; };

FuncStmts     : Stmt ';' FuncStmts                              { $$ = AppendSeq($1, $3); };
FuncStmts     :                                                 { $$ = NULL; };

Stmt          : AssignStmt                                      { $$ = $1; };
Stmt          : PutStmt                                         { $$ = $1; };
Stmt          : IfStmt                                          { $$ = $1; };
Stmt          : WhileStmt                                       { $$ = $1; };
Stmt          :                                                 { $$ = NULL; };

PutStmt       : PUT_TOK ChrLit ')'                          { $$ = PutChrLit($2); };
PutStmt       : PUT_TOK Id ')'                              { $$ = PutVar($2); };
PutStmt       : PUT_TOK StringLit ')'                       { $$ = PutStrLit($2); };

AssignStmt    : Id '=' Expr                                 { $$ = ProcAssign($1, $3); };

IfStmt        : IF_TOK Cond FuncBody                        { $$ = ProcIf($2, $3, NULL); };
IfStmt        : IF_TOK Cond FuncBody ELSE_TOK FuncBody      { $$ = ProcIf($2, $3, $5); };

WhileStmt     : WHILE_TOK Cond FuncBody                     { $$ = ProcWhile($2, $3); };

Expr    :  Term                                             { $$ = $1; } ;
Expr    :  Expr Operator Term                               { $$ = EvalExpr($1, $2, $3); };

Term    :  Term Operator Factor                             { $$ = EvalExpr($1, $2, $3); };
Term    :  Factor                                           { $$ = $1; };

Factor  : '(' Expr ')'                                     { $$ = $2; } ;
Factor  : INTLIT_TOK                                       { $$ = GetImmInt(yytext); };
Factor  : GET_TOK Type ')'                                 { $$ = Get($2); };
Factor  : Id                                               { $$ = GetVarExpr($1); };

ChrLit    : CHRLIT_TOK                                     { $$ = strdup(yytext); };
StringLit : STRLIT_TOK                                     { $$ = strdup(yytext); };

Operator  : '+'                                               { $$ = Add; }
Operator  : '-'                                               { $$ = Sub; }
Operator  : '*'                                               { $$ = Mul; }
Operator  : '/'                                               { $$ = Div; }

Cond      : '(' Expr CondOp Expr ')'                            { $$ = EvalCond($2, $3, $4); };
CondOp    : NOTEQL_TOK                                          { $$ = NotEql; };
CondOp    : EQL_TOK                                             { $$ = Eql; };
CondOp    : LESS_TOK                                            { $$ = Less; };
CondOp    : GRTR_TOK                                            { $$ = Grtr; };
CondOp    : LESS_EQL_TOK                                        { $$ = LessEql; };
CondOp    : GRTR_EQL_TOK                                        { $$ = GrtrEql; };







%%

void
yyerror( char *s)     {
  char msg[MAXLINE];
  sprintf(msg,"ERROR \"%s\" token: \"%s\"",s,yytext);
  PostMessage(GetCurrentColumn(),strlen(yytext),msg);
}
