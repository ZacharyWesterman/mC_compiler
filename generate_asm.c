#include "generate_asm.h"


void generate_asm(tree* ast)
{
  if (ast)
  {
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
    else
    {
      int i;
      for (i=0; i<(ast->numChildren); i++)
        generate_asm(ast->children[i]);
    }
  }
}
