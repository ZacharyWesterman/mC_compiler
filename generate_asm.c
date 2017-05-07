#include "generate_asm.h"

#define OPER_ADD 266
#define OPER_SUB 267
#define OPER_MUL 268
#define OPER_DIV 269

#define OPER_LT  270
#define OPER_GT  271
#define OPER_LTE 272
#define OPER_GTE 273
#define OPER_EQ  274
#define OPER_NEQ 275

#define FUNC_INPUT -1

int scope;

int stack_offs[1000];
int curr_stack_offs;

void generate_asm(tree* ast)
{
  if (ast)
  {
    //Declaring variables
    if (ast->nodeKind == VARDECL)
    {
      //if the variable is local
      if (scope)
      {
        curr_stack_offs -= 4;
        stack_offs[ast->children[1]->val] = curr_stack_offs;
      }
      //if the variable is global
      else
      {
        add_data(LABEL, DATA_LABEL, ast->children[1]->val, 0);
        add_data(WORD, 0, 0, 0);
      }
    }
    //accessing a variable (assumed to be a single integer)
    else if (ast->nodeKind == VAR)
    {
      //if the variable is local
      if (scope)
      {
        int offs = stack_offs[ast->children[0]->val];

        if (offs == FUNC_INPUT)
          add_instr(MOV, R1, R3, 0);
        else
          add_instr(LDR, R1, SP, offs);
      }
      //if the variable is global
      else
      {
        add_instr(LDR, R1, DATA_LABEL, ast->children[0]->val);
        add_instr(LDR, R1, R1, 0);
      }
    }
    //assume function input is a single integer, stored in R0.
    else if (ast->nodeKind == FORMALDECL)
    {
      add_instr(MOV, R3, R0, 0);
      
      stack_offs[ast->children[1]->val] = FUNC_INPUT;
    }
    //calling a function. 
    //Function input is a single integer, stored in R0.
    //Function return value is in R0.
    else if (ast->nodeKind == FUNCCALL)
    {
      if (ast->numChildren > 0)
      {
        generate_asm(ast->children[1]);
        add_instr(MOV, R0, R1, 0);
      }

      add_instr(BL, ast->children[0]->val, 0,0);
      add_instr(MOV, R1, R0, 0);
    }
    //if we are assigning a value to a variable
    else if (ast->nodeKind == ASSIGN)
    {
      generate_asm(ast->children[1]);

      int offs = stack_offs[ast->children[0]->children[0]->val];

      if (offs == FUNC_INPUT)
        add_instr(MOV, R3, R1, 0);
      else
        add_instr(STR, R1, SP, offs);
    }
    //Integer literals are loaded to R1
    else if (ast->nodeKind == INTEGER)
    {
      if ((ast->val) > LITERAL_MAX)
      {
        if (!data_label_exists(CONST_LABEL, ast->val))
        {
          add_data(LABEL, CONST_LABEL, ast->val, 0);
          add_data(WORD, ast->val, 0, 0);
        }

        add_instr(LDR, R1, CONST_LABEL, ast->val);
        add_instr(LDR, R1, R1, 0);
      }
      else
      {
        add_instr(MOV, R1, ast->val, 0);
      }
    }
    //addition or subtraction evaluated on R1 (R2 modified)
    else if (ast->nodeKind == ADDEXPR)
    {
      generate_asm(ast->children[0]);
      add_instr(MOV, R2, R1, 0);
      generate_asm(ast->children[2]);

      if (ast->children[1]->val == OPER_ADD)
        add_instr(ADD, R1, R2, R1);
      else
        add_instr(SUB, R1, R2, R1);
    }
    //multiplication or division evaluated on R1 (R2 modified, R0 also if division)
    else if (ast->nodeKind == TERM)
    {
      generate_asm(ast->children[0]);
      add_instr(MOV, R2, R1, 0);
      generate_asm(ast->children[2]);

      if (ast->children[1]->val == OPER_MUL)
        add_instr(MUL, R1, R1, R2);
      else
      {
        add_instr(MOV, R0, R2, 0);
        add_instr(BL, GLOBL_I_DIV, 0,0);
        add_instr(MOV, R1, R0, 0);
      }
    }
    //relational operations evaluated on R1 (R2 modified)
    else if (ast->nodeKind == EXPR)
    {
      generate_asm(ast->children[0]);
      add_instr(MOV, R2, R1, 0);
      generate_asm(ast->children[2]);

      add_instr(CMP, R2, R1, 0);

      if (ast->children[1]->val == OPER_GT)
      {
        add_instr(MOVGT, R1, 1, 0);
        add_instr(MOVLE, R1, 0, 0);
      }
      else if (ast->children[1]->val == OPER_LT)
      {
        add_instr(MOVLT, R1, 1, 0);
        add_instr(MOVGE, R1, 0, 0);
      }
      else  if (ast->children[1]->val == OPER_GTE)
      {
        add_instr(MOVGE, R1, 1, 0);
        add_instr(MOVLT, R1, 0, 0);
      }
      else  if (ast->children[1]->val == OPER_LTE)
      {
        add_instr(MOVLE, R1, 1, 0);
        add_instr(MOVGT, R1, 0, 0);
      }
      else  if (ast->children[1]->val == OPER_EQ)
      {
        add_instr(MOVEQ, R1, 1, 0);
        add_instr(MOVNE, R1, 0, 0);
      }
      else //OPER_NEQ
      {
        add_instr(MOVNE, R1, 1, 0);
        add_instr(MOVEQ, R1, 0, 0);
      }
    }
    //Functions save all registers except R0 and R1.
    //The stack should be 8-byte aligned at all times.
    else if (ast->nodeKind == FUNCDECL)
    {
      add_instr(LABEL, ast->children[1]->val, 0, 0);
      
      int i;
      add_instr(PUSH, R2, LR, 0); 

      for (i=R3; i<R12; i+=2)
        add_instr(PUSH, i, i+1, 0);         

      curr_stack_offs = 0;
      scope++;
      generate_asm(ast->children[2]);
      generate_asm(ast->children[3]);
      scope--;

      for (i=R12; i>=R3; i-=2)
        add_instr(POP, i-1, i, 0); 

      add_instr(POP, R2, PC, 0); 
    }
    //RETURN statement. moves R1 into R0(return value)
    //By default, statement "return;" returns 0.
    else if (ast->nodeKind == RETURN)
    {
      if (ast->numChildren > 0)
      {
        generate_asm(ast->children[0]);
        add_instr(MOV, R0, R1, 0);
      }
      else
      {
        add_instr(MOV, R0, 0, 0);
      }
    }
    else
    {
      if (ast->nodeKind == PROGRAM)
        scope = 0;
   
      int i;
      for (i=0; i<(ast->numChildren); i++)
        generate_asm(ast->children[i]);
    }
  }
}
