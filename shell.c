#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "table.h"

int current_database = -1;

void shell()
{


	current_database = -1;
	char entrada[1000],inicio[1000], nomeBD[TAM_NOME_BANCO],auxiliar[150];
    int resultado=0, codDB=-1;
    nomeBD[0]='\0';

    char *current_db_name = strdup(">");//inicializa com nenhum banco conectado
	
	printf("\nWelcome to our DBMS Interface\n");	
	
    while(1){       	
        
        fgets(inicio,1000,stdin);
        
		if(strcmp(inicio,"dbms-start\n") == 0){
			printf("\nInterface initialized\n");
			
			while(1){
		
				int nTokens;

				printf("%s",current_db_name);
				
				fgets(entrada,1000,stdin);

				char **tokens = tokenize(remove_newline(entrada),' ',&nTokens);
				
				/**
				 * Opção para criar tabela e banco de dados
				 */
				if (strcmp(strtolower(tokens[0]),"create")==0)
				{
					if (nTokens != 3)
					{
						printf("Invalid number of arguments. Type help to show de interface usage.\n");

						continue;
					}
					if(strcmp(strtolower(tokens[1]),"table")==0)
					{
						if (current_database == -1)
						{
							printf("Not connected to any database.\n");
							continue;
						}
						createTable(entrada,current_database);
					}
					else if(strcmp(strtolower(tokens[1]),"database")==0)
					{
						if (strlen(tokens[2]) > TAM_NOME_BANCO )
						{
							printf("Database name too big.\n");
							continue;
						}

						resultado = checkCreateDB( remove_semicolon(tokens[2]) );//verifica a existência do nome e grava-o no arquivo
						
						if(resultado==-1) 
						{
							printf("Error creating database file.\n");
						}
						if(resultado==-3) 
						{
							printf("Database exists.\n");
						}
						else
						{
							printf("Database created successfully.\n");
						}
					} 
					else
					{
						printf("Invalid command. Type help to show de interface usage.\n");
						continue;
					}   
				}
				/**
				 * Conecta ao banco de dados passado como parâmetro
				 */
				else if(strcmp(strtolower(tokens[0]),"\\c") == 0){
						
					if (nTokens != 2)
					{
						printf("Invalid number of arguments. Type help to show de interface usage.\n");

						continue;
					}
					char *name_db = remove_semicolon(tokens[1]);
					codDB = busca(name_db,1);     //função chamada para conecção no banco, retorna o codigo do banco ao conectar
				   
					if (codDB >= 0)
					{
						strcpy(nomeBD, name_db);  //passa o nome do bd, para a variavel mostrar ao usuario qual o banco conectado
						free(current_db_name);
						
						current_db_name = (char*) malloc (sizeof(char)*(strlen(name_db)+3));

						if (current_db_name == NULL)
						{
							printf("Out of memory.\nAborting...\n");
						}

						strcpy(current_db_name,name_db);
						current_database = codDB;

						strcat(current_db_name,"=#");
						current_db_name[strlen(current_db_name)] = '\0'; 
					}
					else
					{
						printf("No such database '%s'.\n", name_db);
						continue;
					}
				}
				/**
				 * Insere tuplas em uma tabela
				 */
				else if(strcmp(strtolower(tokens[0]),"insert")==0)
				{
					if (current_database == -1)
					{
						printf("Not connected to any database.\n");
						continue;
					}
					
					insert(entrada,current_database);
				}
				/**
				 * Imprime as tabelas do banco de dados atual
				 * ou o esquema de uma tabela
				 */
				else if(strcmp(strtolower(tokens[0]),"\\d")==0)
				{
					if (current_database == -1)
					{
						printf("Not connected to any database.\n");
						continue;
					}
					if (nTokens >= 3)
					{
						printf("Invalid number of arguments. Type help to show de interface usage.\n");

						continue;
					}

					if (nTokens == 1)
					{
						//imprime tabelas do banco de dados
						listaTabelas(current_database);
					}
					else
					{
						printf("NOME DA TABELA %s\n",(tokens[1]));
						//nome da tabelas contem o id do banco a que pertencem
						//char carac=codDB;
						
						/*opcao = leia do disco os nomes das tabelas e quebre os nomes testando cod do banco e nome da tabela
						  se algum algum for igual mande para o leObjeto e para o leSchema caso contr[ario mensagem de erro
						*/
						//strcat(auxiliar,carac);
						strcat(auxiliar,"_");
						strcat(auxiliar,tokens[1]);
						strcat(auxiliar,"\0");
						printf("auxiliar é %s",auxiliar);
						//struct fs_objects objeto = leObjeto(tokens[1]);//para verificar se a tabela esta no banco 
					
						//imprimir o objeto
						
						
						//leSchema (struct fs_objects objeto);
						//tp_table *esquema = leSchema(objeto);
						
						//printf("tipo %c", esquema[0].tipo);
						//printf("tipo %d", esquema[0].tam);
						
						/*
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

						 * */
					}
				   
				} 
				/**
				 * Comando temporário para imprimir tabela
				 */
				else if (strcmp(strtolower(tokens[0]),"select")==0)
				{
					selectTable(entrada,current_database);
				}
				/**
				 * Imprime os registros da tabela passada
				 */
				else if (strcmp(strtolower(tokens[0]),"show")==0)
				{
					if (nTokens != 2)
					{
						printf("Invalid number of arguments. Type help to show de interface usage.\n");

						continue;
					}
					if (current_database == -1)
					{
						printf("Not connected to any database.\n");
						continue;
					}

					char *t = table_name_real(remove_semicolon(tokens[1]),current_database);

					char *file = table_name_real(remove_semicolon(tokens[1]),current_database);
					strcat(file,".dat");
					
					if (existeArquivo(file) == 0)
					{
						printf("Table doesn't exist.\n" );
						continue;
					}
					imprime(t);
					free(file);
					free(t);
				}  
				/**
				 * Lista os bancos existentes
				 */
				else if(strcmp(strtolower(tokens[0]),"\\l")==0)
				{
					if (nTokens != 1)
					{
						printf("Invalid number of arguments. Type help to show de interface usage.\n");

						continue;
					}
					//LISTA os bancos existentes
					listaBancos();
				}   
				/**
				 * Opção para deletar o banco de dados e tabelas
				 */
				else if(strcmp(strtolower(tokens[0]),"drop")==0)
				{
					if (nTokens != 3)
					{
						printf("Invalid number of arguments. Type help to show de interface usage.\n");

						continue;
					}
					else if(strcmp(strtolower(tokens[1]),"table") == 0){
						if (current_database == -1)
						{
							printf("Not connected to any database.\n");
							continue;
						}
						
						char *t = table_name_real(remove_semicolon(tokens[2]),current_database);
						char *exist = table_name_real(remove_semicolon(tokens[2]),current_database);
						strcat(exist,".dat");
						
						if (existeArquivo(exist) == 0)
						{
							printf("Table doesn't exist.\n" );
							continue;
						}		
						
						excluirTabela(t);
						free(exist);
						free(t);
					}
					else if(strcmp(strtolower(tokens[1]),"database") == 0){
						if (current_database == -1)
						{
							printf("Not connected to database for drop.\n");
							continue;
						}
						
						char *exist = table_name_real(remove_semicolon(tokens[2]),current_database);
						strcat(exist,".dat");
						
						if (existeArquivo(exist) != 0)
						{
							printf("The database is not empty for drop, there are existing tables.\n" );
							continue;
						}
						
						dropDatabase(remove_semicolon(tokens[2]));
						
						free(exist);
					}
				}
				/**
				 * Ajuda ao usuário com exemplos da sintaxe dos comandos
				 */
				else if (strcmp(strtolower(tokens[0]),"help")==0)
				{
					if (nTokens != 1)
					{
						printf("Invalid number of arguments. Type help to show de interface usage.\n");
					}

					help();
				}
				/**
				 * Imprime mensagem de copyright
				 */
				else if(strcmp(strtolower(remove_semicolon(tokens[0])),"\\copyright")==0)
				{
					printf("\nDatabase Management System\n");
					printf("\nPermission to use, copy, modify, and distribute this software and its\ndocumentation for any purpose, without fee, and without a written agreement\nis hereby granted, provided that the above copyright notice and this\nparagraph and the following two paragraphs appear in all copies.\n");
					printf("\nTHIS SOFTWARE IS BEING DEVELOPED BY STUDENTS OF DATABASE II CLASS AT UNIVERSIDADE FEDERAL DA FRONTEIRA SUL.\n\n");	
				}
				/**
				 * Comando de saída
				 */
				else if(strcmp(strtolower(remove_semicolon(tokens[0])),"exit")==0)
				{
					strcpy(inicio,"exit\n");
					break;
				} 
				else if(strcmp(strtolower(remove_semicolon(tokens[0])),"quit")==0)
				{
					strcpy(inicio,"exit\n");
					break;
				}
				else if(strcmp(strtolower(remove_semicolon(tokens[0])),"bye")==0)
				{
					strcpy(inicio,"exit\n");
					break;
				}
				else if(strcmp(strtolower(remove_semicolon(tokens[0])),"\\q")==0)
				{
					strcpy(inicio,"exit\n");
					break;
				}
				else
				{
					printf("Invalid command. Type help to show de interface usage.\n");
					continue;
				}
			}  
		}
		else if(strcmp(inicio,"exit\n") == 0) 
		{
			break;
		}
		else
		{
			printf("Invalid command. Type dbms-start to inicializate the interface or type exit.\n");
			continue;
		}

		if(strcmp(inicio,"exit\n") == 0) 
		{
			break;
		}
		else
		{
			printf("Invalid command. Type help to show de interface usage.\n");
			continue;
		}	
	}

	free(current_db_name);
}

/**
 * Funções auxiliares para quebrar string de entrada
 * Retornar somente caracteres mininusculos
 */

char *strtolower(char *str)
{
  char *temp = strdup(str);
  int i;

  for (i = 0; i < strlen(temp) && temp[i] != '\0'; i++)
  {
    temp[i] = tolower(temp[i]);
  }

  return temp;
}

int count_ocurrences(char *str, char delim)
{
  int i, count = 0;
  for (i = 0; i < strlen(str); i++)
  {
    if (str[i] == delim) {
      count++;
    }
  }

  return count;
}

char **tokenize(char *str, char delim, int *size)
{
  int ocurrences = count_ocurrences(str, delim);
  *size = ocurrences + 1;
  char **tokens = (char**) malloc (sizeof(char*) * (ocurrences + 1));

   if (tokens == NULL)
  {
  	printf("Out of memory.\nAborting...\n");
  	abort();
  }

  int current = 0, pos = 0, i;
  char accumulate[2000];

  for (i = 0; i < strlen(str); i++)
  {
    if (str[i] == delim) {
      accumulate[pos] = '\0';
      tokens[current] = strdup(accumulate);
      current++;
      pos = 0;
      continue;
    }

    accumulate[pos++] = str[i];
  }

  accumulate[pos] = '\0';
  tokens[current] = strdup(accumulate);

  return tokens;
}

char *remove_newline(char *str)
{
  char *temp = (char*) malloc (sizeof(char) * (strlen(str)+1) );

  if (temp == NULL)
  {
  	printf("Out of memory.\nAborting...\n");
  	abort();
  }

  int i, pos = 0;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] != '\n') {
      temp[pos++] = str[i];
    }
  }

  temp[pos] = '\0';

  return temp;
}


char *remove_semicolon(char *str)
{
  char *temp = (char*) malloc (sizeof(char) * (strlen(str)+1));

  if (temp == NULL)
  {
  	printf("Out of memory.\nAborting...\n");
  	abort();
  }

  int i, pos = 0;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] != ';') {
      temp[pos++] = str[i];
    }
  }

  temp[pos] = '\0';

  return temp;
}

void example()
{
	int ok = checkCreateDB("UFFS");//cria banco caso não exista

	current_database = busca("UFFS",1);

	if (!ok)
	{	
	    int nrTabelas = 3;
	    int nTabela[nrTabelas];
	    table  *tab[nrTabelas]; 
	    column *colunas;
	    int object, schema;
	    
	    object      = existeArquivo("fs_object.dat");
	    schema      = existeArquivo("fs_schema.dat");
	   
	    nTabela[0]  = existeArquivo(table_name_real("Aluno",current_database));
	    
	    nTabela[1]  = existeArquivo(table_name_real("Inst",current_database));

	    nTabela[2]  = existeArquivo(table_name_real("Inscri",current_database));

	    
	    if(!object || !schema)
	    {
	        if(!nTabela[0]){ 
	    		
	            tab[0] = iniciaTabela(table_name_real("Aluno",current_database));                //Cria a tabela 
	            tab[0] = adicionaCampo(tab[0], "CPF"     , 'I', (sizeof(int))   ,PK,"","");     //Cria os atributos
	            tab[0] = adicionaCampo(tab[0], "Nome"    , 'S', 20              ,NPK,"","");        
	            tab[0] = adicionaCampo(tab[0], "Endereco", 'S', 20              ,NPK,"","");
	            tab[0] = adicionaCampo(tab[0], "Peso"    , 'D', (sizeof(double)),NPK,"","");
	            finalizaTabela(tab[0],current_database);
	            
	         }
	         if(!nTabela[1]){   
	         	
	            tab[1] = iniciaTabela(table_name_real("Inst",current_database)); 
	            tab[1] = adicionaCampo(tab[1], "CodInst"  , 'I', (sizeof(int))   ,PK , "","");
	            tab[1] = adicionaCampo(tab[1], "Nome"     , 'S', 20              ,NPK, "","");
	            tab[1] = adicionaCampo(tab[1], "Endereco" , 'S', 20              ,NPK, "","");
	            tab[1] = adicionaCampo(tab[1], "Reitor"   , 'S', 10              ,NPK, "","");
	            finalizaTabela(tab[1],current_database);
	        }
	        if(!nTabela[2]){
	        	
	            tab[2] = iniciaTabela(table_name_real("Inscri",current_database)); 
	            tab[2] = adicionaCampo(tab[2], "CodMat"     , 'I', (sizeof(int))  ,PK, "","");
	            tab[2] = adicionaCampo(tab[2], "CPF"        , 'I', (sizeof(int))  ,FK, table_name_real("Aluno",current_database),"CPF");
	            tab[2] = adicionaCampo(tab[2], "CodInst"    , 'I', (sizeof(int))  ,FK , table_name_real("Inst",current_database),"CodInst");
	            tab[2] = adicionaCampo(tab[2], "Curso"   , 'S',  20  ,NPK,"","");
	            finalizaTabela(tab[2],current_database);
	        }
	         
	         //Inserção de tuplas na tabela1   
	        colunas = NULL;     
	        colunas = insereValor(tab[0],colunas, "CPF", "123456");
	        colunas = insereValor(tab[0],colunas, "Nome", "Rogerio");
	        colunas = insereValor(tab[0],colunas, "Endereco", "Rua Marechal");
	        colunas = insereValor(tab[0],colunas, "Peso", "81.4");
	        finalizaInsert(table_name_real("Aluno",current_database), colunas); 
	        
	        colunas = NULL;     
	        colunas = insereValor(tab[0],colunas, "CPF", "654321");
	        colunas = insereValor(tab[0],colunas, "Nome", "Ricardo");
	        colunas = insereValor(tab[0],colunas, "Endereco", "RuaClevela");
	        colunas = insereValor(tab[0],colunas, "Peso", "88.9");
	        finalizaInsert(table_name_real("Aluno",current_database), colunas); 

	        colunas = NULL;     
	        colunas = insereValor(tab[0],colunas, "CPF", "1234567");
	        colunas = insereValor(tab[0],colunas, "Nome", "Natan");
	        colunas = insereValor(tab[0],colunas, "Endereco", "RuaDelmi");
	        colunas = insereValor(tab[0],colunas, "Peso", "58.9");
	        finalizaInsert(table_name_real("Aluno",current_database), colunas); 
	            
	        
	        //Inserção de tuplas na tabela2 
	        colunas = NULL;
	        colunas = insereValor(tab[1],colunas, "CodInst", "111");
	        colunas = insereValor(tab[1],colunas, "Nome", "UFFS");
	        colunas = insereValor(tab[1],colunas, "Endereco", "RuadeTerra");
	        colunas = insereValor(tab[1],colunas, "Reitor", "MandaChuva");
	        finalizaInsert(table_name_real("Inst",current_database), colunas);
	        
	        colunas = NULL;
	        colunas = insereValor(tab[1],colunas, "CodInst", "222");
	        colunas = insereValor(tab[1],colunas, "Nome", "CEFET");
	        colunas = insereValor(tab[1],colunas, "Endereco", "RuadePedra");
	        colunas = insereValor(tab[1],colunas, "Reitor", "MandaVento");
	        finalizaInsert(table_name_real("Inst",current_database), colunas);

	        colunas = NULL;
	        colunas = insereValor(tab[1],colunas, "CodInst", "333");
	        colunas = insereValor(tab[1],colunas, "Nome", "UNOESC");
	        colunas = insereValor(tab[1],colunas, "Endereco", "RuadeAsfal");
	        colunas = insereValor(tab[1],colunas, "Reitor", "MandaAgua");
	        finalizaInsert(table_name_real("Inst",current_database), colunas);
	        
	        
	        
	        //Inserção de tupla na tabela3
	        colunas = NULL;
	        colunas = insereValor(tab[2],colunas, "CodMat", "1401");
	        colunas = insereValor(tab[2],colunas, "CPF", "123456");
	        colunas = insereValor(tab[2],colunas, "CodInst", "333");
	        colunas = insereValor(tab[2],colunas, "Curso", "CC");
	        finalizaInsert(table_name_real("Inscri",current_database), colunas);
	        
	        colunas = NULL;
	        colunas = insereValor(tab[2],colunas, "CodMat", "1402");
	        colunas = insereValor(tab[2],colunas, "CPF", "654321");
	        colunas = insereValor(tab[2],colunas, "CodInst", "222");
	        colunas = insereValor(tab[2],colunas, "Curso", "CC");
	        finalizaInsert(table_name_real("Inscri",current_database), colunas);
	        
	        colunas = NULL;
	        colunas = insereValor(tab[2],colunas, "CodMat", "1403");
	        colunas = insereValor(tab[2],colunas, "CPF", "1234567");
	        colunas = insereValor(tab[2],colunas, "CodInst", "111");
	        colunas = insereValor(tab[2],colunas, "Curso", "ADM");
	        finalizaInsert(table_name_real("Inscri",current_database), colunas);

	    }
	}
  
    current_database = -1;//não existe nenhum banco logado
}


char *table_name_real(char *name,int database)
{
	char *table_name = (char*) malloc (sizeof(char)*1000);

	if (table_name == NULL)
	{
		printf("Out of memory.\n");
		abort();
	}

	sprintf(table_name,"%d",database);
    strcat(table_name,"_");
    strcat(table_name,name);

    return table_name;
}


void help()
{
	printf("\nDBMS Interface Help\n");
	printf("Here is a list of commands you can enter:\n\n");
	printf("dbms-start - start the database\n");
	printf("help - Shows this help\n");
	printf("exit - Exits DBMS interface\n");
	printf("\\c <database_name> - Connects to a database\n");
	printf("\\d - Lists all tables in the connected database\n");
	printf("\\l - Lists all databases\n");
	printf("show <table_name> - Displays the table's data\n");
	
	printf("\nFor database creation and insertion on tables, you can type standard SQL queries, like in the examples below.\n");
	printf("\nDatabase creation example:\n");
	printf("\t create database example_db;\n");
	printf("\nTable insertion example:\n");
	printf("\t insert into table1 values(\"val1\", 12.5), (\"val2\", 55.5);\n\n");
	
}
