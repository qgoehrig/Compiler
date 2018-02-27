/* Author:      Quentin Goehrig
   Created:     02/18.18
   Resources:   https://stackoverflow.com/questions/3662899/understanding-the-dangers-of-sprintf
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ScanTokens.h"
#include "SymTab.h"
#include "IOMngr.h"
#include "Scanner.h"

// symbol table entries have two attributes, first line of occurrence and occurrence count
struct Attributes {
  int firstLine;
  int firstColumn;
  int cnt;
};
enum AttrKinds { IGNORE_KIND };

// used with DoFoEntries()
void collectEntries(struct SymEntry * entry, int cnt, void * entryArray) {
  ((struct SymEntry **)entryArray)[cnt] = entry;
}

// Free an entry's attributes, used with InvokeOnEntries
// Used with DoForEntries can be used
void clearAttr(struct SymEntry * entry, int cnt, void * emptyArgs) {
    struct Attributes * attr = GetAttr(entry);
    if (attr) free(attr);
}

// used with qsort to sort list of symbol table entries
int entryCmp(const void * A, const void * B) {
  // A is pointer to element of array which contains a pointer to a struct SymEntry
  const char * strA = GetName(*(struct SymEntry **)A);
  const char * strB = GetName(*(struct SymEntry **)B);
  return strcmp(strA,strB);
}

int main(int argc, char **argv) {
  int Token;
  char message[265];
  char actionMessage[32];
  struct SymTab * table = NULL;

  bool ret = OpenSource("ScannerSource",true);
  if (!ret) {
    printf("Could not open source and listing files\n");
    exit(1);
  }

  int intSum = 0;
  float floatSum = 0.0;
  bool shouldPrint;

  while ((Token = yylex()) != 0) {

    sprintf(actionMessage,"");
    switch(Token) {

      case INIT_TOK: {
        if (!table) table = CreateSymTab(20,"main",NULL);
      } break;
      case IDENT_TOK: {
        if (!table) {
          sprintf(actionMessage," -- No SymbolTable");
          break;
        }
        struct SymEntry * entry = LookupName(table, yytext);
        if(!entry) {
            // If no entry exists, create a new ident and give it an attr with cnt 1
            entry = EnterName(table, yytext);
            struct Attributes * newAttr = malloc(sizeof(struct Attributes));
            newAttr->cnt = 1; //set other stuff here too
            newAttr->firstLine = GetCurrentLine();
            newAttr->firstColumn = (GetCurrentColumn()-yyleng);
            SetAttr(entry, IGNORE_KIND, newAttr);
            sprintf(actionMessage," -- new ident");
        }
        else {
            struct Attributes * attr = GetAttr(entry);
            attr->cnt++;
            sprintf(actionMessage," -- occurence %d", attr->cnt);
        }
      } break;
      case INT_TOK: {
          int i = atoi(yytext);
          //sprintf(actionMessage, "%d", i);
          intSum+=i;
      } break;
      case FLOAT_TOK: {
          float f = atof(yytext);
          //sprintf(actionMessage, "%f", f);
          floatSum+=f;
      } break;
      case DUMP_TOK: {
        fprintf(stderr,"---------\n");
        fprintf(stderr,"intSum = %d\n",intSum);
        fprintf(stderr,"floatSum = %f\n",floatSum);
        // get table statistics, alloc an array to hold entry pointers
        struct Stats * stats = Statistics(table);
        struct SymEntry ** entries = malloc(stats->entryCnt * sizeof(struct SymEntry *));

        // enumerate the table collecting entry pointers into the array
        InvokeOnEntries(table,false,collectEntries,0,entries);

        // sort the entries
        qsort(entries,stats->entryCnt,sizeof(struct SymEntry *),entryCmp);

        // list the contents of the table in sorted order
        fprintf(stderr,"\nContents of Symbol Table\n");
        fprintf(stderr,"                    name  line   col   cnt \n");
        for (int i = 0; i < stats->entryCnt; i++) {
          fprintf(stderr,"%3d %20s %5d %5d %5d\n", i,
                 GetName(entries[i]),
                 ((struct Attributes *) GetAttr(entries[i]))->firstLine,
                 ((struct Attributes *) GetAttr(entries[i]))->firstColumn,
                 ((struct Attributes *) GetAttr(entries[i]))->cnt);
        }
        free(stats);
        free(entries); // Free array to hold entry pointers
         // Free the attributes of entries in the symTable
        InvokeOnEntries(table,false,clearAttr,0,NULL);
      } break;
    }
    // Only print tokens we care about. NEWLINE_TOK, OTHER_TOK, WS_TOk are not
    // currently used. Use snprintf for safe printing.
    shouldPrint = Token != NEWLINE_TOK && Token != OTHER_TOK && Token != WS_TOK;
    if( shouldPrint ) {
        snprintf(message,sizeof(message), "Token#=%d, Length=%lu, Text=\"%s\"%*c%s",Token,yyleng,yytext,(int)(15-yyleng),' ',actionMessage);
        // depending on the token kind, current column may be at the next char in the input
        // or the last char of the token, if current column minus yyleng is neg force it to zero

        int col = GetCurrentColumn()-yyleng;
        if (col < 0) col = 0;
        if(Token == EQUAL_TOK) {
            col++; // Hotfix equals token col to actually highlight equals token
        }
        PostMessage(col,yyleng,message);
    }

  }
  CloseSource();
  DestroySymTab(table);
}
