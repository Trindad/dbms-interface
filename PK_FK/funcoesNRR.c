/*/////////////////////////////////////////////////////////////////////////////////*/
//  Alunos: Natan J. Mai, Ricardo Zanuzzo e Rogério Torchelsen                    //
//  Disciplina: Banco de Dados II                                                 //      
//  Curso: Ciência da Computação                                                  //      
//  Universidade Federal da Fronteira Sul                                         //      
//                                                                                //      
//////////////////////////////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffend.h"
#include "funcoesNRR.h"


/***********************************************************************************|
|* FUNÇÃO: Utilizada na impressão das tabelas, conforme nomeTabela                 */   
    
void imprime(char nomeTabela[]) {
    int j,erro, x;
    struct fs_objects objeto = leObjeto(nomeTabela);    
    tp_table *esquema = leSchema(objeto);

    if(esquema == ERRO_ABRIR_ESQUEMA){
        printf("Erro ao criar o esquema.\n");
        return;
    }

    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ERRO_DE_ALOCACAO){
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);        
    
    column *pagina = getPage(bufferpoll, esquema, objeto, 0);

    if(pagina == ERRO_PARAMETRO){
        printf("Tabela Vazia\n");
        return ;
    }

    // PARA IMPRIMIR PÁGINA
    printf("Número de tuplas: %d\n", bufferpoll[0].nrec);
    for(j=0; j < objeto.qtdCampos*bufferpoll[0].nrec; j++){
        
        if(pagina[j].tipoCampo == 'S')
            printf("%s: %-15s ", pagina[j].nomeCampo,pagina[j].valorCampo);
        else if(pagina[j].tipoCampo == 'I'){
            int *n = (int *)&pagina[j].valorCampo[0];
            printf("%s: %-15d ",pagina[j].nomeCampo, *n);
        }
        else if(pagina[j].tipoCampo == 'C'){
            printf("%s: %-15c ",pagina[j].nomeCampo, pagina[j].valorCampo[0]);
        }
        else if(pagina[j].tipoCampo == 'D'){
            double *n = (double *)&pagina[j].valorCampo[0];
            printf("%s: %-15f ",pagina[j].nomeCampo, *n);
        }
        if(j>=1 && ((j+1)%objeto.qtdCampos)==0){
            printf("\n");
        }
    }
    printf("\n\n");
}

        
/***********************************************************************************|
|* FUNÇÃO: Verifica a existência do arquivo da tabela 'filename'.                  */   
    
int existeArquivo(const char* filename){
    FILE* fptr = fopen(filename, "r");
    if (fptr != NULL){
        fclose(fptr);
        
        return 1;
    }

    return 0;
}

int TrocaArquivosObj(char *nomeTabela, char *linha){
    int x = 0;
    char *tabela = (char *)malloc(sizeof(char) * TAMANHO_NOME_TABELA);

    while(x < TAMANHO_NOME_TABELA){
        tabela[x] = linha[x];
        x++;
    }

    if(strcmp(tabela, nomeTabela) == 0){
        //printf("TABELA TROCA: %s\n", tabela);
        return 1;
    }

    return 0;
}

char * TrocaArquivosSch(char *linha){
    int x = 0;
    char *att = (char *)malloc(sizeof(char) * (TAMANHO_NOME_CAMPO));

    while(x < TAMANHO_NOME_CAMPO){
        att[x] = linha[x];
        x++;
    }

    return att;
}

/***********************************************************************************|
|* FUNÇÃO: Organiza o arquivo para remover espaços vazios                          */   

int procuraObjectArquivo(char *nomeTabela){
    int teste        = 0, 
        cont         = 0, 
        achou        = 0,
        tamanhoTotal = sizeof(struct fs_objects);

    FILE *dicionario, *fp;

    char *table = (char *)malloc(sizeof(char) * tamanhoTotal);

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
        return ERRO_ABRIR_ARQUIVO;

    if((fp = fopen("fs_teste.dat", "a+b")) == NULL)
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
    system("mv fs_teste.dat fs_object.dat");
        
    return 0;
}

int procuraSchemaArquivo(struct fs_objects objeto, int qtd){
    FILE *schema, *newSchema;
    int i = 0, cod = 0, x = 0, teste, achou;
    char *tupla = (char *)malloc(sizeof(char) * 109);
    tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos);

    if((schema = fopen("fs_schema.dat", "a+b")) == NULL)
        return ERRO_REMOVER_ARQUIVO_SCHEMA;
    
    if((newSchema = fopen("fs_nschema.dat", "a+b")) == NULL)
        return ERRO_REMOVER_ARQUIVO_SCHEMA;

    fseek(newSchema, 0, SEEK_SET);

    /*achou = 0;
    while(!feof(schema)){
        //fread(&cod, sizeof(int), 1, schema);
        //printf("COD: %d\n", cod);
        fread(tupla, sizeof(char), 109, schema);
        fwrite(tupla, sizeof(char), 109, newSchema);       
    }*/

    while((fgetc (schema) != EOF)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);
        fseek(newSchema, 0, SEEK_END);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod != objeto.cod){ // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.
                fwrite(&cod, sizeof(int), 1, newSchema);

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                strcpy(esquema[i].nome,tupla);                  // Copia dados do campo para o esquema.
                fwrite(tupla, sizeof(char), TAMANHO_NOME_CAMPO, newSchema);                

                fread(&esquema[i].tipo, sizeof(char),1,schema);      
                fread(&esquema[i].tam, sizeof(int),1,schema);   
                fread(&esquema[i].chave, sizeof(int),1,schema);  
                fwrite(&esquema[i].tipo, sizeof(char), 1, newSchema);                
                fwrite(&esquema[i].tam, sizeof(int), 1, newSchema);                
                fwrite(&esquema[i].chave, sizeof(int), 1, newSchema);                

                fread(tupla, sizeof(char), TAMANHO_NOME_TABELA, schema);
                strcpy(esquema[i].tabelaApt,tupla);
                fwrite(&esquema[i].tabelaApt, sizeof(char), TAMANHO_NOME_TABELA, newSchema);                

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                strcpy(esquema[i].attApt,tupla);
                fwrite(&esquema[i].attApt, sizeof(char), TAMANHO_NOME_CAMPO, newSchema);                
                
                x++;            
            } else {
                fseek(schema, 109, 1); // Pula a quantidade de caracteres para a proxima verificacao (40B do nome, 1B do tipo e 4B do tamanho,4B da chave, 20B do nome da Tabela Apontada e 40B do atributo apontado).
            }
        }
    }
    
    fclose(newSchema);
    fclose(schema);
    
    remove("fs_schema.dat");
    system("mv fs_nschema.dat fs_schema.dat");
   
    return 0;
}


/***********************************************************************************|
|* FUNÇÃO: Exclui a tabela com 'nome'                                              */   

int excluirArquivo(char *nomeTabela){
	struct fs_objects objeto;
	tp_table *esquema;
	int x,erro;
    char str[20]; 
    char dat[5] = ".dat";

    strcpy (str, nomeTabela); 
    strcat (str, dat);              //Concatena e junta o nome com .dat
/*
    if(!existeArquivo(str)){
        system("clear");
        printf("Arquivo não existe!\n");
        return ERRO_VAZIO;
    }
  */  
	abreTabela(nomeTabela, &objeto, &esquema);
	tp_buffer *bufferpoll = initbuffer();
	if(bufferpoll == ERRO_DE_ALOCACAO){
        printf("Erro ao alocar memória para o buffer.\n");
        return ERRO_LEITURA_DADOS;
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);        
    
    column *pagina = getPage(bufferpoll, esquema, objeto, 0);

    procuraSchemaArquivo(objeto, objeto.qtdCampos);

    if(procuraObjectArquivo(nomeTabela) != 0)
       return ERRO_REMOVER_ARQUIVO_OBJECT;
	
    //remove(str);
    
    return SUCCESS;
}


/***********************************************************************************|
|* FUNÇÃO: Inicia os atributos utilizados pela FK e PK. Além de erros.             */   

int iniciaAtributos(struct fs_objects *objeto, tp_table **tabela, tp_buffer **bufferpoll, char *nomeT){
	
    *objeto     = leObjeto(nomeT);
    *tabela     = leSchema(*objeto);
    *bufferpoll = initbuffer();

    if(*tabela == ERRO_ABRIR_ESQUEMA)
        return ERRO_DE_PARAMETRO;

    if(*bufferpoll == ERRO_DE_ALOCACAO)
        return ERRO_DE_PARAMETRO;

    return SUCCESS;
}

int existeAtributo(char *nomeTabela, char *atributo){
    int erro, x;
    struct fs_objects objeto; 
    tp_table *tabela;         
    tp_buffer *bufferpoll;
    
    //if(iniciaAtributos(&objeto, &tabela, &bufferpoll, nomeTabela) != SUCCESS) 
        //return ERRO_DE_PARAMETRO;
	
	objeto     = leObjeto(nomeTabela);
    tabela     = leSchema(objeto);
    bufferpoll = initbuffer();

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);        

    column *pagina = getPage(bufferpoll, tabela, objeto, 0);
    
     if(pagina == ERRO_PARAMETRO){
        return 0;
    }
    
    for(x = 0; x < objeto.qtdCampos * bufferpoll[0].nrec; x++){
        if(strcmp(pagina[x].nomeCampo, atributo) == 0)
            return SUCCESS;
    }
    
    return ERRO_DE_PARAMETRO;
}

/***********************************************************************************|
|* FUNÇÃO: Verifica as condições para chave estrangeira (FK)                       */   

int verificaChaveFK(char *nomeTabela, char *nomeCampo, char *valorCampo, char *tabelaApt, char *attApt){
    int x,j, erro;
    char str[20]; 
    char dat[5] = ".dat";
    struct fs_objects objeto;
    tp_table *tabela;
    tp_buffer *bufferpoll;
    
    strcpy (str, tabelaApt); 
    strcat (str, dat);              //Concatena e junta o nome com .dat
    
    erro = existeAtributo(nomeTabela, nomeCampo);
    if(erro != SUCCESS )
        return ERRO_DE_PARAMETRO;
    
    if(existeAtributo(tabelaApt, attApt))
        return ERRO_CHAVE_ESTRANGEIRA;

    if(iniciaAtributos(&objeto, &tabela, &bufferpoll, tabelaApt) != SUCCESS)
        return ERRO_DE_PARAMETRO;
       
      

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);        

    column *pagina = getPage(bufferpoll, tabela, objeto, 0);


    for(j = 0; j < objeto.qtdCampos * bufferpoll[0].nrec; j++){		
		 
		 
        if(strcmp(pagina[j].nomeCampo, nomeCampo) == 0){
			
            if(pagina[j].tipoCampo == 'S'){     
                if(strcmp(pagina[j].valorCampo, valorCampo) == 0){
                    return SUCCESS;
                }
            }

            else if(pagina[j].tipoCampo == 'I'){ 
				
                int *n = (int *)&pagina[j].valorCampo[0];

                if(*n == atoi(valorCampo)){
					
                    return SUCCESS;
                }
            }

            else if(pagina[j].tipoCampo == 'D'){ 
                double *nn = (double *)&pagina[j].valorCampo[0];

                if(*nn == atof(valorCampo)){
                    return SUCCESS;
                }
            }

            else if(pagina[j].tipoCampo == 'C'){                        
                if(pagina[j].valorCampo == valorCampo){
                    return SUCCESS;
                }
            }else {
				return ERRO_CHAVE_ESTRANGEIRA;
			}
        }            
    }

    return ERRO_CHAVE_ESTRANGEIRA;
}

/***********************************************************************************|
|* FUNÇÃO: Verifica as condições para chave primária (PK)                          */   

int verificaChavePK(char *nomeTabela, char *nomeCampo, char *valorCampo){
    int j, x, erro;
    
    struct fs_objects objeto;
    tp_table *tabela;
    tp_buffer *bufferpoll;
    
    erro = existeAtributo(nomeTabela, nomeCampo);
	if(erro != SUCCESS )
        return ERRO_DE_PARAMETRO;
    

    if(iniciaAtributos(&objeto, &tabela, &bufferpoll, nomeTabela) != SUCCESS) 
        return ERRO_DE_PARAMETRO;

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);        

    column *pagina = getPage(bufferpoll, tabela, objeto, 0);

    for(j = 0; j < objeto.qtdCampos * bufferpoll[0].nrec; j++){

        if(strcmp(pagina[j].nomeCampo, nomeCampo) == 0){
			
            if(pagina[j].tipoCampo == 'S'){        
                if(strcmp(pagina[j].valorCampo, valorCampo) == 0){
                    return ERRO_CHAVE_PRIMARIA;
                }
            }

            else if(pagina[j].tipoCampo == 'I'){ 
                int *n = (int *)&pagina[j].valorCampo[0];

                if(*n == atoi(valorCampo)){
                    return ERRO_CHAVE_PRIMARIA;
                }
            }

            else if(pagina[j].tipoCampo == 'D'){ 
                double *nn = (double *)&pagina[j].valorCampo[0];

                if(*nn == atof(valorCampo)){
                    return ERRO_CHAVE_PRIMARIA;
                }
            }

            else if(pagina[j].tipoCampo == 'C'){                        
                if(pagina[j].valorCampo == valorCampo){
                    return ERRO_CHAVE_PRIMARIA;
                }
            }
        }            
    }

    return SUCCESS;
}

/***********************************************************************************|
|* FUNÇÃO: recebe o nome de uma tabela e engloba as funções leObjeto() e leSchema() */   

int abreTabela(char *nomeTabela, struct fs_objects *objeto, tp_table **tabela){
	*objeto     = leObjeto(nomeTabela);
    *tabela     = leSchema(*objeto);
	
    return 1;
}	