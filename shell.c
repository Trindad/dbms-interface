#include <ctype.h>
#include "shell.h"

void shell()
{
	char entrada[1000], *dividido=NULL, operacao[12], nomeBD[TAM_NOME_BANCO], nomeaux[TAM_NOME_BANCO];
    int resultado=0, codDB=-1;
    nomeBD[0]='\0';

    char *current_db_name = ">";//inicializa com nenhum banco conectado

    while(1){       
        
        int nTokens;

        printf("%s",current_db_name);
        
        fgets(entrada,1000,stdin);

        char **tokens = tokenize(entrada,' ',&nTokens);
        
        if (strcmp(strtolower(tokens[0]),"create\0")==0)
        {

        	if(strcmp(strtolower(tokens[1]),"table\0")==0)
	        {
	           
	        }
	        else if(strcmp(strtolower(tokens[1]),"database\0")==0)
	        {
	            printf("deu certo %s\n",tokens[0]);//o nome dado ao banco será guardado
	            dividido = strtok('\0'," ");
	            printf("sobrou  %s\n",dividido);
	            resultado = checkCreateDB(dividido);//verifica a existência do nome e grava-o no arquivo
	            printf("%d\n",resultado);
	            
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
        else if(strcmp(strtolower(tokens[0]),"connect\0") == 0){
                
            codDB = busca(tokens[1],1);     //função chamada para conecção no banco, retorna o codigo do banco ao conectar
            printf("%d",codDB);

            if (codDB >= 0)
            {
                strcpy(nomeBD, tokens[1]);  //passa o nome do bd, para a variavel mostrar ao usuario qual o banco conectado
                current_db_name = strdup(tokens[1]);
                printf("%s\n",tokens[1]);
                strcat(current_db_name,"=#"); 
            }
            else
            {
                printf("\nNo such database '%s'.\n", tokens[1]);
            }
        }
      
        else if(strcmp(strtolower(tokens[0]),"insert\0")==0)
        {
            insert(entrada);
        }
        else if(strcmp(strtolower(tokens[0]),"\\d\0")==0)
        {
           
        }   
        else if(strcmp(strtolower(tokens[0]),"\\l\n")==0)
        {
            //LISTA os bancos existentes
            resultado = busca(tokens[0],2);
            
            if(resultado==-1) 
            {
                printf("No database created.\n");
            }
        }   
        else if(strcmp(strtolower(tokens[0]),"exit\n")==0)
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
