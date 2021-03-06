#include "table.h"

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


int verificaNomeCampo(char *table_name, char *column)
{
    struct fs_objects objeto,dicio; // Le dicionario
    tp_table *auxT ; // Le esquema
    auxT = abreTabela(table_name, &dicio, &auxT);

    table *tab     = (table *) malloc (sizeof(table));

    if (tab == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    tab->esquema = abreTabela(table_name, &objeto, &tab->esquema);

    auxT = tab->esquema;
    int t = 0;

    while(auxT != NULL)
    {
        if (strcmp(auxT->nome,column) == 0)
        {
            free(tab);
            return 1;
        }
        auxT = auxT->next;
        t++;
    }

    return 0;
}


int verificaNomeTabela(char *nomeTabela)
{
    FILE *dicionario;
    char *tupla = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL){
        free(tupla);
        printf("Out of memory.\nAborting...\n");
        abort();
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


//----------------------------------------
// CRIA TABELA
table *iniciaTabela(char *nome){

    if(verificaNomeTabela(nome)){   // Se o nome já existir no dicionario, retorna erro.
        return ERRO_NOME_TABELA_INVALIDO;
    }

    table *t = (table *)malloc(sizeof(table));

    if (t == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }
    int n = strlen(nome);

    if (n > TAMANHO_NOME_TABELA)
    {
        n = TAMANHO_NOME_TABELA;
    }

    strncpy(t->nome,nome,n+1); // Inicia a estrutura de tabela com o nome da tabela.
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
            printf("Out of memory.\nAborting...\n");
            abort();
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

    } 
    else 
    { 
        for(aux = t->esquema; aux != NULL; aux = aux->next){ // Anda até o final da estrutura de campos.
            if(aux->next == NULL){ // Adiciona um campo no final.   
                
                tp_table *e = (tp_table *)malloc(sizeof(tp_table));
                
                if (e == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }
                memset(e, 0, sizeof(*e));

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

                if(strlen(tabelaApt)>=1)
                    strcpy(e->tabelaApt, tabelaApt); //Copia a Tabela Refenciada da FK de chave passado para o esquema;
                
                if(strlen(attApt)>=1)
                    strcpy(e->attApt, attApt); //Copia o Atributo Refenciado da FK de chave passado para o esquema
                
                aux->next = e; // Faz o campo anterior apontar para o campo inserido.
               
                return t;
            }
        }
    }

    return t; //Retorna estrutura atualizada.
}


int finalizaTabela(table *t, int database){
    
    if(t == NULL) 
    {
        return ERRO_DE_PARAMETRO;
    }

    FILE *esquema, *dicionario;
    tp_table *aux;
    int codTbl = quantidadeTabelas() + 1, qtdCampos = 0; // Conta a quantidade de tabelas já no dicionario e soma 1 no codigo dessa nova tabela.
    char nomeArquivo[TAMANHO_NOME_ARQUIVO];

    if((esquema = fopen("fs_schema.dat","a+b")) == NULL) {
        return ERRO_ABRIR_ARQUIVO;
    }

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

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL) {
        return ERRO_ABRIR_ARQUIVO;
    }

    // char *table_name = (char*) malloc (sizeof(char)*1000);

    // if (!table_name)
    // {
    //     printf("Out of memory.\n");
    // }

    // sprintf(table_name,"%d",database);

    // strcat(table_name,"_");
    // char *name = strdup(t->nome);
    // strcat(table_name,name);
    // strcat(table_name,".dat");
    // free(table_name);
    // free(name);

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


//-----------------------------------------
// INSERE NA TABELA
column *insereValor(table  *tab, column *c, char *nomeCampo, char *valorCampo){
    
    column *aux;

    // Se o valor a ser inserido é o primeiro, adiciona primeiro campo.
    if(c == NULL)
    { 
        column *e = (column *)malloc(sizeof(column));

        if (e == NULL)
        {
            printf("Out of memory.\nAborting...\n");
            abort();
        }

        int tam = retornaTamanhoValorCampo(nomeCampo, tab);
        char tipo = retornaTipoDoCampo(nomeCampo,tab); 

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

        n = strlen(valorCampo)+1;
        
        if (n > tam && tipo == 'S')
        {
            n = tam;
        }

        strncpy(e->valorCampo, valorCampo,n);
        e->valorCampo[n] = '\0';
        e->next = NULL;
        c = e;
        return c;
    } else {
        for(aux = c; aux != NULL; aux = aux->next) { // Anda até o final da lista de valores a serem inseridos e adiciona um novo valor.
            if(aux->next == NULL){

                column *e = (column *)malloc(sizeof(column));

                if (e == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }

                int tam = retornaTamanhoValorCampo(nomeCampo, tab);
                char tipo = retornaTipoDoCampo(nomeCampo,tab); 

                int nTam = strlen(valorCampo)+1;

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
                    // printf("tamanho valorCampo %d %s\n",n, valorCampo);
                    // printf("tam %d\n",tam );
                    n = tam;
                }

                strncpy(e->valorCampo, valorCampo,n);
                e->valorCampo[n] = '\0';
                // printf("valorCampo %s\n",e->valorCampo);
                aux->next = e;
                
                return c;
            }
        }
    }

    return ERRO_INSERIR_VALOR;
}


int finalizaInsert(char *nome, column *c){

    column *auxC, *temp;
    int x = 0, t = 0, erro, j = 0;
    FILE *dados;

    struct fs_objects objeto,dicio; // Le dicionario
    tp_table *auxT ; // Le esquema
    auxT = abreTabela(nome, &dicio, &auxT);

    table *tab     = (table *)malloc(sizeof(table));
    tp_table *tab2 = (tp_table *)malloc(sizeof(tp_table)); 

    if (tab == NULL || tab2 == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    tab->esquema = abreTabela(nome, &objeto, &tab->esquema);
    tab2 = procuraAtributoFK(objeto);
    
    for(j = 0, temp = c; j < objeto.qtdCampos && temp != NULL; j++, temp = temp->next){
        
        int tamTab = 0;

        while(tamTab < objeto.qtdCampos)
        {
            if (strcmp(tab2[tamTab].nome,temp->nomeCampo) != 0)
            {
                tamTab++;
                continue;
            }

            switch(tab2[tamTab].chave){
                case NPK:
                    erro = SUCCESS;
                    break;
                case PK:
                    erro = verificaChavePK(nome, temp , temp->nomeCampo, temp->valorCampo);
                    if(erro == ERRO_CHAVE_PRIMARIA){
                        printf("There was an error due to primary key. Check the table's schema.\n");
                        free(c);    // Libera a memoria da estrutura.
                        free(auxT); // Libera a memoria da estrutura.
                           
                        free(tab); // Libera a memoria da estrutura.
                        free(tab2); // Libera a memoria da estrutura.
                        return ERRO_CHAVE_PRIMARIA;
                    }

                    break;

                case FK:
                    if(tab2[tamTab].chave == 2 && strlen(tab2[tamTab].attApt) != 0 && strlen(tab2[tamTab].tabelaApt) != 0){
                       
                        erro = verificaChaveFK(nome, temp, tab2[tamTab].nome, temp->valorCampo, tab2[tamTab].tabelaApt, tab2[tamTab].attApt);

                        if(erro != SUCCESS){
                            printf("Error due to foreign key constraint.\n");
                            free(c);    // Libera a memoria da estrutura.
                            free(auxT); // Libera a memoria da estrutura.
                           
                            free(tab); // Libera a memoria da estrutura.
                            free(tab2); // Libera a memoria da estrutura.
                            return ERRO_CHAVE_ESTRANGEIRA;
                        }
                    }
                    break;
            }

            tamTab++;
        }
    }
    
    
    if(erro == ERRO_CHAVE_ESTRANGEIRA){
        printf("Error due to foreign key constraint.\n");
        free(c);    // Libera a memoria da estrutura.
        free(auxT); // Libera a memoria da estrutura.
        // free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
        free(tab2); // Libera a memoria da estrutura.
        return ERRO_CHAVE_ESTRANGEIRA;
    }

    if(erro == ERRO_CHAVE_PRIMARIA){
        printf("There was an error due to primary key. Check the table's schema.\n");
        free(c);    // Libera a memoria da estrutura.
        free(auxT); // Libera a memoria da estrutura.
        // free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
        free(tab2); // Libera a memoria da estrutura.
        return ERRO_CHAVE_PRIMARIA;
    }
    if(erro == ERRO_DE_PARAMETRO) {
        printf("Error on insert. Wrong arguments.\nAborting...\n");
        free(c);    // Libera a memoria da estrutura.
        free(auxT); // Libera a memoria da estrutura.
        // free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
        free(tab2); // Libera a memoria da estrutura.
        return ERRO_DE_PARAMETRO;
    }
    
    
    if((dados = fopen(dicio.nArquivo,"a+b")) == NULL){
        free(c);    // Libera a memoria da estrutura.
        free(auxT); // Libera a memoria da estrutura.
        // free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
        free(tab2); // Libera a memoria da estrutura.
        return ERRO_ABRIR_ARQUIVO;
        
    }
    
    auxC = c;
    t = 0;

    while(t < dicio.qtdCampos) 
    {        
        // printf("nome Campo %s %s\n",auxC->nomeCampo,auxT[t].nome );
        if (strcmp(auxC->nomeCampo, auxT[t].nome) == 0)
        {
            if(t >= dicio.qtdCampos)
                t = 0;

            if(auxT[t].tipo == 'S'){ // Grava um dado do tipo string.
                
                if(sizeof(auxC->valorCampo) > auxT[t].tam && sizeof(auxC->valorCampo) != 8){
                    free(tab); // Libera a memoria da estrutura.
                    free(tab2); // Libera a memoria da estrutura.
                    free(c);    // Libera a memoria da estrutura.
                    free(auxT); // Libera a memoria da estrutura.
                    // free(temp); // Libera a memoria da estrutura.
                    fclose(dados);
                    return ERRO_NO_TAMANHO_STRING;
                }
                if(strcmp(auxC->nomeCampo, auxT[t].nome) != 0){
                    free(tab); // Libera a memoria da estrutura.
                    free(tab2); // Libera a memoria da estrutura.
                    free(c);    // Libera a memoria da estrutura.
                    free(auxT); // Libera a memoria da estrutura.
                    // free(temp); // Libera a memoria da estrutura.
                    fclose(dados);
                    return ERRO_NOME_CAMPO;
                }
                char valorCampo[auxT[t].tam];
                strcpy(valorCampo,auxC->valorCampo);
                strcat(valorCampo, "\0");
                fwrite(&valorCampo,sizeof(valorCampo),1,dados);
            }
            else if(auxT[t].tipo == 'I'){ // Grava um dado do tipo inteiro.
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
                        // free(temp); // Libera a memoria da estrutura.
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
                    // free(temp); // Libera a memoria da estrutura.
                    fclose(dados);
                    return ERRO_NO_TIPO_CHAR;
                }
                char valorChar = auxC->valorCampo[0];
                fwrite(&valorChar,sizeof(valorChar),1,dados);           
            }

            auxC = c;
            t++;

        }
        else
        {
            auxC = auxC->next;

            if (auxC == NULL)
            {
                auxC = c;
            }
        }
    }
   
    
    fclose(dados);
    free(tab); // Libera a memoria da estrutura.
    free(tab2); // Libera a memoria da estrutura.
    free(c);    // Libera a memoria da estrutura.
    free(auxT); // Libera a memoria da estrutura.
    // free(temp); // Libera a memoria da estrutura.
    return SUCCESS;
}


/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Utilizada para impressão de tabelas.
    Parametros: Nome da tabela (char).    
    Retorno:    void.
   ---------------------------------------------------------------------------------------------*/
    
void imprime(char nomeTabela[]) {

    int j,erro, x, p;
    struct fs_objects objeto = leObjeto(nomeTabela);    
    
    tp_table *esquema = leSchema(objeto);

    if(esquema == ERRO_ABRIR_ESQUEMA)
    {
        printf("Out of memory...Aborting\n");
        return;
    }

    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ERRO_DE_ALOCACAO)
    {
       printf("Out of memory...Aborting\n");
       return;
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++) {
        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);        
    }
    

    column *pagina = getPage(bufferpoll, esquema, objeto, 0);

    if(pagina == ERRO_PARAMETRO){
        free(bufferpoll);
       return;
    }

    int number_of_tuples = --x;
    int u = x,p_ = 0, v = 0;
    p = 0;
    int controler_ = 0;
    int limit[objeto.qtdCampos];//insere o tamanho máximo que será o espaço para impressão
    
    int ii = 0;

    for (ii = 0; ii < objeto.qtdCampos; ii++)
    {
        limit[ii] = strlen(pagina[ii].nomeCampo);
    }
    /**
     * Obtendo configurações do tamanho das variáveis
     * para impressão, onde o tamanho maior será o do
     * campo independente se for o registro ou o nome
     * da tabela
     */
    while(u){

        column *pagina = getPage(bufferpoll, esquema, objeto, p); 

        if(pagina == ERRO_PARAMETRO){
            free(bufferpoll);
            return;
        }
        for(j=0; j < objeto.qtdCampos*bufferpoll[p].nrec; j++){

            if(pagina[j].tipoCampo == 'S'){
                if (limit[controler_] < strlen(pagina[j].valorCampo))
                {
                    limit[controler_] = strlen(pagina[j].valorCampo);
                }

                free(pagina[j].valorCampo);
            }
            else if(pagina[j].tipoCampo == 'I'){

                int *n = (int *)&pagina[j].valorCampo[0];

                char *str = (char*) malloc (sizeof(char)*50);
                if (str == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }
                sprintf(str,"%d",*n);
                
                if (limit[controler_] < strlen(str))
                {
                    limit[controler_] = strlen(str);
                }
                free(str);
                free(pagina[j].valorCampo);
            }
            else if(pagina[j].tipoCampo == 'C'){

                char *str = (char*) malloc (sizeof(char));
                if (str == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }
                sprintf(str,"%c",pagina[j].valorCampo[0]);

                if (limit[controler_] < strlen(str))
                {
                    limit[controler_] = strlen(str);
                }

                free(str);
                free(pagina[j].valorCampo);
            }
            else if(pagina[j].tipoCampo == 'D'){

                double *n = (double *)&pagina[j].valorCampo[0];

                char *str = (char*) malloc (sizeof(char)*150);
                
                if (str == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }

                sprintf(str,"%f",*n);
                if (limit[controler_] < strlen(str))
                {
                    limit[controler_] = strlen(str)+1;//mais um por causa do '.'
                }
                free(str);
                free(pagina[j].valorCampo);
            }
            if(j >= 1 && ((j+1)%objeto.qtdCampos)==0){
               controler_ = 0;
            }
            else
            {
                controler_++;  
            }

        } 
        u-=bufferpoll[p++].nrec;
    }

    p = 0;
    controler_ = 0;
    ii = 0;
    int tam = 0;

    while(x){

        column *pagina = getPage(bufferpoll, esquema, objeto, p); 

        if(pagina == ERRO_PARAMETRO){
            free(bufferpoll);
            return;
        }
        for(j=0; j < objeto.qtdCampos*bufferpoll[p].nrec; j++){

            if (j == 0)
            {
                /**
                 * Imprime o nome das colunas
                 */
                while(controler_ < objeto.qtdCampos)
                {
                    printf(" %s",pagina[controler_].nomeCampo);
                    tam = (limit[controler_] - strlen(pagina[controler_].nomeCampo))+1;
                    for (v = 0; v < tam; v++) printf(" ");
                    printf("|");
                    controler_++;
                }
                printf("\n");
                p_ = 0;
                while(p_ < objeto.qtdCampos)
                {
                    for (v = 0; v < limit[p_]+2; v++)printf("-");
                    printf("+");
                    p_++;
                }
                printf("\n");
            }

            if(pagina[j].tipoCampo == 'S'){
                wchar_t *ws = (wchar_t*) malloc (sizeof(wchar_t)*501);

                swprintf(ws,500,L"%hs",pagina[j].valorCampo);
                int wn = wcslen(ws);

                // printf(" %d", wn);
                printf(" %s", pagina[j].valorCampo);
                tam = (limit[ii] - wn)+1;
                for (v = 0; v < tam; v++) printf(" ");
                printf("|");
                
                free(ws);
                free(pagina[j].valorCampo);
            }
            else if(pagina[j].tipoCampo == 'I'){
                int *n = (int *)&pagina[j].valorCampo[0];

                printf(" %d",*n);

                char *str = (char*) malloc (sizeof(char)*50);
                if (str == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }
                sprintf(str,"%d",*n);

                tam = ( limit[ii] - strlen(str) )+1;

                for (v = 0; v < tam; v++) printf(" ");
                printf("|");
                free(str);
                free(pagina[j].valorCampo);
            }
            else if(pagina[j].tipoCampo == 'C'){
                printf(" %c",pagina[j].valorCampo[0]);
                tam = (limit[ii] - strlen(pagina[j].valorCampo))+1;
                for (v = 0; v < tam; v++) printf(" ");
                printf("|");
                free(pagina[j].valorCampo);
            }
            else if(pagina[j].tipoCampo == 'D'){
                double *n = (double *)&pagina[j].valorCampo[0];
                printf(" %f",*n);

                char *str = (char*) malloc (sizeof(char)*150);
                if (str == NULL)
                {
                    printf("Out of memory.\nAborting...\n");
                    abort();
                }
                sprintf(str,"%f",*n);

                tam = (limit[ii] - strlen(str))+1;
                for (v = 0; v < tam; v++) printf(" ");
                printf("|");
                free(str);
                 free(pagina[j].valorCampo);

            }
            if(j>=1 && ((j+1)%objeto.qtdCampos)==0){
                ii = 0;
                printf("\n");
            }
            else
            {
                ii++;
            }
        } 

        x-=bufferpoll[p++].nrec;
    }

    if (number_of_tuples == 1) {
        printf("( %d record )\n",number_of_tuples);
    } else {
        printf("( %d records )\n",number_of_tuples);
    }
    printf("\n");
   
    free(pagina);
    free(bufferpoll);
}

/* ---------------------------------------------------------------------------------------------- 
    Objetivo:   Verifica se o nome da tabela 'nomeTabela' está nos primeiros bytes de 'linha'
    Parametros: Nome da tabela, char linha.    
    Retorno:    INT(1 - Está contido, 0 - Não está)                
   ---------------------------------------------------------------------------------------------*/

int TrocaArquivosObj(char *nomeTabela, char *linha){
    int x = 0;
    char *tabela = (char *)malloc(sizeof(char) * TAMANHO_NOME_TABELA);

    if (tabela == NULL)
    {
        printf("Out memory.\nAborting...\n");
        abort();
    }

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

    if (esquema == NULL || vetEsqm == NULL || tupla == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    if((schema = fopen("fs_schema.dat", "a+b")) == NULL){
        printf("Couldn't open schema.\nAborting...\n");
        free(tupla);
        free(esquema);
        free(vetEsqm);
        abort();
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

/**
 * Exclui a tabela caso queira remover o banco
 * Passa o nome da tabela e o indice do banco
 */



int delete_table(char *name)
{
    struct fs_objects object;
    tp_table *schema;

    abreTabela(name, &object, &schema);
    /**
     * Remove dados do objeto e do esquema
     */
    procuraSchemaArquivo(object);
    procuraObjectArquivo(name);

    int n = strlen(name)+5;

    char *file = (char*) malloc (sizeof(char)*n);
    
    strcpy(file,name);

    if (file == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }
    
    strcat(file,".dat"); 

    if(existeArquivo(file)) 
    {
        remove(file);
    }

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

int excluirTabela(char *nomeTabela) {

    struct fs_objects objeto, objeto1;
    tp_table *esquema, *esquema1;
    int x,erro, i, j, k, l, qtTable;
    char *str = (char*) malloc (sizeof(char)*(strlen(nomeTabela)+strlen(".dat")+1)); 
    
    if (str == NULL)
    {
        printf("Out of memory.\nAborting...\n");
        abort();
    }

    strcpy (str, nomeTabela); 
    strcat (str, ".dat");              //Concatena e junta o nome com .dat

    if (existeArquivo(str) == 0)
    {

        delete_table(nomeTabela);

        return SUCCESS;
    }

    abreTabela(nomeTabela, &objeto, &esquema);
    qtTable = quantidadeTabelas();

    char **tupla = (char**) malloc (sizeof(char)*qtTable);

    if (tupla == NULL)
    {
        if(str != NULL)free(str);
        printf("Out of memory.\nAborting...\n");

        abort();
    }

    for(i = 0; i < qtTable; i++) {

        tupla[i] = (char *) malloc (sizeof(char)*TAMANHO_NOME_TABELA);

        if (tupla[i] == NULL)
        {
            if (str != NULL)
            {
                free(str);
                
            }
            if (str != NULL)
            {
                free(tupla);
                
            }
            printf("Out of memory.\nAborting...\n");

            abort();
        }
    }

    tp_table *tab2 = (tp_table *)malloc(sizeof(struct tp_table));

    if (tab2 == NULL)
    {
        if (str != NULL)
        {
            free(str);
            
        }
        if (str != NULL)
        {
            free(tupla);
            
        }
        printf("Out of memory.\nAborting...\n");

        abort();
    }

    tab2 = procuraAtributoFK(objeto);   //retorna o tipo de chave que e cada campo

    FILE *dicionario;

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL) 
    {
        if (str != NULL)
        {
            free(str);
            
        }
        if (tupla != NULL)
        {
            free(tupla);
            
        }
        if(tab2 != NULL)
        {
            free(tab2);
        }
        return ERRO_ABRIR_ARQUIVO;
    }

    k = 0;
    while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        //coloca o nome de todas as tabelas em tupla
        fread(tupla[k], sizeof(char),TAMANHO_NOME_TABELA , dicionario);
        k++;
        fseek(dicionario, 28, 1);
    }

    fclose(dicionario);
    
    for(i = 0; i < objeto.qtdCampos; i++){
        if(tab2[i].chave == PK){
            for(j=0; j < qtTable; j++) {                      //se tiver chave primaria verifica se ela e chave
                if(strcmp(tupla[j], nomeTabela) != 0) {     //estrangeira em outra tabela

                    abreTabela(tupla[j], &objeto1, &esquema1);

                    tp_table *tab3 = (tp_table *)malloc(sizeof(struct tp_table));
                    
                    if (tab3 == NULL)
                    {
                        if(tab2 != NULL)
                        {
                            free(tab2);
                        }
                        if (str != NULL)
                        {
                            free(str);
                            
                        }
                        if (tupla != NULL)
                        {
                            free(tupla);
                            
                        }
                        printf("Out of memory.\nAborting...\n");
                        abort();
                    }
                    tab3 = procuraAtributoFK(objeto1);

                    for(l = 0; l < objeto1.qtdCampos; l++) {   
                        if(tab3[l].chave == FK) { 
                            // printf("'%s' e '%s' \n",nomeTabela, tab3[l].tabelaApt );
                            //verifica se a outra tabela possui
                            if(strcmp(nomeTabela, tab3[l].tabelaApt) == 0) {    //chave estrangeira
                                printf("Cannot delete row due to foreign key constraint!\n");            //se sim, verifica se e da tabela
                                if(tab2 != NULL)
                                {
                                    free(tab2);
                                }
                                if (str != NULL)
                                {
                                    free(str);
                                    
                                }
                                if (tupla != NULL)
                                {
                                    free(tupla);
                                }
                                if(tab3 != NULL)
                                {
                                    free(tab3);
                                }
                                return ERRO_CHAVE_ESTRANGEIRA;                  //anterior
                            }
                        }
                    }
                    if(tab3 != NULL)free(tab3);
                }
            }
        }
    }

    if(tab2 != NULL)free(tab2);

    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ERRO_DE_ALOCACAO){
        if (str != NULL)
        {
            free(str);
            
        }
        if (tupla != NULL)
        {
            free(tupla);
            
        }
        printf("Out of memory.\nAborting...\n");
        return ERRO_LEITURA_DADOS;
    }

    erro = SUCCESS;

    for(x = 0; erro == SUCCESS; x++){
        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);        
    }
    
    if(procuraSchemaArquivo(objeto) != 0){//exclui esquema
        if (str != NULL)
        {
            free(str);
            
        }
        if (tupla != NULL)
        {
            free(tupla);
            
        }
        return ERRO_REMOVER_ARQUIVO_SCHEMA;
        
    }

    if(procuraObjectArquivo(nomeTabela) != 0){//exclui objeto
         if (str != NULL)
        {
            free(str);
            
        }
        if (tupla != NULL)
        {
            free(tupla);
            
        }

        return ERRO_REMOVER_ARQUIVO_OBJECT;
    }

    if(existeArquivo(str)) 
    {
        remove(str);
    }

    if (str != NULL)
    {
        free(str);
        
    }
    if (tupla != NULL)
    {
        free(tupla);
        
    }

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
            free(pagina);
            free(bufferpoll);
            return ERRO_DE_PARAMETRO;
        }
    }
    free(pagina);
    free(bufferpoll);
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

        if(strcmp(pagina[j].nomeCampo, attApt) == 0){
            
            if(pagina[j].tipoCampo == 'S'){     
                if(strcmp(pagina[j].valorCampo, valorCampo) == 0){
                    free(pagina);
                    free(bufferpoll);
                    return SUCCESS;
                }
            }

            else if(pagina[j].tipoCampo == 'I'){ 
                int *n = (int *)&pagina[j].valorCampo[0];
                
                if(*n == atoi(valorCampo)){
                    free(pagina);
                    free(bufferpoll);
                    return SUCCESS;
                }
            }

            else if(pagina[j].tipoCampo == 'D'){ 
                double *nn = (double *)&pagina[j].valorCampo[0];

                if(*nn == atof(valorCampo)){
                    free(pagina);
                    free(bufferpoll);
                    return SUCCESS;
                }
            }

            else if(pagina[j].tipoCampo == 'C'){                        
                if(pagina[j].valorCampo == valorCampo){
                    free(pagina);
                    free(bufferpoll);
                    return SUCCESS;
                }
            }else {
                free(pagina);
                free(bufferpoll);
                return ERRO_CHAVE_ESTRANGEIRA;
            }
        }            
    }
    free(pagina);
    free(bufferpoll);
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
                    free(pagina);
                    free(bufferpoll);
                    return ERRO_CHAVE_PRIMARIA;
                }
            }

            else if(pagina[j].tipoCampo == 'I'){ 
                int *n = (int *)&pagina[j].valorCampo[0];

                if(*n == atoi(valorCampo)){
                    free(pagina);
                    free(bufferpoll);
                    return ERRO_CHAVE_PRIMARIA;
                }
            }

            else if(pagina[j].tipoCampo == 'D'){ 
                double *nn = (double *)&pagina[j].valorCampo[0];

                if(*nn == atof(valorCampo)){
                    free(pagina);
                    free(bufferpoll);
                    return ERRO_CHAVE_PRIMARIA;
                }
            }

            else if(pagina[j].tipoCampo == 'C'){                        
                if(pagina[j].valorCampo == valorCampo){
                    free(bufferpoll);
                    return ERRO_CHAVE_PRIMARIA;
                }
            }
        }            
    }

    free(bufferpoll);

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
