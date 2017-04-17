#include "tree.h"
#include "semantics.h"
#include "symtab.h"

extern tree* ast;
extern symtab* table;

extern int yyparse();

int main() {
  if (!yyparse())
  {
    //printAst(ast, 1);

    checkSemantics(ast);

    //print_symtab(table, 1);
  }
  
  return 0;
}
