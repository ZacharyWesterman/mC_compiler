#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "tree.h"

//Check the completed AST for semantic errors.
//Returns the number of errors found.
int checkSemantics(tree* syntaxTree);

#endif
