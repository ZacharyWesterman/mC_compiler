#ifndef ASM_INSTR_LIST
#define ASM_INSTR_LIST

#include <stdio.h>

#define INSTR_BUF_CT 1000

#define GEN_REG_COUNT 13

#define LITERAL_MAX 255
#define REGISTER 256

#define GLOBL_OUTPUT 0
#define GLOBL_ENTRY -1
#define GLOBL_I_DIV -2


typedef struct instruction instr;

struct instruction
{
  int type;

  int param1;
  int param2;
  int param3;
};


void add_instr(int type, int param1, int param2, int param3);
void gen_header();
void gen_footer();

void output_asm(FILE*);


//enumerate ARM instruction types
enum
{
  SEG_DATA,	//.data segment
  SEG_TEXT,	//.text (program) segment
  SEG_END,	//.end (end of program)

  GLOBL_LBL,	//define a label as global
  LABEL,	//insert a label

  BALIGN,	//byte-align (usu. 4)

  ADC,		//add with carry
  ADD,		//add
  AND,		//AND
  
  B,		//branch
  BIC,		//bit clear
  BL,		//branch with link
  BX,		//branch and exchange
  
  CDP,		//coprocessor data processing
  CMN,		//compare negative
  CMP,		//compare

  EOR,		//exclusive OR

  LDC,		//load coprocessor from memory
  LDM, 		//load multiple registers (pop)
  LDR,		//load register from memory

  MCR,		//move CPU register to coprocessor register
  MLA,		//multiply accumulate
  MOV,		//move register or constant
  MRC,		//move from coprocessor register to CPU register
  MRS,		//move PSR status/flags to register
  MUL,		//multiply
  MVN,		//move negative register

  ORR,		//OR

  RSB,		//reverse subtract
  RSC,		//reverse subtract with carry

  SBC,		//subtract with carry
  STC,		//store coprocessor register to memory
  STM,		//store multiple
  STR,		//store register to memory
  SUB,		//subtract
  SWI,		//software interrupt
  SWP,		//swap register with memory
  TEQ,		//test bitwise equality
  TST,		//test bits

  INSTR_COUNT
};

#endif
