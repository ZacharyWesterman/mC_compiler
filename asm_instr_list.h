#ifndef ASM_INSTR_LIST
#define ASM_INSTR_LIST

#include <stdio.h>

#define INSTR_BUF_CT 1000

#define INSTR_MAX_PARAMS 16

#define GEN_REG_COUNT 13

#define LITERAL_MAX 255
#define REGISTER 256

#define GLOBL_OUTPUT 0
#define GLOBL_ENTRY -1
#define GLOBL_I_DIV -2
#define SUB_LABEL   -3
#define DATA_LABEL  -4
#define CONST_LABEL -5

#define SP (REGISTER+13)
#define LR (REGISTER+14)
#define PC (REGISTER+15)
#define APSR (REGISTER+16)

#define R0 (REGISTER)
#define R1 (REGISTER+1)
#define R2 (REGISTER+2)
#define R3 (REGISTER+3)
#define R4 (REGISTER+4)
#define R5 (REGISTER+5)
#define R6 (REGISTER+6)
#define R7 (REGISTER+7)
#define R8 (REGISTER+8)
#define R9 (REGISTER+9)
#define R10 (REGISTER+10)
#define R11 (REGISTER+11)
#define R12 (REGISTER+12)


typedef struct instruction instr;

struct instruction
{
  int type;

  int param[INSTR_MAX_PARAMS];
};


void add_instr(int type, int param1, int param2, int param3);
void gen_header();
void gen_footer();

void add_data(int type, int param1, int param2, int param3);

void output_asm(FILE*);


//enumerate ARM instruction types
enum
{
  IGNORE,  

  SEG_DATA,	//.data segment
  SEG_TEXT,	//.text (program) segment
  SEG_END,	//.end (end of program)

  WORD,		//store global word (.word)

  GLOBL_LBL,	//define a label as global
  LABEL,	//insert a label
  SUB_LBL,	//unique labels for loops, etc.

  BALIGN,	//byte-align (usu. 4)

  ADC,		//add with carry
  ADD,		//add
  AND,		//AND
  
  BIC,		//bit clear
  B,		//branch
  BL,		//branch with link
  BX,		//branch and exchange
  BEQ,		//branch if equal
  BGE,		//branch if greater or equal
  BGT,		//branch if greater than
  BLE,		//branch if less or equal
  BLT,		//branch if less than
  BNE,		//branch if not equal
  
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

  PUSH,		//push to stack
  POP,		//pop from stack

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
