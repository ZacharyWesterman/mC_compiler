#include <stdlib.h>
#include <stdio.h>
#include "symtab.h"

//There are 8 regular dataTypes. Anything above that is a function signature.
#define diff_types(a, b) ( ((a < 8) && (b >= 8)) || ((a >= 8) && (b < 8)) )

extern char* typeStr[];
extern char* strTable[];

//Make a new symbol table with the given parent.
//Returns a pointer to the new table.
symtab* make_table(symtab* parent)
{
  symtab* this = (symtab*) malloc(sizeof (struct symbol_table));

  this->parent = parent;
  this->sym_count = 0;
  this->children_count = 0;

  return this;
}



//Search the given symbol table for an element with the given
//id, type, and scope. If this table doesn't contain it, search
//recursively through all parents.
//Returns the index if the symbol exists, returns -1 otherwise.
int SYM_lookup(symtab* table, int id_index, int type, int scope)
{
  int i;

  for (i=0; i<(table->sym_count); i++)
  {
    if ((id_index == table->symbols[i].id_index) &&
        (type == table->symbols[i].type) &&
        (scope == table->symbols[i].scope))
    {
      return i;
    }
  }

  if (table->parent)
    return SYM_lookup(table->parent, id_index, type, scope-1);
  else
    return -1;
}

//insert a symbol into the table.
//returns 1 if a new symbol was inserted,
//        0 if the table is full or the
//	    symbol already exists.
int SYM_insert(symtab* table, int id_index, int type, int arr_size, int scope)
{
  if (table->sym_count >= MAX_SYMBOLS)
    return 0;

  int i;

  for (i=0; i<(table->sym_count); i++)
  {
    if ((id_index == table->symbols[i].id_index) &&
        !diff_types(type, table->symbols[i].type))
    {
      return 0;
    }
  }


  table->symbols[table->sym_count].id_index = id_index;
  table->symbols[table->sym_count].type     = type;
  table->symbols[table->sym_count].arr_size = arr_size;
  table->symbols[table->sym_count].scope    = scope;

  table->sym_count++;

  return 1;
}



//create a new table whose parent is the given table.
//if no parent (i.e. global scope), call new_scope(0).
//returns pointer to the new table.
symtab* new_scope(symtab* table)
{
  if (!table || (table->children_count >= MAX_CHILDREN))
    return table;

  symtab* new_child = make_table(table);

  table->children[table->children_count] = new_child;

  table->children_count++;

  return new_child;
}


//leave the current scope if possible.
//if the given table has a parent, returns that parent.
//Otherwise, returns the given table.
//NOTE: does not delete the given table.
symtab* leave_scope(symtab* table)
{
  if (table && table->parent)
  {
    return table->parent;
  }
  else
  {
    return table;
  }
}



//get the size of the array-like variable with the given id.
//returns the array size if the variable is array-like.
//returns -1 otherwise.
//NOTE: strings ARE array-like, but will always return 0, as they
//      do not have a fixed size.
int get_arr_size(symtab* table, int id_index)
{
  symtab* this = table;

  int i;

  while (this)
  {
    for (i=0; i<(this->sym_count); i++)
    {
      if ((this->symbols[i].id_index == id_index) &&
          (this->symbols[i].type < 8) &&
          (this->symbols[i].type > 2))
        return this->symbols[i].arr_size;
    }

    this = this->parent;
  }

  return -1;
}



//Debug function to print information about the given
//symbol table and all children.
void print_symtab(symtab* table, int spacing)
{
  int i;

  for (i=0; i<(table->sym_count); i++)
  {

    int c;
    for (c=0; c<spacing; c++)
      fprintf(stdout, "  ");

    fprintf(stdout, "<%s, ", strTable[table->symbols[i].id_index]);
    if (table->symbols[i].type < 8)
      fprintf(stdout, "%s", typeStr[table->symbols[i].type]);
    else
      fprintf(stdout, "func:%d", table->symbols[i].type);
    fprintf(stdout, ", scope:%d>\n", table->symbols[i].scope);
  }

  for (i=0; i<(table->children_count); i++)
  {
    print_symtab(table->children[i], spacing+1);
  }
}
