#ifndef DATABASE_H
#define DATABASE_H
#include "table.h"

#define TAM_NOME_BANCO 30

typedef struct db_db{
    int cod;
    char nome[TAM_NOME_BANCO];

}db;

int cod_id_banco();

int busca(char *str, int identificacao);

void listaBancos();

void listaTabelas(int database);

void grava_banco(char *str);

int checkCreateDB(char *nome);

void dropDatabase(char *name);//exclui banco de dados

extern char **tokenize(char *str, char delim, int *size);//quebra string 
/*
 Objetivo: verificar se o nome do banco já existe e se não guarda-lo no arquivo fs_database.dat
 */
#endif