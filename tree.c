#ifndef TREE_C
#define TREE_C

#include<tree.h>
#include<stdio.h>
#include<stdlib.h>

#include "strtab.h"

extern int yylineno;

/* string values for ast node types, makes tree output more readable */
char *nodeNames[] =    {"program", "decllist", "decl", 
			"formaldecl", "fdecllist", 
			"statement", "compndstmt", "stmtlist", 
			"assignment", "loop", "conditional",
			"variable", "type", "vardecl", "ldecllist", 
			"funccall","funcbody", "funcdecl", 
			"arglist", "return", 
			"expression", "addexpr", "term", "factor",
			"operator", "identifier",
			"intconst", "charconst", "stringconst"};

char *dataTypes[] = {"void", "int", "char", "string"};


//make a new tree of the given type.
//returns a pointer to the new node.
tree *maketree(int kind) {
  tree *this = (tree *) malloc(sizeof(struct treenode));

  this->line = yylineno;
  this->nodeKind = kind;
  this->numChildren = 0;
  return this;

}


//make a new tree of the given type
//that has the given value.
//returns a pointer to the new node.
tree *maketreeWithVal(int kind, int val) {
  tree *this = (tree *) malloc(sizeof(struct treenode));

  this->line = yylineno;
  this->nodeKind = kind;
  this->numChildren = 0;
  this->val = val;
  return this;

}

//add the given child node to the given parent node.
void addChild(tree *parent, tree *child) {
  if (parent->numChildren == MAXCHILDREN) {
    printf("Cannot add child to parent node\n");
    exit(1);
  }
  nextAvailChild(parent) = child;
  parent->numChildren++;
}


//debug function for printing data in the AST
//in a readable format.
void printAst(tree *node, int nestLevel) 
{
  if ((node->nodeKind == INTEGER) ||
      (node->nodeKind == CHARACTER))
  {
    printf("<%d>\n", node->val);
  }
  else if (node->nodeKind == STRING)
  {  
    printf("<%s>\n", strTable[node->val]);
  }
  else if (node->nodeKind == IDENTIFIER)
  {
    printf("%s '%s'\n", nodeNames[node->nodeKind], strTable[node->val]);
  }
  else
  {
    printf("%s\n", nodeNames[node->nodeKind]);
  }

  int i, j;

  for (i = 0; i < node->numChildren; i++)  {
    for (j = 0; j < nestLevel; j++) 
      printf("    ");
    printAst(getChild(node, i), nestLevel + 1);
  }
}

#endif
