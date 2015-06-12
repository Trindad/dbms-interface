#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "erros.h"

#define TAM_NOME_BANCO 30

typedef struct db_db{

    int cod;
    char nome[TAM_NOME_BANCO];

}db;

//verifica id do novo banco
int cod_id_banco();

//busca por nome do banco se estiver cadastrado retorna a id dele
int busca(char *str);

//criar banco
void grava_banco(char *str);
