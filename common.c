#include "common.h"

// LEITURA DE DICIONARIO E ESQUEMA
struct fs_objects leObjeto(char *nTabela){

    FILE *dicionario;
    char *tupla = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);
    int cod;
    dicionario = fopen("fs_object.dat", "a+b"); // Abre o dicionario de dados.

    struct fs_objects objeto;
    
    if(!verificaNomeTabela(nTabela)){
        printf("Erro GRAVE! na função leObjeto(). Nome da tabela inválido.\nAbortando...\n");
		free(tupla);
        exit(1);
    }

    if (dicionario == NULL) {
        printf("Erro GRAVE! na função leObjeto(). Arquivo não encontrado.\nAbortando...\n\n");
		free(tupla);
        exit(1);
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


