/*  Semantics.c
    Support and semantic action routines.
    Author:      Quentin Goehrig
    Created:     04/18.18
    Resources:

 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "YSemantics.h"

// Shared Data

struct SymTab * IdentifierTable;
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
        AppendSeq(textCode,GenInstr(attr->reference,".word","0",NULL,NULL));
      }

    } break;
  }
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

  InvokeOnEntries(IdentifierTable,true,processGlobalIdentifier,0,dataCode);
  InvokeOnEntries(IdentifierTable,true,processFunctions,0,textCode);

  // run SymTab with InvokeOnEntries putting globals in data seg
  // run SymTab with InvokeOnEntries putting functions in code seq

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
          typeDesc-> funcDesc = malloc(sizeof(struct FuncDesc));
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
  struct Attr * attr = GetAttr(entry);
  struct InstrSeq * seq = GenInstr(attr->reference, NULL, NULL, NULL, NULL);
  AppendSeq(seq, instrs);
  struct InstrSeq * ret = GenInstr(NULL, "jr", NULL, NULL, NULL);
  AppendSeq(seq, ret);
  // lookup name
  // get attr
  // gen instr for function entry
  // include function body code
  // function exit code, i.e. jump return
}

void
PrintVal(char * val) {
    GenInstr(t0, ) // load to tmp reg some value
}


// Returns input from std in
void
Get(char * primaryType) {
    switch(primaryType) {
        case "int": {
            GenInstr()
        }
    }
}
// instr seq?

// Gets the temp register for an imm value
void
GetImmInt(int val) {
    char * regName = Imm(val);

}

void
ProcAssign(char * id,  struct * ExprResult) {
    // put

}

struct ExprResult *
ProcAddOp(struct ExprResult * term1, char op, struct ExprResult * term2) {
    int tmpReg = AvailTmpReg();
    char * tmpRegName = TmpRegName(tmpReg);
    if ( op == '+' ) {
        struct ExprResultGenInstr()
    }
    else if( op == '-' ) {
        GenInstr()
    }
    GenInstr()
}



}
