#include <ctype.h>
#include "shell.h"

void shell()
{
	char entrada[1000], nomeBD[TAM_NOME_BANCO], nomeaux[TAM_NOME_BANCO];
    int resultado=0, codDB=-1;
    nomeBD[0]='\0';

    char *current_db_name = ">";//inicializa com nenhum banco conectado

    while(1){       
        
        int nTokens;

        printf("%s",current_db_name);
        
        fgets(entrada,1000,stdin);

        char **tokens = tokenize(remove_newline(entrada),' ',&nTokens);
        
        if (strcmp(strtolower(tokens[0]),"create")==0)
        {

        	if(strcmp(strtolower(tokens[1]),"table")==0)
	        {
	           
	        }
	        else if(strcmp(strtolower(tokens[1]),"database")==0)
	        {
	     		if (nTokens != 3)
	     		{
	     			printf("Invalid number of arguments.\n");
	     			continue;
	     		}
	     		else if (strlen(tokens[2]) > TAM_NOME_BANCO )
	     		{
	     			printf("Database name too big.\n");
	     			continue;
	     		}

	            resultado = checkCreateDB(tokens[2]);//verifica a existência do nome e grava-o no arquivo
	            
	            if(resultado==-1) 
	            {
	                printf("Error creating database file.\n");
	            }
	            if(resultado==-3) 
	            {
	                printf("Database exist.\n");
	            }
	            else
	            {
	                printf("Database created successfully.\n");
	            }
	        } 
	        else
	       	{
	       		printf("Invalid command.\n");
	       	}   
        }
        else if(strcmp(strtolower(tokens[0]),"\\c") == 0){
                
            if (nTokens != 2)
            {
            	printf("Invalid number of arguments.\n");

            	continue;
            }

            codDB = busca(tokens[1],1);     //função chamada para conecção no banco, retorna o codigo do banco ao conectar
           
            if (codDB >= 0)
            {
                strcpy(nomeBD, tokens[1]);  //passa o nome do bd, para a variavel mostrar ao usuario qual o banco conectado
                current_db_name = strdup(tokens[1]);
                
                strcat(current_db_name,"=#"); 
            }
            else
            {
                printf("\nNo such database '%s'.\n", tokens[1]);
            }
        }
      
        else if(strcmp(strtolower(tokens[0]),"insert")==0)
        {
            insert(entrada);
        }
        else if(strcmp(strtolower(tokens[0]),"\\d")==0)
        {
        	if (nTokens != 1)
            {
            	printf("Invalid number of arguments.\n");

            	continue;
            }
           
        }   
        else if(strcmp(strtolower(tokens[0]),"\\l")==0)
        {
        	if (nTokens != 1)
            {
            	printf("Invalid number of arguments.\n");

            	continue;
            }
            //LISTA os bancos existentes
            resultado = busca(tokens[0],2);
            
            if(resultado==-1) 
            {
                printf("No database created.\n");
            }
        }   
        else if(strcmp(strtolower(tokens[0]),"exit")==0)
        {
            break;
        } 
        else
        {
        	printf("Invalid command.\n");
        }  
    }
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
  char *temp = (char*) malloc (sizeof(char) * strlen(str));

  int i, pos = 0;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] != '\n') {
      temp[pos++] = str[i];
    }
  }

  temp[pos] = '\0';

  return temp;
}