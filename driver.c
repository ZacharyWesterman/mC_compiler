#include "tree.h"
#include "semantics.h"
#include "symtab.h"

#include <stdio.h>

extern tree* ast;
extern symtab* table;

extern int yyparse();
extern void yyset_in(FILE*);

int main(int argc, char* argv[])
{
  FILE *input;
  FILE *output;

  if (argc >= 2)
  {
    if (!(input = fopen(argv[1], "r")))
    {
        fprintf(stderr, "Error opening input file '%s'.\n", argv[1]);
        return -1;
    }

    yyset_in(input);

  }
  else
  {
    fprintf(stderr, "Error: No input file given.\n");
    return -1;
  }

  if (!yyparse())
  {
    fclose(input);

    //printAst(ast, 1);

    if (checkSemantics(ast))
      return -1;

    //print_symtab(table, 1);




    if (argc >= 3)
    {
      if (!(output = fopen(argv[2], "w")))
      {
        fprintf(stderr, "Error opening output file '%s'.\n", argv[2]);
        return -1;
      }
    }
    else
    {
      if (!(output = fopen("arm.s", "w")))
      {
        fprintf(stderr, "Error opening output file 'arm.s'.\n");
        return -1;
      }
    }

    fclose(output);
  }
  else
  {
    fclose(input);
  }



  return 0;
}
