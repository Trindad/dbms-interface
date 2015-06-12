#include <stdlib.h>
#include <string.h>
#include "buffend.h"

int main(){
    char entrada[50], *dividido=NULL, operacao[12], nomeBD[30], nomeaux[30];
	int resultado=0, codDB=0;
	
	nomeBD[0]=NULL;
	
	while(1){		
		/*printf("\n-----------------------------\n");
		printf("| Voce ta ja no nosso shell |\n");
		printf("-----------------------------\n");
		printf("| Só digita os comando teu: |\n");
		printf("-----------------------------\n\n");*/
		
		if(nomeBD[0] == NULL)
			printf("> ");
		else
			printf("%s> ", nomeBD);
        
        fgets(entrada,50,stdin);
		dividido = strtok(entrada," ");
		
		strcpy(operacao,dividido);
		
		if(strcmp(operacao,"createdb\0")==0){
			printf("deu certo %s\n",operacao);//o nome dado ao banco será guardado
			dividido = strtok('\0'," ");
			printf("sobrou  %s\n",dividido);
			resultado = checkCreateDB(dividido);//verifica a existência do nome e grava-o no arquivo
	
			if(resultado==-1)
				printf("erro ao tentar criar arquivo fs_database\n");
			if(resultado==1)
				printf("Banco de dados já existente no sistema");
			else
				printf("Finaliso\n");
		}
				
		if(strcmp(operacao,"conectdb\0")==0){
			//CRIAR AQUI PARTE DO CODIGO QUE IDENTIFICARA QUE A OPERAÇÃO É CONECTDB E CHAMAA A RESPECTIVA FUNÇÃO PRA FAZER ISSO
					
			dividido = strtok('\0'," ;");				//apaga a palavra conectdb e passa para dividido a sobra, que seria o nome do bd
			strcpy(nomeaux, dividido);					//só copia o nome do banco para nomeaux, para depois impressao
				
			codDB = busca(dividido);					//função chamada para conecção no banco, retorna o codigo do banco ao conectar
			
			if (codDB != 0)
				strcpy(nomeBD, nomeaux);				//passa o nome do bd, para a variavel mostrar ao usuario qual o banco conectado
			else
				printf("\nBanco %s não cadastrado!\n", nomeaux);
        
        }
		if(strcmp(operacao,"createtable\0")==0){
			//NAO VAI SER CRIADO, MAS NOSSA IDEIA É QUE AO ESCREVER SOMENTE ISSO, CHAME UMA FUNÇÃO PARA TESTE,
			//ESTA FUNÇAO DE TESTE DEVERIA CONTER O QUE ESTA NO RESTO DESTA MAIN, E NESTA MAIN POSSUIR APENAS O SHELL
		}	
		if(strcmp(operacao,"insert\0")==0){
			//VER COM A SILVANA COMO PODEMOS FAZER ESSA PARTE, POIS A IDEIA É QUE O INSERT DELA SEJA CHAMADA A PARTIR DESTE SHELL
		}
		if(strcmp(operacao,"*d\0")==0){
			//CRIAR AQUI PARTE DO CODIGO QUE IDENTIFICARA QUE A OPERAÇÃO É *D E CHAMAA A RESPECTIVA FUNÇÃO PRA LISTAR TABELAS
		}	
		if(strcmp(operacao,"*l\0")==0){
			//CRIAR AQUI PARTE DO CODIGO QUE IDENTIFICARA QUE A OPERAÇÃO É *L E CHAMAA A RESPECTIVA FUNÇÃO PRA LISTAR BANCOS
		}	
		if(strcmp(operacao,"exit\0")==0){
			break;
		}	
		
		//TODAS AS FUNÇÕES ESTAO SENDO CRIADAS NO buffend.c E buffend.h, MAS A IDEIA DE SEPARAR CONTINUA, SÓ QUE DEPOIS
	}
	
    int nrTabelas = 3;
    int nTabela[nrTabelas];
    table  *tab[nrTabelas]; 
    column *colunas;
    int object, schema;
    
    object      = existeArquivo("fs_object.dat");
    schema      = existeArquivo("fs_schema.dat");
    
    nTabela[0]  = existeArquivo("Aluno.dat");
    nTabela[1]  = existeArquivo("Inst.dat");
    nTabela[2]  = existeArquivo("Inscri.dat");
    
    if(!object || !schema){
    
        if(!nTabela[0]){                                                                    //Se ainda não existe a Aluno, a mesma é criada
            tab[0] = iniciaTabela("Aluno");                                                 //Cria a tabela 
            tab[0] = adicionaCampo(tab[0], "CPF"     , 'I', (sizeof(int))   ,PK,"","");     //Cria os atributos
            tab[0] = adicionaCampo(tab[0], "Nome"    , 'S', 20              ,NPK,"","");        
            tab[0] = adicionaCampo(tab[0], "Endereco", 'S', 20              ,NPK,"","");
            tab[0] = adicionaCampo(tab[0], "Peso"    , 'D', (sizeof(double)),NPK,"","");
            finalizaTabela(tab[0]);
         }
         if(!nTabela[1]){   
            tab[1] = iniciaTabela("Inst"); 
            tab[1] = adicionaCampo(tab[1], "CodInst"  , 'I', (sizeof(int))   ,PK , "","");
            tab[1] = adicionaCampo(tab[1], "Nome"     , 'S', 20              ,NPK, "","");
            tab[1] = adicionaCampo(tab[1], "Endereco" , 'S', 20              ,NPK, "","");
            tab[1] = adicionaCampo(tab[1], "Reitor"   , 'S', 10              ,NPK, "","");
            finalizaTabela(tab[1]);
        }
        if(!nTabela[2]){ 
            tab[2] = iniciaTabela("Inscri"); 
            tab[2] = adicionaCampo(tab[2], "CodMat"     , 'I', (sizeof(int))  ,PK, "","");
            tab[2] = adicionaCampo(tab[2], "CPF"        , 'I', (sizeof(int))  ,FK, "Aluno","CPF");
            tab[2] = adicionaCampo(tab[2], "CodInst"    , 'I', (sizeof(int))  ,FK , "Inst","CodInst");
            tab[2] = adicionaCampo(tab[2], "Curso"   , 'S',  20  ,NPK, "","");
            finalizaTabela(tab[2]);
        }
       
         
         //Inserção de tuplas na tabela1   
        colunas = NULL;     
        colunas = insereValor(tab[0],colunas, "CPF", "123456");
        colunas = insereValor(tab[0],colunas, "Nome", "Rogerio");
        colunas = insereValor(tab[0],colunas, "Endereco", "Rua Marechal");
        colunas = insereValor(tab[0],colunas, "Peso", "81.4");
        finalizaInsert("Aluno", colunas); 
        
        colunas = NULL;     
        colunas = insereValor(tab[0],colunas, "CPF", "654321");
        colunas = insereValor(tab[0],colunas, "Nome", "Ricardo");
        colunas = insereValor(tab[0],colunas, "Endereco", "RuaClevela");
        colunas = insereValor(tab[0],colunas, "Peso", "88.9");
        finalizaInsert("Aluno", colunas); 

        colunas = NULL;     
        colunas = insereValor(tab[0],colunas, "CPF", "1234567");
        colunas = insereValor(tab[0],colunas, "Nome", "Natan");
        colunas = insereValor(tab[0],colunas, "Endereco", "RuaDelmi");
        colunas = insereValor(tab[0],colunas, "Peso", "58.9");
        finalizaInsert("Aluno", colunas); 
            
        
        //Inserção de tuplas na tabela2 
        colunas = NULL;
        colunas = insereValor(tab[1],colunas, "CodInst", "111");
        colunas = insereValor(tab[1],colunas, "Nome", "UFFS");
        colunas = insereValor(tab[1],colunas, "Endereco", "RuadeTerra");
        colunas = insereValor(tab[1],colunas, "Reitor", "MandaChuva");
        finalizaInsert("Inst", colunas);
        
        colunas = NULL;
        colunas = insereValor(tab[1],colunas, "CodInst", "222");
        colunas = insereValor(tab[1],colunas, "Nome", "CEFET");
        colunas = insereValor(tab[1],colunas, "Endereco", "RuadePedra");
        colunas = insereValor(tab[1],colunas, "Reitor", "MandaVento");
        finalizaInsert("Inst", colunas);

        colunas = NULL;
        colunas = insereValor(tab[1],colunas, "CodInst", "333");
        colunas = insereValor(tab[1],colunas, "Nome", "UNOESC");
        colunas = insereValor(tab[1],colunas, "Endereco", "RuadeAsfal");
        colunas = insereValor(tab[1],colunas, "Reitor", "MandaAgua");
        finalizaInsert("Inst", colunas);
        
        
        
        //Inserção de tupla na tabela3
        colunas = NULL;
        colunas = insereValor(tab[2],colunas, "CodMat", "1401");
        colunas = insereValor(tab[2],colunas, "CPF", "123456");
        colunas = insereValor(tab[2],colunas, "CodInst", "333");
        colunas = insereValor(tab[2],colunas, "Curso", "CC");
        finalizaInsert("Inscri", colunas);
        
        colunas = NULL;
        colunas = insereValor(tab[2],colunas, "CodMat", "1402");
        colunas = insereValor(tab[2],colunas, "CPF", "654321");
        colunas = insereValor(tab[2],colunas, "CodInst", "222");
        colunas = insereValor(tab[2],colunas, "Curso", "CC");
        finalizaInsert("Inscri", colunas);
        
        colunas = NULL;
        colunas = insereValor(tab[2],colunas, "CodMat", "1403");
        colunas = insereValor(tab[2],colunas, "CPF", "1234567");
        colunas = insereValor(tab[2],colunas, "CodInst", "111");
        colunas = insereValor(tab[2],colunas, "Curso", "ADM");
        finalizaInsert("Inscri", colunas);

    }
    
    
    imprime("Aluno");        //Imprime os atributos da tabela "Aluno"
    imprime("Inst");
    //excluirTabela("Inst");   //Exclui os dados da tabela do dicionario e remove-a do disco
    imprime("Inscri");
    //excluirTabela("Inscri");
    
    
    return 0;
}
