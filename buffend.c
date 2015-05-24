//BufferPool
#include "buffend.h"

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
            
            free(tupla);
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
                fread(tupla, sizeof(char), TAMANHO_NOME_TABELA, schema);
                strcpy(esquema[i].tabelaApt,tuplaT);
                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                strcpy(esquema[i].attApt,tupla);
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
//--------------------------------------------------
// INICIALIZACAO DO BUFFER
tp_buffer * initbuffer(){
    
    tp_buffer *bp = (tp_buffer*)malloc(sizeof(tp_buffer)*PAGES);
    int i;
    tp_buffer *temp = bp;

    if(bp == NULL)
        return ERRO_DE_ALOCACAO;
    for (i = 0;i < PAGES; i++){
        temp->db=0;
        temp->pc=0;
        temp->nrec=0;
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
int cabecalho(tp_table *s, int num_reg){
    int count, aux;
    aux = 0;
    
    for(count = 0; count < num_reg; count++){
        cria_campo(s[count].tam, 1, s[count].nome, 0); // O segundo parâmetro significa se = 1 Cabecalho se = 0 é valor daquele campo
        aux += s[count].tam;
    }
    printf("\n");
    return aux;
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
//--------------------------------------------------
// FUNCOES AUXILIARES
int tamTupla(tp_table *esquema, struct fs_objects objeto){// Retorna o tamanho total da tupla da tabela.

    int qtdCampos = objeto.qtdCampos, i, tamanhoGeral = 0;
   
    if(qtdCampos){ // Lê o primeiro inteiro que representa a quantidade de campos da tabela.
        for(i = 0; i < qtdCampos; i++)
            tamanhoGeral += esquema[i].tam; // Soma os tamanhos de cada campo da tabela.
    }

    return tamanhoGeral;
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
int verificaNomeTabela(char *nomeTabela)
{

    FILE *dicionario;
    char *tupla = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);
    if((dicionario = fopen("fs_object.dat","a+b")) == NULL){
		free(tupla);
        return ERRO_ABRIR_ARQUIVO;
    }

    while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        fread(tupla, sizeof(char), TAMANHO_NOME_TABELA, dicionario); //Lê somente o nome da tabela

        if(strcmp(tupla, nomeTabela) == 0){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
            free(tupla);
            return 1;
        }
        
        fseek(dicionario, 28, 1);
    }

    fclose(dicionario);
    free(tupla);

    return 0;
}
int quantidadeTabelas(){

    FILE *dicionario;
    int codTbl = 0;

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
        return ERRO_ABRIR_ARQUIVO;

    while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        codTbl++; // Conta quantas vezes é lido uma tupla no dicionario.

        fseek(dicionario, 48, 1);
    }

    fclose(dicionario);


    return codTbl;
}
//---------------------------------------
// INSERE UMA TUPLA NO BUFFER!
char *getTupla(tp_table *campos,struct fs_objects objeto, int from){ //Pega uma tupla do disco a partir do valor de from

    int tamTpl = tamTupla(campos, objeto);
    char *linha=(char *)malloc(sizeof(char)*tamTpl);
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
// CRIA TABELA
table *iniciaTabela(char *nome){
    if(verificaNomeTabela(nome)){   // Se o nome já existir no dicionario, retorna erro.
        return ERRO_NOME_TABELA_INVALIDO;
    }

    table *t = (table *)malloc(sizeof(table)*1);
    strcpy(t->nome,nome); // Inicia a estrutura de tabela com o nome da tabela.
    t->esquema = NULL; // Inicia o esquema da tabela com NULL.
    return t; // Retorna estrutura para criação de uma tabela.
}

table *adicionaCampo(table *t,char *nomeCampo, char tipoCampo, int tamanhoCampo, int tChave, char *tabelaApt, char *attApt){
    
    if(t == NULL) // Se a estrutura passada for nula, retorna erro.
        return ERRO_ESTRUTURA_TABELA_NULA;
    tp_table *aux;  
    if(t->esquema == NULL){ // Se o campo for o primeiro a ser adicionado, adiciona campo no esquema.
        
        
        tp_table *e = (tp_table *)malloc(sizeof(tp_table));
        memset(e, 0, sizeof(*e));
        if (e == NULL)
        {
            return ERRO_DE_ALOCACAO;
        }
        e->next = NULL;
        int n = strlen(nomeCampo)+1;

        if (n > TAMANHO_NOME_CAMPO)
        {
            n = TAMANHO_NOME_CAMPO;
        }

        strncpy(e->nome, nomeCampo,n); // Copia nome do campo passado para o esquema
        e->tipo = tipoCampo; // Copia tipo do campo passado para o esquema
        e->tam = tamanhoCampo; // Copia tamanho do campo passado para o esquema
        e->chave = tChave; // Copia tipo de chave passado para o esquema
        
        if(strlen(tabelaApt) >= 1)
            strcpy(e->tabelaApt, tabelaApt); //Copia a Tabela Refenciada da FK de chave passado para o esquema;
        
        if(strlen(attApt) >= 1)
            strcpy(e->attApt, attApt); //Copia o Atributo Refenciado da FK de chave passado para o esquema
        
        t->esquema = e; 

        return t; // Retorna a estrutura
    } else { 
        for(aux = t->esquema; aux != NULL; aux = aux->next){ // Anda até o final da estrutura de campos.
            if(aux->next == NULL){ // Adiciona um campo no final.   
                tp_table *e = (tp_table *)malloc(sizeof(tp_table));
                   memset(e, 0, sizeof(*e));
                if (e == NULL)
                {
                    return ERRO_DE_ALOCACAO;
                }
                e->next = NULL;

                int n = strlen(nomeCampo)+1;

                if (n > TAMANHO_NOME_CAMPO)
                {
                    n = TAMANHO_NOME_CAMPO;
                }
                strncpy(e->nome, nomeCampo,n); // Copia nome do campo passado para o esquema
                e->tipo = tipoCampo; // Copia tipo do campo passado para o esquema
                e->tam = tamanhoCampo; // Copia tamanho do campo passado para o esquema
                e->chave = tChave; // Copia tipo de chave passado para o esquema

                //if(strlen(tabelaApt)!=0){
                    strcpy(e->tabelaApt, tabelaApt); //Copia a Tabela Refenciada da FK de chave passado para o esquema;
                
                //if(strlen(attApt)!=0)
                    strcpy(e->attApt, attApt); //Copia o Atributo Refenciado da FK de chave passado para o esquema
                
                aux->next = e; // Faz o campo anterior apontar para o campo inserido.
                return t;
            }
        }
    }

    return t; //Retorna estrutura atualizada.
}
int finalizaTabela(table *t){
    if(t == NULL)
        return ERRO_DE_PARAMETRO;

    FILE *esquema, *dicionario;
    tp_table *aux;
    int codTbl = quantidadeTabelas() + 1, qtdCampos = 0; // Conta a quantidade de tabelas já no dicionario e soma 1 no codigo dessa nova tabela.
    char nomeArquivo[TAMANHO_NOME_ARQUIVO];

    if((esquema = fopen("fs_schema.dat","a+b")) == NULL)
        return ERRO_ABRIR_ARQUIVO;

    for(aux = t->esquema; aux != NULL; aux = aux->next){ // Salva novos campos no esquema da tabela, fs_schema.dat

        fwrite(&codTbl         ,sizeof(codTbl)         ,1,esquema);  //Código Tabela
        fwrite(&aux->nome      ,sizeof(aux->nome)      ,1,esquema);  //Nome campo
        fwrite(&aux->tipo      ,sizeof(aux->tipo)      ,1,esquema);  //Tipo campo
        fwrite(&aux->tam       ,sizeof(aux->tam)       ,1,esquema);  //Tamanho campo
        fwrite(&aux->chave     ,sizeof(aux->chave)     ,1,esquema);  //Chave do campo
        fwrite(&aux->tabelaApt ,sizeof(aux->tabelaApt) ,1,esquema);  //Tabela Apontada
        fwrite(&aux->attApt    ,sizeof(aux->attApt)    ,1,esquema);  //Atributo apontado.

        qtdCampos++; // Soma quantidade total de campos inseridos.
    }

    fclose(esquema);

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
        return ERRO_ABRIR_ARQUIVO;

    strcpy(nomeArquivo, t->nome);
    strcat(nomeArquivo, ".dat\0");
    strcat(t->nome, "\0");
    // Salva dados sobre a tabela no dicionario.
    fwrite(&t->nome,sizeof(t->nome),1,dicionario);
    fwrite(&codTbl,sizeof(codTbl),1,dicionario);
    fwrite(&nomeArquivo,sizeof(nomeArquivo),1,dicionario);
    fwrite(&qtdCampos,sizeof(qtdCampos),1,dicionario);

    fclose(dicionario);
    return SUCCESS;
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

char retornaTamanhoTipoDoCampo(char *nomeCampo, table  *tab) {
    
    char tipo = 0;

    tp_table *temp = tab->esquema;

    while(temp != NULL) {

       if (strcmp(nomeCampo,temp->nome) == 0)
       {
            tipo = temp->tipo;

            return tipo;
       }

       temp = temp->next;
    }

    return tipo;
}

//-----------------------------------------
// INSERE NA TABELA
column *insereValor(table  *tab, column *c, char *nomeCampo, char *valorCampo){
    
    column *aux;
    if(c == NULL){ // Se o valor a ser inserido é o primeiro, adiciona primeiro campo.
    
        column *e = (column *)malloc(sizeof(column));

        if (e == NULL)
        {
            return ERRO_DE_ALOCACAO;
        }

        int tam = retornaTamanhoValorCampo(nomeCampo, tab);
        char tipo = retornaTamanhoTipoDoCampo(nomeCampo,tab); 

        int nTam = strlen(valorCampo);

        if (tipo == 'S')
        {
            nTam = tam;
        }

        e->valorCampo = (char *)malloc(sizeof(char) * (nTam+1));
        
        if (e->valorCampo == NULL)
        {
            return ERRO_DE_ALOCACAO;
        }

        int n = strlen(nomeCampo)+1;

        /**
         * Verifica se o nome ultrapassa o limite, se sim trunca 
         */
        if (n > TAMANHO_NOME_CAMPO)
        {
           n = TAMANHO_NOME_CAMPO;
        }

        strncpy(e->nomeCampo, nomeCampo,n); 

        n = strlen(valorCampo) + 1;
        
        if (n > tam && tipo == 'S')
        {
            n = tam;
        }

        strncpy(e->valorCampo, valorCampo,n);

        e->next = NULL;
        c = e;
        return c;
    } else {
        for(aux = c; aux != NULL; aux = aux->next) { // Anda até o final da lista de valores a serem inseridos e adiciona um novo valor.
            if(aux->next == NULL){

                column *e = (column *)malloc(sizeof(column));

                if (e == NULL)
                {
                    return ERRO_DE_ALOCACAO;
                }
                int tam = retornaTamanhoValorCampo(nomeCampo, tab);
                char tipo = retornaTamanhoTipoDoCampo(nomeCampo,tab); 

                int nTam = strlen(valorCampo);

                if (tipo == 'S')
                {
                    nTam = tam;
                }

                e->valorCampo = (char *) malloc (sizeof(char) * (nTam+1));

                if (e->valorCampo == NULL)
                {
                    return ERRO_DE_ALOCACAO;
                }

                e->next = NULL;

                int n = strlen(nomeCampo)+1;

                /**
                 * Verifica se o nome do campo ultrapassa o limite, se sim trunca 
                 */
                if (n > TAMANHO_NOME_CAMPO)
                {
                   n = TAMANHO_NOME_CAMPO;
                }

                strncpy(e->nomeCampo, nomeCampo,n);
                
                 n = strlen(valorCampo) + 1;
        
                if (n > tam && tipo == 'S')
                {
                    n = tam;
                }
                strncpy(e->valorCampo, valorCampo,n);
                aux->next = e;
                return c;
            }
        }
    }

    return ERRO_INSERIR_VALOR;
}
int finalizaInsert(char *nome, column *c){
    column *auxC, *temp;
    int i = 0, x = 0, t, erro, j = 0;
    FILE *dados;

    struct fs_objects objeto,dicio; // Le dicionario
    tp_table *auxT ; // Le esquema
    auxT = abreTabela(nome, &dicio, &auxT);

    table *tab     = (table *)malloc(sizeof(table));
    tp_table *tab2 = (tp_table *)malloc(sizeof(struct tp_table)); 

    tab->esquema = abreTabela(nome, &objeto, &tab->esquema);
    tab2 = procuraAtributoFK(objeto);
    
    for(j = 0, temp = c; j < objeto.qtdCampos && temp != NULL; j++, temp = temp->next){
        switch(tab2[j].chave){
            case NPK:
                erro = SUCCESS;
                break;

            case PK:
                erro = verificaChavePK(nome, temp , temp->nomeCampo, temp->valorCampo);
                if(erro == ERRO_CHAVE_PRIMARIA){
                    printf("Erro GRAVE! na função verificaChavePK(). Erro de Chave Primaria.\nAbortando...\n");
                    free(c);    // Libera a memoria da estrutura.
					free(auxT); // Libera a memoria da estrutura.
					//free(temp); // Libera a memoria da estrutura.   
					free(tab); // Libera a memoria da estrutura.
					free(tab2); // Libera a memoria da estrutura.
                    exit(1);
                }

                break;

            case FK:
                if(tab2[j].chave == 2 && strlen(tab2[j].attApt) != 0 && strlen(tab2[j].tabelaApt) != 0){

                    erro = verificaChaveFK(nome, temp, tab2[j].nome, temp->valorCampo, tab2[j].tabelaApt, tab2[j].attApt);

                    if(erro != SUCCESS){
                        printf("Erro GRAVE! na função verificaChaveFK(). Erro de Chave Estrangeira.\nAbortando...\n");
                        free(c);    // Libera a memoria da estrutura.
						free(auxT); // Libera a memoria da estrutura.
						free(temp); // Libera a memoria da estrutura.
                        free(tab); // Libera a memoria da estrutura.
						free(tab2); // Libera a memoria da estrutura.
                        exit(1);
                    }
                }

                break;
        }
    }
    
    
    if(erro == ERRO_CHAVE_ESTRANGEIRA){
        printf("Erro GRAVE! na função verificaChaveFK(). Erro de Chave Estrangeira.\nAbortando...\n");
        free(c);    // Libera a memoria da estrutura.
		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        exit(1);
    }

    if(erro == ERRO_CHAVE_PRIMARIA){
        printf("Erro GRAVE! na função verificaChavePK(). Erro de Chave Primaria.\nAbortando...\n");
        free(c);    // Libera a memoria da estrutura.
		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        exit(1);
    }
    if(erro == ERRO_DE_PARAMETRO) {
        printf("Erro GRAVE! na função finalizaInsert(). Erro de Parametro.\nAbortando...\n");
        free(c);    // Libera a memoria da estrutura.
		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        exit(1);
    }
    
    
    if((dados = fopen(dicio.nArquivo,"a+b")) == NULL){
		free(c);    // Libera a memoria da estrutura.
		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        return ERRO_ABRIR_ARQUIVO;
        
	}
    
    for(auxC = c, t = 0; auxC != NULL; auxC = auxC->next, t++){
        if(t >= dicio.qtdCampos)
            t = 0;

        if(auxT[t].tipo == 'S'){ // Grava um dado do tipo string.
            
            if(sizeof(auxC->valorCampo) > auxT[t].tam && sizeof(auxC->valorCampo) != 8){
				free(tab); // Libera a memoria da estrutura.
				free(tab2); // Libera a memoria da estrutura.
				free(c);    // Libera a memoria da estrutura.
				free(auxT); // Libera a memoria da estrutura.
				free(temp); // Libera a memoria da estrutura.
				fclose(dados);
                return ERRO_NO_TAMANHO_STRING;
            }
            if(strcmp(auxC->nomeCampo, auxT[t].nome) != 0){
				free(tab); // Libera a memoria da estrutura.
				free(tab2); // Libera a memoria da estrutura.
				free(c);    // Libera a memoria da estrutura.
				free(auxT); // Libera a memoria da estrutura.
				free(temp); // Libera a memoria da estrutura.
				fclose(dados);
                return ERRO_NOME_CAMPO;
            }
            char valorCampo[auxT[t].tam];
            strcpy(valorCampo, auxC->valorCampo);
            strcat(valorCampo, "\0");
            fwrite(&valorCampo,sizeof(valorCampo),1,dados);
        }
        else if(auxT[t].tipo == 'I'){ // Grava um dado do tipo inteiro.
            i = 0;
            while (i < strlen(auxC->valorCampo)){
                if(auxC->valorCampo[i] < 48 || auxC->valorCampo[i] > 57){ 
					free(tab); // Libera a memoria da estrutura.
					free(tab2); // Libera a memoria da estrutura.
					free(c);    // Libera a memoria da estrutura.
					free(auxT); // Libera a memoria da estrutura.
					//free(temp); // Libera a memoria da estrutura.
					fclose(dados);
                    return ERRO_NO_TIPO_INTEIRO;
                }
                i++;
            }

            int valorInteiro = 0;
            
            sscanf(auxC->valorCampo,"%d",&valorInteiro);
            fwrite(&valorInteiro,sizeof(valorInteiro),1,dados);
        }
        else if(auxT[t].tipo == 'D'){ // Grava um dado do tipo double.
            x = 0;
            while (x < strlen(auxC->valorCampo)){
                if((auxC->valorCampo[x] < 48 || auxC->valorCampo[x] > 57) && (auxC->valorCampo[x] != 46)){ 
					free(tab); // Libera a memoria da estrutura.
					free(tab2); // Libera a memoria da estrutura.
					free(c);    // Libera a memoria da estrutura.
					free(auxT); // Libera a memoria da estrutura.
					free(temp); // Libera a memoria da estrutura.
					fclose(dados);
                    return ERRO_NO_TIPO_DOUBLE;
                }
                x++;
            }

            double valorDouble = convertD(auxC->valorCampo);
            fwrite(&valorDouble,sizeof(valorDouble),1,dados);
        }
        else if(auxT[t].tipo == 'C'){ // Grava um dado do tipo char.

            if(strlen(auxC->valorCampo) > (sizeof(char)))
            {
				free(tab); // Libera a memoria da estrutura.
				free(tab2); // Libera a memoria da estrutura.
				free(c);    // Libera a memoria da estrutura.
				free(auxT); // Libera a memoria da estrutura.
				free(temp); // Libera a memoria da estrutura.
				fclose(dados);
                return ERRO_NO_TIPO_CHAR;
            }
            char valorChar = auxC->valorCampo[0];
            fwrite(&valorChar,sizeof(valorChar),1,dados);           
        }

    }
    
    fclose(dados);
    free(tab); // Libera a memoria da estrutura.
	free(tab2); // Libera a memoria da estrutura.
    free(c);    // Libera a memoria da estrutura.
    free(auxT); // Libera a memoria da estrutura.
    free(temp); // Libera a memoria da estrutura.
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
    
    if(page >= PAGES)
        return ERRO_PAGINA_INVALIDA;

    if(buffer[page].nrec == 0) //Essa página não possui registros
        return ERRO_PARAMETRO;
    
    column *colunas = (column *)malloc(sizeof(column)*objeto.qtdCampos*buffer[page].nrec); //Aloca a quantidade de campos necessária
    
    if(colunas == NULL)
        return ERRO_DE_ALOCACAO;
    
    int i=0, j=0, t=0, h=0;
    
    while(i < buffer[page].position){
        t=0;
        if(j >= objeto.qtdCampos)
            j=0;
        colunas[h].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam);
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
//----------------------------------------

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Utilizada para impressão de tabelas.
    Parametros: Nome da tabela (char).    
    Retorno:    void.
   ---------------------------------------------------------------------------------------------*/
    
void imprime(char nomeTabela[]) {

    int j,erro, x;
    struct fs_objects objeto = leObjeto(nomeTabela);    
    
    tp_table *esquema = leSchema(objeto);

    if(esquema == ERRO_ABRIR_ESQUEMA){
        printf("Erro GRAVE ao Criar o ESQUEMA.\nAbortando...\n");
        exit(1);
    }

    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ERRO_DE_ALOCACAO){
        printf("Erro GRAVE alocar memória para o BUFFER.\nAbortando...\n");
        exit(1);
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);        
    

    column *pagina = getPage(bufferpoll, esquema, objeto, 0);

    if(pagina == ERRO_PARAMETRO){
         printf("Erro GRAVE ao abrir a TABELA.\nAbortando...\n");
        exit(1);
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

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verificação de existência de um arquivo.
    Parametros: Nome do arquivo.    
    Retorno:    INT 1 (existe) , 0 (não existe).
   ---------------------------------------------------------------------------------------------*/
    
int existeArquivo(const char* filename){
    FILE* fptr = fopen(filename, "r");
    if (fptr != NULL){
        fclose(fptr);
        
        return 1;
    }

    return 0;
}

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verifica se o nome da tabela 'nomeTabela' está nos primeiros bytes de 'linha'
    Parametros: Nome da tabela, char linha.    
    Retorno:    INT(1 - Está contido, 0 - Não está)                
   ---------------------------------------------------------------------------------------------*/

int TrocaArquivosObj(char *nomeTabela, char *linha){
    int x = 0;
    char *tabela = (char *)malloc(sizeof(char) * TAMANHO_NOME_TABELA);

    while(x < TAMANHO_NOME_TABELA){
        tabela[x] = linha[x];
        x++;
    }

    if(strcmp(tabela, nomeTabela) == 0){
        return 1;
    }

    free(tabela);
    return 0;
}

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Retorna vetor de esquemas com todos os atributos chaves (PK, FK e NPK)
    Parametros: Objeto da tabela.
    Retorno:    Vetor de esquemas vetEsqm
   ---------------------------------------------------------------------------------------------*/

tp_table *procuraAtributoFK(struct fs_objects objeto){
    FILE *schema;
    int cod = 0, chave, i = 0;
    char *tupla = (char *)malloc(sizeof(char) * 109);
    tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos);
    tp_table *vetEsqm = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos);

    if((schema = fopen("fs_schema.dat", "a+b")) == NULL){
        printf("Erro GRAVE ao abrir o ESQUEMA.\nAbortando...\n");
        free(tupla);
		free(esquema);
		free(vetEsqm);
        exit(1);
    }
    
    while((fgetc (schema) != EOF) && i < objeto.qtdCampos){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod == objeto.cod){
                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
                strcpy(esquema[i].nome,tupla);                  // Copia dados do campo para o esquema.

                fread(&esquema[i].tipo , sizeof(char), 1 , schema);      
                fread(&esquema[i].tam  , sizeof(int) , 1 , schema);   
                fread(&chave, sizeof(int) , 1 , schema);  

                fread(tupla, sizeof(char), TAMANHO_NOME_TABELA, schema);
                strcpy(esquema[i].tabelaApt,tupla);

                fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);           
                strcpy(esquema[i].attApt,tupla);
                
                strcpy(vetEsqm[i].tabelaApt, esquema[i].tabelaApt);
                strcpy(vetEsqm[i].attApt, esquema[i].attApt);
                strcpy(vetEsqm[i].nome, esquema[i].nome);
                vetEsqm[i].chave = chave;

                i++;
            } else {
                fseek(schema, 109, 1);
            }
        }
    }
    free(tupla);
	free(esquema);

    return vetEsqm;
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

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Função para exclusão de tabelas.
    Parametros: Nome da tabela (char).    
    Retorno:    INT
                SUCCESS, 
                ERRO_REMOVER_ARQUIVO_OBJECT, 
                ERRO_REMOVER_ARQUIVO_SCHEMA, 
                ERRO_LEITURA_DADOS.
   ---------------------------------------------------------------------------------------------*/

int excluirTabela(char *nomeTabela){
    struct fs_objects objeto;
    tp_table *esquema;
    int x,erro;
    char str[20]; 
    char dat[5] = ".dat";

    strcpy (str, nomeTabela); 
    strcat (str, dat);              //Concatena e junta o nome com .dat

    abreTabela(nomeTabela, &objeto, &esquema);
    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ERRO_DE_ALOCACAO){
        printf("Erro ao alocar memória para o buffer.\n");
        return ERRO_LEITURA_DADOS;
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);        
    
    if(procuraSchemaArquivo(objeto) != 0)
        return ERRO_REMOVER_ARQUIVO_SCHEMA;

    if(procuraObjectArquivo(nomeTabela) != 0)
       return ERRO_REMOVER_ARQUIVO_OBJECT;
        
    remove(str);
    
    return SUCCESS;
}

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Inicializa os atributos necessários para a verificação de FK e PK.
    Parametros: Objeto da tabela, Tabela, Buffer e nome da tabela.
    Retorno:    INT
                SUCCESS,
                ERRO_DE_PARAMETRO,
   ---------------------------------------------------------------------------------------------*/

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

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verifica a existência do atributo antes de adicionar na tabela
    Parametros: Nome da tabela, coluna C.    
    Retorno:    INT 
                SUCCESS,
                ERRO_DE_PARAMETRO
   ---------------------------------------------------------------------------------------------*/

int existeAtributo(char *nomeTabela, column *c){
    int erro, x;
    struct fs_objects objeto; 
    tp_table *tabela;         
    tp_buffer *bufferpoll;
    column *aux;

    if(iniciaAtributos(&objeto, &tabela, &bufferpoll, nomeTabela) != SUCCESS) 
        return ERRO_DE_PARAMETRO;

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);        

    column *pagina = getPage(bufferpoll, tabela, objeto, 0);
    
    if(pagina == NULL){
        pagina = getPage(bufferpoll, tabela, objeto, 1);
    }
    
     if(pagina != NULL){
        erro = SUCCESS;
        for(x = 0; x < objeto.qtdCampos; x++){
            for(aux = c; aux != NULL; aux=aux->next){
                if(strcmp(pagina[x].nomeCampo, aux->nomeCampo) == 0)
                    erro++;
            }
        }
        if(erro != objeto.qtdCampos){
            return ERRO_DE_PARAMETRO;
        }
    }
 
    return SUCCESS; 
}

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Gera as verificações em relação a chave FK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo, Tabela Apontada e Atributo Apontado.
    Retorno:    INT
                SUCCESS,
                ERRO_DE_PARAMETRO,
                ERRO_CHAVE_ESTRANGEIRA
   ---------------------------------------------------------------------------------------------*/

int verificaChaveFK(char *nomeTabela,column *c, char *nomeCampo, char *valorCampo, char *tabelaApt, char *attApt){
    int x,j, erro;
    char str[20]; 
    char dat[5] = ".dat";
    struct fs_objects objeto;
    tp_table *tabela;
    tp_buffer *bufferpoll;
    
    strcpy (str, tabelaApt); 
    strcat (str, dat);              //Concatena e junta o nome com .dat
    
    erro = existeAtributo(nomeTabela, c);
    /*if(erro != SUCCESS )
        return ERRO_DE_PARAMETRO;*/
        
    //if(existeAtributo(tabelaApt, c))
        //return ERRO_CHAVE_ESTRANGEIRA;

    if(iniciaAtributos(&objeto, &tabela, &bufferpoll, tabelaApt) != SUCCESS)
        return ERRO_DE_PARAMETRO;
       

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);        

    column *pagina = getPage(bufferpoll, tabela, objeto, 0);

    //printf("asf: %d\n", bufferpoll[0].nrec);

    for(j = 0; j < objeto.qtdCampos * bufferpoll[0].nrec; j++){     
        
       // printf("VALORC: %s\n VC: %s\n", pagina[j].valorCampo, valorCampo); 

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

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Gera as verificações em relação a chave PK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo
    Retorno:    INT
                SUCCESS,
                ERRO_DE_PARAMETRO,
                ERRO_CHAVE_PRIMARIA
   ---------------------------------------------------------------------------------------------*/

int verificaChavePK(char *nomeTabela, column *c, char *nomeCampo, char *valorCampo){
    int j, x, erro;
    
    struct fs_objects objeto;
    tp_table *tabela;
    tp_buffer *bufferpoll;
    
    erro = existeAtributo(nomeTabela, c);
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

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Recebe o nome de uma tabela e engloba as funções leObjeto() e leSchema().
    Parametros: Nome da Tabela, Objeto da Tabela e tabela.
    Retorno:    tp_table
   ---------------------------------------------------------------------------------------------*/

tp_table *abreTabela(char *nomeTabela, struct fs_objects *objeto, tp_table **tabela){
    *objeto     = leObjeto(nomeTabela);
    *tabela     = leSchema(*objeto);

    return *tabela;
}
