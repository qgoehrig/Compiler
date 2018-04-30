/* Semantics.h
 The action and supporting routines for performing semantics processing.
 */

#include "SymTab.h"
#include "IOMngr.h"
#include "Scanner.h"
#include "YCodeGen.h"

/* Declaration of semantic record data types used in grammar.y %union */
struct IdList {
  struct SymEntry * entry;
  struct IdList * next;
};

enum BaseTypes { IntBaseType, ChrBaseType };

struct FuncDesc {
  enum BaseTypes returnType;
  struct InstrSeq * funcCode;
};


enum DeclTypes { PrimType, FuncType };
struct TypeDesc {
  enum DeclTypes declType;
  union {
    enum BaseTypes primDesc;
    struct FuncDesc * funcDesc;
  };
};

// Symbol Table Structures
struct Attr {
  struct TypeDesc * typeDesc;
  char * reference;
};

// Supporting Routines

void InitSemantics();
void ListIdentifierTable();

struct TypeDesc * MakePrimDesc(enum BaseTypes type);
struct TypeDesc * MakeFuncDesc(enum BaseTypes returnType);

// Semantics Actions
void                    Finish();

void                    ProcDecls(struct IdList * idList, enum BaseTypes baseType);
struct IdList *         AppendIdList(struct IdList * item, struct IdList * list);
struct IdList *         ProcName(char * id, enum DeclTypes type);
void                    ProcFunc(char * id, struct InstrSeq * instrs);
