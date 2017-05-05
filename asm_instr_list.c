#include <stdlib.h>
#include "asm_instr_list.h"

extern char* strTable[];

instr* instr_list = 0;
int current_instr = 0;
int instr_max = 0;

int sub_lbl_ctr = 0;

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


void gen_fn_output()
{
  add_instr(LABEL, GLOBL_OUTPUT,0,0);

  add_instr(PUSH, R7, LR, 0);
  add_instr(PUSH, R4, R5, 0);
  add_instr(PUSH, R2, R3, 0);
  add_instr(PUSH, R0, R1, 0);

  add_instr(CMP, R0, 0, 0);
  add_instr(BGT, SUB_LABEL, 0, 0);
  add_instr(BLT, SUB_LABEL, 1, 0);

  add_instr(MOV, R0, 48, 0);
  add_instr(STR, R0, SP, -6);
  add_instr(MOV, R0, 10, 0);
  add_instr(STR, R0, SP, -5);
  add_instr(SUB, R1, SP, 5);

  add_instr(MOV, R7, 4, 0);
  add_instr(MOV, R0, 1, 0);
  add_instr(MOV, R2, 2, 0);

  add_instr(SWI, 0,0,0);

  add_instr(B, SUB_LABEL, 5, 0);

  add_instr(MOV, R3, R0, 0);

  add_instr(POP, R0, R1, 0);
  add_instr(POP, R2, R3, 0);
  add_instr(POP, R4, R5, 0);
  add_instr(POP, R7, PC, 0);

  sub_lbl_ctr = 6;
}


void gen_header()
{
  add_instr(SEG_TEXT, 0,0,0);
  add_instr(GLOBL_LBL, GLOBL_ENTRY,0,0);

  add_instr(B, GLOBL_ENTRY,0,0);

  gen_fn_output();

  add_instr(LABEL, GLOBL_ENTRY,0,0);
  add_instr(BL, 1,0,0); //break to "main" function

  add_instr(MOV, REGISTER+7,1,0); //then call to exit
  add_instr(SWI, 0,0,0);
}


void gen_footer()
{
  add_instr(SEG_END, 0,0,0);
}


void output_register(FILE* file_out, int reg)
{
  if (reg > LITERAL_MAX)
  {
    if (reg == SP)
      fprintf(file_out, "sp");
    else if (reg == LR)
      fprintf(file_out, "lr");
    else if (reg == PC)
      fprintf(file_out, "pc");
    else if (reg == APSR)
      fprintf(file_out, "apsr");
    else
      fprintf(file_out, "r%d", reg-REGISTER);
  }
  else
    fprintf(file_out, "#%d", reg);
}


void output_label(FILE* file_out, int param1, int param2)
{
  if (param1 == GLOBL_ENTRY)
    fprintf(file_out, "_start");
  else if (param1 == GLOBL_I_DIV)
    fprintf(file_out, "_int_div");
  else if (param1 == SUB_LABEL)
    fprintf(file_out, ".l_%d", param2);
  else
    fprintf(file_out, "f_%s", strTable[param1]);
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
        output_label(file_out, param1, param2);
        fprintf(file_out, "\n");
      }
      else if (type == LABEL)
      {
        output_label(file_out, param1, param2);
        fprintf(file_out, ":\n");
      }
      else if (type == LABEL)
      {
        output_label(file_out, param1, param2);
        fprintf(file_out, ":\n");
      }

      else if (type == ADD)
      {
        fprintf(file_out, "add ");
        output_register(file_out, param1);

        fprintf(file_out, ", ");
        output_register(file_out, param2);

        fprintf(file_out, ", ");
        output_register(file_out, param3);

        fprintf(file_out, "\n");
      }

      else if ((type >= B) && (type <= BNE))
      {
        if (type == B)
          fprintf(file_out, "b ");
        else if (type == BL)
          fprintf(file_out, "bl ");
        else if (type == BX)
          fprintf(file_out, "bx ");
        else if (type == BEQ)
          fprintf(file_out, "beq ");
        else if (type == BGE)
          fprintf(file_out, "bge ");
        else if (type == BGT)
          fprintf(file_out, "bgt ");
        else if (type == BLE)
          fprintf(file_out, "ble ");
        else if (type == BLT)
          fprintf(file_out, "blt ");
        else //type == BNE
          fprintf(file_out, "bne ");

        output_label(file_out, param1, param2);
        fprintf(file_out, "\n");
      }

      else if (type == CMP)
      {
        fprintf(file_out, "cmp ");
        output_register(file_out, param1);

        fprintf(file_out, ", ");
        output_register(file_out, param2);

        fprintf(file_out, "\n");
      }

      else if (type == MOV)
      {
        fprintf(file_out, "mov ");
        output_register(file_out, param1);

        fprintf(file_out, ", ");
        output_register(file_out, param2);  

        fprintf(file_out, "\n");
      }

      else if (type == PUSH)
      {
        fprintf(file_out, "push {");
        output_register(file_out, param1);
        
        fprintf(file_out, ", ");
        output_register(file_out, param2); 

        fprintf(file_out, "}\n");
      }
      else if (type == POP)
      {
        fprintf(file_out, "pop {");
        output_register(file_out, param1);
        
        fprintf(file_out, ", ");
        output_register(file_out, param2); 

        fprintf(file_out, "}\n");
      }

      else if (type == STR)
      {
        fprintf(file_out, "str ");
        output_register(file_out, param1);

        fprintf(file_out, ", [");
        output_register(file_out, param2);

        fprintf(file_out, ", ");
        output_register(file_out, param3);

        fprintf(file_out, "]\n");
      }
      else if (type == SUB)
      {
        fprintf(file_out, "sub ");
        output_register(file_out, param1);

        fprintf(file_out, ", ");
        output_register(file_out, param2);

        fprintf(file_out, ", ");
        output_register(file_out, param3);

        fprintf(file_out, "\n");
      }
      else if (type == SWI)
      {
        fprintf(file_out, "swi 0\n");
      }

      else if (type != IGNORE)
      {
        fprintf(file_out, "ERROR\n");
      }
    }
  }
}
