#ifndef SYMTAB_H
#define SYMTAB_H

#include "func_signature.h"

#define MAX_SYMBOLS 100

#define MAX_CHILDREN 100

enum SCOPE
{
  GLOBAL,
  LOCAL
};

enum SYM_TYPES
{
  VOID,
  INT,
  CHAR,
  _STRING,

  VOID_ARR,
  INT_ARR,
  CHAR_ARR,
  STRING_ARR,

  TYPE_COUNT
};


typedef struct symbol sym;
typedef struct symbol_table symtab;


struct symbol
{
  int id_index;

  int type;
  int arr_size;

  int scope;
};


struct symbol_table
{
  symtab* parent;

  int sym_count;
  sym symbols[MAX_SYMBOLS];

  int children_count;
  symtab* children[MAX_CHILDREN];
};


//Make a new symbol table with the given parent.
//Returns a pointer to the new table.
symtab* make_table(symtab* parent);



//Search the given symbol table for an element with the given
//id, type, and scope. If this table doesn't contain it, search
//recursively through all parents.
//Returns the index if the symbol exists, returns -1 otherwise.
int SYM_lookup(symtab* table, int id_index, int type, int scope);

//insert a symbol into the table.
//returns 1 if a new symbol was inserted,
//        0 if the table is full or the
//	    symbol already exists.
int SYM_insert(symtab* table, int id_index, int type, int arr_size, int scope);



//create a new table whose parent is the given table.
//if no parent (i.e. global scope), call new_scope(0).
//returns pointer to the new table.
symtab* new_scope(symtab* table);

//leave the current scope if possible.
//if the given table has a parent, returns that parent.
//Otherwise, returns the given table.
//NOTE: does not delete the given table.
symtab* leave_scope(symtab* table);



//get the size of the array-like variable with the given id.
//returns the array size if the variable is array-like.
//returns -1 otherwise.
//NOTE: strings ARE array-like, but will always return 0, as they
//      do not have a fixed size.
int get_arr_size(symtab* table, int id_index);



//Debug function to print information about the given 
//symbol table and all children.
void print_symtab(symtab* table, int spacing);

#endif
