#ifndef SHELL_H
#define SHELL_H
#include "database.h"
#include "./insert/semantic/semantic.h"

extern int current_database;//armazena o identificador do banco atual, se tiver 0 nenhum banco esta logado

void shell();

void executeLine(int option);

char *strtolower(char *str);//converte para minuscolos as letras

int count_ocurrences(char *str, char delim);

char **tokenize(char *str, char delim, int *size);


#endif

