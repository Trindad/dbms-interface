#ifndef DATABASE_H
#define DATABASE_H
#include "table.h"

#define TAM_NOME_BANCO 30//limite para o tamanho do nome do banco de dados

typedef struct db_db{
    int cod;//identificador do banco
    char nome[TAM_NOME_BANCO];//nome do banco

}db;

int id_max; //identificador máximo do banco para controle de inserção

int cod_id_db(int flag);//retorna o número de banco de dados cadastrados

int busca(char *str, int identificacao);//busca o banco

void listaBancos();//lista os bancos de dados existentes

void listaTabelas(int database);//lista as tabelas relacionadas ao banco de dados passado como indice

void grava_banco(char *str);//grava o nome do banco em um arquivo

int checkCreateDB(char *nome);//cria o banco de dados caso não exista atribuindo um indice a ele

int dropDatabase(char *name);//exclui banco de dados

extern char **tokenize(char *str, char delim, int *size);//quebra string pelo demitador 

#endif