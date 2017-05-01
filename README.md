This compiler generates ARM assembly (for the Raspberry Pi, specifically).

Files required to build compiler:

       scanner.l   parser.y
        strtab.c   strtab.h
          tree.c   tree.h
        symtab.c   symtab.h
func_signature.c   func_signature.h
     semantics.h   semantics.c 

        driver.c
        makefile

Dependencies:

flex
yacc
gcc
make

To build, run make.

