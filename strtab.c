#include <string.h>
#include <stdlib.h>
#include "strtab.h"

int ID_COUNT = 0;

//insert a string into the string table.
//(automatically determine string length)
//
//returns the index of the string if successful.
//returns -1 if the table is full.
int ST_insert(const char *id) 
{
  int i;
  for (i=0; i<ID_COUNT; i++)
  	if (strcmp(id, strTable[i]) == 0)
       return i;
  
  if (ID_COUNT < MAXIDS)
  {
    int count=0;
    while (id[count] != 0)
      count++;

    strTable[ID_COUNT] = (char*)malloc((count+1)*sizeof(char));
    strcpy(strTable[ID_COUNT], id);
    strTable[ID_COUNT][count] = 0;

    ID_COUNT++;
    return ID_COUNT-1;
  }
  else
  {
    return -1;
  }
}


//insert a string into the string table.
//(given string length)
//
//returns the index of the string if successful.
//returns -1 if the table is full.
int ST_insert_yy(const char *id, int id_len)
{
  int i;
  for (i=0; i<ID_COUNT; i++)
  	if (strcmp(id, strTable[i]) == 0)
       return i;
  
  if (ID_COUNT < MAXIDS)
  {
    strTable[ID_COUNT] = (char *)malloc(id_len *sizeof(char));
    strcpy(strTable[ID_COUNT], id);
    strTable[ID_COUNT][id_len-1] = 0;

    ID_COUNT++;
    return ID_COUNT-1;
  }
  else
  {
    return -1;
  }
}

//Check whether the given string is already in the table.
//
//returns the index if true, -1 otherwise.
int ST_lookup(char *id) 
{
  int i;
  for (i=0; i<ID_COUNT; i++)
  	if (strcmp(id, strTable[i]) == 0)
       return i;
  
  return -1;
}
