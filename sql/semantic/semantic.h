#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../../tuple.h"

void insert(char *str, int current_database);

void createTable(char *str, int current_database);

table *start(char *name);

char *table_name_cat(char *name,int database);

int table_exist(char *tablename,int database);

extern char **tokenize(char *str, char delim, int *size);//quebra string

#endif