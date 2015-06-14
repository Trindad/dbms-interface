#include "shell.h"

void shell()
{
	char entrada[1000], *dividido=NULL, operacao[12], nomeBD[TAM_NOME_BANCO], nomeaux[TAM_NOME_BANCO];
    int resultado=0, codDB=-1;
    nomeBD[0]='\0';

    char *current_db_name = ">";//inicializa com nenhum banco conectado

    while(1){       
        
        printf("%s",current_db_name);
        
        fgets(entrada,1000,stdin);

        char *temp = strdup(entrada);
        dividido = strtok(temp," ");

        strcpy(operacao,dividido);
        
        if (strcmp(operacao,"create\0")==0)
        {
        	if(strcmp(operacao,"table\0")==0)
	        {
	           
	        }
	        else if(strcmp(operacao,"database\0")==0)
	        {
	            printf("deu certo %s\n",operacao);//o nome dado ao banco será guardado
	            dividido = strtok('\0'," ");
	            printf("sobrou  %s\n",dividido);
	            resultado = checkCreateDB(dividido);//verifica a existência do nome e grava-o no arquivo
	            printf("%d\n",resultado);
	            
	            if(resultado==-1) 
	            {
	                fprintf(stderr,"erro ao tentar criar arquivo do banco de dados %s\n",dividido);
	            }
	            if(resultado==-3) 
	            {
	                fprintf(stderr, "Banco já existe com este nome %s\n",dividido);
	            }
	            else
	                printf("Database created successfully\n");
	        } 
	        else
	       	{
	       		printf("Invalid command\n");
	       	}   
        }
        
        else if(strcmp(operacao,"connect\0")==0){
            //CRIAR AQUI PARTE DO CODIGO QUE IDENTIFICARA QUE A OPERAÇÃO É CONECTDB E CHAMAA A RESPECTIVA FUNÇÃO PRA FAZER ISSO
                    
            dividido = strtok('\0'," ;");               //apaga a palavra conectdb e passa para dividido a sobra, que seria o nome do bd
           
            strcat(dividido,"\n");                      //concatena o \n para comparação na conectdb
            strcpy(nomeaux, dividido);                  //só copia o nome do banco para nomeaux, para depois impressao
                
            codDB = busca(dividido,1);                  //função chamada para conecção no banco, retorna o codigo do banco ao conectar
            printf("%d",codDB);

            if (codDB >= 0)
            {
                strcpy(nomeBD, nomeaux);                //passa o nome do bd, para a variavel mostrar ao usuario qual o banco conectado
                current_db_name = strdup(nomeaux);
                printf("%s\n",nomeaux);
                strcat(current_db_name,"=#"); 
            }
            else
            {
                printf("\nBanco %s não cadastrado!\n", nomeaux);
            }
        }
      
        else if(strcmp(operacao,"insert\0")==0)
        {
            insert(entrada);
        }
        else if(strcmp(operacao,"\\d\0")==0)
        {
            //CRIAR AQUI PARTE DO CODIGO QUE IDENTIFICARA QUE A OPERAÇÃO É *D E CHAMAA A RESPECTIVA FUNÇÃO PRA LISTAR TABELAS
        }   
        else if(strcmp(operacao,"\\l\n")==0)
        {
            //LISTA os bancos existentes
            resultado = busca(operacao,2);
            
            if(resultado==-1) 
            {
                printf("Não existem bancos cadastrados.\n");
            }
        }   
        else if(strcmp(operacao,"exit\n")==0)
        {
            break;
        } 
        else
        {
        	fprintf(stderr, "Comando inválido %s\n",entrada);
        }  
    }
}