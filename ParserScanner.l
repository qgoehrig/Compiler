Letter	[A-Za-z]
Digit   [0-9]

%{
#include "y.tab.h"
#include "IOMngr.h"
#define YY_INPUT(buf,result,max_size) \
  { int c = GetSourceChar(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
  }

bool shouldLog = false;
lineComment = true;
int commLayers = 0;

#define RETURN(val) \
  { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n",val,(unsigned long)yyleng,yytext); \
    return(val); \
  }

int commentLevel = 0;
#define IN_COMMENT ( lineComment || commLayers > 0)

%}

%%

  /* Combine comment nesting from Scanner.l with RDScanner.l regular
     expressions except for real literal. Use RETURN(token-symbolic-name)
     where token-symbolic-name appears in yacc file.

     Removed:
     float                           { if (!IN_COMMENT) RETURN(FLOAT_TOK); }
     -?{Digit}+\.{Digit}+			{ if (!IN_COMMENT) RETURN(FLOATLIT_TOK); }
   */

"int"                           { if (!IN_COMMENT) RETURN(INT_TOK); }
\;                              { if (!IN_COMMENT) RETURN(';'); }
"\/\/"							{ lineComment = true; }
"/*"							{ commLayers++; }
"*/"							{ commLayers--; }
chr                				{ if (!IN_COMMENT) RETURN(CHR_TOK); }
{Letter}({Letter}|{Digit})*		{ if (!IN_COMMENT) RETURN(IDENT_TOK); }
-?{Digit}+						{ if (!IN_COMMENT) RETURN(INTLIT_TOK); }
:=							{ if (!IN_COMMENT) RETURN(ASSIGN_TOK); }
"\("							{ if (!IN_COMMENT) RETURN('('); }
"\)"							{ if (!IN_COMMENT) RETURN(')'); }
"-"								{ if (!IN_COMMENT) RETURN('-'); }
"+"								{ if (!IN_COMMENT) RETURN('+'); }
"*"								{ if (!IN_COMMENT) RETURN('*'); }
"/"								{ if (!IN_COMMENT) RETURN('/'); }
"\{"							{ if (!IN_COMMENT) RETURN('{'); }
"\}"							{ if (!IN_COMMENT) RETURN('}'); }
","								{ if (!IN_COMMENT) RETURN(','); }
[\n]                            { lineComment = false; }
[ \t]                           { }
.                               { }
%%
