#include "generate_asm.h"

#define OPER_ADD 266
#define OPER_MUL 268

int scope;

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

      }
      //if the variable is global
      else
      {
        add_data(LABEL, DATA_LABEL, ast->children[1]->val, 0);
        add_data(WORD, 0, 0, 0);
      }
    }
    //Integer literals are loaded to R0
    else if (ast->nodeKind == INTEGER)
    {
      if ((ast->val) > LITERAL_MAX)
      {
        if (!data_label_exists(CONST_LABEL, ast->val))
        {
          add_data(LABEL, CONST_LABEL, ast->val, 0);
          add_data(WORD, ast->val, 0, 0);
        }

        add_instr(LDR, R0, CONST_LABEL, ast->val);
        add_instr(LDR, R0, R0, 0);
      }
      else
      {
        add_instr(MOV, R0, ast->val, 0);
      }
    }
    //addition or subtraction
    else if (ast->nodeKind == ADDEXPR)
    {
      generate_asm(ast->children[0]);
      add_instr(MOV, R2, R0, 0);
      generate_asm(ast->children[2]);

      if (ast->children[1]->val == OPER_ADD)
        add_instr(ADD, R0, R2, R0);
      else
        add_instr(SUB, R0, R2, R0);
    }
    //multiplication or division
    else if (ast->nodeKind == TERM)
    {
      generate_asm(ast->children[0]);
      add_instr(MOV, R2, R0, 0);
      generate_asm(ast->children[2]);

      if (ast->children[1]->val == OPER_MUL)
        add_instr(MUL, R0, R0, R2);
      else
      {
        add_instr(MOV, R1, R0, 0);
        add_instr(MOV, R0, R2, 0);
        add_instr(BL, GLOBL_I_DIV, 0,0);
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

      scope++;
      generate_asm(ast->children[2]);
      scope--;

      for (i=R12; i>=R3; i-=2)
        add_instr(POP, i-1, i, 0); 

      add_instr(POP, R2, PC, 0); 
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
