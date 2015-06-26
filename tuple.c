#include "tuple.h"

int tamTupla(tp_table *esquema, struct fs_objects objeto){// Retorna o tamanho total da tupla da tabela.

    int qtdCampos = objeto.qtdCampos, i, tamanhoGeral = 0;
   
    if(qtdCampos){ // Lê o primeiro inteiro que representa a quantidade de campos da tabela.
        for(i = 0; i < qtdCampos; i++)
            tamanhoGeral += esquema[i].tam; // Soma os tamanhos de cada campo da tabela.
    }

    return tamanhoGeral;
}

//---------------------------------------
// INSERE UMA TUPLA NO BUFFER!
char *getTupla(tp_table *campos,struct fs_objects objeto, int from){ //Pega uma tupla do disco a partir do valor de from

    int tamTpl = tamTupla(campos, objeto);
    char *linha=(char *)malloc(sizeof(char)*tamTpl);

    if (linha == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        exit(1);
    }
    FILE *dados;

    from = from * tamTpl;

    dados = fopen(objeto.nArquivo, "r");

    if (dados == NULL)
        return ERRO_DE_LEITURA;

    fseek(dados, from, 1);

    if(fgetc (dados) != EOF){
        fseek(dados, -1, 1);
        fread(linha, sizeof(char), tamTpl, dados); //Traz a tupla inteira do arquivo
    }
    else{       //Caso em que o from possui uma valor inválido para o arquivo de dados
        fclose(dados);
        return ERRO_DE_LEITURA;
    }
    fclose(dados);
    return linha;
}


void setTupla(tp_buffer *buffer,char *tupla, int tam, int pos){ //Coloca uma tupla de tamanho "tam" no buffer e na página "pos"
    int i=buffer[pos].position;
    for (;i<buffer[pos].position + tam;i++)
        buffer[pos].data[i] = *(tupla++);
}

char retornaTipoDoCampo(char *nomeCampo, table  *tab) {
    
    // printf("%s %s\n",nomeCampo,tab->nome );
    char tipo = 0;

    tp_table *temp = tab->esquema;

    if (temp == NULL)
    {
        fprintf(stderr, "Erro ao abrir esquema\n" ); 
    }

    while(temp != NULL) {

        // printf("\nnomeCampo %s Esquema %s\n", nomeCampo,temp->nome);
       if (strcmp(nomeCampo,temp->nome) == 0)
       {
            tipo = temp->tipo;

            return tipo;
       }

       temp = temp->next;
    }

    return tipo;
}


int retornaTamanhoValorCampo(char *nomeCampo, table  *tab) {
    
    int tam = 0;

    tp_table *temp = tab->esquema;

    while(temp != NULL) {

       if (strcmp(nomeCampo,temp->nome) == 0)
       {
            tam = temp->tam;

            return tam;
       }

       temp = temp->next;
    }

    return tam;
}