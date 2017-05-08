#include "asm_instr_list.h"
#include "optimize_asm.h"

extern instr* instr_list;
extern int current_instr;


void OPT_all()
{
  OPT_imm_rewrite();
}


//remove register writes if they are immediately overwritten.
void OPT_imm_rewrite()
{
  int l_check;
  int r_check;

  int i;
  for (i=0; i<current_instr-1; i++)
  {
    l_check = 0;
    r_check = 0;

    if ((instr_list[i].type == MOV) ||
        (instr_list[i].type == LDR))
      l_check = 1;
    else if ((instr_list[i].type == ADD) ||
             (instr_list[i].type == SUB) || 
             (instr_list[i].type == MUL))
      l_check = 1;

    
    if ((instr_list[i+1].type == MOV) ||
        (instr_list[i+1].type == LDR))
      r_check = 2;
    else if ((instr_list[i+1].type == ADD) ||
             (instr_list[i+1].type == SUB) || 
             (instr_list[i+1].type == MUL))
      r_check = 3;


    if (l_check && r_check)
    {
      if ((instr_list[i+1].param[0] == instr_list[i].param[0]) &&
          (instr_list[i+1].param[1] != instr_list[i].param[0]))
      {
        if (r_check == 3)
        {
          if (instr_list[i+1].param[2] != instr_list[i].param[0])
            rmv_instr(i);
        }
        else
          rmv_instr(i);
      }
    }
        
  }
}
