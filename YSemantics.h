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

// Operators and Expressions
enum Operators { ADD, SUB, MUL, DIV };

struct ExprResult {
    struct InstrSeq * instrs;
    int exprType;
    int registerNum;
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
struct InstrSeq *       ProcAssign(char * id, struct ExprResult * exprResult);
struct InstrSeq *       Put(char * val);
struct ExprResult *     Get(enum BaseTypes baseType);
struct ExprResult *     GetImmInt(char * textVal);
struct ExprResult *     EvalExpr(struct ExprResult * expr1, enum Operators op, struct ExprResult * expr2 );
