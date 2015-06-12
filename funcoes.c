#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_NOME_BANCO 30

typedef struct db_db{

    int cod;
    char nome[TAM_NOME_BANCO];

}db;

//verifica id do novo banco
int cod_id_banco(){

    int cod_id = 0;
    FILE *file;
    char ch;
    db *database = (db *)malloc(sizeof(db));

    file = fopen("fs_database.dat","r");
    if(file != NULL){
        while(ch!=EOF){
            ungetc(ch,file);
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);
            printf("\nNome: %s", database->nome);
            cod_id++;
            ch = getc(file);
        }
    }
    else printf("\nErro leitura arquivo fs_database");
    fclose(file);
    return cod_id;
}

//busca por nome do banco se estiver cadastrado retorna a id dele
int busca(char *str){

    strcat(str,"\n");

    FILE *file;
    db *database = (db *)malloc(sizeof(db));
    int cod= (cod_id_banco()-1), cont = 1;

    //cod =
    file = fopen("fs_database.dat", "r");
    if(file != NULL){
       for(; cont < cod; cont++){
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);
            printf("\nNome: %s", database->nome);
            if(!strcmp(str,database->nome)){
                fclose(file);
                return database->cod;
            }
       }
    }
    else printf("\nBanco %s nao esta cadastrado!\n", str);
    fclose(file);
    return 0;
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
