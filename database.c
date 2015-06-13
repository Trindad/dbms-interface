#include "database.h"

int cod_id_banco(){

    int cod_id = 0;
    FILE *file;
    char ch = '\0';
    db *database = (db *) malloc (sizeof(db));

    if (database == NULL)
    {
        fprintf(stderr, "Alocação de memória para o banco de dados falhou.\n");
        exit(1);
    }

    file = fopen("fs_database.dat","r");
    if(file != NULL){
        while(ch!=EOF){
            ungetc(ch,file);
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);
            cod_id++;
            ch = getc(file);
        }
    }
    else printf("\nErro leitura arquivo fs_database");
    fclose(file);
    return cod_id;
}

//busca por nome do banco se estiver cadastrado retorna a id dele
int busca(char *str, int identificacao){//a identificacao indicara qual if será executado
    
    //strcat(str,"\n");
    int i;
    
    for(i=0;i<strlen(str);i++){
            if(str[i]=='\0')        
                    printf(",");
            else
                printf("%c",str[i]);
                    
    }
   
    FILE *file;
    db *database = (db *)malloc(sizeof(db));
    
    if(!existeArquivo("fs_database.dat"))
        return -1;
    
    int cod= (cod_id_banco()), cont = 1;

    file = fopen("fs_database.dat", "r");  
         
    if(file != NULL){
       for(; cont < cod; cont++){
           
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);

            if(identificacao==1){
                    /*for(i=0;i<strlen(database->nome);i++){
                        if(database->nome[i]=='\n')
                            printf(",");
                        else
                            printf("%c",database->nome[i]);
                    }*/

                if(!strcmp(str,database->nome)){
                    //printf("\n--> banco encontrado\n");
                    fclose(file);
                    return database->cod;
                }
            }
            
            if(identificacao==2)
                printf("\nBanco de dados: %s", database->nome);
       }
    }
    else 
        printf("\nImpossivel acessar fs_database!\n");
    
    fclose(file);
    return -2;
}

//criar banco
void grava_banco(char *str){

    FILE *file;
    db *database = (db *)malloc(sizeof(db));

    file = fopen("fs_database.dat", "a+");
    if(file != NULL){
        database->cod = cod_id_banco();                                 //id proximo banco
        strcpy(database->nome,str);
        fwrite(&database->cod, sizeof(int), 1, file);
        fwrite(&database->nome, sizeof(char), TAM_NOME_BANCO, file);
        fclose(file);
    }
}

int checkCreateDB(char *nome){
    
    int resultado=0;
    
    if(!existeArquivo("fs_database.dat")){//se o arquivo com os nomes dos bancos não existir ele é criado.
        grava_banco(nome);
        return 0;
    }
    
    //verificando se o nome do banco já existe em fs_database.dat
    resultado = busca(nome,1);//se encontrar o banco já cadastrado retorna seu id
    
    if(resultado==-2)//retorno de funcao indicando sucesso
        grava_banco(nome);
        
    else
        return -3;
    
    return 0;
    
}