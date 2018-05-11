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

enum BaseTypes { IntBaseType, ChrBaseType, BoolBaseType };

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
enum Operators { Add, Sub, Mul, Div, Increment, Decrement };

struct ExprResult {
    struct InstrSeq * instrs;
    int exprType;
    int registerNum;
};

// Conditionals and Branching
enum CondOps { NotEql, Eql, Less, Grtr, LessEql, GrtrEql };

struct CondResult {
    struct InstrSeq * instrs;
    enum CondOps condOp;
    char * label;
};

enum BoolOps { And, Or, Not };

// Supporting Routines

void InitSemantics();
void ListIdentifierTable();

struct TypeDesc * MakePrimDesc(enum BaseTypes type);
struct TypeDesc * MakeFuncDesc(enum BaseTypes returnType);

// Semantics Actions
void                    Finish();

void                    ProcDecls(struct IdList * idList, enum BaseTypes baseType);
void                    ProcFunc(char * id, struct InstrSeq * instrs);
struct IdList *         AppendIdList(struct IdList * item, struct IdList * list);
struct IdList *         ProcName(char * id, enum DeclTypes type);
struct InstrSeq *       ProcIf(struct CondResult * condResult, struct InstrSeq * thenBody, struct InstrSeq * elseBody);
struct InstrSeq *       ProcWhile(struct CondResult * condResult, struct InstrSeq * body );
struct InstrSeq *       ProcAssign(char * id, struct ExprResult * exprResult);
struct InstrSeq *       PutChrLit(char * val);
struct InstrSeq *       PutStrLit(const char * string);
struct InstrSeq *       PutVar(char * id);
struct InstrSeq *       IncrVar(char * id, char * amount);
struct ExprResult *     Get(enum BaseTypes baseType);
struct ExprResult *     GetImmInt(char * textVal);
struct ExprResult *     GetVarExpr(char * id);
struct ExprResult *     EvalExpr(struct ExprResult * expr1, enum Operators op, struct ExprResult * expr2);
struct ExprResult *     EvalBoolExpr(struct ExprResult * expr1, enum CondOps condOp, struct ExprResult * expr2);
struct ExprResult *     AndOrExpr(struct ExprResult * expr1, char * op, struct ExprResult * expr2);
struct ExprResult *     NegateExpr(struct ExprResult * original);
struct CondResult *     EvalCond(struct ExprResult * expr1, enum CondOps condOp, struct ExprResult * expr2);
struct CondResult *     EvalBoolCond(struct ExprResult * boolExpr);
