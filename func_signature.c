#include "func_signature.h"
#include <stdio.h>

extern int prev_param_count;
extern int prev_param_types[];

int signature_count;

signature signatures[MAX_FUNCS];


//determines the parameter count and all parameter types
//from the given tree node, recursively.
//NOTE: This function assumes the first node is of type FUNCDECL!
void gen_sig_params(tree* root, int* params, int* param_count)
{
  if ((root->nodeKind == FUNCDECL) && ((*param_count) < MAX_PARAMS))
  {
    if (root->children[2]->nodeKind == FDECLLIST)
      gen_sig_params(root->children[2], params, param_count);
    else if (root->children[2]->nodeKind == FORMALDECL)
    {
      params[(*param_count)] = root->children[2]->children[0]->val;
      (*param_count)++;
    }
  }
  else if ((root->nodeKind == FDECLLIST) && ((*param_count) < MAX_PARAMS-1))
  {
    params[(*param_count)] = root->children[0]->children[0]->val;
    (*param_count)++;

    if (root->children[1]->nodeKind == FDECLLIST)
      gen_sig_params(root->children[1], params, param_count);
    else if (root->children[1]->nodeKind == FORMALDECL)
    {
      params[(*param_count)] = root->children[1]->children[0]->val;
      (*param_count)++;
    }
  }
}


//copies the data from one function signature to another
void copy_sig(signature* to, signature* from)
{
  to->return_type = from->return_type;
  to->param_count = from->param_count;

  int p;
  for (p=0; p<(from->param_count); p++)
    to->param_types[p] = from->param_types[p];
}


//checks if two signatures are equivalent
//(same return type and parameter count, and
// all parameters are of the same type)
int sigs_equal(signature* arg1, signature* arg2)
{
  if (((arg1->return_type) == (arg2->return_type)) &&
      ((arg1->param_count) == (arg2->param_count)) )
  {
    int same = 1;

      int p;
      for (p=0; p<(arg1->param_count); p++)
      {
        if ((arg1->param_types[p]) != (arg2->param_types[p]))
        {
          same = 0;
        }
      }

      return same;
  }
  else
  {
    return 0;
  }
}


//takes the given tree node and determines the function
//signature from it. If the signature already exists, 
//returns the index of the existing signature, otherwise
//adds the new signature to the list and returns the
//new index. Returns -1 if the list is full.
//NOTE: This function assumes the first node is of type FUNCDECL!
int get_signature(tree* root)
{
  if (signature_count >= MAX_FUNCS)
    return -1;

  signature tsig;
  tsig.param_count = 0;

  tsig.return_type = root->children[0]->val;

  gen_sig_params(root, tsig.param_types, &tsig.param_count);

  
  int i;
  for (i=0; i<signature_count; i++)
  {
    if (sigs_equal(&(signatures[i]), &tsig))
      return i;
  }

  copy_sig(&(signatures[signature_count]), &tsig);
  signature_count++;

  return (signature_count - 1);
}


//gives the return type of a function
//according to the given signature index
int get_sig_type(int sig_index)
{
  return signatures[sig_index].return_type;
}


//returns the parameter count a function signature
//according to the given index
int get_sig_param_count(int sig_index)
{
  return signatures[sig_index].param_count;
}


//returns 0 if called and defined functions have the same parameters
//returns 1 if different number of parameters
//returns 2 if parameter mismatch
int check_called_func(int sig_index)
{
  int param_existing = signatures[sig_index].param_count;

  int i;

  if (prev_param_count != param_existing)
    return 1;

  for (i=0; i<param_existing; i++)
  {
    if (prev_param_types[i] != signatures[sig_index].param_types[i])
      return 2;
  }
  
  return 0;
}

