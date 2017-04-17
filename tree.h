#ifndef TREE_H
#define TREE_H

#define MAXCHILDREN 12

typedef struct treenode tree; 

//tree node
struct treenode {
  int line;
  int nodeKind;
  int numChildren;
  int val;
  tree *parent;
  tree *children[MAXCHILDREN];
};

//make a new tree of the given type.
//returns a pointer to the new node.
tree *maketree(int kind);

//make a new tree of the given type
//that has the given value.
//returns a pointer to the new node.
tree *maketreeWithVal(int kind, int val);

//add the given child node to the given parent node.
void addChild(tree *parent, tree *child);

//debug function for printing data in the AST
//in a readable format.
void printAst(tree *root, int nestLevel);


enum nodeTypes {PROGRAM, DECLLIST, DECL,
		FORMALDECL, FDECLLIST,
		STMT, CMPNDSTMT, STMTLIST,
		ASSIGN, LOOP, CONDITIONAL,
		VAR, TYPESPEC, VARDECL, LDECLLIST,
		FUNCCALL, FUNCBODY, FUNCDECL,
		ARGLIST, RETURN,
		EXPR, ADDEXPR, TERM, FACTOR,
		OPER, IDENTIFIER,
		INTEGER, CHARACTER, STRING};


/* tree manipulation macros */ 

#define nextAvailChild(node) node->children[node->numChildren] 
#define getChild(node, index) node->children[index]
#endif
