#include <stdlib.h>
#include "asm_instr_list.h"

extern char* strTable[];

instr* instr_list = 0;
int current_instr = 0;
int instr_max;

void add_instr(int type, int param1, int param2, int param3)
{
  if (!instr_list)
  {
    instr_list = (instr*)malloc(sizeof(instr)*INSTR_BUF_CT);

    instr_max = INSTR_BUF_CT;
  }


  if (current_instr < instr_max)
  {
    instr_list[current_instr].type = type;

    instr_list[current_instr].param1 = param1;
    instr_list[current_instr].param2 = param2;
    instr_list[current_instr].param3 = param3;

    current_instr++;
  }
}


void gen_header()
{
  add_instr(SEG_TEXT, 0,0,0);
  add_instr(GLOBL_LBL, GLOBL_ENTRY,0,0);

  add_instr(B, GLOBL_ENTRY,0,0);

  add_instr(LABEL, GLOBL_ENTRY,0,0);
  add_instr(BL, 1,0,0); //break to "main" function

  add_instr(MOV, REGISTER+7,1,0); //then call to exit
  add_instr(SWI, 0,0,0);
}


void gen_footer()
{
  add_instr(SEG_END, 0,0,0);
}


void output_asm(FILE* file_out)
{
  if (instr_list)
  {
    int i;
    for (i=0; i<current_instr; i++)
    {
      int type = instr_list[i].type;

      int param1 = instr_list[i].param1;
      int param2 = instr_list[i].param2;
      int param3 = instr_list[i].param3;

      //print instructions to the output
      if (type == SEG_DATA)
      {
        fprintf(file_out, ".data\n");
      }
      else if (type == SEG_TEXT)
      {
        fprintf(file_out, ".text\n");
      }
      else if (type == SEG_END)
      {
        fprintf(file_out, ".end\n\n");
      }

      else if (type == GLOBL_LBL)
      {
        fprintf(file_out, ".global ");

        if (param1 == GLOBL_ENTRY)
          fprintf(file_out, "_start");
        else if (param1 == GLOBL_I_DIV)
          fprintf(file_out, "_int_div");
        else
          fprintf(file_out, "fn_%s", strTable[param1]);

        fprintf(file_out, "\n");
      }
      else if (type == LABEL)
      {
        if (param1 == GLOBL_ENTRY)
          fprintf(file_out, "_start");
        else if (param1 == GLOBL_I_DIV)
          fprintf(file_out, "_int_div");
        else
          fprintf(file_out, "fn_%s", strTable[param1]);

        fprintf(file_out, ":\n");
      }

      else if (type == B)
      {
        fprintf(file_out, "b ");

        if (param1 == GLOBL_ENTRY)
          fprintf(file_out, "_start");
        else if (param1 == GLOBL_I_DIV)
          fprintf(file_out, "_int_div");
        else
          fprintf(file_out, "fn_%s", strTable[param1]);

        fprintf(file_out, "\n");
      }
      else if (type == BL)
      {
        fprintf(file_out, "bl ");

        if (param1 == GLOBL_ENTRY)
          fprintf(file_out, "_start");
        else if (param1 == GLOBL_I_DIV)
          fprintf(file_out, "_int_div");
        else
          fprintf(file_out, "fn_%s", strTable[param1]);

        fprintf(file_out, "\n");
      }
      else if (type == BX)
      {
        fprintf(file_out, "bx ");

        if (param1 == GLOBL_ENTRY)
          fprintf(file_out, "_start");
        else if (param1 == GLOBL_I_DIV)
          fprintf(file_out, "_int_div");
        else
          fprintf(file_out, "fn_%s", strTable[param1]);

        fprintf(file_out, "\n");
      }

      else if (type == MOV)
      {
        fprintf(file_out, "mov ");

        if (param1 > LITERAL_MAX)
          fprintf(file_out, "r%d, ", param1-REGISTER);
        else
          fprintf(file_out, "ERROR, ");
        
        if (param2 > LITERAL_MAX)
          fprintf(file_out, "r%d", param2-REGISTER);
        else
          fprintf(file_out, "#%d", param2);

        fprintf(file_out, "\n");
      }

      else if (type == SWI)
      {
        fprintf(file_out, "swi 0\n");
      }

      else
      {
        fprintf(file_out, "ERROR\n");
      }
    }
  }
}
