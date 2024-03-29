Letter	[A-Za-z]
Digit   [0-9]

%{
  #include "IOMngr.h"
  #define YY_INPUT(buf,result,max_size) \
  { int c = GetSourceChar(); \
		result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

    bool shouldLog = false;

    #define RETURN(val) \
    { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n",val,yyleng,yytext); \
    return(val); \
    }

    #include "YSemantics.h"
    #include "y.tab.h"

    bool lineComment = false;
    int commLayers = 0;
    int commentLevel = 0;
    #define IN_COMMENT ( lineComment || commLayers > 0)

%}

%%
";"                             { if (!IN_COMMENT) RETURN(';'); }
"\/\/"							{ lineComment = true; }
"/*"							{ commLayers++; }
"*/"							{ commLayers--; }
","                             { if (!IN_COMMENT) RETURN(','); }
":"                             { if (!IN_COMMENT) RETURN(':'); }
"="                             { if (!IN_COMMENT) RETURN('='); }
\#\#                            { if (!IN_COMMENT) RETURN(NOTEQL_TOK);}
\=\=                            { if (!IN_COMMENT) RETURN(EQL_TOK); }
\<                              { if (!IN_COMMENT) RETURN(LESS_TOK); }
\>                              { if (!IN_COMMENT) RETURN(GRTR_TOK); }
\>\=                            { if (!IN_COMMENT) RETURN(GRTR_EQL_TOK); }
\<\=                            { if (!IN_COMMENT) RETURN(LESS_EQL_TOK); }
"if"                            { if (!IN_COMMENT) RETURN(IF_TOK); }
"else"                          { if (!IN_COMMENT) RETURN(ELSE_TOK); }
"while"                         { if (!IN_COMMENT) RETURN(WHILE_TOK); }
"get("                          { if (!IN_COMMENT) RETURN(GET_TOK); }
"put("                          { if (!IN_COMMENT) RETURN(PUT_TOK); }
"int"                           { if (!IN_COMMENT) RETURN(INT_TOK); }
"chr"                		    { if (!IN_COMMENT) RETURN(CHR_TOK); }
"decl"                          { if (!IN_COMMENT) RETURN(DECL_TOK); }
"impl"                          { if (!IN_COMMENT) RETURN(IMPL_TOK); }
\"(.)*\"                        { if (!IN_COMMENT) RETURN(STRLIT_TOK); }
\'\\.\'                         { if (!IN_COMMENT) RETURN(CHRLIT_TOK); }
\'.\'                           { if (!IN_COMMENT) RETURN(CHRLIT_TOK); }
\+\+                            { if (!IN_COMMENT) RETURN(INCR_TOK); }
\-\-                            { if (!IN_COMMENT) RETURN(DECR_TOK); }
\&\&                            { if (!IN_COMMENT) RETURN(AND_TOK); }
\|\|                            { if (!IN_COMMENT) RETURN(OR_TOK); }
\!                              { if (!IN_COMMENT) RETURN(NOT_TOK); }
\*                              { if (!IN_COMMENT) RETURN('*'); }
\/                              { if (!IN_COMMENT) RETURN('/'); }
\+                              { if (!IN_COMMENT) RETURN('+'); }
\-                              { if (!IN_COMMENT) RETURN('-'); }
{Letter}({Letter}|{Digit})*		{ if (!IN_COMMENT) RETURN(IDENT_TOK); }
-?{Digit}+			            { if (!IN_COMMENT) RETURN(INTLIT_TOK); }
"{"                             { if (!IN_COMMENT) RETURN('{'); }
"}"                             { if (!IN_COMMENT) RETURN('}'); }
")"                             { if (!IN_COMMENT) RETURN(')'); }
"("                             { if (!IN_COMMENT) RETURN('('); }
[ \t]                           { }
[\n]                            { lineComment = false; }
.                               { }
%%
