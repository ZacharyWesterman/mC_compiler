#include <stdlib.h>
#include "asm_instr_list.h"

extern char* strTable[];

instr* instr_list = 0;
int current_instr = 0;
int instr_max = 0;

instr* data_list = 0;
int current_data = 0;
int data_max = 0;

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

    instr_list[current_instr].param[0] = param1;
    instr_list[current_instr].param[1] = param2;
    instr_list[current_instr].param[2] = param3;

    current_instr++;
  }
}


void set_instr(int type, int param1, int param2, int param3, int i)
{
  if ((i < instr_max) && (i >= 0))
  {
    instr_list[i].type = type;

    instr_list[i].param[0] = param1;
    instr_list[i].param[1] = param2;
    instr_list[i].param[2] = param3;
  }
}


void ins_instr(int type, int param1, int param2, int param3, int i)
{
  if (!instr_list)
  {
    instr_list = (instr*)malloc(sizeof(instr)*INSTR_BUF_CT);

    instr_max = INSTR_BUF_CT;
  }


  if ((i < instr_max) && (i >= 0))
  {
    add_instr(instr_list[current_instr-1].type,
              instr_list[current_instr-1].param1,
              instr_list[current_instr-1].param2,
              instr_list[current_instr-1].param3);

    int j;
    for (j=i; j<current_instr-1; j++)
    {
      set_instr(instr_list[j].type,
                instr_list[j].param1,
                instr_list[j].param2,
                instr_list[j].param3,
                j+1);
    }

    set_instr(instr_list[current_instr-1].type,
              instr_list[current_instr-1].param1,
              instr_list[current_instr-1].param2,
              instr_list[current_instr-1].param3,
              i);
  }
}


void add_data(int type, int param1, int param2, int param3)
{
  if (!data_list)
  {
    data_list = (instr*)malloc(sizeof(instr)*INSTR_BUF_CT);

    data_max = INSTR_BUF_CT;
  }


  if (current_data < data_max)
  {
    data_list[current_data].type = type;

    data_list[current_data].param[0] = param1;
    data_list[current_data].param[1] = param2;
    data_list[current_data].param[2] = param3;

    current_data++;
  }
}

int data_label_exists(int param1, int param2)
{
  int i;
  for (i=0; i<current_data; i++)
  {
    if ((data_list[i].type == LABEL) &&
        (data_list[i].param[0] == param1) &&
        (data_list[i].param[1] == param2))
      return 1;
  }

  return 0;
}


void gen_fn_output()
{
  //enter function
  add_instr(LABEL, GLOBL_OUTPUT,0,0);

  add_instr(PUSH, R7, LR, 0);
  add_instr(PUSH, R4, R5, 0);
  add_instr(PUSH, R2, R3, 0);
  add_instr(PUSH, R0, R1, 0);

  //compare R0 to #0
  add_instr(CMP, R0, 0, 0);
  add_instr(BGT, SUB_LABEL, 1, 0);
  add_instr(BLT, SUB_LABEL, 0, 0);

  //if R0=0, "0\n"
  add_instr(MOV, R0, 48, 0);
  add_instr(STR, R0, SP, -6);
  add_instr(MOV, R0, 10, 0);
  add_instr(STR, R0, SP, -5);
  add_instr(SUB, R1, SP, 6);

  //call to print
  add_instr(MOV, R7, 4, 0);
  add_instr(MOV, R0, 1, 0);
  add_instr(MOV, R2, 2, 0);

  add_instr(SWI, 0,0,0);

  //exit function
  add_instr(B, SUB_LABEL, 5, 0);

  //if R0 is negative
  add_instr(LABEL, SUB_LABEL, 0, 0);
  add_instr(MOV, R3, R0, 0);

  //move "-" into memory
  add_instr(MOV, R0, 45, 0);
  add_instr(STR, R0, SP, -5);
  add_instr(SUB, R1, SP, 5);

  //call to print
  add_instr(MOV, R7, 4, 0);
  add_instr(MOV, R0, 1, 0);
  add_instr(MOV, R2, 1, 0);

  add_instr(SWI, 0,0,0);

  //make R0 positive and continue
  add_instr(MOV, R0, 0, 0);
  add_instr(SUB, R0, R0, R3);
  
  //if R0 is positive
  add_instr(LABEL, SUB_LABEL, 1, 0);

  //counter
  add_instr(MOV, R4, 0, 0);

  //find appropriate place on stack to store temp data
  add_instr(SUB, R3, SP, 30);

  add_instr(B, SUB_LABEL, 3, 0); //go to loop test
  add_instr(LABEL, SUB_LABEL, 2, 0); //begin loop

  //divide number by 10
  add_instr(MOV, R1, 10, 0);
  add_instr(BL, GLOBL_I_DIV, 0, 0); 

  //convert remainder to character and store
  add_instr(ADD, R1, R1, 48);
  add_instr(STR, R1, R3, 0);

  //increment
  add_instr(ADD, R3, R3, 1);
  add_instr(ADD, R4, R4, 1);

  //loop test
  add_instr(LABEL, SUB_LABEL, 3, 0);
  add_instr(CMP, R0, 0, 0);
  add_instr(BGT, SUB_LABEL, 2, 0);

  //number is currently inverted.
  //prepare to rearrange chars.
  add_instr(MOV, R1, R3, 0);
  add_instr(SUB, R0, R3, 1);

  add_instr(ADD, R7, R3, R4);

  //inversion loop
  add_instr(LABEL, SUB_LABEL, 4, 0);
  add_instr(LDR, R2, R0, 0);
  add_instr(STR, R2, R1, 0);

  //increment
  add_instr(ADD, R1, R1, 1);
  add_instr(SUB, R0, R0, 1);

  //loop test
  add_instr(CMP, R7, R1, 0);
  add_instr(BGT, SUB_LABEL, 4, 0);

  //append newline to end
  add_instr(ADD, R4, R4, 1);
  add_instr(MOV, R0, 10, 0);
  add_instr(STR, R0, R7, 0);

  //call to print
  add_instr(MOV, R7, 4, 0);
  add_instr(MOV, R0, 1, 0);
  add_instr(MOV, R2, R4, 0);

  add_instr(MOV, R1, R3, 0);
  add_instr(SWI, 0,0,0);

  //leave function
  add_instr(LABEL, SUB_LABEL, 5, 0);

  add_instr(POP, R0, R1, 0);
  add_instr(POP, R2, R3, 0);
  add_instr(POP, R4, R5, 0);
  add_instr(POP, R7, PC, 0);
}


void gen_int_div()
{
  //enter function
  add_instr(LABEL, GLOBL_I_DIV, 0, 0);
  add_instr(PUSH, R2, LR, 0);
  
  add_instr(MOV, R2, 0, 0);

  add_instr(B, SUB_LABEL, 7, 0); //go to loop test
  add_instr(LABEL, SUB_LABEL, 6, 0); //begin loop

  add_instr(SUB, R0, R0, R1);
  add_instr(ADD, R2, R2, 1);

  //loop test
  add_instr(LABEL, SUB_LABEL, 7, 0);
  add_instr(CMP, R0, R1, 0);
  add_instr(BGE, SUB_LABEL, 6, 0);

  add_instr(MOV, R1, R0, 0);
  add_instr(MOV, R0, R2, 0);

  //leave function
  add_instr(POP, R2, PC, 0);
}


void gen_header()
{
  add_instr(SEG_TEXT, 0,0,0);
  add_instr(BALIGN, 4, 0, 0);
  add_instr(GLOBL, GLOBL_ENTRY,0,0);

  add_instr(B, GLOBL_ENTRY,0,0);

  gen_fn_output();
  gen_int_div();

  sub_lbl_ctr = 8;

  add_instr(LABEL, GLOBL_ENTRY,0,0);

  add_instr(BL, 1,0,0); //break to "main" function

  add_instr(MOV, R7, 1, 0); //then call to exit
  add_instr(SWI, 0,0,0);
}


void gen_footer()
{
  add_instr(SEG_DATA, 0,0,0);
  add_instr(BALIGN, 4, 0,0);
  
  int i;
  for(i=0; i<current_data; i++)
  {
    add_instr(data_list[i].type,
              data_list[i].param[0],
              data_list[i].param[1],
              data_list[i].param[2]);
  }

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
  else if (param1 == DATA_LABEL)
    fprintf(file_out, "d_%s", strTable[param2]);
  else if (param1 == CONST_LABEL)
    fprintf(file_out, "c_%d", param2); 
  else
    fprintf(file_out, "f_%s", strTable[param1]);
}

void output_pushpop(FILE* file_out, instr* _inst)
{
  fprintf(file_out, "{");
  output_register(file_out, _inst->param[0]);

  int i;
  for (i=1; (i<INSTR_MAX_PARAMS) && (_inst->param[i]); i++)
  {
    fprintf(file_out, ", ");
    output_register(file_out, _inst->param[i]);
  }

  fprintf(file_out, "}");
}


void output_str_ldr(FILE* file_out, instr* _inst)
{
  output_register(file_out, _inst->param[0]);

  if (_inst->param[1] < 0)
  {
    fprintf(file_out, ", =");
    output_label(file_out, _inst->param[1], _inst->param[2]);
  }
  else
  {
    fprintf(file_out, ", [");
   
    output_register(file_out, _inst->param[1]);

    if (_inst->param[2])
    {
      fprintf(file_out, ", ");
      output_register(file_out, _inst->param[2]);
    }

    fprintf(file_out, "]");
  }
}


void output_asm(FILE* file_out)
{
  if (instr_list)
  {
    int i;
    for (i=0; i<current_instr; i++)
    {
      int type = instr_list[i].type;

      int param1 = instr_list[i].param[0];
      int param2 = instr_list[i].param[1];
      int param3 = instr_list[i].param[2];

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

      else if (type == WORD)
      {
        fprintf(file_out, ".word %d\n", param1);
      }

      else if (type == GLOBL)
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

      else if ((type >= ADD) && (type <= ORR))
      {
        if (type == ADD)
          fprintf(file_out, "add ");
        else if (type == SUB)
          fprintf(file_out, "sub ");
        else if (type == MUL)
          fprintf(file_out, "mul ");
        else if (type == AND)
          fprintf(file_out, "and ");
        else if (type == BIC)
          fprintf(file_out, "bic ");
        else if (type == EOR)
          fprintf(file_out, "eor ");
        else if (type == ORR)
          fprintf(file_out, "orr ");

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
      else if (type == BALIGN)
      {
        fprintf(file_out, ".balign %d\n", param1);
      }

      else if (type == CMP)
      {
        fprintf(file_out, "cmp ");
        output_register(file_out, param1);

        fprintf(file_out, ", ");
        output_register(file_out, param2);

        fprintf(file_out, "\n");
      }

      else if ((type == LDR) || (type == STR))
      {
        if (type == LDR)
          fprintf(file_out, "ldr ");
        else if (type == STR)
          fprintf(file_out, "str ");

        output_str_ldr(file_out, &instr_list[i]);
        fprintf(file_out, "\n");
      }

      else if ((type >= MOV) && (type <= SWP))
      {
        if (type == MOV)
          fprintf(file_out, "mov ");
        else if (type == MOVEQ)
          fprintf(file_out, "moveq ");
        else if (type == MOVNE)
          fprintf(file_out, "movne ");
        else if (type == MOVLT)
          fprintf(file_out, "movlt ");
        else if (type == MOVGT)
          fprintf(file_out, "movgt ");
        else if (type == MOVLE)
          fprintf(file_out, "movle ");
        else if (type == MOVGE)
          fprintf(file_out, "movge ");
        else if (type == SWP)
          fprintf(file_out, "swp ");

        output_register(file_out, param1);

        fprintf(file_out, ", ");
        output_register(file_out, param2);  

        fprintf(file_out, "\n");
      }

      else if (type == PUSH)
      {
        fprintf(file_out, "push ");
        output_pushpop(file_out, &instr_list[i]);
        fprintf(file_out, "\n");
      }
      else if (type == POP)
      {
        fprintf(file_out, "pop ");
        output_pushpop(file_out, &instr_list[i]);
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
