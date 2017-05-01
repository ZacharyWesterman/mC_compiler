%{
#include<stdio.h>
#include<tree.h>
#include "y.tab.h"

int yylineno;

extern int yylex();
  
enum dataType {T_VOID, T_INT, T_CHAR, T_STRING,
               P_VOID, P_INT, P_CHAR, P_STRING};


tree *ast;  /* pointer to AST root */

%}

%union 
{
  int value;
  struct treenode *node;
  char *strval;
}

%token <value> KWD_IF KWD_ELSE
%token <value> LPAREN RPAREN
%token <value> LSQ_BRKT RSQ_BRKT 
%token <value> LCRLY_BRKT RCRLY_BRKT 
%token <value> OPER_ASGN 
%token <value> OPER_ADD OPER_SUB
%token <value> OPER_MUL OPER_DIV
%token <value> OPER_LT OPER_GT OPER_LTE OPER_GTE
%token <value> OPER_EQ OPER_NEQ 
%token <value> COMMA SEMICLN
%token <value> KWD_VOID KWD_INT KWD_CHAR KWD_STRING
%token <value> KWD_RETURN
%token <value> KWD_WHILE
%token <value> ID
%token <value> INTCONST CHARCONST STRCONST
%token <value> ERROR
%token <value> ILLEGAL_TOK

%nonassoc NOT_ELSE
%nonassoc KWD_ELSE

%type <node> program declList decl
%type <node> formalDecl fDeclList
%type <node> statement compndStmt stmtList
%type <node> assignStmt loopStmt condStmt
%type <node> var typespec varDecl lDeclList
%type <node> funcCall funcBody funcDecl
%type <node> argList returnStmt
%type <node> expression addExpr term factor 
%type <node> relop addop mulop

%start program 

%%

program	: declList
		  {
		    tree *progNode = maketree(PROGRAM);
		    addChild(progNode, $1);
		    ast = progNode;
		  }
		;


declList	: decl
		  {
		    $$ = $1;
		  }
		| declList decl
		  {
		    tree *declList = maketree(DECLLIST);
		    addChild(declList, $1);
		    addChild(declList, $2);
		    $$ = declList;
		  }
		;


decl		: varDecl
		  {
		    $$ = $1;
		  }
		| funcDecl
		  {
		    $$ = $1;
		  }
		;


varDecl	: typespec ID LSQ_BRKT INTCONST RSQ_BRKT SEMICLN
		  {
		    tree *declNode = maketree(VARDECL);
		
		    $1->val = $1->val + 4; //we have an array variable

		    addChild(declNode, $1);
		    addChild(declNode, maketreeWithVal(IDENTIFIER, $2));
		    addChild(declNode, maketreeWithVal(INTEGER, $4));
		    $$ = declNode;
		  }
		| typespec ID SEMICLN
		  {
		    tree *declNode = maketree(VARDECL);
		    addChild(declNode, $1);
		    addChild(declNode, maketreeWithVal(IDENTIFIER, $2));
		    $$ = declNode;
		  }
		;


typespec	: KWD_VOID
		  {
		    $$ = maketreeWithVal(TYPESPEC, T_VOID);
		  }
		| KWD_INT
		  {
		    $$ = maketreeWithVal(TYPESPEC, T_INT);
		  }
		| KWD_CHAR
		  {
		    $$ = maketreeWithVal(TYPESPEC, T_CHAR);
		  }
		| KWD_STRING
		  {
		    $$ = maketreeWithVal(TYPESPEC, T_STRING);
		  }
		;


funcDecl	: typespec ID LPAREN fDeclList RPAREN funcBody 
		  {
		    tree *funcNode = maketree(FUNCDECL);
		    addChild(funcNode, $1);
		    addChild(funcNode, maketreeWithVal(IDENTIFIER, $2));
		    addChild(funcNode, $4);
		    addChild(funcNode, $6);
		    $$ = funcNode;
		  }
		| typespec ID LPAREN RPAREN funcBody
		  {
		    tree *funcNode = maketree(FUNCDECL);
		    addChild(funcNode, $1);
		    addChild(funcNode, maketreeWithVal(IDENTIFIER, $2));
		    addChild(funcNode, $5);
		    $$ = funcNode;
		  }
		;


fDeclList	: formalDecl
		  {
		    $$ = $1;
		  }
		| formalDecl COMMA fDeclList
		  {
		    tree *declNode = maketree(FDECLLIST);
		    addChild(declNode, $1);
		    addChild(declNode, $3);
		    $$ = declNode;
		  }
		;


formalDecl	: typespec ID
		  {
		    tree *declNode = maketree(FORMALDECL);
		    addChild(declNode, $1);
		    addChild(declNode, maketreeWithVal(IDENTIFIER, $2));
		    $$ = declNode;
		  }
		| typespec ID LSQ_BRKT RSQ_BRKT
		  {
		    tree *declNode = maketree(FORMALDECL);

		    $1->val = $1->val + 4; //we have an array declaration

		    addChild(declNode, $1);
		    addChild(declNode, maketreeWithVal(IDENTIFIER, $2));
		    $$ = declNode;
		  }
		;


funcBody	: LCRLY_BRKT lDeclList stmtList RCRLY_BRKT
		  {
		    tree *funcNode = maketree(FUNCBODY);
		    addChild(funcNode, $2);
		    addChild(funcNode, $3);
		    $$ = funcNode;
		  }
		;


lDeclList	:
		  {
		    $$ = maketree(LDECLLIST);
		  }
		| varDecl lDeclList
		  {
		    tree *declNode = maketree(LDECLLIST);
		    addChild(declNode, $1);
		    addChild(declNode, $2);
		    $$ = declNode;
		  }
		;


stmtList	:
		  {
		    $$ = maketree(STMTLIST);
		  }
		| statement stmtList
		  {
		    tree *stmtNode = maketree(STMTLIST);
		    addChild(stmtNode, $1);
		    addChild(stmtNode, $2);
		    $$ = stmtNode;
		  }
		;


statement	: compndStmt
		  {
		    $$ = $1;
		  }
		| assignStmt
		  {
		    $$ = $1;
		  }
		| condStmt
		  {
		    $$ = $1;
		  }
		| loopStmt
		  {
		    $$ = $1;
		  }
		| returnStmt
		  {
		    $$ = $1;
		  }
		;


compndStmt	: LCRLY_BRKT stmtList RCRLY_BRKT
		  {
		    $$ = $2;
		  }
		;


assignStmt	: var OPER_ASGN expression SEMICLN
		  {
		    tree *asgnNode = maketree(ASSIGN);
		    addChild(asgnNode, $1);
		    addChild(asgnNode, $3);
		    $$ = asgnNode;
		  }
		| expression SEMICLN
		  {
		    $$ = $1;
		  }
		;


condStmt	: KWD_IF LPAREN expression RPAREN statement	%prec NOT_ELSE
		  {
		    tree *condNode = maketree(CONDITIONAL);
		    addChild(condNode, $3);
		    addChild(condNode, $5);
		    $$ = condNode;
		  }
		| KWD_IF LPAREN expression RPAREN statement KWD_ELSE statement
		  {
		    tree *condNode = maketree(CONDITIONAL);
		    addChild(condNode, $3);
		    addChild(condNode, $5);
		    addChild(condNode, $7);
		    $$ = condNode;
		  }
		;


loopStmt	: KWD_WHILE LPAREN expression RPAREN statement
		  {
		    tree *loopNode = maketree(LOOP);
		    addChild(loopNode, $3);
		    addChild(loopNode, $5);
		    $$ = loopNode;
		  }
		;


returnStmt	: KWD_RETURN SEMICLN
		  {
		    tree *retNode = maketree(RETURN);
		    $$ = retNode;
		  }
		| KWD_RETURN expression SEMICLN
		  {
		    tree *retNode = maketree(RETURN);
		    addChild(retNode, $2);
		    $$ = retNode;
		  }
		;


var		: ID
		  {
		    tree *varNode = maketree(VAR);
		    addChild(varNode, maketreeWithVal(IDENTIFIER, $1));
		    $$ = varNode;
		  }
		| ID LSQ_BRKT addExpr RSQ_BRKT
		  {
		    tree *varNode = maketree(VAR);
		    addChild(varNode, maketreeWithVal(IDENTIFIER, $1));
		    addChild(varNode, $3);
		    $$ = varNode;
		  }
		;


expression	: addExpr
		  {
		    $$ = $1;
		  }
		| expression relop addExpr
		  {
		    tree *constNode = maketree(EXPR);
		    addChild(constNode, $1);
		    addChild(constNode, $2);
		    addChild(constNode, $3);
		    $$ = constNode;
		  }
		;


relop		: OPER_GTE
		  {
		    $$ = maketreeWithVal(OPER, OPER_GTE);
		  }
		| OPER_GT
		  {
		    $$ = maketreeWithVal(OPER, OPER_GT);
		  }
		| OPER_LTE
		  {
		    $$ = maketreeWithVal(OPER, OPER_LTE);
		  }
		| OPER_LT
		  {
		    $$ = maketreeWithVal(OPER, OPER_LT);
		  }
		| OPER_EQ
		  {
		    $$ = maketreeWithVal(OPER, OPER_EQ);
		  }
		| OPER_NEQ
		  {
		    $$ = maketreeWithVal(OPER, OPER_NEQ);
		  }
		;


addExpr	: term
		  {
		    $$ = $1;
		  }
		| addExpr addop term
		  {
		    tree *constNode = maketree(ADDEXPR);
		    addChild(constNode, $1);
		    addChild(constNode, $2);
		    addChild(constNode, $3);
		    $$ = constNode;
		  }
		;


addop		: OPER_ADD
		  {
		    $$ = maketreeWithVal(OPER, OPER_ADD);
		  }
		| OPER_SUB
		  {
		    $$ = maketreeWithVal(OPER, OPER_SUB);
		  }
		;


term		: factor
		  {
		    $$ = $1;
		  }
		| term mulop factor
		  {
		    tree *constNode = maketree(TERM);
		    addChild(constNode, $1);
		    addChild(constNode, $2);
		    addChild(constNode, $3);
		    $$ = constNode;
		  }
		;


mulop		: OPER_MUL
		  {
		    $$ = maketreeWithVal(OPER, OPER_MUL);
		  }
		| OPER_DIV
		  {
		    $$ = maketreeWithVal(OPER, OPER_DIV);
		  }
		;


factor	: LPAREN expression RPAREN
		  {
		    $$ = $2;
		  }
		| var
		  {
		    $$ = $1;
		  }
		| funcCall
		  {
		    $$ = $1;
		  }
		| INTCONST
		  {
		    $$ = maketreeWithVal(INTEGER, $1);
		  }
		| CHARCONST
		  {
		    $$ = maketreeWithVal(CHARACTER, $1);
		  }
		| STRCONST
		  {
		    $$ = maketreeWithVal(STRING, $1);
		  }
		;


funcCall	: ID LPAREN argList RPAREN
		  {
		    tree *funcNode = maketree(FUNCCALL);
		    addChild(funcNode, maketreeWithVal(IDENTIFIER, $1));
		    addChild(funcNode, $3);
		    $$ = funcNode;
		  }
		| ID LPAREN RPAREN
		  {
		    tree *funcNode = maketree(FUNCCALL);
		    addChild(funcNode, maketreeWithVal(IDENTIFIER, $1));
		    $$ = funcNode;
		  }
		;


argList	: expression
		  {
		    tree *argNode = maketree(ARGLIST);
		    addChild(argNode, $1);
		    $$ = argNode;
		  }
		| argList COMMA expression
		  {
		    tree *argNode = maketree(ARGLIST);
		    addChild(argNode, $1);
		    addChild(argNode, $3);
		    $$ = argNode;
		  }
		;


%%


int yyerror(char * msg) {
  printf("error: %d: %s\n", yylineno, msg);
  return 0;
}
