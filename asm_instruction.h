#ifdef ASM_INSTRUCTION_H
#define ASM_INSTRUCTION_H

#define ASM_MAX_PARAMS 3

#define GEN_REG_COUNT 13


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

  INSTR_COUNT
}


struct instruction
{
  int type;

  int param[ASM_MAX_PARAMS];
};

#endif
