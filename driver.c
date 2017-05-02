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
  NONE = 	0x00,
  HELP = 	0x01,
  PRINT_AST = 	0x02,

  UNKNOWN = 	0x04
};


int read_cflags(int argc, char* argv[])
{
  int cflags_out = NONE;

  char buf[64];

  int i;
  for (i=1; i<argc; i++)
  {
    strcpy(buf, argv[i]);
  
    //we have a flag
    if (buf[0] == '-')
    {
      if (!strcmp(buf, "-h") || !strcmp(argv[i], "--help"))
        cflags_out |= HELP;
      if (!strcmp(buf, "-a") || !strcmp(argv[i], "--ast"))
        cflags_out |= PRINT_AST;
      else
        cflags_out |= UNKNOWN;


      //fprintf(stdout, "%s [%x]\n", buf, cflags_out);
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


void print_help(char* argv[])
{
  fprintf(stdout, "Usage: %s <flags> [input] <output>\n", argv[0]);
  fprintf(stdout, "Flags:\n");
  fprintf(stdout, "\t-h, --help\tDisplay this help text.\n");
  fprintf(stdout, "\t-a, --ast\tDisplay the abstract syntax tree.\n");
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

  if ((cflags & HELP) || (cflags & UNKNOWN))
  {
    print_help(argv);
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
      print_help(argv);
      return -1;
    }

    if (!yyparse())
    {
      fclose(input);


      if (cflags & PRINT_AST)
        printAst(ast, 1);


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
    }
  }

  return 0;
}
