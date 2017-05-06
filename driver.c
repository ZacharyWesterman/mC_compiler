#include "tree.h"
#include "semantics.h"
#include "symtab.h"

#include "generate_asm.h"

#include <stdio.h>
#include <string.h>

extern tree* ast;
extern symtab* table;

extern int yyparse();
extern void yyset_in(FILE*);

extern int ST_insert(const char*);

extern instr* instr_list;


//compiler flags
enum
{
  NONE = 	0x00,
  HELP = 	0x01,
  PRINT_AST = 	0x02,

  LIST_ASM = 	0x04,
  NO_OUTPUT = 	0x08,

  GEN_MAKEFILE =0x10,

  UNKNOWN = 	0x20
};


int read_cflags(int argc, char* argv[]);

int get_in_param(int argc, char* argv[]);

int get_out_param(int argc, char* argv[], int in_param);

void print_help(char* argv[]);

void create_makefile(const char* finput);


//MAIN
int main(int argc, char* argv[])
{
  int cflags;
  int in_param;
  int out_param;

  cflags = read_cflags(argc, argv);

  in_param = get_in_param(argc, argv);
  out_param = get_out_param(argc, argv, in_param);


  //make symbol "output" defined (will be symbol 0)
  ST_insert("output");
  //function "main" will be the entry point (symbol 1)
  ST_insert("main");

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

      char foutbuf[64];
      foutbuf[0] = 0;

      if (!(cflags & NO_OUTPUT))
      {
        if (out_param)
        {
          if (!(output = fopen(argv[out_param], "w")))
          {
            fprintf(stderr, "Error opening output file '%s'.\n", argv[out_param]);
            return -1;
          }

          strcpy(foutbuf, argv[out_param]);
        }
        else
        {
          if (!(output = fopen("arm.s", "w")))
          {
            fprintf(stderr, "Error opening output file 'arm.s'.\n");
            return -1;
          }

          strcpy(foutbuf, "arm.s");
        }
      }


      //if we get to this point, we can generate asm code.

      if ((cflags & GEN_MAKEFILE) && !(cflags & NO_OUTPUT))
        create_makefile(foutbuf);


      //assembly header
      gen_header();

      //generate ARM assembly from AST
      generate_asm(ast);

      //assembly footer
      gen_footer();

      //output assembly
      if (cflags & LIST_ASM)
        output_asm(stdout);      

      if (!(cflags & NO_OUTPUT))
        output_asm(output);
    }
  }

  return 0;
}
//End of MAIN




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
      else if (!strcmp(buf, "-a") || !strcmp(argv[i], "--ast"))
        cflags_out |= PRINT_AST;
      else if (!strcmp(buf, "-l") || !strcmp(argv[i], "--list-asm"))
        cflags_out |= LIST_ASM;
      else if (!strcmp(buf, "-n") || !strcmp(argv[i], "--no-output"))
        cflags_out |= NO_OUTPUT;
      else if (!strcmp(buf, "-m") || !strcmp(argv[i], "--makefile"))
        cflags_out |= GEN_MAKEFILE;
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

int get_out_param(int argc, char* argv[], int in_param)
{
  int i;
  for (i=in_param+1; i<argc; i++)
    if (argv[i][0] != '-')
      return i;

  return 0;
}


void print_help(char* argv[])
{
  fprintf(stdout, "Usage: %s <flags> [input] <output>\n", argv[0]);
  fprintf(stdout, "Flags:\n");
  fprintf(stdout, "\t-h, --help\tDisplay this help text.\n");
  fprintf(stdout, "\t-a, --ast\tDisplay the abstract syntax tree.\n");
  fprintf(stdout, "\t-l, --list-asm\tShow generated ARM assembly.\n");
  fprintf(stdout, "\t-n, --no-output\tDo not create an output file.\n");
  fprintf(stdout, "\t-m, --makefile\tGenerate a makefile for the output.\n");
}


void create_makefile(const char* finput)
{
  FILE* makefile = fopen("Makefile", "w");

  if (makefile)
  {
    char buf[64];
    strcpy(buf, finput);

    int length = 0;
    while (buf[length] != 0)
      length++;

    while (buf[length] != '.')
      length--;

    buf[length] = 0;


    fprintf(makefile, "%s: %s.o\n\tld -o %s %s.o\n\n", buf, buf, buf, buf);
    fprintf(makefile, "%s.o: %s\n\tas -o %s.o %s\n\n", buf, finput, buf, finput);
    fprintf(makefile, "clean:\n\trm -vf %s.o %s", buf, buf);

    fclose(makefile);
  }
}
