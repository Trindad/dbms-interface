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
	char entrada[1000], nomeBD[TAM_NOME_BANCO],auxiliar[150];
    int resultado = 0, codDB = -1;
    nomeBD[0]='\0';

    char *current_db_name = strdup(">");//inicializa com nenhum banco conectado
	char *start;
    
    start = strdup("dbms-start");//este comando posteriormente como start do banco, no momento ele é automatico
	printf("\nWelcome to the DBMS Interface.\nType 'help' '\\h' for help.\n\n");	

	/**
	 * ****************************
	 * 
	 *   Comandos do shell
	 *
	 * ****************************
	 */
	using_history ();//função para usar o histórico
	read_history (".history_file");

	while(1)
	{
		int nTokens;

		strcpy(entrada, readline(current_db_name));

		/**
		 * Adiciona ao histórico
		 */
		if (entrada[0])
        {
			char *expansion;
			int result;

			result = history_expand (entrada, &expansion);
			if (result)
			fprintf (stderr, "%s", expansion);

			if (result < 0 || result == 2)
			{
			  free (expansion);
			  continue;
			}

			add_history (expansion);
			strncpy (entrada, expansion, sizeof (entrada) - 1);
			free (expansion);

			write_history (".history_file");//adiciona no histórico
        }

		char **tokens = tokenize( trim_white_space(remove_newline(entrada)),' ',&nTokens);
		
		/**
		 * Opção para criar tabela e banco de dados
		 */
		if (strcmp(strtolower(tokens[0]),"create")==0)
		{
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
				if (nTokens >= 5)
				{
					printf("Invalid command. Type help to show de interface usage.\n");
					continue;
				}
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
				printf("Invalid number of arguments. Type help to show the interface usage.\n");

				continue;
			}
			char *name_db = remove_semicolon(tokens[1]);
			codDB = busca(name_db,1); //função chamada para conecção no banco, retorna o codigo do banco ao conectar
		
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
				printf("Invalid number of arguments. Type help to show the interface usage.\n");

				continue;
			}
			else if (nTokens == 1)
			{
				//imprime tabelas do banco de dados
				listaTabelas(current_database);
			}
			else
			{
				//imprime o esquema de uma tabela
				char *t = table_name_real(remove_semicolon(tokens[1]),current_database);

				if(!verificaNomeTabela(t)){
			        printf("Invalid table name.\n");
					free(t);
			        continue;			    
			    }

				struct fs_objects objeto = leObjeto(t);//para verificar se a tabela esta no banco 						
				

				show_schema(objeto,tokens[1]);

				free(t);
			}
		   
		} 
		/**
		 * Comando temporário para imprimir tabela
		 */
		else if (strcmp(strtolower(tokens[0]),"select")==0)
		{
			if (current_database == -1)
			{
				printf("Not connected to any database.\n");
				continue;
			}
			
			selectTable(entrada,current_database);
		}
		/**
		 * Imprime os registros da tabela passada
		 */
		else if (strcmp(strtolower(tokens[0]),"show")==0)
		{
			if (nTokens != 2)
			{
				printf("Invalid number of arguments. Type help to show the interface usage.\n");

				continue;
			}
			if (current_database == -1)
			{
				printf("Not connected to any database.\n");
				continue;
			}
			if (verificaNomeTabela(table_name_real(remove_semicolon(tokens[1]),current_database) ) == 0 )
			{
				printf("Table %s doesn't exist.\n",remove_semicolon(tokens[1]));
				continue;
			}

			char *t = table_name_real(remove_semicolon(tokens[1]),current_database);

			char *file = table_name_real(remove_semicolon(tokens[1]),current_database);
			strcat(file,".dat");
			
			if (existeArquivo(file) == 0)
			{
				printf("Table is empty.\n" );
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
				printf("Invalid number of arguments. Type help to show the interface usage.\n");

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
				printf("Invalid number of arguments. Type help to show the interface usage.\n");

				continue;
			}
			else if(strcmp(strtolower(tokens[1]),"table") == 0){
				
				if (current_database == -1)
				{
					printf("Not connected to any database.\n");
					continue;
				}
				if (verificaNomeTabela(table_name_real(remove_semicolon(tokens[2]),current_database) ) == 0 )
				{
					printf("Table %s doesn't exist.\n",remove_semicolon(tokens[2]));
					continue;
				}
				
				char *t = table_name_real(remove_semicolon(tokens[2]),current_database);
				char *exist = table_name_real(remove_semicolon(tokens[2]),current_database);
			
				int ok = excluirTabela(t);

				if (ok == SUCCESS)
				{
					printf("Table deleted successfully.\n");
				}

				free(exist);
				free(t);
			}
			else if(strcmp(strtolower(tokens[1]),"database") == 0){
				
				char *exist = table_name_real(remove_semicolon(tokens[2]),current_database);
				strcat(exist,".dat");
				
				if (existeArquivo(exist) != 0)
				{
					printf("The database is not empty for drop, there are existing tables.\n" );
					continue;
				}
				
				exist = remove_semicolon(tokens[2]);
				codDB = busca(exist,1);
				
				if(codDB == current_database)
				{
                	printf("Cannot drop the currently open database.\n");
                    continue;
				}

				int drop = dropDatabase(remove_semicolon(tokens[2]));

				if(drop == 1)printf("Database deleted successfully.\n");
                
                free(exist);
			}
		}
		/**
		 * Ajuda ao usuário com exemplos da sintaxe dos comandos
		 */
		else if (strcmp(strtolower(tokens[0]),"help")==0 || strcmp(strtolower(tokens[0]),"\\h")==0)
		{
			if (nTokens != 1)
			{
				printf("Invalid number of arguments. Type help to show the interface usage.\n");
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
			break;
		} 
		else if(strcmp(strtolower(remove_semicolon(tokens[0])),"quit")==0)
		{
			break;
		}
		else if(strcmp(strtolower(remove_semicolon(tokens[0])),"bye")==0)
		{
			break;
		}
		else if(strcmp(strtolower(remove_semicolon(tokens[0])),"\\q")==0)
		{
			break;
		}
		else
		{
			printf("Invalid command. Type help to show the interface usage.\n");
			continue;
		}
	}  

	free(start);
	free(current_db_name);
}

char *trim_white_space(char *str)
{
  char *end;

  while(isspace(*str)) str++;

  if(*str == 0)  
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  //novo \0
  *(end+1) = 0;

  return str;

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
	printf("help - Shows this help\n");
	printf("\\h - Shows this help\n");
	printf("exit - Exits DBMS interface\n");
	printf("bye - Exits DBMS interface\n");
	printf("quit - Exits DBMS interface\n");
	printf("\\q - Exits DBMS interface\n");
	printf("\\c <database_name> - Connects to a database\n");
	printf("\\d - Lists all tables in the connected database\n");
	printf("\\d <table_name> - Display schema of table\n");
	printf("\\l - Lists all databases\n");
	printf("show <table_name> - Displays the table's data\n");
	printf("select * from <table_name> - Displays the table's data\n");
	printf("drop <database_name> - Drops database\n");
	printf("drop <table_name> - Drops table\n");
	
	printf("\nFor database creation and insertion on tables, you can type standard SQL queries, like in the examples below.\n");
	printf("\nDatabase creation example:\n");
	printf("\t create database example_db;\n");
	printf("\nTable insertion example:\n");
	printf("\t insert into table1 values(\"val1\", 12.5), (\"val2\", 55.5);\n\n");
	printf("Table creation example:\n");
	printf("\t create table table1 (column1 string(100), column2 double, constraint primary key (column1) , constraint foreign key ( column2 ) references table2 (column2) );");
	printf("\n");
}
