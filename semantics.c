#include <stdio.h>
#include "symtab.h"
#include "semantics.h"
#include "func_signature.h"

//Check whether a function with the given id exists.
//Only checks the global scope.
//returns 1 if true, 0 otherwise.
int func_exists(int id_index);

//Check whether a variable with the given id exists.
//First checks the current scope, then global.
//returns 1 if true, 0 otherwise.
int var_exists(int id_index);

//Get the signature of a function with the given id.
//Searches the global scope for the function and returns
//the appropriate function signature index if it exists,
//otherwise returns -1.
int get_func_index(int id_index);

//Get the type of the variable with the given id.
//Searches the local, then global for the variable and returns
//the appropriate type value if it exists,
//otherwise returns -1.
int get_var_type(int id_index);




//for outputting error messages with specific data
extern char* strTable[];
const char* typeStr[] = {"void",   "int",   "char",   "string",
                         "void[]", "int[]", "char[]", "string[]"};

//for keeping track of a function's parameters
int prev_param_count;
int prev_param_types[MAX_PARAMS];


//the current scope
symtab *table;
int current_scope;

//keep track of the last dataType
int lastType;




//Check the completed AST for semantic errors.
//Returns the number of errors found.
int checkSemantics(tree* this)
{
  //exit if an invalid node
  if (!this)
    return 0;

  int error = 0;
  int check_subnodes = 1;

  int varType;

  
  /* NODE CHECKING ON ENTRANCE */
  //


  //We enter the program, initialize variables
  if (this->nodeKind == PROGRAM)
  {
    current_scope = GLOBAL;
    table = make_table(0);

    lastType = -1;

    prev_param_count = 0;
  }
  //A variable is being declared
  else if (this->nodeKind == VARDECL)
  {
    int type = this->children[0]->val;
    int id = this->children[1]->val;
    int arr_size;
    
    if (this->numChildren > 2)
      arr_size = this->children[2]->val;
    else
      arr_size = 0;

    //if an array is initialized with a negative size.
    //currently, ints can't be negative, but maybe later.
    if (arr_size < 0)
    {
      fprintf(stderr, "Error: %d: Negative array size on '%s'\n",\
      this->line, strTable[id]);
      error++;
    }

    //if the variable has already been declared in the current scope
    if (!SYM_insert(table, id, type, arr_size, current_scope))
    {
      fprintf(stderr, "Error: %d: Variable '%s' already declared\n",\
      this->line, strTable[id]);
      return 1;
    }
  }
  //we are entering a function
  else if (this->nodeKind == FUNCDECL)
  {
    int sig = get_signature(this);
    int type = sig + 8;

    int id = this->children[1]->val;

    //if the function has been previously defined
    if (!SYM_insert(table, id, type, 0, current_scope))
    {
      fprintf(stderr, "Error: %d: Function '%s()' already defined",\
      this->line, strTable[id]);
  
      return 1;
    }

    current_scope = LOCAL;
    table = new_scope(table);
  }
  //in a function, the formal declarations
  else if (this->nodeKind == FORMALDECL)
  {
    int type = this->children[0]->val;
    int id = this->children[1]->val;

    //if we have duplicate parameters
    if (!SYM_insert(table, id, type, 0, current_scope))
    {
      fprintf(stderr, "Error: %d: Parameter '%s' already defined\n",\
      this->line, strTable[id]);
      return 1;
    }
  }
  //accessing a variable
  else if (this->nodeKind == VAR)
  {
    int id = this->children[0]->val;

    //we are trying to access an undeclared variable
    if (!var_exists(id))
    {
      fprintf(stderr, "Error: %d: Undeclared variable '%s'\n",\
      this->line, strTable[id]);
      return 1;
    }
  }
  //check assignments for type mismatch
  else if (this->nodeKind == ASSIGN)
  {
    int id = this->children[0]->children[0]->val;

    int is_indexed = (this->children[0]->numChildren) > 1;

    varType = get_var_type(id);

    if (is_indexed)
      if (varType > _STRING)
        varType = varType - 4;
      else
        varType = CHAR;

    //we will scan the subnodes manually
    check_subnodes = 0;
  }
  //integer literal
  else if (this->nodeKind == INTEGER)
  {
    lastType = INT;
  }
  //character literal
  else if (this->nodeKind == CHARACTER)
  {
    lastType = CHAR;
  }
  //string literal
  else if (this->nodeKind == STRING)
  {
    lastType = _STRING;
  }



  /* CHECK SEMANTICS FOR ALL SUBNODES */
  //
  //
  int i;
  if (check_subnodes)
    for (i=0; i<(this->numChildren); i++)
    {
      error += checkSemantics(this->children[i]);
    }
  //
  //
  


  /* NODE CHECKING ON EXIT */
  //
  
  //We are exiting a function
  if (this->nodeKind == FUNCDECL)
  {
    current_scope = GLOBAL;
    table = leave_scope(table);
  }
  //accessing a variable (check indexing and type)
  else if (this->nodeKind == VAR)
  {
    int id = this->children[0]->val;
    
    //if the variable is indexed
    if (this->numChildren > 1)
    {
      //if the type used for indexing is a non-integer
      if (lastType != INT)
      {
        fprintf(stderr, "Error: %d: Variable '%s[]' indexed with a non-integer value\n",\
        this->line, strTable[id]);
        error++;

        lastType = get_var_type(id);
      }
      else //the index type is an integer
      {
        int arr_size = get_arr_size(table, id);

        lastType = get_var_type(id);

        if (arr_size > -1) //the variable can be indexed
        {
          //the given index is out of bounds
          if ( ((lastType != _STRING) && //we don't know for strings
                (this->children[1]->val >= arr_size)) ||
               (this->children[1]->val < 0) ) //ints can't be negative yet, but maybe later.
          {
            fprintf(stderr, "Error: %d: Index on '%s' out of bounds\n",\
            this->line, strTable[id]);
            return 1;
          }
        }
        else //the variable is not of any array-like type
        {
          fprintf(stderr, "Error: %d: Attempting to index non-array variable '%s'\n",\
          this->line, strTable[id]);
          return 1;
        }

        if (lastType > _STRING)
          lastType = lastType - 4;
        else if (lastType == _STRING)
          lastType = CHAR;

      }
    }
    else //if a non-indexed variable, just update the type
    {
      lastType = get_var_type(id);
    }
    
  }
  //we are calling a function
  else if (this->nodeKind == FUNCCALL)
  {
    int id = this->children[0]->val;

    //the function is undefined
    if (!func_exists(id))
    {
      fprintf(stderr, "Error: %d: Function '%s()' not defined\n",\
      this->line, strTable[id]);

      prev_param_count = 0;
      return 1;
    }

    int ref_func = get_func_index(id);

    symtab* global;
    if (table->parent)
      global = table->parent;
    else
      global = table;

    //get the function signature
    int func_sig = global->symbols[ref_func].type - 8;

    lastType = get_sig_type(func_sig);

    //check that all the parameters are correct
    int call_check = check_called_func(func_sig);

    //if incorrect number of parameters
    if (call_check == 1)
    {
      fprintf(stderr, "Error: %d: Incorrect number of parameters for function '%s()'\n",\
      this->line, strTable[id]);
      error++;
    }
    //if one or more parameters is of an incorrect type
    else if (call_check == 2)
    {
      fprintf(stderr, "Error: %d: Parameter mismatch in function call '%s()'\n",\
      this->line, strTable[id]);
      error++;
    }

    prev_param_count = 0;
  }
  //append parameter types to the parameter type list
  else if (this->nodeKind == ARGLIST)
  {
    if (prev_param_count < MAX_PARAMS)
    {
      prev_param_types[prev_param_count] = lastType;
 
      prev_param_count++;
    }
  }
  //check assignments for type mismatch
  else if (this->nodeKind == ASSIGN)
  {
    int id = this->children[0]->children[0]->val;

    //check the LHS for errors
    error += checkSemantics(this->children[0]);

    if (!error)
    {
      int asgn_var_type = varType;

      //check the RHS for errors
      error += checkSemantics(this->children[1]);

      //if the type on the LHS is not the same as the type on the RHS
      if (asgn_var_type != lastType)
      {
        fprintf(stderr, "Error: %d: Type mismatch in assignment of '%s'\n",\
        this->line, strTable[id]);
        error++;
      }
    }
  }


  return error;
}


//Check whether a function with the given id exists.
//Only checks the global scope.
//returns 1 if true, 0 otherwise.
int func_exists(int id_index)
{
  symtab* global;

  if (table->parent)
    global = table->parent;
  else
    global = table;


  int i;
  for (i=0; i<(global->sym_count); i++)
    if ((global->symbols[i].id_index == id_index) &&
        (global->symbols[i].type >= 8))
      return 1;

  return 0;
}


//Check whether a variable with the given id exists.
//First checks the current scope, then global.
//returns 1 if true, 0 otherwise.
int var_exists(int id_index)
{
  symtab* this = table;


  int i;

  while (this)
  {
    for (i=0; i<(this->sym_count); i++)
      if ((this->symbols[i].id_index == id_index) &&
          (this->symbols[i].type < 8))
        return 1;

    this = this->parent;
  }

  return 0;
}


//Get the signature of a function with the given id.
//Searches the global scope for the function and returns
//the appropriate function signature index if it exists,
//otherwise returns -1.
int get_func_index(int id_index)
{
  symtab* this;
  if (table->parent)
    this = table->parent;
  else
    this = table;

  int i;
  for (i=0; i<(this->sym_count); i++)
    if ((this->symbols[i].id_index == id_index) &&
        (this->symbols[i].type >= 8))
      return i;
  
  return -1;
}


//Get the type of the variable with the given id.
//Searches the local, then global for the variable and returns
//the appropriate type value if it exists,
//otherwise returns -1.
int get_var_type(int id_index)
{
  symtab* this = table;
  
  int i;

  while (this)
  {
    for (i=0; i<(this->sym_count); i++)
      if ((this->symbols[i].id_index == id_index) &&
          (this->symbols[i].type < 8))
        return this->symbols[i].type;

    this = this->parent;
  }

  return -1;
}
