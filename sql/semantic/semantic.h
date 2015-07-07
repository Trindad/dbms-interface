#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../../tuple.h"

void insert(char *str, int current_database);//insere uma ou mais tuplas no banco

void createTable(char *str, int current_database);//cria uma tabela adicionando suas colunas e informações

table *start(char *name);//inicia a tabela

char *table_name_cat(char *name,int database);//concatena o indice do banco com o nome da tabela passado como parâmetro

int table_exist(char *tablename,int database);//verifica se a tabela existe

extern char **tokenize(char *str, char delim, int *size);//quebra string

void selectTable(char *sql, int index_database);//mostra as tuplas da tabela

#endif