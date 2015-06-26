//BufferPool
#include "buffend.h"

//--------------------------------------------------
// INICIALIZACAO DO BUFFER
tp_buffer * initbuffer(){
    
    tp_buffer *bp = (tp_buffer*)malloc(sizeof(tp_buffer)*PAGES);
    int i = 0;
    tp_buffer *temp = bp;

    if(bp == NULL){
        printf("Out of memory.\nAborting...\n");
        exit(1);
    }

    for (i = 0;i < PAGES; i++){
        temp->db=0;
        temp->pc=0;
        temp->nrec=0;
        temp->position = 0;
        temp++;
    }

    return bp;
}

//--------------------------------------------------
// IMPRESSAO BUFFER
void cria_campo(int tam, int header, char *val, int x){
    int i;
    char aux[30];

    if(header){

        for(i = 0; i <= 30 && val[i] != '\0'; i++){
            aux[i] = val[i];
        }
        for(;i<30;i++)
            aux[i] = ' ';
        aux[i] ='\0';
        printf("%s", aux);

    }

    else{
        for(i = 0; i < x; i++)
            printf(" ");
    }
}
int drawline(tp_buffer *buffpoll, tp_table *s, struct fs_objects objeto, int p, int num_page){
     
    if (num_page > PAGES || p > SIZE){
        return ERRO_DE_PARAMETRO;
    }
    int *pos_ini, aux = (p * tamTupla(s,objeto)) , num_reg = objeto.qtdCampos;
    pos_ini = &aux;
    int count, pos_aux, bit_pos;
    union c_double cd;
    union c_int ci;
    int x = 0;
    
    count = pos_aux = bit_pos = 0;
    
    for(count = 0; count < num_reg; count++){
        pos_aux = *(pos_ini);
        bit_pos = 0;

        switch(s[count].tipo){
            case 'S':
                x = 0;
                while(buffpoll[num_page].data[pos_aux] != '\0'){
            
                    printf("%c", buffpoll[num_page].data[pos_aux]);
                    if ((buffpoll[num_page].data[pos_aux++] & 0xc0) != 0x80) bit_pos++; //Conta apenas bits que possam ser impressos (UTF8)
                x++;
                }
                
                cria_campo((TAMANHO_NOME_CAMPO - (bit_pos)), 0, (char*)' ', (30 - x));
                break;
            
            case 'I':
                while(pos_aux < *(pos_ini) + s[count].tam){
                    ci.cnum[bit_pos++] = buffpoll[num_page].data[pos_aux++];
                }
                printf("%d", ci.num); //Controla o número de casas até a centena
                cria_campo((TAMANHO_NOME_CAMPO - (bit_pos)), 0, (char*)' ', 28);
                break;
                
            case 'D':
                while(pos_aux < *(pos_ini) + s[count].tam){
                    cd.double_cnum[bit_pos++] = buffpoll[num_page].data[pos_aux++]; // Cópias os bytes do double para área de memória da union
                }
                printf("%.3lf", cd.dnum);
                cria_campo((TAMANHO_NOME_CAMPO - (bit_pos)), 0, (char*)' ', 24);
                break;
            
            case 'C': 
                printf("%c", buffpoll[num_page].data[pos_aux]);
                if(s[count].tam < strlen(s[count].nome)){
                    bit_pos = strlen(s[count].nome);
                }
                else{
                    bit_pos = s[count].tam;
                }
                cria_campo((bit_pos - 1), 0, (char*)' ', 29);   
                break;
            
            default: 
                return ERRO_IMPRESSAO;
                break;
        }
        *(pos_ini) += s[count].tam;     
    }
    printf("\n");
    return SUCCESS;
}

int printbufferpoll(tp_buffer *buffpoll, tp_table *s,struct fs_objects objeto, int num_page){
    
    int aux, i, num_reg = objeto.qtdCampos;
    
    
    if(buffpoll[num_page].nrec == 0){
        return ERRO_IMPRESSAO;  
    }
    
    i = aux = 0;
    
    aux = cabecalho(s, num_reg);
    

    while(i < buffpoll[num_page].nrec){ // Enquanto i < numero de registros * tamanho de uma instancia da tabela
        drawline(buffpoll, s, objeto, i, num_page);
        i++;
    }
    return SUCCESS;
}

int colocaTuplaBuffer(tp_buffer *buffer, int from, tp_table *campos, struct fs_objects objeto){//Define a página que será incluida uma nova tupla
    
    char *tupla = getTupla(campos,objeto,from);

    if(tupla == ERRO_DE_LEITURA)
        return ERRO_LEITURA_DADOS;

    int i=0, found=0;
    while (!found && i < PAGES)//Procura pagina com espaço para a tupla.
    {
        if(SIZE - buffer[i].position > tamTupla(campos, objeto)){// Se na pagina i do buffer tiver espaço para a tupla, coloca tupla.
            setTupla(buffer, tupla, tamTupla(campos, objeto), i);
            found = 1;
            buffer[i].position += tamTupla(campos, objeto); // Atualiza proxima posição vaga dentro da pagina.
            buffer[i].nrec += 1;
            break;
        }
        i++;// Se não, passa pra proxima página do buffer.
    }
    
    if (!found)
        return ERRO_BUFFER_CHEIO;

    return SUCCESS;
}


//----------------------------------------
// EXCLUIR TUPLA BUFFER
column * excluirTuplaBuffer(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, int nTupla){
    column *colunas = (column *)malloc(sizeof(column)*objeto.qtdCampos);

    if(colunas == NULL)
        return ERRO_DE_ALOCACAO;

    if(buffer[page].nrec == 0) //Essa página não possui registros
        return ERRO_PARAMETRO;
    
    int i, tamTpl = tamTupla(campos, objeto), j=0, t=0;
    i = tamTpl*nTupla; //Calcula onde começa o registro

    while(i < tamTpl*nTupla+tamTpl){
        t=0;
        
        colunas[j].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam); //Aloca a quantidade necessária para cada campo
        colunas[j].tipoCampo = campos[j].tipo;  // Guarda o tipo do campo
        strcpy(colunas[j].nomeCampo, campos[j].nome);   //Guarda o nome do campo
    
        while(t < campos[j].tam){
            colunas[j].valorCampo[t] = buffer[page].data[i];    //Copia os dados
            t++;
            i++;
        }
        j++;
    }
    j = i;
    i = tamTpl*nTupla;
    for(; i < buffer[page].position; i++, j++) //Desloca os bytes do buffer sobre a tupla excluida
        buffer[page].data[i] = buffer[page].data[j];

    buffer[page].position -= tamTpl;
    buffer[page].nrec--;

    return colunas; //Retorna a tupla excluida do buffer
}


//----------------------------------------
// RETORNA PAGINA DO BUFFER
column * getPage(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page){
    
    if(page >= PAGES) {
        printf("Invalid page.\n" );
        return ERRO_PAGINA_INVALIDA;
    }

    //Essa página não possui registros
    if(buffer[page].nrec == 0) {
        printf("Page empty.\n" );
        return ERRO_PARAMETRO;
    }
    
    column *colunas = (column *)malloc(sizeof(column)*objeto.qtdCampos*buffer[page].nrec); //Aloca a quantidade de campos necessária
    
    if(colunas == NULL)
        return ERRO_DE_ALOCACAO;
    
    int i=0, j=0, t=0, h=0;
    
    while(i < buffer[page].position){
        t=0;
        if(j >= objeto.qtdCampos)
            j=0;
        colunas[h].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam);

        if (colunas[h].valorCampo == NULL)
        {
            printf("Out of memory.\nAborting...");
            exit(1);
        }
        colunas[h].tipoCampo = campos[j].tipo;  //Guarda tipo do campo
        strcpy(colunas[h].nomeCampo, campos[j].nome); //Guarda nome do campo
        
        while(t < campos[j].tam){
            colunas[h].valorCampo[t] = buffer[page].data[i]; //Copia os dados
            t++;
            i++;
        }
        h++;
        j++;
    }
    return colunas; //Retorna a 'page' do buffer
}
