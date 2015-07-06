/**
 * Funções que manipulam inserções no banco
 */
#ifndef TUPLE_H
#define TUPLE_H

#include "table.h"

/**
 * Retorna o tamanho do valor do campo passado como parâmetro
 */
int retornaTamanhoValorCampo(char *nomeCampo, table  *tab);

/**
 * Retorna o tipo do campo passado pela string nomeCampo
 * na tabela tabém passada como parâmetro, retornando o 
 * tipo do campo, se é string, char, double ou int
 */
char retornaTipoDoCampo(char *nomeCampo, table  *tab);

int tamTupla(tp_table *esquema, struct fs_objects objeto);
/*
    Esta função calcula, usando o esquema e o dicionário de dados, o tamanho da tupla de uma 
    tabela, retornando o mesmo.

    *esquema - Estrutura que contém o esquema da tabela (nome de campo, tipo de campo, etc)
    *objeto  - Estrutura que contém informações sobre a tabela (nome da tabela, nome do arquivo da 
               tabela, etc)

*/

void setTupla(tp_buffer *buffer,char *tupla, int tam, int pos);

char *getTupla(tp_table *campos,struct fs_objects objeto, int from);

#endif