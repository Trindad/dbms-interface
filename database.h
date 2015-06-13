#ifndef DATABASE_H
#define DATABASE_H
#include "buffend.h"

#define TAM_NOME_BANCO 30

typedef struct db_db{
    int cod;
    char nome[TAM_NOME_BANCO];

}db;

int cod_id_banco();

int busca(char *str, int identificacao);

void grava_banco(char *str);

int checkCreateDB(char *nome);
/*
 Objetivo: verificar se o nome do banco já existe e se não guarda-lo no arquivo fs_database.dat
 */
#endif