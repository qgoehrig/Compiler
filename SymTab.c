#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "SymTab.h"

/* The symbol table entry structure proper.
*/
struct SymEntry {
  struct SymTab * table;
  char *name;
  int attrKind;
  void *attributes;
  struct SymEntry *next;
};

/* The symbol table structure proper. The hash table array Contents
   is dynamically allocated according to size
*/
struct SymTab {
  struct SymTab * parent;
  char *scopeName;
  int size;
  struct SymEntry **contents;
};

struct SymTab *
CreateSymTab(int size, char * scopeName, struct SymTab * parentTable) {
}

struct SymTab *
DestroySymTab(struct SymTab *aTable) {
}

int
HashName(int size, const char *name) {
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name) {
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
}

int
GetAttrKind(struct SymEntry *anEntry) {
}

void *
GetAttr(struct SymEntry *anEntry) {
}

const char *
GetName(struct SymEntry *anEntry) {
}

struct SymTab *
GetTable(struct SymEntry *anEntry) {
}

const char *
GetScopeName(struct SymTab *aTable) {
}

char *
GetScopePath(struct SymTab *aTable) {
}

struct SymTab *
GetParentTable(struct SymTab *aTable) {
}

void
InvokeOnEntries(struct SymTab *aTable, bool includeParentTable,
             entryWorkFunc workFunc, int startCnt, void * withArgs) {
}

struct Stats *
Statistics(struct SymTab *aTable) {
}
