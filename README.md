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
          optmize_asm.c   optimize_asm.h    
            driver.c
            makefile

Dependencies:

	flex
	yacc
	gcc
	make

To build, run make.

Flags 
```
-h, --help: Displays help text
-a, --ast: Displays abstract syntax tree
-o, --opt: Optimize generated assembly IR
-s, --sym-tab: Displays the symbol table
-l, --list-asm: Shows generated ARM assembly
-n, --no-output: No output created
-m, --makefile: Generates a MAKEFILE for the output
```

Optimizations 
```
Immediate rewrite optimization:
  If a value is loaded into a register and then
  immediately overwritten with an unrelated value, 
  the first load is removed from code gen.
```
