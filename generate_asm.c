#include "generate_asm.h"


void generate_asm(tree* ast)
{
  if (ast)
  {
    //Integer literals are loaded to R0
    if (ast->nodeKind == INTEGER)
    {
      if ((ast->val) > LITERAL_MAX)
      {
        add_data(LABEL, CONST_LABEL, ast->val, 0);
        add_data(WORD, ast->val, 0, 0);

        add_instr(LDR, R0, CONST_LABEL, ast->val);
        add_instr(LDR, R0, R0, 0);
      }
      else
      {
        add_instr(MOV, R0, ast->val, 0);
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

      generate_asm(ast->children[2]);

      for (i=R12; i>=R3; i-=2)
        add_instr(POP, i-1, i, 0); 

      add_instr(POP, R2, PC, 0); 
    }
    else
    {
      int i;
      for (i=0; i<(ast->numChildren); i++)
        generate_asm(ast->children[i]);
    }
  }
}
