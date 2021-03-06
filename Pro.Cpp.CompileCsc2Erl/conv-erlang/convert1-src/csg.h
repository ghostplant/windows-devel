#ifndef _CSubCodeGen_H_
#define _CSubCodeGen_H_

#include <stdio.h>

/* wordClass */
enum {CSGVar, CSGConst, CSGFld, CSGTyp, CSGProc, CSGSProc, CSGAddr, CSGInst};

/* form */
enum {CSGInteger, CSGBoolean, CSGArray, CSGStruct};

typedef struct CSGTypeDesc *CSGType;
typedef struct CSGNodeDesc *CSGNode;

typedef struct CSGTypeDesc {
  signed char form;  // integer, array, struct
  CSGNode fields;  // linked list of the fields in a struct
  CSGType base;  // base type (array element type)
  int size;  // total size of the type
  int len;  // number of array elements
} CSGTypeDesc;

typedef struct CSGNodeDesc {
  signed char wordClass;  // Var, Const, Field, Type, Proc, SProc, Addr, Inst
  signed char lev;  // 0 = global, 1 = local
  CSGNode next;  // linked list of all objects in same scope
  CSGNode dsc;  // Proc: link to procedure scope (head)
  CSGType type;  // type
  CSSIdent name;  // name
  long val, ext;  // Const: value; Var: address; Fld: offset; SProc: number; Type: size
  char op;  // operation of instruction
  CSGNode x, y;  // the two operands
  CSGNode prv, nxt;  // previous and next instruction
  int line;  // line number for printing purposes
  CSGNode truePort, falsePort;  // Jmp: truePort and falsePort chains; Proc: truePort = entry point
  CSGNode original;  // Pointer to original node object if a copy was made
} CSGNodeDesc;

extern CSGType CSGautoType, CSGboolType;
extern char CSGcurlev;
extern CSGNode CSGpc;

extern void CSGMakeConstNodeDesc(CSGNode *x, CSGType typ, long val);
extern void CSGMakeNodeDesc(CSGNode *x, CSGNode y);
extern void CSGField(CSGNode *x, CSGNode y);
extern void CSGIndex(CSGNode *x, CSGNode y);
extern void CSGInitLabel(CSGNode *lbl);
extern void CSGSetLabel(CSGNode *lbl);
extern void CSGFixLink(CSGNode lbl);
extern void CSGBJump(CSGNode lbl);
extern void CSGFJump(CSGNode *lbl);
extern void CSGTestBool(CSGNode *x);
extern void CSGOp1(int op, CSGNode *x);
extern void CSGOp2(int op, CSGNode *x, CSGNode y);
extern void CSGRelation(int op, CSGNode *x, CSGNode y);
extern void CSGStore(CSGNode x, CSGNode y);
extern void CSGAdjustLevel(int n);
extern void CSGParameter(CSGNode *x, CSGType ftyp);
extern void CSGCall(CSGNode x);
extern void CSGIOCall(CSGNode x, CSGNode y);
extern void CSGEntryPoint(CSGNode scope);
extern void CSGEnter();
extern void CSGReturn();
extern void CSGOpen(void);
extern void CSGDecode(CSGNode globscope);
extern void CSGInit(void);

extern void CSGProcedureArgs(CSSIdent CSSid);
extern void CSGAddIdentArray(CSGNode x, int len);

#endif /* _CSubCodeGen_H_ */
