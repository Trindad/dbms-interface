#ifndef SHELL_H
#define SHELL_H
#include "database.h"
#include "insert/semantic/semantic.h"

//define das opções da interface
#define INSERT 1
#define CONNECT	2
#define SHOW	3
#define EXIT	4
#define CREATE 5

extern int current_database;//armazena o identificador do banco atual, se tiver 0 nenhum banco esta logado

void shell();

void executeLine(int option);

#endif

