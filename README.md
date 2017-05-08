This compiler generates ARM assembly (for the Raspberry Pi, specifically).

Zach Westerman, Ryan Haynes.

Files required to build compiler:

           scanner.l   parser.y
            strtab.c   strtab.h
              tree.c   tree.h
            symtab.c   symtab.h
	func_signature.c   func_signature.h
         semantics.c   semantics.h
	asm_instr_list.c   asm_instr_list.h
	  generate_asm.c   generate_asm.h
          optmize_asm.c optimize_asm.h    
            driver.c
            makefile

Dependencies:

	flex
	yacc
	gcc
	make

To build, run make.

