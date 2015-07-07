#ifndef TABLE_H
#define TABLE_H

#include "common.h"
#include "buffend.h"

/**
 * Funções que manipulam a tabela do SGBD
 */


table *iniciaTabela(char *nomeTabela);
/*
    Esta função inicia um estrutura do tipo table, como nome de tabela passado. 
    Retorna:
        -> a estrutura do tipo table iniciada;
        -> ERRO_NOME_TABELA_INVALIDO se o nome passado já existir no dicionário.
    *nomeTabela - Nome de uma tabela, a qual deseja-se inserir.
*/


int finalizaTabela(table *t,int database);
/*
    Esta função finaliza a tabela preveamente estrutura pelas funcoes iniciaTabela() e adicionaCampo(). 
    Escreve nos arquivos fs_object.dat e fs_schema.dat, a estrutura passada.
    Retorna: 
        -> SUCCESS quando teve sucesso na sua operaçãoç;
        -> ERRO_ABRIR_ARQUIVO quando teve problemas ao abrir os arquivos fs_object.dat e fs_schema.dat;
        -> ERRO_PARAMETRO quando a estrutura passada é inválida.
    *t - Estrutura da tabela à ser criada.
*/

int verificaNomeTabela(char *nomeTabela);
/*
    Esta função verifica se um nome de tabela já está inserido no dicionario. 
    Retorna:
        -> 1 se o nome existe no dicionario;
        -> 0 se existe no dicionário.
    *nomeTabela - Nome de uma tabela,  a qual deseja-se saber se existe no dicionario.
*/

int quantidadeTabelas();
/*
    Esta função conta quantas tabelas já estão inseridas dentro do dicionario, para poder colocar 
    um código válido para a próxima tabela. Retorna a quantidade exata de tabelas.
*/

table *adicionaCampo(table *t,char *nomeCampo, char tipoCampo, int tamanhoCampo, int tChave, char *tabelaApt, char *attApt);
/*
    Esta função encadeia a lista de campos na estrutura de uma tabela que vai ser criada. 
    Retorna:
        -> a estrutura com a coluna inserida na lista.
    *t - Estrutura da tabela à ser criada.
    *nomeCampo - Nome do campo que irá ser inserido na lista de campos.
    tipoCampo - Tipo do campo que irá ser inserido na lista de campos.
    tamanhoCampo - Tamanho do campo que irá ser inserido na lista de campos.
*/


void imprime(char nomeTabela[] );
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Utilizada para impressão de tabelas.
    Parametros: Nome da tabela (char).    
    Retorno:    void.
---------------------------------------------------------------------------------------------*/


int excluirTabela(char *nomeTabela);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Função para exclusão de tabelas.
    Parametros: Nome da tabela (char).    
    Retorno:    INT
                SUCCESS, 
                ERRO_REMOVER_ARQUIVO_OBJECT, 
                ERRO_REMOVER_ARQUIVO_SCHEMA, 
                ERRO_LEITURA_DADOS.
---------------------------------------------------------------------------------------------*/

tp_table *procuraAtributoFK(struct fs_objects objeto);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Retorna vetor de esquemas com todos os atributos chaves (PK, FK e NPK)
    Parametros: Objeto da tabela.
    Retorno:    Vetor de esquemas vetEsqm
---------------------------------------------------------------------------------------------*/

int verificaChaveFK(char *nomeTabela, column *c, char *nomeCampo, char *valorCampo, char *tabelaApt, char *attApt);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Gera as verificações em relação a chave FK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo, Tabela Apontada e Atributo Apontado.
    Retorno:    INT
                SUCCESS,
                ERRO_DE_PARAMETRO,
                ERRO_CHAVE_ESTRANGEIRA
   ---------------------------------------------------------------------------------------------*/

int verificaChavePK(char *nomeTabela, column *c, char *nomeCampo, char *valorCampo);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Gera as verificações em relação a chave pK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo
    Retorno:    INT
                SUCCESS,
                ERRO_DE_PARAMETRO,
                ERRO_CHAVE_PRIMARIA
   ---------------------------------------------------------------------------------------------*/


tp_table *abreTabela(char *nomeTabela, struct fs_objects *objeto, tp_table **tabela);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Recebe o nome de uma tabela e engloba as funções leObjeto() e leSchema().
    Parametros: Nome da Tabela, Objeto da Tabela e tabela.
    Retorno:    tp_table
   ---------------------------------------------------------------------------------------------*/

int existeAtributo(char *nomeTabela, column *c);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verifica a existência do atributo antes de adicionar na tabela
    Parametros: Nome da tabela, coluna C.    
    Retorno:    INT 
                SUCCESS,
                ERRO_DE_PARAMETRO
   ---------------------------------------------------------------------------------------------*/

int iniciaAtributos(struct fs_objects *objeto, tp_table **tabela, tp_buffer **bufferpoll, char *nomeT);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Inicializa os atributos necessários para a verificação de FK e PK.
    Parametros: Objeto da tabela, Tabela, Buffer e nome da tabela.
    Retorno:    INT
                SUCCESS,
                ERRO_DE_PARAMETRO,
   ---------------------------------------------------------------------------------------------*/

/************************************************************************************************
 ************************************************************************************************/

column *insereValor(table  *tab, column *c, char *nomeCampo, char *valorCampo);
/*
    Esta função inicia e aloca dinâmicamente uma lista de valores que vão ser inseridos em uma tabela.
    Retorna: 
        -> estrutura iniciada e alocad com o valor passado por parâmetro.
        -> ERRO_DE_PARAMETRO, quando a estrutura enviada for inválida.
    *c - Estrutura de valores que vão ser inseridos em uma tabela.
    *nomeCampo - Nome do campo que o usuário vai inserir um valor.
    *valorCampo - Valor do campo que vai ser inserido.
*/
    
int cabecalho(tp_table *s, int num_reg);

int finalizaInsert(char *nome, column *c);
/*
    Esta função finaliza a inserção de valores em uma tabela. Assume que o usuário entrou com todos
    os campos de uma tupla completa.
    Retorna: 
        -> ERRO_ABRIR_ARQUIVO, quando ocorreu um erro ao abrir o arquivo fs_object.dat ou fs_schema.dat;
        -> ERRO_NO_TAMANHO_STRING, quando ocorreu um erro no tamanho da string inserida;
        -> ERRO_NOME_CAMPO, quando o nome do campo passado na estrutura;
        -> ERRO_NO_TIPO_INTEIRO, quando o valor passado não é um inteiro;
        -> ERRO_NO_TIPO_DOUBLE, quando o valor passado não é um double;
        -> ERRO_NO_TIPO_CHAR, quando o valor passado é maior que 1byte;
        -> SUCCESS, quando a função teve sucesso em sua operação de inserção na tabela.
    *nome - Nome da tabela que vai ser inserido os valores da estrutura *c.
    *c - Estrutura com o valores que vão ser inseridos na tabela *nome.
*/

int verificaNomeCampo(char *table_name, char *column);//verifica se a coluna existe na tabela

#endif
