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
  struct SymEntry **contents; //List of SymEntries
};

struct SymTab *
CreateSymTab(int size, char * scopeName, struct SymTab * parentTable) {
    struct SymTab *tab = malloc(sizeof(struct SymTab));
    tab->scopeName = (scopeName) ? strdup(scopeName) : NULL;
    tab->size = size;
    tab->contents = malloc(size * sizeof(struct SymEntry));
    tab->parent = parentTable;
    printf("SymTab Initialized\n");
    return tab;
}

struct SymTab *
DestroySymTab(struct SymTab *aTable) {
    //InvokeOnEntires( aTable, false, deleteFunct, 0, withArgs)
}

int
HashName(int size, const char *name) {
    int i = 0;
    int chrSum = 0;
    while(name[i]) {
        chrSum += (int) name[i];
        i++;
    }
    return chrSum % (size - 1);
}

//Given a list of SymEntries, finds the one matching a name, or null
struct SymEntry *
FindNameInList(struct SymEntry *anEntry, const char *name) {
    struct SymEntry * entryIndex = anEntry;
    printf("searching for %s at mem %p\n", name, entryIndex);
    // Check to see if a name exists at this location
    while( entryIndex ) {
        //printf("searching at index %p\n", entryIndex);
        if( strcmp(entryIndex->name, name) == 0 ) {
            printf("found name %s\n", name);
            return entryIndex;
        }
        entryIndex = entryIndex->next;
    }
    //printf("could not find name %s\n", name);
    return NULL;
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {

    printf("searching for name %s\n", name);

    if( !aTable ) {
        printf("could not find name %s\n", name);
        return NULL;
    }

    //Check current table
    int tabSize = aTable->size;
    int hashVal = HashName(tabSize, name);
    struct SymEntry * entry = *(aTable->contents + hashVal);
    struct SymEntry * search = FindNameInList(entry, name);
    // Recursively check parent table if no result found
    return search ? search : LookupName(aTable->parent, name);

}

struct SymEntry *
AddNameToList(struct SymTab *aTable, const char *name, int hashVal) {
    //Create a new SymEntry if none exists in original content
    struct SymEntry * entryIndex = *(aTable->contents + hashVal);
    while(entryIndex) {
        entryIndex = entryIndex->next;
    }
    entryIndex = malloc(sizeof(struct SymEntry));
    entryIndex->name = strdup(name);
    entryIndex->table = aTable;
    entryIndex->next = NULL;
    *(aTable->contents + hashVal) = entryIndex;
    printf("added entry with name %s at content[ %d ]\n", name, hashVal);
    return entryIndex;
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name) {
    int hashVal = HashName(aTable->size, name);
    struct SymEntry * myEntry = *(aTable->contents + hashVal);
    //printf("entering at %p\n", myEntry);
    // Lookup name, if no entry with name found in SymEntry list, add one.
    struct SymEntry * lookup = FindNameInList(myEntry, name);
    return lookup ? lookup : AddNameToList(aTable, name, hashVal);
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
    anEntry->attrKind = kind;
}

int
GetAttrKind(struct SymEntry *anEntry) {
    return anEntry->attrKind;
}

void *
GetAttr(struct SymEntry *anEntry) {
    return NULL;
}

const char *
GetName(struct SymEntry *anEntry) {
    return anEntry->name;
}

struct SymTab *
GetTable(struct SymEntry *anEntry) {
    return anEntry->table;
}

const char *
GetScopeName(struct SymTab *aTable) {
    return aTable->scopeName;
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
