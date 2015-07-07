#include "database.h"

int cod_id_db(int flag){

    int cod_id = 0;
    FILE *file;
    char ch = '\0';
    db *database = (db *) malloc (sizeof(db));

    if (database == NULL)
    {
        fprintf(stderr, "Out of memory.\nAborting...\n");
        abort();
    }

    file = fopen("fs_database.dat","r");

    if(file != NULL)
    {
        if(flag == 1)
        {                       //quantidade de bancos ja cadastrados
            while(ch!=EOF)
            {
                ungetc(ch,file);
                fread(&database->cod,sizeof(int),1,file);
                fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);
                cod_id++;
                ch = getc(file);
            }
        }
        else
        {                       //id novo banco
            int quant_db = cod_id_db(1), cont = 1;
            
            for(; cont < quant_db; cont++)
            {
                fread(&database->cod,sizeof(int),1,file);
                fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);
                cod_id = database->cod;
            }
            cod_id++;
        }
    }
    else
    {
        printf("Cannot read fs_database.\n");
        return 0;
    }

    fclose(file);
    if(database != NULL)free(database);

    return cod_id;
}

void reescreve(char *str)
{
    FILE *file;

    int quant_db = cod_id_db(1), aux = 0, cont = 0;

    db *database = (db *)malloc(sizeof(db));
    db *aux_arq = (db *)malloc(sizeof(db)*quant_db+1);

    if (database == NULL || aux_arq == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    file = fopen("fs_database.dat", "r");

    if(file != NULL)
    {
       for(cont = 0; cont < (quant_db-1); cont++){

            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);

            if(strcmp(database->nome,str) != 0)
            {
                strcpy(aux_arq[aux].nome,database->nome);
                aux_arq[aux].cod = database->cod;
                aux++;
            }
        }

        fclose(file);
        file = fopen("fs_database.dat","w");
        
        if(file != NULL)
        {
            for(cont = 0; cont < aux; cont++){
                fwrite(&aux_arq[cont].cod, sizeof(int), 1, file);
                fwrite(&aux_arq[cont].nome, sizeof(char), TAM_NOME_BANCO, file);
            }
        }
        else
        {
            if(database != NULL) free(database);
            if(aux_arq != NULL) free(aux_arq);
            printf("Error while writing in database file.\n");
            return;
        }
    }
    else
    {
        if(database != NULL) free(database);
        if(aux_arq != NULL) free(aux_arq);
        printf("Cannot open fs_database!\n");
        return;
    }

    if(database != NULL) free(database);
    if(aux_arq != NULL) free(aux_arq);
    fclose(file);
}

//busca por nome do banco se estiver cadastrado retorna a id dele
int busca(char *str, int identificacao){//a identificacao indicara qual if será executado
	
    id_max = 0;

	int cod_retorno;
    FILE *file;
    db *database = (db *)malloc(sizeof(db));

    if (database == NULL)
    {
        printf("Out of memory.\n");
        abort();
    }
    
    if(!existeArquivo("fs_database.dat"))
        return -1;
    
    int cod = (cod_id_db(1)), cont = 1;

    file = fopen("fs_database.dat", "r");  
         
    if(file != NULL)
    {
       for(; cont < cod; cont++){
           
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);
            
            if (id_max < database->cod) id_max = database->cod;

            if(identificacao == 1)
            {
                if(!strcmp(str,database->nome))
                {
                    fclose(file);
                    cod_retorno = database->cod;
                    if(database != NULL)free(database);
                    return cod_retorno;
                }
            }
            if(identificacao == 2)
            {
                printf("\nDatabase: %s", database->nome);
            }
       }
    }
    else 
    {
        printf("\nCannot open fs_database!\n");
    }
    
    if(database != NULL)free(database);
    fclose(file);
    return -2;
}

int dropDatabase(char *str){

    FILE *dicionario;

    int cnt = 0, cod_db = busca(str,1), n = 0;

    char *nome_tabela = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA+1);
    
    if (nome_tabela == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    char **tables;
    tables = (char**) malloc (sizeof(char*)*200);

    if (tables == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }


    if(cod_db == -2)
    {
        printf("There is no registered table named %s\n", str);
        return 0;
    }
    if((dicionario = fopen("fs_object.dat","a+b")) == NULL)//abrindo dicionario
    {
        if(nome_tabela != NULL)free(nome_tabela);
        if(tables != NULL)free(tables);
        printf("Error while opening the database file\n");
        return 0;
    }
    while(fgetc (dicionario) != EOF)
    {
        fseek(dicionario, -1, 1);
        fread(nome_tabela, sizeof(char), TAMANHO_NOME_TABELA, dicionario); //Lê somente o nome da tabela
        n = 0;
        char **aux_name = tokenize(nome_tabela,'_',&n);

        if (n >= 2) 
        {
            n = atoi(aux_name[0]);

            // Verifica se o nome dado pelo usuario existe no dicionario de dados.
            if(n == cod_db)
            { 
				cnt++;
            }
        }
        if(aux_name != NULL)free(aux_name);
        fseek(dicionario, 28, 1);
    }

	if(cnt == 0)
    {   
        reescreve(str);

        if(nome_tabela != NULL)free(nome_tabela);
        fclose(dicionario);
		return 1;
	}
	else
	{
        fclose(dicionario);

        if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
        {
            if( nome_tabela != NULL)free(nome_tabela);
            if( tables != NULL)free(tables);

            printf("Error while opening the database file\n");
            return 0;
        }

        cnt = 0;
        n = 0;

        while(fgetc (dicionario) != EOF)
        {
            fseek(dicionario, -1, 1);
            fread(nome_tabela, sizeof(char), TAMANHO_NOME_TABELA, dicionario); //Lê somente o nome da tabela

            n = strlen(nome_tabela);

            if (n >= 2) {

                char **str = tokenize(nome_tabela,'_',&n);
                n = atoi(str[0]);

                // Verifica se o nome dado pelo usuario existe no dicionario de dados.
                if(n == cod_db)
                { 
                    tables[cnt] = strdup(nome_tabela);
                    
                    if (tables[cnt] == NULL)
                    {
                        if(tables != NULL)free(tables);
                        if(nome_tabela != NULL)free(nome_tabela);
                        printf("Out of memory.\nAborting...\n");
                        abort();
                    }
                    cnt++;
                }
                if(str != NULL) free(str);
            }

            fseek(dicionario, 28, 1);
        }
        
        n = cnt;

        while(cnt > 0)
        {
            // printf("%s\n",tables[cnt-1]);
            int del = delete_table(tables[cnt-1]);
            
            if (del == SUCCESS)
            {
                cnt--;
            }
        }
        reescreve(str);
        
        if(tables != NULL)free(tables);
        if(nome_tabela != NULL)free(nome_tabela);

        fclose(dicionario);

        return 1;
    }
    return 0;
}
//criar banco
void grava_banco(char *str){

    FILE *file;
    db *database = (db *)malloc(sizeof(db));

    if (database == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    file = fopen("fs_database.dat", "a+");
    if(file != NULL){
        database->cod = id_max+1;                                 //id proximo banco
        strcpy(database->nome,str);
        fwrite(&database->cod, sizeof(int), 1, file);
        fwrite(&database->nome, sizeof(char), TAM_NOME_BANCO, file);
    }
    if(database != NULL)free(database);
    fclose(file);
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
    
    int quant_db = (cod_id_db(1)), cont = 1;

    file = fopen("fs_database.dat", "r");  
         
    if(file != NULL)
    {
       for(; cont < quant_db; cont++){
           
            fread(&database->cod,sizeof(int),1,file);
            fread(&database->nome,sizeof(char),TAM_NOME_BANCO,file);

            printf("%s\n", database->nome);
       }
    }
    else 
    {
        printf("\nCannot open fs_database!\n");
    }
    
    if(database != NULL)free(database);
    fclose(file);
}

/**
 * Imprime tabelas do banco passado como indice
 */
void listaTabelas(int database)
{
    FILE *dicionario;
    int i = 0, cnt = 0;
    char *nome_tabela = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL){
        if(nome_tabela != NULL)free(nome_tabela);
        printf("No table created.\n");
        return;
    }

    while(fgetc (dicionario) != EOF)
    {
        fseek(dicionario, -1, 1);

        fread(nome_tabela, sizeof(char), TAMANHO_NOME_TABELA, dicionario); //Lê somente o nome da tabela
        int n = 0;
        char **str = tokenize(nome_tabela,'_',&n);

        if (n >= 2) 
        {
            n = atoi(str[0]);

            if(n == database){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
                char *temp = nome_tabela;
                for (i = 0; i < strlen(str[0]); i++)
                {
                    temp++;
                }

                temp++;
				cnt++;
				
                printf("%s\n",temp);
            }	
        }
        fseek(dicionario, 28, 1);
    }
	if(cnt == 0){
		printf("No table created!\n");
	}

    fclose(dicionario);
}
