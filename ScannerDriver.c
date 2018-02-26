/* ScannerDriver.c

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
  float floatSum = 0;
  int tokenCount = 0;
  int numPosts = 0;

  while ((Token = yylex()) != 0) {

    memset(actionMessage, 0, sizeof actionMessage);
    memset(message, 0, sizeof message);
    sprintf(actionMessage,"");
    tokenCount++;
    //printf("token= %s\n",yytext);
    switch(Token) {

      case INIT_TOK: {
        if (!table) table = CreateSymTab(20,"main",NULL);
      } break;
      case IDENT_TOK: {
        // place the identifier in the table (if it exists), if new then create and init
        // attributes structure, if already in table then update attributes cnt field, in
        // each case set actionMessage with one of
        // sprintf(actionMessage," -- No SymbolTable");
        // sprintf(actionMessage," -- occurrence %d",attr->cnt);
        // sprintf(actionMessage," -- new ident");
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
        // code
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
      } break;
    }
    //printf("Q=>Token#=%d, Length=%lu, Text=\"%s\"%*c%s\n",Token,yyleng,yytext,(int)(15-yyleng),' ',actionMessage);
    // Handle checking of length ultimate
    sprintf(message,"Token#=%d, Length=%lu, Text=\"%s\"%*c%s",Token,yyleng,yytext,(int)(15-yyleng),' ',actionMessage);
    //printf("%s\n", message);
    // depending on the token kind, current column may be at the next char in the input
    // or the last char of the token, if current column minus yyleng is neg force it to zero
    int col = GetCurrentColumn()-yyleng;
    if (col < 0) col = 0;
    numPosts++;
    //printf("col=%d, len=%d, msg=%s\n", col, yyleng, message);
    PostMessage(col,yyleng,message);
  }
  CloseSource();
}