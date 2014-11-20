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
|* FUNÇÃO: Utilizada no controle do 'menu', com todas as opcoes.                   */   

void menu(){
    int acao, opcao, y;
    char c[5];
    y = 1;

    while(y){
        system("clear");
        
        printf(" -> Tabela Carro\n"
            " -> Tabela Modelo \n"
            " -> Tabela Fabricante \n"
            " -> Tabela Proprietário\n");

        printf( "\nDentre as tabelas acima, digite: \n1 - Inserir valores\n2 - Imprimir valores\n3 - Excluir valores\n4 - Sair");
        printf("\nOpcao -> ");
        scanf("%d",&opcao);

        if(opcao == 1){     //inserir valores
            system("clear");                                        
            printf(" 1- Tabela Carro\n"
                " 2- Tabela Modelo \n"
                " 3- Tabela Fabricante \n"
                " 4- Tabela Proprietário\n"
                " 5- Voltar\n");

            printf( "\nEscolha a tabela, dentre uma das mencionadas acima, para inserir valores");
            printf("\nOpcao -> ");
            scanf("%d",&acao);
        
            switch (acao){

                case 1:     
                    insere(1);

                    break;  

                case 2:
                    insere(2);

                    break;  

                case 3:
                    insere(3);

                    break;     

                case 4:
                    insere(4);

                    break;        

                case 5:     
                    break;
        
                default:
                    printf("\nOpção invalida! Digite algum número para voltar\n");
                    scanf("%s", c);

                    break;      //default
            }   
                    
        }
        else if(opcao == 2){ //imprimir tabelas
        
            system("clear");
            printf(" 1- Tabela Carro\n"
                   " 2- Tabela Modelo \n"
                   " 3- Tabela Fabricante \n"
                   " 4- Tabela Proprietário\n"
                   " 5- Retornar ao menu Principal\n"
                   " 6- Sair\n");

            printf( "\nEscolha a tabela para imprimir, dentre uma das mencionadas acima.");
            printf("\nOpcao -> ");
            scanf("%d",&acao);
            
            switch (acao){
                case 1:
                    system("clear");
                    imprime("Carro");
                    printf("Digite algum número para voltar\n");
                    scanf("%s", c);

                    break;

                case 2:
                    system("clear");
                    imprime("Modelo");
                    printf("Digite algum número para voltar\n");
                    scanf("%s", c);
                
                    break;

                case 3:
                    system("clear");
                    imprime("Fabricante");
                    printf("Digite algum número para voltar\n");
                    scanf("%s", c);

                    break;  

                case 4:
                    system("clear");
                    imprime("Proprietario");
                    printf("Digite algum número para voltar\n");
                    scanf("%s", c);
                
                    break;

                case 5:
                                        
                    break;

                case 6:     //opcao sair do programa
                    return;
                    break;
        
                default:
                    printf("\nOpção invalida! Digite algum número para voltar\n");
                    scanf("%s", c);

                    break;  //default
            }
            
        }
        else if(opcao == 3){
            
            system("clear");
            printf(" 1- Tabela Carro\n"
                " 2- Tabela Modelo \n"
                " 3- Tabela Fabricante \n"
                " 4- Tabela Proprietário\n"
                " 5- Sair\n");

            printf( "\nEscolha a tabela, dentre uma das mencionadas acima para exclui-la");
            printf("\nOpcao -> ");
            scanf("%d",&acao);
        
            switch (acao){
                case 1:
                    excluirArquivo("Carro");
                    printf("Digite alguma tecla para voltar.\n");
                    scanf("%s", c);
                    break;

                case 2:
                    excluirArquivo("Modelo");
                    printf("Digite alguma tecla para voltar.\n");
                    scanf("%s", c);

                    break;

                case 3:
                    excluirArquivo("Fabricante");
                    printf("Digite alguma tecla para voltar.\n");
                    scanf("%s", c);

                    break;

                case 4:
                    excluirArquivo("Proprietario");
                    printf("Digite alguma tecla para voltar.\n");
                    scanf("%s", c);
                
                    break;            

                case 5:     //opcao sair do programa
                    break;
        
                default:
                    printf("\nOpção invalida! Digite algum número para voltar\n");
                    scanf("%s", c);

                    break;   //default
            }
            
        }else if(opcao == 4){    //sair
            return;
        }
    } 
}

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
|* FUNÇÃO: Inserir tuplas na tabela selecionada conforme 'menu'.                   */   
    
int insere(int menu){
    int nrTabelas,erro, object, schema, carro, modelo, fabricante, proprietario;  
    char var[100];
    nrTabelas = 4;
    table  *tab[nrTabelas];
    column *colunas = NULL;
    
    object         = existeArquivo("fs_object.dat");
    schema         = existeArquivo("fs_schema.dat");
    carro          = existeArquivo("Carro.dat");
    modelo         = existeArquivo("Modelo.dat");
    fabricante     = existeArquivo("Fabricante.dat");
    proprietario   = existeArquivo("Proprietario.dat");
    if(!object || !schema){

        //Carro
        if(!carro){
            tab[0] = iniciaTabela("Carro");   
            tab[0] = adicionaCampo(tab[0], "Chassi"     , 'S', 20);
            tab[0] = adicionaCampo(tab[0], "Kms"        , 'I', (sizeof(int)));
            tab[0] = adicionaCampo(tab[0], "DataCompra" , 'S', 10);
            tab[0] = adicionaCampo(tab[0], "Ano"        , 'I', (sizeof(int)));
            tab[0] = adicionaCampo(tab[0], "CodModelo"  , 'I', (sizeof(int)));
            tab[0] = adicionaCampo(tab[0], "Motor"      , 'S', 10);
            erro   = finalizaTabela(tab[0]);
        }

        //Modelo
        if(!modelo){
            tab[1] = iniciaTabela("Modelo"); 
            tab[1] = adicionaCampo(tab[1], "CodModelo"     , 'I', (sizeof(int)));
            tab[1] = adicionaCampo(tab[1], "NomeMod"       , 'S', 20);
            tab[1] = adicionaCampo(tab[1], "CodFabricante" , 'I', (sizeof(int)));
            erro   = finalizaTabela(tab[1]);
        }

        //Fabricante
        if(!fabricante){
            tab[2] = iniciaTabela("Fabricante"); 
            tab[2] = adicionaCampo(tab[2], "CodFabricante"  , 'I', (sizeof(int)));
            tab[2] = adicionaCampo(tab[2], "NomeFab"        , 'S', 20);
            erro   = finalizaTabela(tab[2]);
        }

        //Proprietário
        if(!proprietario){
            tab[3] = iniciaTabela("Proprietario");
            tab[3] = adicionaCampo(tab[3], "Rg"        , 'S', 12);    
            tab[3] = adicionaCampo(tab[3], "Nome"      , 'S', 45);    
            tab[3] = adicionaCampo(tab[3], "DataNasc"  , 'S', 15);    
            tab[3] = adicionaCampo(tab[3], "Email"     , 'S', 50);    
            erro   = finalizaTabela(tab[3]); 
        }
    }

    switch (menu){
        case 1:
            printf("\nDigite o codigo do Carro (Chassi):");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "Chassi", var, "Carro", 1);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;

            printf("\nDigite a Kilometragem do Carro:");
            scanf("%s", var);
            colunas = insereValor(colunas, "Kms", var, "Carro", 0);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;            

            printf("\nDigite a Data da Compra:");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "DataCompra", var, "Carro", 0);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;                        

            printf("\nDigite o Ano do Carro:");
            scanf("%s", var);
            colunas = insereValor(colunas, "Ano", var, "Carro", 0);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;            
            
            printf("\nDigite o codigo do Modelo do carro:");
            scanf("%s", var);
            colunas = insereValor(colunas, "CodModelo", var, "Carro", 2);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;            

            printf("\nDigite a potência Motor do carro:");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "Motor", var, "Carro", 0);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;            
            
            erro = finalizaInsert("Carro", colunas);    

            break;

        case 2:
            printf("\nDigite o codigo do modelo do carro:");
            scanf("%s", var);
            colunas = insereValor(colunas, "CodModelo", var, "Modelo", 1);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            printf("\nDigite o Nome do Modelo:");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "NomeMod", var, "Modelo", 0);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            printf("\nDigite o codigo do Fabricante:");
            scanf("%s", var);
            colunas = insereValor(colunas, "CodFabricante", var, "Modelo", 2);              
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            erro = finalizaInsert("Modelo", colunas);   

            break;
            
        case 3: 
            printf("\nDigite o codigo do Fabricante:");
            scanf("%s", var);
            colunas = insereValor(colunas, "CodFabricante", var, "Fabricante", 1);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   
            
            printf("\nDigite o Nome do Fabricante:");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "NomeFab", var, "Fabricante", 0);         
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            erro = finalizaInsert("Fabricante", colunas);                       

            break;
            
        case 4:  
            printf("\nDigite RG do Proprietário do Veículo:");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "Rg", var, "Proprietario", 1);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            printf("\nDigite o Nome do Proprietário do Veículo (até 11 caracteres):");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "Nome", var, "Proprietario", 0);
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            printf("\nDigite a data de nascimento do Proprietário do Veículo:");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "DataNasc", var, "Proprietario", 0); 
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            printf("\nDigite o Email do Proprietário do Veículo (até 11 caracteres):");
            getchar();
            scanf("%[^\n]", var);
            colunas = insereValor(colunas, "Email", var, "Proprietario", 0);           
            if (!colunas) return ERRO_CHAVE_PRIMARIA;   

            erro = finalizaInsert("Proprietario", colunas);     

            break;

        default:
            printf("\nOpção Invalida!\n");

            break;
    }

    if(erro != 0){
        printf("Erro na criação das tabelas!\n");
        return ERRO_CRIACAO_TABELA;
    }
    
    return SUCCESS;
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

/***********************************************************************************|
|* FUNÇÃO: Exclui a tabela com 'nome'                                              */   

void excluirArquivo(char nome[]){

    char str[20]; 
    char dat[5] = ".dat";
    
    strcpy (str, nome); 
    strcat (str, dat);              //Concatena e junta o nome com .dat

    if(!existeArquivo(str)){
        system("clear");
        printf("Arquivo não existe!\n");
        return;
    }

    FILE* fptr = fopen(str, "w");
    if(fptr != NULL){   
        fclose(fptr);

        return;
    }

    while(getc(fptr) != EOF){       //Adiciona \0 no arquivo inteiro.
        fwrite("\0", 1, 1, fptr);
    }

    printf("\nTodos os registros da tabela %s foram excluídos.\n"
           "Digite algum número para voltar\n", nome);
}

int verificaNomeChave(char *nomeTabela, char *nomeCampo, char *valorCampo, int chave){
    int j, x, erro;

    if(chave == 0)
        return SUCCESS;

    printf("T: %s\n", nomeTabela);
    struct fs_objects objeto = leObjeto(nomeTabela);    
    tp_table *tabela         = leSchema(objeto);

    if(tabela == ERRO_ABRIR_ESQUEMA){
        printf("Erro ao criar o esquema.\n");
        return ERRO_DE_PARAMETRO;
    }

    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ERRO_DE_ALOCACAO){
        printf("Erro ao alocar memória para o buffer.\n");
        return ERRO_DE_PARAMETRO;
    }


    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);        

    column *pagina = getPage(bufferpoll, tabela, objeto, 0);

    if(chave == 1){
        for(j = 0; j < objeto.qtdCampos*bufferpoll[0].nrec; j++){

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
    }

    return 9;
}