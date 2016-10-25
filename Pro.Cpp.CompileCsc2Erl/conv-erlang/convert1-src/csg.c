#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "css.h"
#include "csg.h"

enum {ineg, iadd, isub, imul, idiv, imod, iparam, iload,
      istore, imove, /*icmpeq, icmplt, icmple, */iblbs, iblbc, icall, ibr, iret,
      iread, iwrite, iwrl, inop, isetvalue, igetvalue, ilocalvar, ioffset, ientry};

CSGType CSGautoType, CSGboolType;
char CSGcurlev;
CSGNode CSGpc;

static CSGNode code, localvar, callparam;

/*****************************************************************************/

// This function adds a new instruction at the end of the instruction list
// and sets the operation to op and the operands to x and y.
static CSGNode PutOpNodeNode(int op, CSGNode x, CSGNode y)
{
  CSGNode i;

  CSGpc->nxt = malloc(sizeof(CSGNodeDesc));
  i = CSGpc;
  CSGpc = CSGpc->nxt;
  CSGpc->wordClass = CSGInst;
  CSGpc->op = inop;
  CSGpc->prv = i;
  CSGpc->nxt = NULL;

  i->wordClass = CSGInst;
  i->op = op;
  i->x = x;
  i->y = y;
  i->type = CSGautoType;
  i->lev = 0;

  return i;
}

static CSGNode PutOpNode(int op, CSGNode x)
{
  return PutOpNodeNode(op, x, NULL);
}


static CSGNode PutOp(int op)
{
  return PutOpNodeNode(op, NULL, NULL);
}

// This function SETS the member fields of a CSGNode object.
void CSGMakeConstNodeDesc(CSGNode *x, CSGType typ, long val)
{
  (*x)->wordClass = CSGConst;
  (*x)->type = typ;
  (*x)->val = val;
  (*x)->lev = CSGcurlev;
}

// This function make a COPY of a CSGNode object
void CSGMakeNodeDesc(CSGNode *x, CSGNode y)
{
  register int i;

  (*x)->wordClass = y->wordClass;
  (*x)->type = y->type;
  (*x)->val = y->val;
  (*x)->lev = y->lev;
  (*x)->truePort = y->truePort;
  strcpy((*x)->name, y->name);

  // Make the 'original' field of the copy to point back to original Node object
  (*x)->original = y->original? y->original : y;
}

/*****************************************************************************/

static void Load(CSGNode *x)
{
  if ((*x)->wordClass == CSGAddr) {
    *x = PutOpNode(iload, *x);
  } else if (((*x)->wordClass == CSGVar) && ((*x)->lev == 0)) {
  	//CSSError("global variable cound be only const");
    //*x = PutOpNodeNode(iadd, *x, GP);
    //*x = PutOpNode(iload, *x);
  }
}

void CSGOp2(int op, CSGNode *x, CSGNode y);

/*void CSGField(CSGNode *x, CSGNode y)  // x = x.y
{
  if ((*x)->wordClass == CSGVar) {
    if ((*x)->lev == 0) *x = PutOpNodeNode(iadd, *x, GP); else *x = PutOpNodeNode(iadd, *x, FP);
  }
  *x = PutOpNodeNode(iadd, *x, y);
  (*x)->type = y->type;
  (*x)->wordClass = CSGAddr;
}*/


void CSGIndex(CSGNode *x, CSGNode y)  /* x = x[y] */
{
  CSGNode z;

  if (y->wordClass == CSGConst)
    CSSStopIf((y->val < 0) || ((*x)->type->len <= y->val), "index out of bounds");
  z = malloc(sizeof(CSGNodeDesc));

  CSGMakeConstNodeDesc(&z, CSGautoType, (*x)->type->base->size);
  //CSGOp2(CSStimes, &y, z);
  z = *x;

  /*if ((*x)->wordClass != CSGAddr) {
    if ((*x)->wordClass != CSGInst) {
      if ((*x)->lev > 0) {
        *x = PutOpNodeNode(iadd, *x, FP);
      } else {
        *x = PutOpNodeNode(iadd, *x, GP);
      }
    }
  }*/
  *x = PutOpNodeNode(ioffset, *x, y);
  (*x)->type = z->type->base;
  (*x)->wordClass = CSGAddr;
}


/*****************************************************************************/


// The folloing five functions deal with control transfer.  Mostly, they
// remember where the control transferring instructions are so that their
// targets can be set later if the targets have not yet been compiled.

void CSGInitLabel(CSGNode *lbl)
{
  *lbl = NULL;
}


void CSGSetLabel(CSGNode *lbl)
{
  *lbl = CSGpc;
}

// This function sets the target of a forward jump, call, or branch once
// the target is being compiled.
void CSGFixLink(CSGNode lbl)
{
  if (lbl != NULL) {
    if ((lbl->op == icall) || (lbl->op == ibr)) {
      lbl->x = CSGpc;
    } else {
      lbl->y = CSGpc;
    }
  }
}

void CSGBJump(CSGNode lbl)
{
  PutOpNode(ibr, lbl);
}

void CSGFJump(CSGNode *lbl)
{
  PutOpNode(ibr, *lbl);
  *lbl = CSGpc->prv;
}

/*****************************************************************************/

static void TestInt(CSGNode x)
{
  CSSStopIf(x->type->form != CSGInteger, "type integer expected");
}


void CSGTestBool(CSGNode *x)
{
  CSSStopIf((*x)->type->form != CSGBoolean, "type boolean expected");
  Load(x);
}

/*****************************************************************************/

void CSGOp1(int op, CSGNode *x)  /* x = op x */
{
  Load(x);
  if (op == CSSplus) {
    TestInt(*x);
  } else if (op == CSSminus) {
    TestInt(*x);
    *x = PutOpNode(ineg, *x);
  }
}

void CSGOp2(int op, CSGNode *x, CSGNode y)  /* x = x op y */
{
  CSSStopIf((*x)->type != y->type, "incompatible types");
  Load(x);
  Load(&y);
  switch (op) {
    case CSSplus: *x = PutOpNodeNode(iadd, *x, y); break;
    case CSSminus: *x = PutOpNodeNode(isub, *x, y); break;
    case CSStimes: *x = PutOpNodeNode(imul, *x, y); break;
    case CSSdiv: *x = PutOpNodeNode(idiv, *x, y); break;
    case CSSmod: *x = PutOpNodeNode(imod, *x, y); break;
  }
}

void CSGRelation(int op, CSGNode *x, CSGNode y)
{
  TestInt(*x);
  TestInt(y);
  Load(x);
  Load(&y);
  switch (op) {
    case CSSeql: /* *x = PutOpNodeNode(icmpeq, *x, y);*/ *x = PutOpNode(iblbc, *x); (*x)->dsc=y; (*x)->ext=(long)" =="; break;
    case CSSneq: /* *x = PutOpNodeNode(icmpeq, *x, y);*/ *x = PutOpNode(iblbs, *x); (*x)->dsc=y; (*x)->ext=(long)" =="; break;
    case CSSlss: /* *x = PutOpNodeNode(icmplt, *x, y);*/ *x = PutOpNode(iblbc, *x); (*x)->dsc=y; (*x)->ext=(long)" <"; break;
    case CSSleq: /* *x = PutOpNodeNode(icmple, *x, y);*/ *x = PutOpNode(iblbs, *x); (*x)->dsc=y; (*x)->ext=(long)" >"; break;
    case CSSgtr: /* *x = PutOpNodeNode(icmple, *x, y);*/ *x = PutOpNode(iblbc, *x); (*x)->dsc=y; (*x)->ext=(long)" >"; break;
    case CSSgeq: /* *x = PutOpNodeNode(icmplt, *x, y);*/ *x = PutOpNode(iblbs, *x); (*x)->dsc=y; (*x)->ext=(long)" <"; break;
  }
  (*x)->type = CSGboolType;
  (*x)->falsePort = NULL;
  (*x)->truePort = CSGpc->prv;
}

/*****************************************************************************/

void CSGStore(CSGNode x, CSGNode y)  /* x = y */
{
  Load(&y);
  CSSStopIf(x->type->form != y->type->form, "incompatible assignment");
  CSSStopIf(x->type->form != CSGInteger, "only basic type assignments supported");
  if ((x->wordClass == CSGInst) || (x->wordClass == CSGAddr)) {
    PutOpNodeNode(istore, y, x);
  /*} else if ((x->wordClass == CSGVar) && (x->lev == 0)) {
    //x = PutOpNodeNode(iadd, x, GP);
    PutOpNodeNode(istore, y, x);*/
  } else {
    PutOpNodeNode(imove, y, x);
  }
}

void CSGAdjustLevel(int l)
{
  CSGcurlev += l;
}

void CSGParameter(CSGNode *x, CSGType ftyp)
{
  CSSStopIf(ftyp != CSGautoType, "auto type expected");
  Load(x);
  
  //CSGNode nx=malloc(sizeof(CSGNodeDesc));
  //*nx=**x;
  (*x)->next = callparam;
  callparam = (*x);

  //*x = PutOpNode(iparam, *x);
}


/*****************************************************************************/


void CSGCall(CSGNode x)
{
  x->ext=(long)callparam;
  callparam = NULL;
  PutOpNode(icall, x);
}


void CSGIOCall(CSGNode x, CSGNode y)
{
  CSGNode z;

  if (x->val == 1) {
    TestInt(y);
    z = PutOp(iread);
    CSGStore(y, z);
  } else if (x->val == 2) {
    TestInt(y);
    Load(&y);
    PutOpNode(iwrite, y);
  } else if (x->val == 3) {
    PutOp(iwrl);
  /*} else if (x->val == 4) {
    Load(&y);
    PutOpNode(isetvalue, y);
  } else if (x->val == 5) {
    Load(&y);
    PutOpNode(igetvalue, y);*/
  } else {
    CSSError("no such built-in function");
  }
}


void CSGEntryPoint(CSGNode scope)
{
  /*if (entrypc != NULL) CSSError("multiple program entry points");
  entrypc = CSGpc;*/
  //if (main_entry)
  //  PutOp(ientrypc);
  PutOpNode(ientry, scope);
  
  localvar=NULL;
}

void CSGAddIdentArray(CSGNode x, int len)
{
  x->ext=len;
  x->nxt=localvar;
  localvar=x;
}

void CSGOpen(void)
{
  CSGcurlev = 0;
  CSGpc = malloc(sizeof(CSGNodeDesc));
  CSGpc->wordClass = CSGInst;
  CSGpc->op = inop;
  CSGpc->prv = code;
  CSGpc->nxt = NULL;
  code->nxt = CSGpc;
}


void CSGEnter()
{
  PutOpNode(ilocalvar, localvar);
  /* size: The size of local variables */
  //CSGNode x = malloc(sizeof(CSGNodeDesc));
  //CSGMakeConstNodeDesc(&x, CSGautoType, size);
  //PutOpNode(ienter, x);
}


void CSGReturn()
{
  /* The size of formal parameters, shows how much to unwind the stack */
  // PutOp(ileave); Not using ileave in our implementation
  CSGNode x = malloc(sizeof(CSGNodeDesc));
  CSGMakeConstNodeDesc(&x, CSGautoType, 0);
  PutOpNode(iret, x);
}

static void PrintBrakNode(CSGNode x)
{
  while (x!= NULL && x->wordClass != CSGInst)
    x=x->nxt;
  if (x == NULL) {
    printf(" unknown brak wordClass");
  } else {
    printf(" %d", x->line);
  }
}

// This function Prints the CSGNode information based on the Node type
static void PrintNode(CSGNode x)
{
  /*if (x == GP) {
    printf(" GP");
  } else if (x == FP) {
    printf(" FP");
  } else {*/
    switch (x->wordClass) {
      case CSGVar:
          if ((x->type == CSGautoType))
              printf(" _%s", x->name);
              //printf(" %s#%ld", x->name, x->val);
          else
              printf(" _%s[]", x->name);
              //printf(" %s_base#%ld", x->name, x->val);
          break;
      case CSGConst: printf(" %ld", x->val); break;
      //case CSGFld: printf(" %s_offset#%ld", x->name, x->val); break;
      case CSGInst: case CSGAddr: printf(" T%d", x->line); break;
      //case CSGProc: printf(" (%d)", x->truePort->line); break;
      default: printf(" unknown wordClass");
    }
  //}
}

void CSGDecode(CSGNode globscope)
{
  register CSGNode i, j;
  register int cnt;
  const char *comp=NULL;

  i = globscope;
  printf("global");
  while (i != NULL) {
    //printf("> %s(%d) ", i->name, i->wordClass);
    if (i->wordClass==CSGVar) {
      PrintNode(i);
      if (i->type != CSGautoType)
         printf("%ld", i->ext);
    }
    i = i -> next;
  }
  puts("");

  // assign line numbers
  //int selfEntry;
  cnt = 1;
  i = code;
  while (i != NULL) {
    if (/*i->op==iparam || i->op==icmpeq || i->op==icmple || i->op==icmplt*/i->op==ioffset || i->op==inop || i->op==ilocalvar)
      i->line = -1;
    else if (i->op==ientry)
      cnt = i->line = -1;
    else
      i->line = ++cnt;
    i = i->nxt;
  }


  i = code;
  while (i != NULL) {
    if (~i->line)
      printf("(%d) :", i->line);
    switch (i->op) {
      case inop: /*printf(" nop");*/ break;
      case iret: printf(" ret"); break;

      case ientry:
          printf("entry");
          j = i-> x;
          while (j -> wordClass != CSGProc) {
            printf(" _%s", j->name);
            j = j ->next;
          }
          printf(" | %s |", j->name);

          //selfEntry = i->line;
          if (i->nxt != NULL && i->nxt->op==ilocalvar) {
            j = i->nxt->x;
            while (j != NULL) {
              PrintNode(j);
              if (j->type != CSGautoType)
                 printf("%ld", j->ext);
              j=j->nxt;
            }
            i->nxt->line = i->line;
            i = i->nxt;
          }
          printf("\n");
          break;
      case icall:
          printf(" call %s |", i->x->name/*i->x->truePort->line ? i->x->truePort->line : selfEntry*/);
          j = (CSGNode)i->x->ext;
          while (j != NULL) {
          	PrintNode(j);
            j = j->next;
          }
          break;
      //case iparam: /*printf(" arg"); PrintNode(i->x);*/ break;
      /*case isetvalue: printf(" ans = "); PrintNode(i->x); break;
      case igetvalue:
          PrintNode(i->x);
          j=i->prv;
          while (j!=NULL) {
            if (j->op==icall)
              break;
            else if (j->op==ientrypc)
              j=NULL;
            else
              j=j->prv;
          }
          if (j!=NULL)
            printf(" = T%d", j->line);
          else
            printf(" = 0");
          break;*/

      case iload: printf(" T%d = =[] _%s", i->line, i->x->x->name);PrintNode(i->x->y); break;
      case istore: printf(" _%s =",i->y->x->name); printf(" []= _%s",i->y->x->name); PrintNode(i->y->y); PrintNode(i->x);  break;
      case ioffset: /*printf(" T%d = ", i->line); PrintNode(i->x); printf(" >> "); PrintNode(i->y);*/ break;;

      case imove: PrintNode(i->y); printf(" ="); PrintNode(i->x); break;
      case iadd: printf(" T%d = ", i->line); PrintNode(i->x); printf(" +"); PrintNode(i->y); break;
      case isub: printf(" T%d = ", i->line); PrintNode(i->x); printf(" -"); PrintNode(i->y); break;
      case imul: printf(" T%d = ", i->line); PrintNode(i->x); printf(" *"); PrintNode(i->y); break;
      case idiv: printf(" T%d = ", i->line); PrintNode(i->x); printf(" div"); PrintNode(i->y); break;
      case imod: printf(" T%d = ", i->line); PrintNode(i->x); printf(" rem"); PrintNode(i->y); break;
      case ineg: printf(" T%d = ", i->line); printf(" -"); PrintNode(i->x); break;
      
      //case icmpeq: comp=" ==";/*printf(" T%d = ", i->line); PrintNode(i->x); printf(" =="); PrintNode(i->y);*/ break;
      //case icmple: comp=" >";/*printf(" T%d = ", i->line); PrintNode(i->x); printf(" >"); PrintNode(i->y);*/ break;
      //case icmplt: comp=" <";/*printf(" T%d = ", i->line); PrintNode(i->x); printf(" <"); PrintNode(i->y);*/ break;

      case iblbc: printf(" ifnot"); PrintNode(i->x); printf("%s", (const char*)i->ext); PrintNode(i->dsc); printf(" goto"); PrintBrakNode(i->y); break;
      case iblbs: printf(" if"); PrintNode(i->x); printf("%s", (const char*)i->ext); PrintNode(i->dsc); printf(" goto"); PrintBrakNode(i->y); break;
      case ibr: printf(" goto"); PrintBrakNode(i->x); break;

      case iread: printf(" T%d = read", i->line); break;
      case iwrite: printf(" write"); PrintNode(i->x); break;
      case iwrl: printf(" writeln"); break;

      default: printf(" unknown instruction");
    }
    if (~i->line)
      printf("\n");
    i = i->nxt;
  }
}


void CSGInit(void)
{
  //entrypc = NULL;
  code = malloc(sizeof(CSGNodeDesc));
  code->wordClass = CSGInst;
  code->op = inop;
  code->prv = NULL;
  code->nxt = NULL;

  CSGautoType = malloc(sizeof(CSGTypeDesc));
  CSGautoType->form = CSGInteger;
  CSGautoType->size = 8;
  CSGboolType = malloc(sizeof(CSGTypeDesc));
  CSGboolType->form = CSGBoolean;
  CSGboolType->size = 8;

  //GP = malloc(sizeof(CSGNodeDesc));
  CSGautoType->form = CSGInteger;
  CSGautoType->size = 8;
  //FP = malloc(sizeof(CSGNodeDesc));
  CSGautoType->form = CSGInteger;
  CSGautoType->size = 8;
}
