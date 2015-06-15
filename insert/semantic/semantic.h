#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../../tuple.h"

void insert(char *str, int current_database);

table *start(char *name);

char *table_name_cat(char *name,int database);

#endif