/* Author:      Quentin Goehrig
   Created:     03/13.18
   Resources:
*/

Letter	[A-Za-z]
Digit   [0-9]

%{
#include "RDTokens.h"
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

%}

%%
;								{ RETURN(SEMI_TOK); }
int                				{ RETURN(INT_TOK); }
chr                				{ RETURN(CHR_TOK); }
{Letter}({Letter}|{Digit})*		{ RETURN(IDENT_TOK); }
-?{Digit}+						{ RETURN(INTLIT_TOK); }
-?{Digit}+\.{Digit}+			{ RETURN(FLOATLIT_TOK); }
:=								{ RETURN(ASSIGN_TOK); }
"\("							{ RETURN(LPAREN_TOK); }
"\)"							{ RETURN(RPAREN_TOK); }
"-"								{ RETURN(MINUS_TOK); }
"+"								{ RETURN(PLUS_TOK); }
"*"								{ RETURN(TIMES_TOK); }
"/"								{ RETURN(DIV_TOK); }
"\{"							{ RETURN(LBRACE_TOK); }
"\}"							{ RETURN(RBRACE_TOK); }
","								{ RETURN(COMMA_TOK); }
.					         	{ }
[\t]                            { }
[\n]							{ }
%%
