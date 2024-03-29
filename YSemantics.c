/*  Semantics.c
    Support and semantic action routines.
    Author:      Quentin Goehrig
    Created:     04/18.18
    Resources:
    https://stackoverflow.com/questions/42958229/how-to-implement-not-operation-in-mips

 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "YSemantics.h"

// Shared Data

struct SymTab * IdentifierTable;
struct SymTab * StringLitTable;
enum AttrKinds { VOID_KIND = -1, INT_KIND, STRING_KIND, STRUCT_KIND };

char * BaseTypeNames[2] = { "int", "chr"};
char * TypeNames[2] = { "", "func"};

// corresponds to enum Operators
char * Ops[] = { "add", "sub", "mul", "div"};
//char * BoolOps[] = { "and", "or", "not" };

// corresponds to negation of enum Comparisons
// enum Comparisons { LtCmp, LteCmp, GtCmp, GteCmp, EqCmp, NeqCmp };
char * Branches[] = { "bge", "bgt", "ble", "blt", "bne", "beq"};
// comparison set instructions, in order with enum Comparisons
//char * CmpSetReg[] = { "slt", "sle", "sgt", "sge", "seq", "sne" };


// Supporting Routines
void
PostMessageAndExit(int col, char * message) {
  PostMessage(col,1,message);
  CloseSource();
  exit(0);
}

void
InitSemantics() {
  IdentifierTable = CreateSymTab(100,"global",NULL);
  StringLitTable = CreateSymTab(100, "strLits", NULL);
}

char *
StringForType(struct TypeDesc * desc) {
  switch (desc->declType) {
    case PrimType: {
      return strdup(BaseTypeNames[desc->primDesc]);
    } break;
    case FuncType: {
      return strdup(BaseTypeNames[desc->funcDesc->returnType]);
    } break;
  }
}

struct TypeDesc *
MakePrimDesc(enum BaseTypes type) {
  struct TypeDesc * desc = malloc(sizeof(struct TypeDesc));
  desc->declType = PrimType;
  desc->primDesc = type;
  return desc;
}

struct TypeDesc *
MakeFuncDesc(enum BaseTypes returnType) {
  struct TypeDesc * desc = malloc(sizeof(struct TypeDesc));
  desc->declType = FuncType;
  desc->funcDesc = malloc(sizeof(struct FuncDesc));
  desc->funcDesc->returnType = returnType;
  desc->funcDesc->funcCode = NULL;
  return desc;
}

void displayEntry(struct SymEntry * entry, int cnt, void * ignore) {
  char * scope = GetScopePath(GetTable(entry));
  printf("%3d %-20s %-15s",cnt,scope,GetName(entry));
  free(scope);
  int attrKind = GetAttrKind(entry);
  struct Attr * attr = GetAttr(entry);
  switch (attrKind) {
    case VOID_KIND: {
    } break;
    case INT_KIND: {
    } break;
    case STRING_KIND: {
    } break;
    case STRUCT_KIND: {
      if (attr) {
        printf("%-10s",attr->reference);
        printf("     ");
        struct TypeDesc * desc = attr->typeDesc;
        char * typeStr = StringForType(desc);
        printf("%-10s ",typeStr);
        free(typeStr);
      }
      else {
        printf("empty");
      }
    } break;
  }
  printf("\n");
}

void
ListIdentifierTable() {
  printf("\nIdentifier Symbol Table\n");
  printf("Num Scope                Name           Ref       Kind Type\n");
  InvokeOnEntries(IdentifierTable,true,displayEntry,0,NULL);
}

void
FreeTypeDesc(struct TypeDesc * desc) {
  switch (desc->declType) {
    case PrimType: {
    } break;
    case FuncType: {
      if (desc->funcDesc->funcCode) FreeSeq(desc->funcDesc->funcCode);
      free(desc->funcDesc);
    } break;
  }
  free(desc);
}

// provided to the SymTab for destroying table contents
void
FreeEntryAttr(struct SymEntry * entry) {
  int attrKind = GetAttrKind(entry);
  struct Attr * attr = GetAttr(entry);
  switch (attrKind) {
    case VOID_KIND: {
    } break;
    case INT_KIND: {
    } break;
    case STRING_KIND: {
    } break;
    case STRUCT_KIND: {
      if (attr) {
        if (attr->typeDesc)  FreeTypeDesc(attr->typeDesc);
        if (attr->reference) free(attr->reference);
        free(attr);
      }
    } break;
  }
}

void processGlobalIdentifier(struct SymEntry * entry, int cnt, void * dataCode) {
  struct Attr * attr = GetAttr(entry);
  switch (attr->typeDesc->declType) {
    case PrimType: {
      AppendSeq(dataCode,GenInstr(attr->reference,".word","0",NULL,NULL));
    } break;
    case FuncType: {
      // nothing to do here
    } break;
  }
}

void processFunctions(struct SymEntry * entry, int cnt, void * textCode) {
  struct Attr * attr = GetAttr(entry);
  switch (attr->typeDesc->declType) {
    case PrimType: {
      // nothing to do here
    } break;
    case FuncType: {
      if (!attr->typeDesc->funcDesc->funcCode) {
        PostMessageAndExit(GetCurrentColumn(),"function never implemented");
      }
      AppendSeq(textCode, attr->typeDesc->funcDesc->funcCode);
    } break;
  }
}

void processStringLit(struct SymEntry * entry, int cnt, void * dataCode) {
  struct Attr * attr = GetAttr(entry);
  AppendSeq(dataCode, GenInstr(attr->reference, ".asciiz", GetName(entry), NULL, NULL));
}

// Semantics Actions

void
Finish() {
  printf("Finish\n");
  ListIdentifierTable();

  struct InstrSeq * textCode = GenInstr(NULL,".text",NULL,NULL,NULL);
  struct InstrSeq * dataCode = GenInstr(NULL,".data",NULL,NULL,NULL);

  // form module preamble
  struct SymEntry * mainEntry = LookupName(IdentifierTable,"main");
  if (!mainEntry) {
    PostMessageAndExit(GetCurrentColumn(),"no main function for module");
  }
  // should make sure main takes no arguments
  struct Attr * mainAttr = GetAttr(mainEntry);
  // need to keep spim happy
  AppendSeq(textCode,GenInstr(NULL,".globl","__start",NULL,NULL));
  AppendSeq(textCode,GenInstr("__start",NULL,NULL,NULL,NULL));
  AppendSeq(textCode,GenInstr(NULL,"jal",mainAttr->reference,NULL,NULL));
  AppendSeq(textCode,GenInstr(NULL,"li","$v0","10",NULL));
  AppendSeq(textCode,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  // run SymTab with InvokeOnEntries putting globals in data seg
  InvokeOnEntries(IdentifierTable,true,processGlobalIdentifier,0,dataCode);

  // run StringLitTable adding string lits in data seq
  //AppendSeq(dataCode, GenInstr("__iobuf", ".space", "4", NULL, NULL));
  InvokeOnEntries(StringLitTable,false, processStringLit ,0,dataCode);

  // run SymTab with InvokeOnEntries putting functions in code seq
  InvokeOnEntries(IdentifierTable,true,processFunctions,0,textCode);

  // combine and write
  struct InstrSeq * moduleCode = AppendSeq(textCode,dataCode);
  WriteSeq(moduleCode);

  // free code
  FreeSeq(moduleCode);
  CloseCodeGen();
  fprintf(stderr,"Finish \n");
}

void
ProcDecls(struct IdList * idList, enum BaseTypes baseType) {

  while( idList ) {
      struct SymEntry * entry = idList->entry;
      struct Attr * curAttr = GetAttr(entry);
      struct TypeDesc * typeDesc = curAttr->typeDesc;
      enum DeclTypes type = typeDesc->declType;
      if( type == PrimType ) {
          typeDesc->primDesc = baseType;
      }
      else if( type == FuncType ) {
          typeDesc->funcDesc = malloc(sizeof(struct FuncDesc));
      }
      SetAttr(entry, STRUCT_KIND, curAttr);
      idList = idList->next;
  }
  // walk IdList items
    // switch for prim or func type
      // names on IdList are only specified as PrimType or FuncType
      // set type desc
    // for Sem1 everthing is in global scope, otherwise check scope
    // attr reference string is id name with prepended "_"
}

// Q: Adds name to end of list, unless we want to prepend here?
struct IdList *
AppendIdList(struct IdList * item, struct IdList * list) {
    item->next = list;
    return item;
}

struct IdList *
ProcName(char * id, enum DeclTypes type) {
    struct SymEntry * entry = LookupName(IdentifierTable, id);
    if(entry != NULL) {
        printf("ProcName error, id exists\n");
        return NULL;
    }
    entry = EnterName(IdentifierTable, id);
    struct IdList * node = malloc(sizeof(struct IdList));
    struct TypeDesc * desc = malloc(sizeof(struct TypeDesc));
    desc->declType = type;
    struct Attr * newAttr = malloc(sizeof(struct Attr));
    newAttr->typeDesc = desc;
    char * ref = malloc((strlen(id) + 1) * sizeof(char));
    strcpy(ref, "_");
    strcat(ref, id);
    newAttr->reference = ref;
    SetAttr(entry, STRUCT_KIND, newAttr);
    node->entry = entry;
    return node;
  // get entry for id, error if it exists
  // enter id in symtab
  // create IdList node for entry
  // create and partially init type descriptor, completed in ProcDecls
  // create, init and set attr struct
}

void
ProcFunc(char * id, struct InstrSeq * instrs) {
  struct SymEntry * entry = LookupName(IdentifierTable, id);
  if( entry == NULL ) {
      PostMessageAndExit(GetCurrentColumn(), "Funct Id not declared");
  }
  struct Attr * attr = GetAttr(entry);
  struct InstrSeq * seq = GenInstr(attr->reference, NULL, NULL, NULL, NULL);
  AppendSeq(seq, instrs);
  struct InstrSeq * ret = GenInstr(NULL, "jr", "$ra", NULL, NULL);
  AppendSeq(seq, ret);

  attr->typeDesc->funcDesc->funcCode = seq;
  // lookup name
  // get attr
  // gen instr for function entry
  // include function body code
  // function exit code, i.e. jump return
}

// Formats a Chr Lit to print
struct InstrSeq *
PutChrLit(char * val) {
    int t0 = AvailTmpReg();
    char * t0Txt = TmpRegName(t0);

    int strLen = strlen(val);
    char * placeholder = malloc(sizeof(char) * 4);
    struct InstrSeq * seq;// = GenInstr( NULL, "li", t0Txt, val, NULL );
    if ( strLen == 3 ) {
        int intVal = val[1];
        snprintf(placeholder, 4, "%d", intVal);
        seq = GenInstr(NULL, "li", TmpRegName(t0), placeholder, NULL);
    }
    else {
        if (val[2] == 'n') {
            snprintf(placeholder, 4, "%d", 10);
            seq = GenInstr(NULL, "li", TmpRegName(t0), placeholder, NULL);
        } else if (val[2] == 't') {
            snprintf(placeholder, 4, "%d", 9);
            seq = GenInstr(NULL, "li", TmpRegName(t0), placeholder, NULL);
        }
    }
    free(placeholder);
    AppendSeq( seq, GenInstr( NULL, "li", "$v0", "11", NULL ));
    AppendSeq( seq, GenInstr( NULL, "move", "$a0", t0Txt, NULL ));
    AppendSeq( seq, GenInstr( NULL, "syscall", NULL, NULL, NULL ));
    ReleaseTmpReg(t0);
    return seq;
}

struct InstrSeq *
PutVar(char * id) {
    struct SymEntry * entry = LookupName(IdentifierTable, id);
    if(entry == NULL) {
        PostMessageAndExit(GetCurrentColumn(), "Id not found");
    }
    struct Attr * attr = GetAttr(entry);
    int t0 = AvailTmpReg();
    char * t0Txt = TmpRegName(t0);
    struct InstrSeq * seq = GenInstr(NULL, "lw", t0Txt, attr->reference, NULL);
    int primDesc = attr->typeDesc->primDesc;
    switch( primDesc ) {
        case IntBaseType: {
            AppendSeq(seq, GenInstr(NULL, "li", "$v0", "1", NULL));
            break;
        }
        case ChrBaseType: {
            AppendSeq(seq, GenInstr(NULL, "li", "$v0", "11", NULL));
            break;
        }
    }
    AppendSeq(seq, GenInstr(NULL, "move", "$a0", t0Txt, NULL));
    AppendSeq(seq, GenInstr(NULL, "syscall", NULL, NULL, NULL));
    ReleaseTmpReg(t0);
    return seq;
}

// Returns input from std in
struct ExprResult *
Get(enum BaseTypes baseType) {

    int valReg = AvailTmpReg();
    char * regTxt = TmpRegName(valReg);
    struct ExprResult * exprResult = malloc(sizeof(struct ExprResult));
    struct InstrSeq * seq;
    // read_int
    switch( baseType ) {
        case IntBaseType: {
            seq = GenInstr( NULL, "li", "$v0", "11", NULL );
            AppendSeq( seq, GenInstr( NULL, "li", "$v0", "5", NULL ));
            AppendSeq( seq, GenInstr( NULL, "syscall", NULL, NULL, NULL ));
            AppendSeq( seq, GenInstr( NULL, "move", regTxt, "$v0", NULL ));
            exprResult->exprType = IntBaseType;
            break;
        }
        case ChrBaseType: {
            break;
        }
        default: {

        }
    }
    exprResult->instrs = seq;
    exprResult->registerNum = valReg;
    return exprResult;
}

// Gets the temp register for an imm value
struct ExprResult *
GetImmInt(char * textVal) {
    struct ExprResult * exprResult = malloc(sizeof(struct ExprResult));
    int reg = AvailTmpReg();
    struct InstrSeq * seq = GenInstr(NULL, "li", TmpRegName(reg), textVal, NULL);
    exprResult->instrs = seq;
    exprResult->registerNum = reg;
    exprResult->exprType = IntBaseType;
    return exprResult;
}

struct ExprResult *
GetVarExpr(char * id) {
    struct SymEntry * entry = LookupName(IdentifierTable, id);
    if(entry == NULL) {
        PostMessageAndExit(GetCurrentColumn(), "Id not found");
    }
    struct Attr * attr = GetAttr(entry);
    struct ExprResult * exprResult = malloc(sizeof(struct ExprResult));
    int t0 = AvailTmpReg();
    char * t0Txt = TmpRegName(t0);
    exprResult->registerNum = t0;
    // TODO: Add checking for declType: Temp fix below
    int declType = attr->typeDesc->declType;
    if(declType == PrimType) {
        exprResult->exprType = attr->typeDesc->primDesc;
    }
    else {
        exprResult->exprType = 0;
    }
    exprResult->instrs = GenInstr(NULL, "lw", t0Txt, attr->reference, NULL);
    return exprResult;
}

// struct ExprResult *
// GetNegateFactor(struct ExprResult * exprRes) {
//     int regNum = exprRes->registerNum;
//     struct seq = exprRes->instrs;
//     int t0 = AvailTmpReg();
//     char * t0Txt = TmpRegName(t0);
//     GenInstr(NULL, "li", t0Txt, "-1", NULL);
//     AppendSeq(seq, GenInstr(NULL, "mult", TmpRegName(regNum), t0Txt, NULL));
//     return exprRes;
// GRAM: Factor  :  '-' Factor { $$ = GetNegateFactor($2); };
// }

struct InstrSeq *
ProcAssign(char * id,  struct ExprResult * exprResult) {
    struct InstrSeq * seq = exprResult->instrs;
    struct SymEntry * entry = LookupName(IdentifierTable, id);
    if(entry == NULL) {
        PostMessageAndExit(GetCurrentColumn(), "The Id is never declared");
    }
    else {
        // TODO: Make sure types are the same
        // TODO: Verify correct type of attribute?
        struct Attr * attr = GetAttr(entry);
        int reg = exprResult->registerNum;
        char * regName = TmpRegName(reg);
        char * ref = attr->reference;
        AppendSeq( seq, GenInstr(NULL, "sw", regName, ref, NULL) );
    }
    return seq;
}

struct ExprResult *
EvalExpr(struct ExprResult * expr1, enum Operators op, struct ExprResult * expr2) {
    struct ExprResult * exprRes = malloc(sizeof(struct ExprResult));
    exprRes->registerNum = AvailTmpReg();
    exprRes->exprType = IntBaseType;
    exprRes->instrs = expr1->instrs;
    AppendSeq(exprRes->instrs, expr2->instrs);
    char * t0Txt = TmpRegName(exprRes->registerNum);
    char * regLeft = TmpRegName(expr1->registerNum);
    char * regRight = TmpRegName(expr2->registerNum);
    struct InstrSeq * newSeq;
    switch( op ) {
        case Add:
            newSeq = GenInstr(NULL, "add", t0Txt, regLeft, regRight);
            break;
        case Sub:
            newSeq = GenInstr(NULL, "sub", t0Txt, regLeft, regRight);
            break;
        case Mul:
            newSeq = GenInstr(NULL, "mul", t0Txt, regLeft, regRight);
            break;
        case Div:
            newSeq = GenInstr(NULL, "div", t0Txt, regLeft, regRight);
            break;
        default:
            PostMessageAndExit(GetCurrentColumn(), "Invalid op");
    }
    AppendSeq(exprRes->instrs, newSeq);
    ReleaseTmpReg(expr1->registerNum);
    ReleaseTmpReg(expr2->registerNum);
    free(expr1);
    free(expr2);
    return exprRes;
}

// Evaluate a conditional statement, used in fors, ifs, etc.
// struct CondResult *
// EvalCond(struct ExprResult * expr1, enum CondOps condOp, struct ExprResult * expr2) {
//     struct CondResult * result = malloc(sizeof(struct CondResult));
//     result->condOp = condOp;
//     result->instrs = expr1->instrs;
//     AppendSeq(result->instrs, expr2->instrs);
//     result->label = GenLabel();
//     char * op;
//     switch (condOp) {
//         case NotEql: op = "beq";
//             break;
//         case Eql: op = "bne";
//             break;
//         case Less: op = "bge";
//             break;
//         case Grtr: op = "ble";
//             break;
//         case GrtrEql: op = "blt";
//             break;
//         case LessEql: op = "bgt";
//             break;
//         default:
//             PostMessageAndExit(GetCurrentColumn(), "Invalid Comparison Operator");
//     }
//     AppendSeq(result->instrs, GenInstr(NULL, op, TmpRegName(expr1->registerNum),
//         TmpRegName(expr2->registerNum), result->label));
//     //ReleaseTmpReg(expr1->registerNum);
//     //ReleaseTmpReg(expr2->registerNum);
//     //free expr1, expr2...
//     return result;
// }

struct ExprResult *
EvalBoolExpr(struct ExprResult * expr1, enum CondOps condOp, struct ExprResult * expr2) {
    printf("BOOL EVALUATED\n");
    struct ExprResult * newExpr = malloc(sizeof(struct ExprResult));
    newExpr->instrs = expr1->instrs;
    AppendSeq(newExpr->instrs, expr2->instrs);
    newExpr->exprType = BoolBaseType;
    int truth = AvailTmpReg(); char * truthTxt = TmpRegName(truth);
    newExpr->registerNum = truth;
    char * left = TmpRegName(expr1->registerNum);
    char * right = TmpRegName(expr2->registerNum);
    struct InstrSeq * seq;
    switch( condOp ) {
        case Eql:
            seq = GenInstr(NULL, "seq", truthTxt, left, right);
            break;
        case NotEql:
            seq = GenInstr(NULL, "sne", truthTxt, left, right);
            break;
        case Less:
            seq = GenInstr(NULL, "slt", truthTxt, left, right);
            break;
        case Grtr:
            seq = GenInstr(NULL, "sgt", truthTxt, left, right);
            break;
        case LessEql:
            seq = GenInstr(NULL, "sle", truthTxt, left, right);
            break;
        case GrtrEql:
            seq = GenInstr(NULL, "sge", truthTxt, left, right);
            break;
        default:
            PostMessageAndExit(GetCurrentColumn(), "Invalid Comparison Operator");
    }
    AppendSeq(newExpr->instrs, seq);
    ReleaseTmpReg(expr1->registerNum);
    ReleaseTmpReg(expr2->registerNum);
    return newExpr;
}

//Cond      : '(' Expr CondOp Expr ')'                            { $$ = EvalCond($2, $3, $4); };

// Return a boolean expression
struct ExprResult *
AndOrExpr(struct ExprResult * expr1, char * op, struct ExprResult * expr2) {
    if(expr1->exprType != BoolBaseType && expr2->exprType != BoolBaseType) {
        PostMessageAndExit(GetCurrentColumn(), "Expressions not boolean types");
    }
    char * opStr;
    struct ExprResult * newExpr = malloc(sizeof(struct ExprResult));
    int t0 = AvailTmpReg();
    char * t0Txt = TmpRegName(t0);
    newExpr->exprType = BoolBaseType;
    newExpr->registerNum = t0;
    newExpr->instrs = expr1->instrs;
    AppendSeq(newExpr->instrs, expr2->instrs);
    AppendSeq(newExpr->instrs, GenInstr(NULL, op, t0Txt,
        TmpRegName(expr1->registerNum), TmpRegName(expr1->registerNum)));
    ReleaseTmpReg(expr1->registerNum);
    ReleaseTmpReg(expr2->registerNum);
    return newExpr;
}

// Negate an existing boolean expression
struct ExprResult *
NegateExpr(struct ExprResult * original) {
    if( original->exprType != BoolBaseType ) {
        PostMessageAndExit(GetCurrentColumn(), "Expressions not boolean types");
    }
    char * regName = TmpRegName(original->registerNum);
    AppendSeq(original->instrs, GenInstr(NULL, "nor", regName, regName, regName));
    return original;
}

struct CondResult *
EvalBoolCond(struct ExprResult * boolExpr) {

    if( boolExpr->exprType != BoolBaseType ) {
        PostMessageAndExit(GetCurrentColumn(), "Not boolean expression");
    }
    struct CondResult * condResult = malloc(sizeof(struct CondResult));
    condResult->instrs = boolExpr->instrs;
    condResult->label = GenLabel();
    AppendSeq(condResult->instrs, GenInstr(NULL, "beq", "$zero",
        TmpRegName(boolExpr->registerNum), condResult->label));
    //ReleaseTmpReg(boolExpr->registerNum);
    //free(condResult->label);
    //free(condResult);
    return condResult;
}

struct InstrSeq *
ProcIf(struct CondResult * condResult, struct InstrSeq * thenBody, struct InstrSeq * elseBody) {
    struct InstrSeq * seq = condResult->instrs;
    AppendSeq(seq, thenBody);
    if( elseBody == NULL ) {
        AppendSeq(seq, GenInstr(condResult->label, NULL, NULL, NULL, NULL));
    }
    else {
        char * elseLabel = GenLabel();
        AppendSeq(seq, GenInstr(NULL, "b", elseLabel, NULL, NULL));
        AppendSeq(seq, GenInstr(condResult->label, NULL, NULL, NULL, NULL));
        AppendSeq(seq, elseBody);
        AppendSeq(seq, GenInstr(elseLabel, NULL, NULL, NULL, NULL));
    }
    free(condResult->label);
    free(condResult);
    return seq;
}

struct InstrSeq *
ProcWhile(struct CondResult * condResult, struct InstrSeq * body) {
    char * loopLabel = GenLabel();
    struct InstrSeq * seq = GenInstr(loopLabel, NULL, NULL, NULL, NULL);
    AppendSeq(seq, condResult->instrs);
    AppendSeq(seq, body);
    AppendSeq(seq, GenInstr(NULL, "b", loopLabel, NULL, NULL));
    AppendSeq(seq, GenInstr(condResult->label, NULL, NULL, NULL, NULL));
    free(loopLabel);
    //free(condResult->label);
    //free(condResult);
    return seq;
}

struct InstrSeq *
PutStrLit(const char * string) {
    struct SymEntry * strEntry = LookupName(StringLitTable, string);
    struct Attr * attr;
    char * strLabel;

    // If first strlit of this name, create new entry and attr for label
    if( strEntry == NULL ) {
        strLabel = GenLabel();
        strEntry = EnterName(StringLitTable, string);
        attr = malloc(sizeof(struct Attr));
        // reference acts as label here
        attr->reference = strLabel;
        SetAttr(strEntry, STRING_KIND, attr);
    }
    else {
        attr = GetAttr(strEntry);
        strLabel = attr->reference;
    }
    struct InstrSeq * seq = GenInstr(NULL, "li", "$v0", "4", NULL);
    AppendSeq(seq, GenInstr(NULL, "la", "$a0", strLabel, NULL));
    AppendSeq(seq, GenInstr(NULL, "syscall", NULL, NULL, NULL));
    return seq;
}

struct InstrSeq *
IncrVar(char * id, char * amount) {
    struct SymEntry * entry = LookupName(IdentifierTable, id);
    if(entry == NULL) {
        PostMessageAndExit(GetCurrentColumn(), "Id not defined");
    }
    struct Attr * attr = GetAttr(entry);
    char * ref = attr->reference;
    int t0 = AvailTmpReg();
    char * t0Txt = TmpRegName(t0);
    struct InstrSeq * seq = GenInstr(NULL, "lw", t0Txt, ref, NULL);
    AppendSeq(seq, GenInstr(NULL, "addi", t0Txt, t0Txt, amount));
    AppendSeq(seq, GenInstr(NULL, "sw", t0Txt, ref, NULL));
    ReleaseTmpReg(t0);
    return seq;
}
