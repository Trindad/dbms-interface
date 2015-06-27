#ifndef SHELL_H
#define SHELL_H
#include "database.h"
#include "./sql/semantic/semantic.h"

extern int current_database;//armazena o identificador do banco atual, se tiver 0 nenhum banco esta logado

void shell();

void executeLine(int option);

char *strtolower(char *str);//converte para minuscolos as letras

int count_ocurrences(char *str, char delim);//verifica a ocorência de um caracter na string passado pela variável delim

extern char **tokenize(char *str, char delim, int *size);//quebra string 

char *remove_newline(char *str);//remove \n da sting

void example();//cria banco e tabelas, colunas e insere valores.

char *table_name_real(char *name,int database);//retorna o nome real da tabela gravada no banco

char *remove_semicolon(char *str); //remove ; do comando sql

void help();//função para ajuda ao usuário

#endif

