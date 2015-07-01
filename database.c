#include "database.h"

int cod_id_banco(){

    int cod_id = 0;
    FILE *file;
    char ch = '\0';
    db *database = (db *) malloc (sizeof(db));

    if (database == NULL)
    {
        fprintf(stderr, "Out of memory.\n");
        abort();
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
    else printf("\nCannot read fs_database");
    fclose(file);
    return cod_id;
}

void reescreve(char *str)
{
    FILE *file;
    int quant_db = cod_id_banco(), aux = 0, cont = 0;
    db *database = (db *)malloc(sizeof(db));
    db *aux_arq = (db *)malloc(sizeof(db)*quant_db);

    if (database == NULL)
    {
        printf("Out of memory.\n");
        abort();
    }
    file = fopen("fs_database.dat", "r");
    if(file != NULL)
    {
       for(; cont < (quant_db-1); cont++){
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);
            if(strcmp(database->nome,str)!=0){
                strcpy(aux_arq[aux].nome,database->nome);
                aux_arq[aux].cod = database->cod;
                aux++;
            }
        }
        fclose(file);
        file = fopen("fs_database.dat","w");
        if(file != NULL){
            for(cont = 0; cont < aux; cont++){
                fwrite(&aux_arq[cont].cod, sizeof(int), 1, file);
                fwrite(&aux_arq[cont].nome, sizeof(char), TAM_NOME_BANCO, file);
            }
        }
        else
            printf("\nError while writing in database file");
    }
    else
    {
        printf("\nCannot open fs_database!\n");
    }
    fclose(file);
}

//busca por nome do banco se estiver cadastrado retorna a id dele
int busca(char *str, int identificacao){//a identificacao indicara qual if será executado
   
    FILE *file;
    db *database = (db *)malloc(sizeof(db));

    if (database == NULL)
    {
        printf("Out of memory.\n");
        abort();
    }
    
    if(!existeArquivo("fs_database.dat"))
        return -1;
    
    int cod = (cod_id_banco()), cont = 1;

    file = fopen("fs_database.dat", "r");  
         
    if(file != NULL)
    {
       for(; cont < cod; cont++){
           
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);

            if(identificacao==1)
            {
                if(!strcmp(str,database->nome))
                {
                    fclose(file);
                    return database->cod;
                }
            }
            if(identificacao==2)
            {
                printf("\nDatabase: %s", database->nome);
            }
       }
    }
    else 
    {
        printf("\nCannot open fs_database!\n");
    }
    
    fclose(file);
    return -2;
}

void dropDatabase(char *name){

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
    
    int id = -2;
    
    if(!existeArquivo("fs_database.dat")){//se o arquivo com os nomes dos bancos não existir ele é criado.
        grava_banco(nome);
        return 0;
    }
    
    //verificando se o nome do banco já existe em fs_database.dat
    id = busca(nome,1);//se encontrar o banco já cadastrado retorna seu id
   
    //retorno de funcao indicando sucesso
    if(id == -2)
    {
        grava_banco(nome);
    }  
    else
    {
        return -3;
    }
    
    return 0;
    
}

/**
 * Imprime o nome dos banco de dados existentes.
 */
void listaBancos() 
{
    FILE *file;
    db *database = (db *)malloc(sizeof(db));

    if (database == NULL)
    {
        printf("Out of memory.\n");
        abort();
    }
    
    if(!existeArquivo("fs_database.dat")) {
        printf("No database created.\n");
    }
    
    int cod = (cod_id_banco()), cont = 1;

    file = fopen("fs_database.dat", "r");  
         
    if(file != NULL)
    {
       for(; cont < cod; cont++){
           
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);

            printf("%s\n", database->nome);
       }
    }
    else 
    {
        printf("\nCannot open fs_database!\n");
    }
    
    fclose(file);
}

/**
 * Imprime tabelas do banco passado como indice
 */
void listaTabelas(int database)
{
    FILE *dicionario;
    int i = 0;
    char *nome_tabela = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL){
        free(nome_tabela);
        printf("No table created.\n");
        return;
    }

    while(fgetc (dicionario) != EOF)
    {
        fseek(dicionario, -1, 1);

        fread(nome_tabela, sizeof(char), TAMANHO_NOME_TABELA, dicionario); //Lê somente o nome da tabela
        int n = 0;
        char **str = tokenize(nome_tabela,'_',&n);
        if (n >= 2) {
            n = atoi(str[0]);

            if(n == database){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
                char *temp = nome_tabela;
                for (i = 0; i < strlen(str[0]); i++)
                {
                    temp++;
                }

                temp++;

                printf("%s\n",temp);
            }
        }
        
        fseek(dicionario, 28, 1);
    }

    fclose(dicionario);
}
