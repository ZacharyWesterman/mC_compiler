#ifndef FUNC_SIGNATURE_H
#define FUNC_SIGNATURE_H

#include "tree.h"

#define MAX_PARAMS 100

#define MAX_FUNCS 1000

typedef struct func_sig signature;

struct func_sig
{
  int return_type;
  
  int param_count;
  
  int param_types[MAX_PARAMS];
};

//takes the given tree node and determines the function
//signature from it. If the signature already exists, 
//returns the index of the existing signature, otherwise
//adds the new signature to the list and returns the
//new index. Returns -1 if the list is full.
//NOTE: This function assumes the first node is of type FUNCDECL!
int get_signature(tree* root);

//gives the return type of a function
//according to the given signature index
int get_sig_type(int sig_index);

//returns the parameter count a function signature
//according to the given index
int get_sig_param_count(int sig_index);

//returns 0 if called and defined functions have the same parameters
//returns 1 if different number of parameters
//returns 2 if parameter mismatch
int check_called_func(int sig_index);

#endif
