Link github
	https://github.com/Trindad/PK_FK
Universidade Federal da Fronteira Sul – UFFS 
Curso: Ciência da Computação
Componente Curricular: Banco de Dados II
Professor: Dr. Denio Duarte
Alunos: EDIMAR ROQUE MARTELLO JUNIOR, GEOVANI KOSTULSKI, LEONARDO BELINSKI, SILVANA TRINDADE

===============================================================================

Introdução:

       A seguinte parte do trabalho é destinada a criação de uma interface ao sgbd que está sendo desenvolvida pelas turmas de banco da dados 2.

Funcionamento:

A interface foi escrita no arquivo shell.c .O usuário digita o comando desejado e a interface chama as funções de interpretação. o comando de insert é tratado com o BISON.

A partir dessa versão existe a possibilidade da criação de vários bancos, listagem dos bancos, de tabelas do sgbd e de inserção de dados via interface.
A interface é inicializada no começo do arquivo exemploMain.c com a chamada da função shell(); A partir daí os comandos podem ser digitados.
Para seu desenvolvimento foi utilizado como referência o  SGBD PostgresSQL. Os comandos interpretados pelo SGBD podem ser minúsculos ou maiúsculos, portanto pode se ter CREATE DATABASE my_db ou create datebase my_db, para isso foi desenvolvido função para converter os comandos de entrada para minúsculo (strtolower() -- que recebe como entrada a string que deseja ser convertida, retornando a string convertida para minúsculo, esta função esta implementada em shell.c).

*Alterações estruturas dos trabalhos anteriores:
       Algumas estruturas sofreram alterações para que o programa suportasse a anexação de tabelas a banco de dados, permitindo a criação de tabelas com o mesmo nome em bancos de dados diferentes.
       As seguintes alterações foram realizadas:

- Adicionada a estrutura (struct) fs_database
typedef struct fs_database{//Estrutura que guarda os nomes dos bancos
	char nome[TAMANHO_NOME_DATABASE];	
	int cod;	//código do banco
} fs_database;
 
- Adicionado um elemento da estrutura fs_database na estrutura fs_objects
  struct fs_objects { // Estrutura usada para carregar fs_objects.dat
 	char nome[TAMANHO_NOME_TABELA];
 	int cod;	
 char nArquivo[TAMANHO_NOME_ARQUIVO];
 	int qtdCampos;
	fs_database bd; //banco de dados ao qual o objeto pertence
 };
 
- Adicionado um elemento da estrutura fs_objects na estrutura tp_table
typedef struct tp_table{ // Estrutura usada para carregar fs_schema.dat
 	char nome[TAMANHO_NOME_CAMPO]; 
 	char tipo;
 	int tam;	
 	int chave;
 	char tabelaApt[TAMANHO_NOME_TABELA]; 
 	char attApt[TAMANHO_NOME_CAMPO];
 	struct tp_table *next;
	struct fs_objects objetos; //objetos que pertence a um esquema
 }tp_table;

	Foi criada a estrutura fs_database para a criação de banco de dados, onde é possível anexar cada banco criado a um índice (cod) para a sua identificação e de suas tabelas.
	Para carregar os objetos das tabelas foi anexada a estrutura fs_objects um elemento que representa a estrutura fs_database, assim anexando o banco a tabela, e permitindo que seja identificada se uma tabela pertence a um determinando banco.
	Para anexar o esquema das tabelas aos bancos, foi adicionado um elemento (struct fs_objects objetos) na estrutura tp_table que representa a estrutura fs_objects (objetos da tabela) na estrutura tp_table.

COMPILAR:
1º Passo
	Estar no diretório root do projeto
2º Passo
	Entrar no diretório sintatic, por exemplo
		cd insert/sintatic
	Em seguida 
		lex c.l
		bison c.y
		gcc c.tab.c -o bf -g -ll -ly
3° Passo
	Voltar ao diretório root
4º Passo
	Compilar com GCC
		gcc *.c insert/semantic/semantic.c -o <nome_executavel> -g -ll -ly
5º Passo 
	Executar
		./<nome_executavel>

SHELL
	Conectar no banco
		\c <nome_db>
	Criar banco
		create database <nome_banco>
	Listar bancos
		\l 
	Listar tabelas
		\d
	Mostrar dados de uma tabela
		show <nome_tabela>
	Inserção
		Segue sintaxe SQL normal
		Exemplos
			insert into Aluno (CPF,Nome) values (0013,"Mary");

			insert into Aluno (CPF,Nome,Endereco,Peso) values (0444, "Joca", "Rua dos martins", 258.0), (033, "Mariano", "Avenida Getulio Prestes", 110.5), (055, "Emiliano", "Fernando picareta", 55.5);

	Sair do Shell
		exit

	