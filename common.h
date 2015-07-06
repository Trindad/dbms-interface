#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "erros.h"
/**
 * Funções genéricas, utilizadas para a manipulação do banco
 */
#define SIZE 512        // Tamanho da página.
#define PAGES 1024 				// Número de páginas.
#define TAMANHO_NOME_ARQUIVO 20 // Tamanho do nome do arquivo.
#define TAMANHO_NOME_DATABASE 30 // Tamanho do nome do arquivo.
#define TAMANHO_NOME_CAMPO 40 	// Tamanho do nome dos campos de uma tabela.
#define TAMANHO_NOME_TABELA 20 	// Tamanho do nome da tabela.

// Union's utilizados na conversão de variáveis do tipo inteiro e double.
union c_double{
	
	double dnum;
	char double_cnum[sizeof(double)];	
};

union c_int{
	
	int  num;
	char cnum[sizeof(int)];
};

struct fs_objects { // Estrutura usada para carregar fs_objects.dat
	char nome[TAMANHO_NOME_TABELA];		//  Nome da tabela.
	int cod;							// Código da tabela.
	char nArquivo[TAMANHO_NOME_ARQUIVO];// Nome do arquivo onde estão armazenados os dados da tabela.
	int qtdCampos;						// Quantidade de campos da tabela.
	// fs_database bd;						//banco de dados ao qual o objeto pertence
};

typedef struct tp_table{ // Estrutura usada para carregar fs_schema.dat
	char nome[TAMANHO_NOME_CAMPO];	// Nome do Campo.                    40bytes   
	char tipo;						// Tipo do Campo.                     1bytes
	int tam;						// Tamanho do Campo.                  4bytes
	int chave;						// Tipo da chave                      4bytes
	char tabelaApt[TAMANHO_NOME_TABELA]; //Nome da Tabela Apontada        20bytes 
	char attApt[TAMANHO_NOME_CAMPO];	//Nome do Atributo Apontado       40bytes
	struct tp_table *next;			// Encadeamento para o próximo campo.
	struct fs_objects objetos;				//objetos que pertence a um esquema
}tp_table;


typedef struct column{ // Estrutura utilizada para inserir em uma tabela, excluir uma tupla e retornar valores de uma página.
	char tipoCampo;						// Tipo do Campo.
	char nomeCampo[TAMANHO_NOME_CAMPO];	//Nome do Campo.
	char *valorCampo;					// Valor do Campo.
	struct column *next;				// Encadeamento para o próximo campo.
}column;

typedef struct table{ // Estrutura utilizada para criar uma tabela.
	char nome[TAMANHO_NOME_TABELA]; // Nome da tabela.
	tp_table *esquema;				// Esquema de campos da tabela.
}table;

typedef struct tp_buffer{ // Estrutura utilizada para armazenar o buffer.
   unsigned char db; 		//Dirty bit
   unsigned char pc; 		//Pin counter
   unsigned int nrec; 		//Número de registros armazenados na página.
   char data[SIZE]; 		// Dados
   unsigned int position; 	// Próxima posição válida na página.
}tp_buffer;


int existeArquivo(char* filename);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verificação de existência de um arquivo.
    Parametros: Nome do arquivo.    
    Retorno:    INT 1 (existe) , 0 (não existe).
   ---------------------------------------------------------------------------------------------*/

int TrocaArquivosObj(char *nomeTabela, char *linha);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verifica se o nome da tabela 'nomeTabela' está nos primeiros bytes de 'linha'
    Parametros: Nome da tabela, char linha.    
    Retorno:    INT(1 - Está contido, 0 - Não está)                
   ---------------------------------------------------------------------------------------------*/


int procuraObjectArquivo(char *nomeTabela);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Copia todas as informações menos a tabela com nome NomeTabela, que será removida.
    Parametros: Nome da tabela que será removida do object.dat.
    Retorno:    INT
                SUCCESS,
                ERRO_ABRIR_ARQUIVO
   ---------------------------------------------------------------------------------------------*/

struct fs_objects leObjeto(char *nTabela);
/*
	Esta função busca, no arquivo fs_object.dat, pelo nome da tabela retornando as informações que
    estão no dicionário em uma estrutura fs_objects. Caso o nome da tabela não exista, o programa 
    aborta.

	*nTabela - Nome da tabela a ser buscado no dicionário de dados
*/

tp_table *leSchema (struct fs_objects objeto);
/*
    Esta função busca, no arquivo fs_schema.dat, pelas informações do objeto, carregando o esquema 
    da tabela que é retornadado em tp_table.
    Caso o nome da tabela não exista, o programa aborta 
    *objeto - Objeto, já previamente inicializado em leObjeto(nTabela), que contém as informações 
              sobre uma determinada tabela.
*/

int procuraSchemaArquivo(struct fs_objects objeto);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Copia todas as informações menos a tabela do objeto, que será removida.
    Parametros: Objeto que será removido do schema.
    Retorno:    INT
                SUCCESS,
                ERRO_REMOVER_ARQUIVO_SCHEMA
 ---------------------------------------------------------------------------------------------*/
 
 void *show_schema(struct fs_objects objeto,char *name_table);
/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   
    Parametros: 
    Retorno:   
                
 ---------------------------------------------------------------------------------------------*/
 
/**
 * Funções para conversões
 */
int pot10(int n);

int strtam(char n[]);

int convertI(char u[]);

double get_decimal(char u[]);

double get_inteiro(char v[]);

double convertD(char u[]);

int *tamanho_maximo_campo(struct fs_objects objeto);//verifica o tamanho máximo dos campos para imprimir formatado o esquema
