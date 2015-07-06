#include "common.h"

// LEITURA DE DICIONARIO E ESQUEMA
struct fs_objects leObjeto(char *nTabela){

    FILE *dicionario;
    char *tupla = (char *) malloc (sizeof(char)*TAMANHO_NOME_TABELA);
    int cod = 0;
    dicionario = fopen("fs_object.dat", "a+b"); // Abre o dicionario de dados.

    struct fs_objects objeto;
    
    if(!verificaNomeTabela(nTabela)){
        printf("Invalid table name.\n");
		free(tupla);
        return objeto;
    }

    if (dicionario == NULL) {
        printf("Database dictionary not found.\n");
		free(tupla);
        return objeto;
    }
    

    while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        fread(tupla, sizeof(char), TAMANHO_NOME_TABELA , dicionario); //Lê somente o nome da tabela

        if(strcmp(tupla, nTabela) == 0){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
            strcpy(objeto.nome, tupla);
            fread(&cod,sizeof(int),1,dicionario);   // Copia valores referentes a tabela pesquisada para a estrutura.
            objeto.cod=cod;
            fread(tupla,sizeof(char),TAMANHO_NOME_TABELA,dicionario);
            strcpy(objeto.nArquivo, tupla);
            fread(&cod,sizeof(int),1,dicionario);
            objeto.qtdCampos = cod;
            //objeto var bd recebe nome bd de bd
            
            return objeto;
        }
        fseek(dicionario, 28, 1); // Pula a quantidade de caracteres para a proxima verificacao(4B do codigo, 20B do nome do arquivo e 4B da quantidade de campos).
    }
    free(tupla);

    return objeto;
}

tp_table *leSchema (struct fs_objects objeto){
   
    FILE *schema;
    int i = 0, cod = 0;
    char *tupla = (char *)malloc(sizeof(char)*TAMANHO_NOME_CAMPO);
    char *tuplaT = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA+1);
    tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos); // Aloca esquema com a quantidade de campos necessarios.

    if(esquema == NULL){
        free(tupla);
        free(tuplaT);
        return ERRO_DE_ALOCACAO;
    }

    schema = fopen("fs_schema.dat", "a+b"); // Abre o arquivo de esquemas de tabelas.

    if (schema == NULL){
        free(tupla);
        free(tuplaT);
        free(esquema);
        return ERRO_ABRIR_ESQUEMA;
    }
    while((fgetc (schema) != EOF) && (i < objeto.qtdCampos)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod == objeto.cod){ // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.
                
                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
 
                strcpy(esquema[i].nome,tupla);                  // Copia dados do campo para o esquema.
               
                fread(&esquema[i].tipo, sizeof(char),1,schema);

                fread(&esquema[i].tam, sizeof(int),1,schema);

                fread(&esquema[i].chave, sizeof(int),1,schema);
                
                fread(tuplaT, sizeof(char), TAMANHO_NOME_TABELA, schema);
                strcpy(esquema[i].tabelaApt,tuplaT);

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                strcpy(esquema[i].attApt,tupla);
                if (i >= 1 )
                {
                    esquema[i-1].next = &esquema[i];
                }
                esquema[i].next = NULL;

                i++;
            } else {
                fseek(schema, 109, 1); // Pula a quantidade de caracteres para a proxima verificacao (40B do nome, 1B do tipo e 4B do tamanho,4B da chave, 20B do nome da Tabela Apontada e 40B do atributo apontado).
            }
        }
    }

    free(tupla);
    free(tuplaT);
    fclose(schema);

    return esquema;
}

//guardo o tamanho máximo de cada campo para impressão
int *tamanho_maximo_campo(struct fs_objects objeto)
{

    int i = 0, cod = 0;
    FILE *schema;

    char *tupla = (char *) malloc (sizeof(char)*TAMANHO_NOME_CAMPO);
    char *tuplaT = (char *) malloc (sizeof(char)*TAMANHO_NOME_TABELA+1);

    if (tupla == NULL || tuplaT == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    if (tuplaT == NULL)
    {
        free(tupla);

        printf("Out of memory.\nAborting...\n");
        abort();
    }

    tp_table *esquema = (tp_table *) malloc (sizeof(tp_table)*objeto.qtdCampos); // Aloca esquema com a quantidade de campos necessarios.

    if(esquema == NULL)
    {
        free(tupla);
        free(tuplaT);

        printf("Out of memory.\nAborting...\n");
        abort();
    }

    schema = fopen("fs_schema.dat", "a+b"); // Abre o arquivo de esquemas de tabelas.

    if (schema == NULL)
    {
        free(tupla);
        free(tuplaT);
        free(esquema);

        printf("Error while reading schema.\n");
        return NULL;
    }
    if (objeto.qtdCampos == 0)
    {
        printf("Empty table.\n");
        return NULL;
    }

    int *tamanho_campo = (int*) malloc (sizeof(int)*5);//aloca 3 informações para adicionar

    if (tamanho_campo == NULL)
    {
        free(tupla);
        free(tuplaT);
        free(esquema);
        free(schema);

        printf("Out of memory.\nAborting...\n");
        abort();
    }

    tamanho_campo[0] = strlen("Column");
    tamanho_campo[1] = strlen("Type");
    tamanho_campo[2] = strlen("Size");
    tamanho_campo[3] = strlen("Index");

    // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
    while((fgetc (schema) != EOF) && (i < objeto.qtdCampos))
    { 
        fseek(schema, -1, 1);

        // Le o codigo da tabela.
        if(fread(&cod, sizeof(int), 1, schema))
        {   
            // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.
            if(cod == objeto.cod)
            { 
                
                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                
                if (strlen(tupla) > tamanho_campo[0])
                {
                    tamanho_campo[0] = strlen(tupla);
                }                 

                fread(&esquema[i].tipo, sizeof(char),1,schema);

                if (esquema[i].tipo == 'S')
                {
                    if (tamanho_campo[1] < strlen("string"))
                    {
                        tamanho_campo[1] = strlen("string");
                    }
                }
                else if (esquema[i].tipo == 'C')
                {
                    if (tamanho_campo[1] < strlen("char"))
                    {
                        tamanho_campo[1] = strlen("char");
                    }
                }
                else if (esquema[i].tipo == 'D' )
                {
                    if (tamanho_campo[1] < strlen("double"))
                    {
                        tamanho_campo[1] = strlen("double");
                    }
                }
                else if (esquema[i].tipo == 'I' )
                {
                    if (tamanho_campo[1] < strlen("integer"))
                    {
                        tamanho_campo[1] = strlen("integer");
                    }
                }
                
                fread(&esquema[i].tam, sizeof(int),1,schema);

                char *str = (char*) malloc (sizeof(char)*1000);

                if (str == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }

                sprintf(str,"%d",esquema[i].tam);

                if (tamanho_campo[2] < strlen(str))
                {
                    tamanho_campo[2] = strlen(str);
                }
                
                free(str);

                fread(&esquema[i].chave, sizeof(int),1,schema);
               
                if (esquema[i].chave == 1)
                {
                    if (tamanho_campo[3] < strlen("primary key"))
                    {
                        tamanho_campo[3] = strlen("primary key");
                    }
                }
                else if (esquema[i].chave == 2)
                {
                    if (tamanho_campo[3] < strlen("foreign key"))
                    {
                        tamanho_campo[3] = strlen("foreign key");
                    }
                }
                
                fread(tuplaT, sizeof(char), TAMANHO_NOME_TABELA, schema);
               

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                
                if (i >= 1 )
                {
                    esquema[i-1].next = &esquema[i];
                }
                esquema[i].next = NULL;

                i++;
            } 
            else 
            {
                fseek(schema, 109, 1); // Pula a quantidade de caracteres para a proxima verificacao (40B do nome, 1B do tipo e 4B do tamanho,4B da chave, 20B do nome da Tabela Apontada e 40B do atributo apontado).
            }

        }
    }

    free(tupla);
    free(tuplaT);

    fclose(schema);

    return tamanho_campo;//retorna o tamanho máximo de cada campo a ser imprimido

}

void show_schema(struct fs_objects objeto, char *name_table)
{
    printf("         Table \"%s\"\n",name_table);
	int i = 0, cod = 0;
	FILE *schema;

	char *tupla = (char *) malloc (sizeof(char)*TAMANHO_NOME_CAMPO);
    char *tuplaT = (char *) malloc (sizeof(char)*TAMANHO_NOME_TABELA+1);

    if (tupla == NULL || tuplaT == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    if (tuplaT == NULL)
    {
        free(tupla);

        printf("Out of memory.\nAborting...\n");
        abort();
    }

    tp_table *esquema = (tp_table *) malloc (sizeof(tp_table)*objeto.qtdCampos); // Aloca esquema com a quantidade de campos necessarios.

    if(esquema == NULL)
    {
        free(tupla);
        free(tuplaT);

        printf("Out of memory.\nAborting...\n");
        abort();
    }

    int *limit = tamanho_maximo_campo(objeto);

    schema = fopen("fs_schema.dat", "a+b"); // Abre o arquivo de esquemas de tabelas.

    //esquema da tabela não existe
    if (schema == NULL)
    {
        free(tupla);
        free(tuplaT);
        free(esquema);

        printf("Error while reading schema.\n");
        return;
    }

    //nenhum campo foi adicionado na tabela
    if (objeto.qtdCampos == 0 )
    {
        printf("Empty table.\n");
        return;
    }

    /**
     * Imprime formatado pelo tamanho máximo
     */
    int size = 0;

    printf(" Column");
    size = limit[0]-strlen("Column")+1;
    for (i = 0; i < size; i++) printf(" ");
    printf("|");

    printf(" Type");
    size = limit[1]-strlen("Type")+1;
    for (i = 0; i < size; i++) printf(" ");
    printf("|");

    printf(" Size");
    size = limit[2]-strlen("Size")+1;
    for (i = 0; i < size; i++) printf(" ");
    printf("|");

    printf(" Index");
    size = limit[3]-strlen("Index")+1;
    for (i = 0; i < size; i++) printf(" ");
    printf("|");

	printf("\n");

    int c = 0;

    while(c < 4)
    {
        for (i = 0; i < limit[c]+2; i++)printf("-");
        printf("+");
        c++;
    }
    
    printf("\n");
    i = 0;

    while((fgetc (schema) != EOF) && (i < objeto.qtdCampos)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod == objeto.cod){ // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.
                
                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                
                printf(" %s", tupla);
                size = limit[0]-strlen(tupla)+1;
                for (c = 0; c < size; c++)printf(" ");
                printf("|");
               
                fread(&esquema[i].tipo, sizeof(char),1,schema);
                
                if (esquema[i].tipo == 'S')
                {
                   printf(" string");
                   size = limit[1]-strlen("string")+1;
                   for (c = 0; c < size; c++)printf(" ");
                   printf("|");
                }
                else if (esquema[i].tipo == 'C')
                {
                    printf(" char");
                    size = limit[1]-strlen("char")+1;
                    for (c = 0; c < size; c++)printf(" ");
                    printf("|");
                }
                else if (esquema[i].tipo == 'D' )
                {
                    printf(" double");
                    size = limit[1]-strlen("double")+1;
                    for (c = 0; c < size; c++)printf(" ");
                    printf("|");
                }
                else if (esquema[i].tipo == 'I' )
                {
                    printf(" integer");
                    size = limit[1]-strlen("integer")+1;
                    for (c = 0; c < size; c++)printf(" ");
                    printf("|");
                }
                
                fread(&esquema[i].tam, sizeof(int),1,schema);
                printf(" %d",esquema[i].tam);

                char *str = (char*) malloc (sizeof(char)*(limit[2]+1));
                if (str == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }
                sprintf(str,"%d", esquema[i].tam);
                size = limit[2]-strlen(str)+1;
                for (c = 0; c < size; c++)printf(" ");
                printf("|");
                
                fread(&esquema[i].chave, sizeof(int),1,schema);

                if (esquema[i].chave == 0)
                {
                   printf(" ");
                   size = limit[3]+1;
                   for (c = 0; c < size; c++)printf(" ");
                   printf("|");
                }
                if (esquema[i].chave == 1)
                {
                    printf(" primary key");
                    size = limit[3]-strlen("primary key")+1;
                    for (c = 0; c < size; c++)printf(" ");
                    printf("|");
                    
                }
                else if (esquema[i].chave == 2)
                {
                    printf(" foreign key");
                    size = limit[3]-strlen("foreign key")+1;
                    for (c = 0; c < size; c++)printf(" ");
                    printf("|");
                }
                
                fread(tuplaT, sizeof(char), TAMANHO_NOME_TABELA, schema);

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                
                if (i >= 1 )
                {
                    esquema[i-1].next = &esquema[i];
                }
                esquema[i].next = NULL;

                i++;
                printf("\n");
            } 
            else
            {
                fseek(schema, 109, 1); // Pula a quantidade de caracteres para a proxima verificacao (40B do nome, 1B do tipo e 4B do tamanho,4B da chave, 20B do nome da Tabela Apontada e 40B do atributo apontado).
            }
        }
    }

    free(tupla);
    free(tuplaT);

    fclose(schema);
}

int pot10(int n)
{
    if(n == 0)
        return 1;
    return 10 * pot10(n-1);
}
int strtam(char n[])
{
    if(n[0]==0)
        return 0;
    return 1+strtam(n+1);
}
int convertI(char u[])
{
    if(strtam(u) == 0)
        return 0;
    return (u[0]-48)*pot10(strtam(u)-1) + convertI(u+1);
}
double get_decimal(char u[]) 
{
    double decimal=0;
    int i,tamanho = strtam(u);
    for(i=0;i<tamanho && u[i]!='.';i++); // posiciona o indice i encima do ponto
    decimal=convertI(u+i+1);///(pot10(tamanho-i-1));
    decimal=(double)convertI(u+i+1)/(double)(pot10(tamanho-i-1));
    return decimal;
}
double get_inteiro(char v[]) 
{
    double inteiro=0;
    int i,tamanho = strtam(v);
    char u[10];
    strcpy(u,v); //copia o vetor para uma vaiável auxiliar para preservar sua originalidade
    for(i=0;i<tamanho && u[i]!='.';i++); // posiciona o indice i encima do ponto
    u[i]=0; // separa a parte inteira da parte decimal, inserindo um null no lugar do ponto
    inteiro=convertI(u);
    return inteiro;
}
double convertD(char u[]) 
{
    return get_inteiro(u)+get_decimal(u);
    //Soma inteiro com decimal.ss
}


/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verificação de existência de um arquivo.
    Parametros: Nome do arquivo.    
    Retorno:    INT 1 (existe) , 0 (não existe).
   ---------------------------------------------------------------------------------------------*/
    
int existeArquivo(char* filename){
    
    FILE* fptr = fopen(filename, "r");

    if (fptr != NULL){
        fclose(fptr);
        
        return 1;
    }

    return 0;
}


/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Copia todas as informações menos a tabela com nome NomeTabela, que será removida.
    Parametros: Nome da tabela que será removida do object.dat.
    Retorno:    INT
                SUCCESS,
                ERRO_ABRIR_ARQUIVO
   ---------------------------------------------------------------------------------------------*/

int procuraObjectArquivo(char *nomeTabela){
    int teste        = 0, 
    cont         = 0, 
    achou        = 0,
    tamanhoTotal = sizeof(struct fs_objects);

    char *table = (char *)malloc(sizeof(char) * tamanhoTotal);

    if (table == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    FILE *dicionario, *fp;

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
        return ERRO_ABRIR_ARQUIVO;

    if((fp = fopen("fs_nobject.dat", "a+b")) == NULL)
        return ERRO_ABRIR_ARQUIVO;
        
    fseek(dicionario, 0, SEEK_SET);
    fseek(fp, 0, SEEK_SET);

    while(cont < quantidadeTabelas()){
        fread(table, sizeof(char), tamanhoTotal, dicionario);
        teste = TrocaArquivosObj(nomeTabela, table);
        
        if(teste == 0){                                         //NÃO É IGUAL
            fseek(fp, 0, SEEK_END);
            fwrite(table, sizeof(char), tamanhoTotal, fp);            
        }

        else if(achou != 1){                                    //É IGUAL E NÃO TINHA SIDO DESCOBERTO.
            achou = 1;
            fread(table, sizeof(char), 0, dicionario);
        }
        cont++;
    }

    fclose(fp);
    fclose(dicionario);
    remove("fs_object.dat");
    system("mv fs_nobject.dat fs_object.dat");
        
    return SUCCESS;
}


/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Copia todas as informações menos a tabela do objeto, que será removida.
    Parametros: Objeto que será removido do schema.
    Retorno:    INT
                SUCCESS,
                ERRO_REMOVER_ARQUIVO_SCHEMA
   ---------------------------------------------------------------------------------------------*/

int procuraSchemaArquivo(struct fs_objects objeto){

    FILE *schema, *newSchema;
    int cod = 0;
    char *tupla = (char *)malloc(sizeof(char) * 109);

    tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos);

    if (esquema == NULL || tupla == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }
    
    if((schema = fopen("fs_schema.dat", "a+b")) == NULL)
        return ERRO_REMOVER_ARQUIVO_SCHEMA;
    
    if((newSchema = fopen("fs_nschema.dat", "a+b")) == NULL)
        return ERRO_REMOVER_ARQUIVO_SCHEMA;

    fseek(newSchema, 0, SEEK_SET);

    while((fgetc (schema) != EOF)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);
        fseek(newSchema, 0, SEEK_END);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod != objeto.cod){
                fwrite(&cod, sizeof(int), 1, newSchema);

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                strcpy(esquema[0].nome,tupla);                  // Copia dados do campo para o esquema.
                fwrite(tupla, sizeof(char), TAMANHO_NOME_CAMPO, newSchema);                

                fread(&esquema[0].tipo , sizeof(char), 1 , schema);      
                fread(&esquema[0].tam  , sizeof(int) , 1 , schema);   
                fread(&esquema[0].chave, sizeof(int) , 1 , schema);  

                fwrite(&esquema[0].tipo , sizeof(char), 1, newSchema);                
                fwrite(&esquema[0].tam  , sizeof(int) , 1, newSchema);                
                fwrite(&esquema[0].chave, sizeof(int) , 1, newSchema);                

                fread(tupla, sizeof(char), TAMANHO_NOME_TABELA, schema);
                strcpy(esquema[0].tabelaApt,tupla);
                fwrite(&esquema[0].tabelaApt, sizeof(char), TAMANHO_NOME_TABELA, newSchema);                

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);           
                strcpy(esquema[0].attApt,tupla);
                fwrite(&esquema[0].attApt, sizeof(char), TAMANHO_NOME_CAMPO, newSchema);                
                
            } else {
                fseek(schema, 109, 1);
            }
        }
    }
    
    fclose(newSchema);
    fclose(schema);
    
    remove("fs_schema.dat");
    system("mv fs_nschema.dat fs_schema.dat");
   
    return SUCCESS;
}




