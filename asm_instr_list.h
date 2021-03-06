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
void ins_instr(int type, int param1, int param2, int param3, int i);
void set_instr(int type, int param1, int param2, int param3, int i);
void rmv_instr(int i);

void gen_header();
void gen_footer();

void add_data(int type, int param1, int param2, int param3);
int data_label_exists(int param1, int param2);

void output_asm(FILE*);


//enumerate ARM instruction types
enum
{
  IGNORE,  

  SEG_DATA,	//.data segment
  SEG_TEXT,	//.text (program) segment
  SEG_END,	//.end (end of program)

  WORD,		//store global word (.word)

  GLOBL,	//define a label as global
  LABEL,	//insert a label

  BALIGN,	//byte-align (usu. 4)

  //ADC,		//add with carry
  ADD,		//add
  SUB,		//subtract
  MUL,		//multiply
  AND,		//bitwise AND
  BIC,		//bit clear [dest = op1 AND (NOT op2)]
  EOR,		//bitwise exclusive OR
  ORR,		//bitwise OR

  B,		//branch
  BL,		//branch with link
  BX,		//branch and exchange
  BEQ,		//branch if equal
  BGE,		//branch if greater or equal
  BGT,		//branch if greater than
  BLE,		//branch if less or equal
  BLT,		//branch if less than
  BNE,		//branch if not equal
  
  //CDP,		//coprocessor data processing
  //CMN,		//compare negative
  CMP,		//compare

  //LDC,		//load coprocessor from memory
  //LDM, 		//load multiple registers (pop)
  LDR,		//load register from memory
  STR,		//store register to memory

  //MCR,		//move CPU register to coprocessor register
  //MLA,		//multiply accumulate (
  //MRC,		//move from coprocessor register to CPU register
  //MRS,		//move PSR status/flags to register
  //MVN,		//move negative register
  MOV,		//move register or constant
  MOVEQ,	//move if equal
  MOVNE,	//move if not equal
  MOVLT,	//move if less than
  MOVGT,	//move if greater than
  MOVLE,
  MOVGE,

  SWP,		//swap registers

  PUSH,		//push to stack
  POP,		//pop from stack

  //RSB,		//reverse subtract
  //RSC,		//reverse subtract with carry

  //SBC,		//subtract with carry
  //STC,		//store coprocessor register to memory
  //STM,		//store multiple registers (push)
  SWI,		//software interrupt

  TEQ,		//test bitwise equality
  TST,		//test bits

  INSTR_COUNT
};

#endif
