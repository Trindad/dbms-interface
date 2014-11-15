#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "erros.h"

#define SIZE 512 				// Tamanho da página.
#define PAGES 1024 				// Número de páginas.
#define TAMANHO_NOME_CAMPO 40 	// Tamanho do nome dos campos de uma tabela.
#define TAMANHO_NOME_TABELA 20 	// Tamanho do nome da tabela.
#define TAMANHO_NOME_ARQUIVO 20 // Tamanho do nome do arquivo.

struct fs_objects { // Estrutura usada para carregar fs_objects.dat
	char nome[TAMANHO_NOME_TABELA];		//  Nome da tabela.
	int cod;							// Código da tabela.
	char nArquivo[TAMANHO_NOME_ARQUIVO];// Nome do arquivo onde estão armazenados os dados da tabela.
	int qtdCampos;						// Quantidade de campos da tabela.
};

typedef struct tp_table{ // Estrutura usada para carregar fs_schema.dat
	char nome[TAMANHO_NOME_CAMPO];	// Nome do Campo.
	char tipo;						// Tipo do Campo.
	int tam;						// Tamanho do Campo.
	struct tp_table *next;			// Encadeamento para o próximo campo.
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

// Union's utilizados na conversão de variáveis do tipo inteiro e double.

union c_double{
	
	double dnum;
	char double_cnum[sizeof(double)];	
};

union c_int{
	
	int  num;
	char cnum[sizeof(int)];
};

/*
	Esta função tem por objetivo criar e inicializar uma estrutura do tipo tp_buffer
	que será usada para carregar tuplas na memória

*/
tp_buffer * initbuffer();

/*
	Esta função busca, no arquivo fs_object.dat, pelo nome da tabela retornando as informações que estão no dicionário em uma estrutura fs_objects
	Caso o nome da tabela não exista, o programa aborta 
	*nTabela - Nome da tabela a ser buscado no dicionário de dados
*/
struct fs_objects leObjeto(char *nTabela);

/*
	Esta função busca, no arquivo fs_schema.dat, pelas informações do objeto, carregando o esquema da tabela que é retornadado em tp_table
	Caso o nome da tabela não exista, o programa aborta 
	*objeto - Objeto, já previamente inicializado em leObjeto(nTabela), que contém as informações sobre uma determinada tabela
*/
tp_table *leSchema (struct fs_objects objeto);

/*
	Esta função calcula, usando o esquema e o dicionário de dados, o tamanho da tupla de uma tabela, retornando o mesmo.
	*esquema - Estrutura que contém o esquema da tabela (nome de campo, tipo de campo, etc)
	*objeto - Estrutura que contém informações sobre a tabela (nome da tabela, nome do arquivo da tabela, etc)

*/
int tamTupla(tp_table *esquema, struct fs_objects objeto);

/*
	Esta função imprime todos os dados carregados numa determinada página do buffer
	*buffer - Estrutura para armazenar tuplas na memória
	*s - Estrutura que armazena esquema da tabela para ler os dados do buffer
	*objeto - Estrutura que armazena dados sobre a tabela que está no buffer
	*num_page - Número da página a ser impressa
*/
int printbufferpoll(tp_buffer *buffpoll, tp_table *s,struct fs_objects objeto, int num_page);
/*
	Esta função insere uma tupla em uma página do buffer em que haja espaço suficiente. Retorna ERRO_BUFFER_CHEIO caso não haja espeço para a tupla
	*buffer - Estrutura para armazenar tuplas na meméria
	*from - Número da tupla a ser posta no buffer. Este número é relativo a ordem de inserção da tupla na tabela em disco
	*campos - Estrutura que armazena esquema da tabela para ler os dados do buffer
	*objeto - Estrutura que armazena dados sobre a tabela que está no buffer
*/
int colocaTuplaBuffer(tp_buffer *buffer, int from, tp_table *campos, struct fs_objects objeto);
/*
	Esta função conta quantas tabelas já estão inseridas dentro do dicionario, para poder colocar um código válido para a próxima tabela. Retorna a quantidade exata de tabelas.
*/
int quantidadeTabelas();
/*
	Esta função verifica se um nome de tabela já está inserido no dicionario. 
	Retorna:
		-> 1 se o nome existe no dicionario;
		-> 0 se existe no dicionário.
	*nomeTabela - Nome de uma tabela,  a qual deseja-se saber se existe no dicionario.
*/
int verificaNomeTabela(char *nomeTabela);
/*
	Esta função inicia um estrutura do tipo table, como nome de tabela passado. 
	Retorna:
		-> a estrutura do tipo table iniciada;
		-> ERRO_NOME_TABELA_INVALIDO se o nome passado já existir no dicionário.
	*nomeTabela - Nome de uma tabela, a qual deseja-se inserir.
*/
table *iniciaTabela(char *nomeTabela);
/*
	Esta função encadeia a lista de campos na estrutura de uma tabela que vai ser criada. 
	Retorna:
		-> a estrutura com a coluna inserida na lista.
	*t - Estrutura da tabela à ser criada.
	*nomeCampo - Nome do campo que irá ser inserido na lista de campos.
	tipoCampo - Tipo do campo que irá ser inserido na lista de campos.
	tamanhoCampo - Tamanho do campo que irá ser inserido na lista de campos.
*/
table *adicionaCampo(table *t,char *nomeCampo, char tipoCampo, int tamanhoCampo);
/*
	Esta função finaliza a tabela preveamente estrutura pelas funcoes iniciaTabela() e adicionaCampo(). 
	Escreve nos arquivos fs_object.dat e fs_schema.dat, a estrutura passada.
	Retorna: 
		-> SUCCESS quando teve sucesso na sua operaçãoç;
		-> ERRO_ABRIR_ARQUIVO quando teve problemas ao abrir os arquivos fs_object.dat e fs_schema.dat;
		-> ERRO_PARAMETRO quando a estrutura passada é inválida.
	*t - Estrutura da tabela à ser criada.
*/
int finalizaTabela(table *t);
/*
	Esta função inicia e aloca dinâmicamente uma lista de valores que vão ser inseridos em uma tabela.
	Retorna: 
		-> estrutura iniciada e alocad com o valor passado por parâmetro.
		-> ERRO_DE_PARAMETRO, quando a estrutura enviada for inválida.
	*c - Estrutura de valores que vão ser inseridos em uma tabela.
	*nomeCampo - Nome do campo que o usuário vai inserir um valor.
	*valorCampo - Valor do campo que vai ser inserido.
*/
column *insereValor(column *c, char *nomeCampo, char *valorCampo);
/*
	Esta função finaliza a inserção de valores em uma tabela. Assume que o usuário entrou com todos os campos de uma tupla completa.
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
int finalizaInsert(char *nome, column *c);
/*
	Esta função recupera uma página do buffer e retorna a mesma em uma estrutura do tipo column
	A estrutura column possui informações de como manipular os dados
	*buffer - Estrutura para armazenar tuplas na meméria
	*campos - Estrutura que armazena esquema da tabela para ler os dados do buffer
	*objeto - Estrutura que armazena dados sobre a tabela que está no buffer
	*page - Número da página a ser recuperada (0 a PAGES)
*/
column * getPage(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page);
/*
	Esta função uma determinada tupla do buffer e retorna a mesma em uma estrutura do tipo column; 
	A estrutura column possui informações de como manipular os dados
	*buffer - Estrutura para armazenar tuplas na meméria
	*campos - Estrutura que armazena esquema da tabela para ler os dados do buffer
	*objeto - Estrutura que armazena dados sobre a tabela que está no buffer
	*page - Número da página a ser recuperada uma tupla (0 a PAGES)
	*nTupla - Número da tupla a ser excluida, este número é relativo a página do buffer e não a todos os registros carregados
*/
column * excluirTuplaBuffer(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, int nTupla);
