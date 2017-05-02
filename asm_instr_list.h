#ifndef ASM_INSTR_LIST
#define ASM_INSTR_LIST

#define INSTR_BUF_CT 1000

#define GEN_REG_COUNT 13

typedef struct instruction instr;

struct instruction
{
  int type;

  int param1;
  int param2;
  int param3;
};


void add_instr(int type, int param1, int param2, int param3);


//enumerate ARM instruction types
enum
{
  DATA_SEG,
  PROGRAM_SEG,
  END_PROGRAM,

  ALIGN_BYTES,  
  GLOBAL_DEF,

  MOVE_REG,
  WRITE_MEM,
  READ_MEM,

  STACK_PUSH,
  STACK_POP,

  CALL_FUNC,
  JUMP_UNCOND,
  JUMP_GT,
  JUMP_LT,
  JUMP_EQ,
  JUMP_GTE,
  JUMP_LTE,
  JUMP_NEQ,

  LABEL,

  ADD,
  SUB,
  MUL,

  INSTR_COUNT
};

#endif
