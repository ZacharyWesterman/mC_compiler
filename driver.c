#include "tree.h"
#include "semantics.h"
#include "symtab.h"

#include <stdio.h>
#include <string.h>

extern tree* ast;
extern symtab* table;

extern int yyparse();
extern void yyset_in(FILE*);

extern int ST_insert(const char*);



//compiler flags
enum
{
  HELP = 1,
  PRINT_AST = 2,

  UNKNOWN = 4
};


int read_cflags(int argc, char* argv[])
{
  int cflags_out = 0;

  int i;
  for (i=1; i<argc; i++)
  {
    //we have a flag
    if (argv[i][0] == '-')
    {
      //long flag
      if (argv[i][1] == '-')
      {
        if (!strcmp(&argv[i][2], "help"))
          cflags_out |= HELP;
        else
          cflags_out |= UNKNOWN;
      }
      //short flag
      else
      {
        if (!strcmp(&argv[i][1], "h"))
          cflags_out |= HELP;
        else
          cflags_out |= UNKNOWN;
      }
    }
  }

  return cflags_out;
}


int get_in_param(int argc, char* argv[])
{
  int i;
  for (i=1; i<argc; i++)
    if (argv[i][0] != '-')
      return i;

  return 0;
}

int get_out_param(int argc, int in_param)
{
  if (in_param && (argc > (in_param + 1)))
    return (in_param + 1);

  return 0;
}


void print_help()
{
  fprintf(stdout, "Usage: ./mcc <flags> [input] <output>\n");
  fprintf(stdout, "Flags:\n\t-h, --help\tDisplay this help text.\n");
}


int main(int argc, char* argv[])
{
  int cflags;
  int in_param;
  int out_param;

  cflags = read_cflags(argc, argv);

  in_param = get_in_param(argc, argv);
  out_param = get_out_param(argc, in_param);


  //make symbol "output" defined (will be symbol 0)
  ST_insert("output");

  if ((cflags & HELP) || (cflags & UNKNOWN) || !cflags)
  {
    print_help();
  }
  else
  {
    FILE *input;
    FILE *output;

    if (in_param)
    {
      if (!(input = fopen(argv[in_param], "r")))
      {
          fprintf(stderr, "Error opening input file '%s'.\n", argv[in_param]);
          return -1;
      }

      yyset_in(input);

    }
    else
    {
      print_help();
      return -1;
    }

    if (!yyparse())
    {
      fclose(input);

      //printAst(ast, 1);

      if (checkSemantics(ast))
        return -1;

      //print_symtab(table, 1);


      if (out_param)
      {
        if (!(output = fopen(argv[out_param], "w")))
        {
          fprintf(stderr, "Error opening output file '%s'.\n", argv[out_param]);
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
    

    fclose(input);
  }

  return 0;
}
