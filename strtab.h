#ifndef STRTAB_H
#define STRTAB_H

#define MAXIDS 1000

char *strTable[MAXIDS];

//insert a string into the string table.
//(automatically determine string length)
//
//returns the index of the string if successful.
//returns -1 if the table is full.
int ST_insert(const char *id);

//insert a string into the string table.
//(given string length)
//
//returns the index of the string if successful.
//returns -1 if the table is full.
int ST_insert_yy(const char *id, int id_len);

//Check whether the given string is already in the table.
//
//returns the index if true, -1 otherwise.
int ST_lookup(char *id);

#endif
