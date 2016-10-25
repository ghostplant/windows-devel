#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "css.h"
#include "csg.h"

#define VAR_TYPE "auto"

static int sym;
static CSGNode globscope;
static CSSIdent currProcName;

static void InsertObj(CSGNode *root, signed char wordClass, CSGType type, CSSIdent name, long val)
{
  register CSGNode curr;

  for (curr = *root; curr != NULL; curr=curr->next)
    CSSStopIf(!strcmp(curr->name, name), "duplicate symbol");
  curr = malloc(sizeof(CSGNodeDesc));
  curr->next = *root;
  curr->wordClass = wordClass;
  curr->type = type;
  strcpy(curr->name, name);
  curr->val = val;
  curr->dsc = 0;
  curr->lev = 0;
  *root=curr;
}

static CSGNode FindObj(CSGNode *root, CSSIdent *id)
{
  register CSGNode obj;

  obj = *root;
  while (obj != NULL && strcmp(obj->name, *id) != 0)
    obj = obj -> next;
  if (obj != NULL)
    CSSStopIf(((obj->wordClass == CSGVar) || (obj->wordClass == CSGFld)) && ((obj->lev != 0) && (obj->lev != CSGcurlev)), "object cannot be accessed");
  return obj;
}

static CSGNode AddToList(CSGNode *root, CSSIdent *id)
{
  register CSGNode curr;
  int satisfy;
  
  if (*root != NULL) {
    curr = *root;
    while ((satisfy = (curr->lev != CSGcurlev) || (strcmp(curr->name, *id) != 0)) && curr->next != NULL)
      curr = curr->next;
    CSSStopIf(!satisfy, "duplicate identifier");
  }
  curr = malloc(sizeof(CSGNodeDesc));
  curr->wordClass = -1;
  curr->lev = CSGcurlev;
  strcpy(curr->name, *id);
  curr->dsc = NULL;
  curr->type = NULL;
  curr->val = 0;
  curr->next = *root;
  *root = curr;
  
  return curr;
}

static void InitObj(CSGNode obj, signed char wordClass, CSGNode dsc, CSGType type, long val)
{
  obj->wordClass = wordClass;
  obj->dsc = dsc;
  obj->type = type;
  if (wordClass==CSGProc)
    obj->truePort = (CSGNode)val;
  else
    obj->val = val;
}

/*************************************************************************/

static void Expression(CSGNode *x);
static void DesignatorM(CSGNode *x);

static void Factor(CSGNode *x)
{
  register CSGNode obj;

  switch (sym) {
    case CSSident:
      obj = FindObj(&globscope, &CSSid);
      CSSStopIf(obj == NULL, "unknown identifier");
      CSGMakeNodeDesc(x, obj);
      sym = CSSGet();  // consume ident before calling Designator
      DesignatorM(x);
      break;
    case CSSnumber:
      CSGMakeConstNodeDesc(x, CSGautoType, CSSval);
      sym = CSSGet();
      break;
    case CSSlparen:
      sym = CSSGet();
      Expression(x);
      CSSStopIf(sym != CSSrparen, "')' expected");
      sym = CSSGet();
      break;
    default: CSSError("factor expected");
  }
}

static void Term(CSGNode *x)
{
  register int op;
  CSGNode y;

  Factor(x);
  while ((sym == CSStimes) || (sym == CSSdiv) || (sym == CSSmod)) {
    op = sym; 
    sym = CSSGet();
    y = malloc(sizeof(CSGNodeDesc));
    Factor(&y);
    CSGOp2(op, x, y);
  }
}

static void SimpleExpression(CSGNode *x)
{
  register int op;
  CSGNode y;

  if ((sym == CSSplus) || (sym == CSSminus)) {
    op = sym; 
    sym = CSSGet();
    Term(x);
    CSGOp1(op, x);
  } else {
    Term(x);
  }
  while ((sym == CSSplus) || (sym == CSSminus)) {
    op = sym; 
    sym = CSSGet();
    y = malloc(sizeof(CSGNodeDesc));
    assert(y != NULL);
    Term(&y);
    CSGOp2(op, x, y);
  }
}

static void EqualityExpr(CSGNode *x)
{
  register int op;
  CSGNode y;

  SimpleExpression(x);
  if ((sym == CSSlss) || (sym == CSSleq) || (sym == CSSgtr) || (sym == CSSgeq)) {
    y = malloc(sizeof(CSGNodeDesc));
    op = sym; 
    sym = CSSGet();
    SimpleExpression(&y);
    CSGRelation(op, x, y);
  }
}

static void Expression(CSGNode *x)
{
  register int op;
  CSGNode y;

  EqualityExpr(x);
  if ((sym == CSSeql) || (sym == CSSneq)) {
    op = sym; 
    sym = CSSGet();
    y = malloc(sizeof(CSGNodeDesc));
    assert(y != NULL);
    EqualityExpr(&y);
    CSGRelation(op, x, y);
  }
}

static void ConstExpression(CSGNode *expr)
{
  Expression(expr);
  CSSStopIf((*expr)->wordClass != CSGConst, "constant expression expected");
}


/*static void FieldList(CSGType type)
{
  register CSGNode curr;

  VariableDeclaration(&(type->fields));
  while (sym != CSSrbrace) {
    VariableDeclaration(&(type->fields));
  }
  curr = type->fields;
  CSSStopIf(curr == NULL, "empty structs are not allowed");
  while (curr != NULL) {
    curr->wordClass = CSGFld;
    curr->val = type->size;
    type->size += curr->type->size;
    if (type->size > 0x7fffffff) CSSError("struct too large");
    curr = curr->next;
  }
}

static void StructType(CSGType *type)
{
  register CSGNode obj;
  register int oldinstruct;
  CSSIdent id;

  assert(sym == CSSstruct);
  sym = CSSGet();
  if (sym != CSSident) CSSError("identifier expected");
  strcpy(id, CSSid);
  sym = CSSGet();
  if (sym != CSSlbrace) {
    obj = FindObj(&globscope, &id);
    if (obj == NULL) CSSError("unknown struct type");
    if ((obj->wordClass != CSGTyp) || (obj->type->form != CSGStruct)) CSSError("struct type expected");
    *type = obj->type;
  } else {
    sym = CSSGet();
    *type = malloc(sizeof(CSGTypeDesc));
    if ((*type) == NULL) CSSError("out of memory");
    (*type)->form = CSGStruct;
    (*type)->fields = NULL;
    (*type)->size = 0;
    oldinstruct = instruct;
    instruct = 1;
    FieldList(*type);
    instruct = oldinstruct;
    if (sym != CSSrbrace) CSSError("'}' expected");
    sym = CSSGet();
    obj = AddToList(&globscope, &id);
    InitObj(obj, CSGTyp, NULL, *type, (*type)->size);
  }
}*/

static void Type(CSGType *type)
{
  register CSGNode obj;

  CSSStopIf(sym == CSSstruct, "please use array instead of struct type");
  /*if (sym == CSSstruct) {
    StructType(type);
    return;
  }*/
  CSSStopIf(sym != CSSident, "identifier expected");
  obj = FindObj(&globscope, &CSSid);
  CSSStopIf(obj == NULL, "unknown type");
  sym = CSSGet();
  CSSStopIf(obj->wordClass != CSGTyp, "type expected");
  *type = obj->type;
}

static void RecurseArray(CSGType *type)
{
  register CSGType typ;
  CSGNode expr;

  expr = malloc(sizeof(CSGNodeDesc));
  sym = CSSGet();
  ConstExpression(&expr);
  CSSStopIf(expr->type != CSGautoType, "constant auto expression required");
  CSSStopIf(sym != CSSrbrak, "']' expected");
  sym = CSSGet();
  CSSStopIf(sym == CSSlbrak, "recurse array not allowed");
  /*if (sym == CSSlbrak)
    RecurseArray(type);*/
  typ = malloc(sizeof(CSGTypeDesc));
  typ->form = CSGArray;
  typ->len = expr->val;
  typ->base = *type;
  CSSStopIf(0x7fffffff / typ->len < typ->base->size, "array size too large");

  typ->size = typ->len * typ->base->size;
  *type = typ;
}

static void IdentArray(CSGNode *root, CSGType type)
{
  CSGNode obj;
  int len = -1;

  CSSStopIf(sym != CSSident, "identifier expected");
  obj = AddToList(root, &CSSid);
  sym = CSSGet();
  if (sym == CSSlbrak) {
    RecurseArray(&type);
    len = type->len;
  }
  CSGAddIdentArray(obj, len);

  //if (instruct == 0) tos -= type->size;
  InitObj(obj, CSGVar, NULL, type,0/*tos*/);
}


static void IdentList(CSGNode *root, CSGType type)
{
  IdentArray(root, type);
  while (sym == CSScomma) {
    sym = CSSGet();
    IdentArray(root, type);
  }
}

static void VariableDeclaration(CSGNode *root)
{
  CSGType type;
  Type(&type);
  IdentList(root, type);
  CSSStopIf(sym != CSSsemicolon, "';' expected");
  sym = CSSGet();
}


static void ConstantDeclaration(CSGNode *root)
{
  register CSGNode obj;
  CSGType type;
  CSGNode expr;
  CSSIdent id;

  expr = malloc(sizeof(CSGNodeDesc));
  sym = CSSGet();
  Type(&type);
  CSSStopIf(type != CSGautoType, "only auto type supported");
  CSSStopIf(sym != CSSident, "identifier expected");
  strcpy(id, CSSid);
  sym = CSSGet();

  CSSStopIf(sym != CSSbecomes, "'=' expected");
  sym = CSSGet();
  ConstExpression(&expr);
  CSSStopIf(expr->type != CSGautoType, "constant auto expression required");
  obj = AddToList(root, &id);
  
  InitObj(obj, CSGConst, NULL, type, expr->val);

  CSSStopIf(sym != CSSsemicolon, "';' expected");
  sym = CSSGet();
}


/*************************************************************************/


static void DesignatorM(CSGNode *x)
{
  register CSGNode obj;
  CSGNode y;

  // CSSident already consumed
  while ((sym == CSSperiod) || (sym == CSSlbrak)) {
    /*if (sym == CSSperiod) {
      sym = CSSGet();
      if ((*x)->type->form != CSGStruct) CSSError("struct type expected");
      if (sym != CSSident) CSSError("field identifier expected");
      obj = FindObj(&(*x)->type->fields, &CSSid);
      sym = CSSGet();
      if (obj == NULL) CSSError("unknown identifier");
      CSGField(x, obj);
    } else {*/
      sym = CSSGet();
      CSSStopIf((*x)->type->form != CSGArray, "array type expected");
      y = malloc(sizeof(CSGNodeDesc));
      Expression(&y);
      CSGIndex(x, y);
      CSSStopIf(sym != CSSrbrak, "']' expected");
      sym = CSSGet();
    //}
  }
}


static void AssignmentM(CSGNode *x)
{
  CSGNode y;
  // CSSident already consumed
  y = malloc(sizeof(CSGNodeDesc));
  DesignatorM(x);
  CSSStopIf(sym != CSSbecomes, "'=' expected");
  sym = CSSGet();
  Expression(&y);
  CSGStore(*x, y);
  CSSStopIf(sym != CSSsemicolon, "';' expected");
  sym = CSSGet();
}


static void ExpList(CSGNode proc)
{
  register int argcnt=(int)proc->dsc, actcnt=1;
  CSGNode x;

  x = malloc(sizeof(CSGNodeDesc));
  Expression(&x);
  // CSSStopIf(!proc->dsc, "too many parameters");
  // CSSStopIf(x->type != curr->type, "incorrect type");

  while (CSGParameter(&x, CSGautoType), sym == CSScomma) {
    x = malloc(sizeof(CSGNodeDesc));
    sym = CSSGet();
    Expression(&x);
    actcnt++;
    // CSSStopIf(curr == NULL || curr->dsc != proc, "too many parameters");
    // CSSStopIf(x->type != curr->type, "incorrect type");
  }
  CSSStopIf(argcnt<actcnt, "too many parameters");
  CSSStopIf(argcnt>actcnt, "too few parameters");
}

static void ProcedureCallM(CSGNode obj, CSGNode *x)
{
  CSGNode y;
  CSGMakeNodeDesc(x, obj);
  sym = CSSGet();
  if ((*x)->wordClass == CSGSProc) { // built-in proc
    y = malloc(sizeof(CSGNodeDesc));
    if ((*x)->val == 1) {
      CSSStopIf(sym != CSSident, "identifier expected");
      obj = FindObj(&globscope, &CSSid);
      CSSStopIf(obj == NULL, "unknown identifier");
      CSGMakeNodeDesc(&y, obj);
      sym = CSSGet();  // consume ident before calling Designator
      DesignatorM(&y);
    } else if ((*x)->val == 2/* || (*x)->val == 4 || (*x)->val == 5*/) {
      Expression(&y);
    }
    CSGIOCall(*x, y);
  } else { // user-def proc
    if (sym != CSSrparen) {
      ExpList(obj);
    } else {
      CSSStopIf(obj->dsc != 0, "too few parameters");
    }
    CSGCall(*x);
  }
  CSSStopIf(sym != CSSrparen, "')' expected");
  sym = CSSGet();
  CSSStopIf(sym != CSSsemicolon, "';' expected");
  sym = CSSGet();
}


static void StatementSequence(void);

// This function parses if statements - helpful for CFG creation.
static void IfStatement(void)
{
  CSGNode label;
  CSGNode x;

  x = malloc(sizeof(CSGNodeDesc));
  sym = CSSGet();
  CSSStopIf(sym != CSSlparen, "'(' expected");
  CSGInitLabel(&label);
  sym = CSSGet();

  Expression(&x);
  CSGTestBool(&x);

  CSGFixLink(x->falsePort);
  if (sym != CSSrparen) CSSError("')' expected");
  sym = CSSGet();
  if (sym != CSSlbrace) CSSError("'{' expected");
  sym = CSSGet();
  StatementSequence();
  if (sym != CSSrbrace) CSSError("'}' expected");
  sym = CSSGet();
  if (sym == CSSelse) {
    sym = CSSGet();
    CSGFJump(&label);
    CSGFixLink(x->truePort);
    if (sym != CSSlbrace) CSSError("'{' expected");
    sym = CSSGet();
    StatementSequence();
    if (sym != CSSrbrace) CSSError("'}' expected");
    sym = CSSGet();
  } else {
    CSGFixLink(x->truePort);
  }
  CSGFixLink(label);
}

// This function parses while statements - helpful for CFG creation.
static void WhileStatement(void)
{
  CSGNode label;
  CSGNode x;

  x = malloc(sizeof(CSGNodeDesc));
  assert(x != NULL);
  assert(sym == CSSwhile);
  sym = CSSGet();
  if (sym != CSSlparen) CSSError("'(' expected");
  sym = CSSGet();
  CSGSetLabel(&label);
  Expression(&x);
  
  CSGTestBool(&x);
  CSGFixLink(x->falsePort);
  if (sym != CSSrparen) CSSError("')' expected");
  sym = CSSGet();
  if (sym != CSSlbrace) CSSError("'{' expected");
  sym = CSSGet();
  StatementSequence();
  if (sym != CSSrbrace) CSSError("'}' expected");
  sym = CSSGet();
  CSGBJump(label);
  CSGFixLink(x->truePort);
}

static void Statement(void)
{
  register CSGNode obj;
  CSGNode x;

  switch (sym) {
    case CSSif: IfStatement(); break;
    case CSSwhile: WhileStatement(); break;
    case CSSident:
      obj = FindObj(&globscope, &CSSid);
      CSSStopIf(obj == NULL, "unknown identifier");
      sym = CSSGet();
      x = malloc(sizeof(CSGNodeDesc));
      if (sym == CSSlparen) {
        ProcedureCallM(obj, &x);
      } else {
        CSGMakeNodeDesc(&x, obj);
        AssignmentM(&x);
      }
      break;
    case CSSsemicolon: break;  /* empty statement */
    default: CSSError("unknown statement");
  }
}


static void StatementSequence(void)
{
  while (sym != CSSrbrace)
    Statement();
}


/*************************************************************************/

static void FPSection(CSGNode *root, int *paddr)
{
  register CSGNode obj;
  CSGType type;

  Type(&type);
  CSSStopIf(type != CSGautoType, "only basic type formal parameters allowed");
  CSSStopIf(sym != CSSident, "identifier expected");
  obj = AddToList(root, &CSSid);

  sym = CSSGet();
  CSSStopIf(sym == CSSlbrak, "no array parameters allowed");
  InitObj(obj, CSGVar, *root, type, 0);
  //*paddr += type->size;

  //CSGProcedureArgs(CSSid);
}

static void FormalParameters(CSGNode *root, int *argcnt)
{
  register CSGNode curr;
  int paddr;

  //paddr = 16;
  FPSection(root, &paddr);
  (*argcnt)++;
  while (sym == CSScomma) {
    sym = CSSGet();
    FPSection(root, &paddr);
    (*argcnt)++;
  }

  /*curr = (*root)->next;
  while (curr != NULL) {
    paddr -= curr->type->size;
    curr->val = paddr;
    curr = curr->next;
  }*/
}


static void ProcedureHeading(CSGNode *proc)
{
  int i, argcnt = 0;
  CSSStopIf(sym != CSSident, "function name expected");
  CSSStopIf(CSSid[0]<'a' || CSSid[0]>'z', "first character of procedure name should be lower case letter");
  for (i=1;CSSid[i];i++)
  	CSSStopIf(CSSid[i]=='_', " charactor '_' for function name is not allowed");

  strcpy(currProcName, CSSid);

  CSGNode procpnt = AddToList(&globscope, &currProcName);
  InitObj(*proc, CSGProc, NULL, NULL, (long)CSGpc);
  
  CSGAdjustLevel(1);
  sym = CSSGet();
  CSSStopIf(sym != CSSlparen, "'(' expected");
  sym = CSSGet();
  if (sym != CSSrparen)
    FormalParameters(proc, &argcnt);
  procpnt->dsc = (CSGNode)argcnt;
  
  CSSStopIf(strcmp(currProcName, "main")==0 && argcnt>0, "main() should be no args");
  
  CSSStopIf(sym != CSSrparen, "')' expected");
  sym = CSSGet();

  CSGEntryPoint(*proc);
}


static void ProcedureBody(CSGNode *proc)
{
  register int returnsize;
  register CSGNode curr;

  //tos = 0;
  while ((sym == CSSconst) || (sym == CSSstruct) || ((sym == CSSident) && (strcmp(CSSid, VAR_TYPE) == 0))) {
    if (sym == CSSconst) {
      ConstantDeclaration(proc);
    } else {
      VariableDeclaration(proc);
    }
  }

  CSGEnter();
  /*assert((*proc)->dsc == NULL);
  (*proc)->dsc = (*proc)->next;
  if (-tos > 32768) CSSError("maximum stack frame size of 32kB exceeded");
  returnsize = 0;
  curr = (*proc)->dsc;
  while ((curr != NULL) && (curr->dsc == *proc)) {
    returnsize += 8;
    curr = curr->next;
  }*/
  StatementSequence();
  CSGReturn();
  CSGAdjustLevel(-1);
}

static void ProcedureDeclaration(void)
{
  assert(sym == CSSvoid);
  sym = CSSGet();
  ProcedureHeading(&globscope);

  CSSStopIf(sym != CSSlbrace, "'{' expected");
  sym = CSSGet();
  ProcedureBody(&globscope);
  CSSStopIf(sym != CSSrbrace, "'}' expected");
  sym = CSSGet();
}


/*************************************************************************/


static void Program(void)
{
  CSGOpen();
  //instruct = 0;
  sym = CSSGet();
  while ((sym != CSSvoid) && (sym != CSSeof)) {
    if (sym == CSSconst) {
      ConstantDeclaration(&globscope);
    } else {
      VariableDeclaration(&globscope);
    }
  }
  CSSStopIf(sym != CSSvoid, "procedure expected");
  while (sym == CSSvoid) {
    ProcedureDeclaration();
    while (strcmp(globscope->name, currProcName) != 0)
      globscope=globscope->next;
  }
  CSSStopIf(sym != CSSeof, "unrecognized characters at end of file");
}

static void Compile(char *filename)
{
  fprintf(stderr, "compiling %s\n", filename);

  globscope = NULL;
  InsertObj(&globscope, CSGTyp, CSGautoType, VAR_TYPE, 8);

  InsertObj(&globscope, CSGSProc, NULL, "scan", 1);
  InsertObj(&globscope, CSGSProc, NULL, "print", 2);
  InsertObj(&globscope, CSGSProc, NULL, "println", 3);
  //InsertObj(&globscope, CSGSProc, NULL, "var2ret", 4);
  //InsertObj(&globscope, CSGSProc, NULL, "ret2var", 5);

  CSSInit(filename);
  Program();
}

int main(int argc, char *argv[])
{
  CSGInit();
  if (argc >= 2) {
    Compile(argv[1]);
  } else
    return 0;
  CSGDecode(globscope);

  return 0;
}
