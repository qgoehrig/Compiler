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
int                { RETURN(INT_TOK); }
chr                { RETURN(CHR_TOK); }
.					         { }
%%
