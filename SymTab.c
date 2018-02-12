/* Author:      Quentin Goehrig
   Created:     02/01.18
   Resources:   https://www.tutorialspoint.com/c_standard_library/c_standard_library_quick_guide.htm
                https://www.geeksforgeeks.org/memset-c-example/
                https://www.tutorialspoint.com/c_standard_library/string_h.htm
*/

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
    struct SymTab * tab = malloc(sizeof(struct SymTab));
    tab->scopeName = (scopeName) ? strdup(scopeName) : NULL;
    tab->size = size;
    tab->contents = calloc(size, sizeof(struct SymEntry *));
    tab->parent = parentTable ? parentTable : NULL;
    return tab;
}

void
EntryFree(struct SymEntry * entry, int cnt, void * withArgs) {
      free(entry->name);
      free(entry);
}


void
InvokeOnEntries(struct SymTab *aTable, bool includeParentTable,
             entryWorkFunc workFunc, int startCnt, void * withArgs) {
                 int size = aTable->size;
                 for(int i = 0; i < size; i++) {
                     int cnt = startCnt;
                     if(aTable->contents[i]) {
                         struct SymEntry * entryIndex = aTable->contents[i];
                         while( entryIndex ) {
                             struct SymEntry * nextEntry = entryIndex->next;
                             workFunc(entryIndex, startCnt, withArgs);
                             startCnt++;

                             //startCnt
                             entryIndex = nextEntry;
                         }
                     }
                 }
                 if( includeParentTable && aTable->parent) {
                     InvokeOnEntries(aTable->parent, true, workFunc, startCnt, withArgs);
                 }
}


struct SymTab *
DestroySymTab(struct SymTab *aTable) {
    struct SymTab * parent = aTable->parent;
    InvokeOnEntries(aTable, false, EntryFree, 0, NULL);
    free(aTable->scopeName);
    free(aTable->contents);
    free(aTable);
    return parent;
}

int
HashName(int size, const char *name) {
    int i = 0;
    int chrSum = 0;
    while(name[i]) {
        chrSum += (int) name[i];
        i++;
    }
    return chrSum % size;
}

//Given a list of SymEntries, finds the one matching a name, or null
struct SymEntry *
FindNameInList(struct SymEntry *anEntry, const char *name) {

    struct SymEntry * entryIndex = anEntry;
    // Check to see if a name exists at this location
    while( entryIndex ) {
        if( strcmp(entryIndex->name, name) == 0 ) {
            return entryIndex;
        }
        entryIndex = entryIndex->next;
    }
    //Return NULL if entry was not found
    return NULL;
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {

    if( !aTable ) {
        return NULL;
    }
    //Check current table
    int tabSize = aTable->size;
    int hashVal = HashName(tabSize, name);
    struct SymEntry * entry = aTable->contents[hashVal];
    struct SymEntry * search = FindNameInList(entry, name);
    // Recursively check parent table if no result found
    return search ? search : LookupName(aTable->parent, name);
}

struct SymEntry *
AddNameToList(struct SymTab *aTable, const char *name, int hashVal) {

    struct SymEntry * entryIndex = calloc(1, sizeof(struct SymEntry));
    entryIndex->name = (name) ? strdup(name) : NULL;
    entryIndex->table = aTable;
    entryIndex->next = NULL;
    entryIndex->attributes = NULL;

    struct SymEntry * head = aTable->contents[hashVal];
    if( !head ) {
        aTable->contents[hashVal] = entryIndex;
    }
    else {
        entryIndex->next = head;
        aTable->contents[hashVal] = entryIndex; //Append to start of list
    }
    return entryIndex;
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name) {

    int hashVal = HashName(aTable->size, name);
    struct SymEntry * myEntry = aTable->contents[hashVal];
    // Lookup name, if no entry with name found in SymEntry list, add one.
    struct SymEntry * lookup = FindNameInList(myEntry, name);
    return (lookup) ? lookup : AddNameToList(aTable, name, hashVal);
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
    anEntry->attributes = attributes;
    anEntry->attrKind = kind;
}

int
GetAttrKind(struct SymEntry *anEntry) {
    return anEntry ? anEntry->attrKind : -9999;
}

void *
GetAttr(struct SymEntry *anEntry) {
    return anEntry ? anEntry->attributes : NULL;
}

const char *
GetName(struct SymEntry *anEntry) {
    return anEntry ? anEntry->name : NULL;
}

struct SymTab *
GetTable(struct SymEntry *anEntry) {
    return anEntry ? anEntry->table : NULL;
}

const char *
GetScopeName(struct SymTab *aTable) {
    return aTable ? aTable->scopeName : NULL;
}

char *
GenScopePath(struct SymTab * aTable) {
    char * retStr = strdup(aTable->scopeName);
    struct SymTab * cTable = aTable->parent;

    while( cTable ) {
        char * prev = strdup(retStr);
        char * next = strdup(cTable->scopeName);
        free(retStr);
        retStr = malloc(strlen(prev) + strlen(next) + 2); //nullchar & '>'
        strcpy(retStr, next);
        strcat(retStr, ">");
        strcat(retStr, prev);
        free(prev);
        free(next); //free intermediate strings
        cTable = cTable->parent;
    }
    return retStr;
}

char *
GetScopePath(struct SymTab *aTable) {
    return GenScopePath(aTable);
}

struct SymTab *
GetParentTable(struct SymTab *aTable) {
    return aTable ? aTable->parent : NULL;
}

struct Stats *
Statistics(struct SymTab *aTable) {
    int size = aTable->size;
    int counts[size];
    memset(counts, 0, sizeof counts);
    int min = INT_MAX;
    int max = INT_MIN;
    int avg;

    for(int i = 0; i < size; i++) {
        if(aTable->contents[i]) {
            struct SymEntry * entry = aTable->contents[i];
            while( entry ) {
                counts[i]++;
                entry = entry->next;
            }
        }
    }

    int total = 0;
    for(int i = 0; i < size; i++) {
        if( counts[i] > max ) {
            max = counts[i];
        }
        if( counts[i] < min ) {
            min = counts[i];
        }
        total+=counts[i];
    }
    avg = total / size;
    struct Stats * myStats = malloc(sizeof( struct Stats ));
    myStats->minLen = min;
    myStats->maxLen = max;
    myStats->avgLen = avg;
    myStats->entryCnt = total;
    return myStats;

}
