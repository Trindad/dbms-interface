#include <stdlib.h>
#include <string.h>
#include "buffend.h"
#include "funcoesNRR.h"

int main(){
    int nrTabelas = 4;
    int nTabela[nrTabelas];
    table  *tab[nrTabelas];	
    column *colunas = NULL;
    int object, schema,erro;
    
    object      = existeArquivo("fs_object.dat");
    schema		= existeArquivo("fs_schema.dat");
    nTabela[0]  = existeArquivo("tabela1.dat");
    nTabela[1]  = existeArquivo("tabela2.dat");
    nTabela[2]  = existeArquivo("tabela3.dat");
    nTabela[3]  = existeArquivo("tabela4.dat");
     if(!object || !schema){
    
		if(!nTabela[0]){ 															//Se ainda não existe a tabela1, a mesma é criada
			tab[0] = iniciaTabela("tabela1");  												//Cria a tabela 
			tab[0] = adicionaCampo(tab[0], "atrib11" , 'I', (sizeof(int))	,PK,"","");		//Cria os atributos
			tab[0] = adicionaCampo(tab[0], "atrib12" , 'I', (sizeof(int))	,NPK,"","");
			tab[0] = adicionaCampo(tab[0], "atrib13" , 'S', 10				,NPK,"","");
			tab[0] = adicionaCampo(tab[0], "atrib14" , 'D', (sizeof(double)),NPK,"","");
			finalizaTabela(tab[0]);
		 }
		 if(!nTabela[1]){	//Se ainda não existe a tabela2, a mesma é criada
			tab[1] = iniciaTabela("tabela2"); 
			tab[1] = adicionaCampo(tab[1], "atrib21" , 'I', (sizeof(int))	,PK, "","");
			tab[1] = adicionaCampo(tab[1], "atrib22" , 'S', 20				,NPK, "","");
			tab[1] = adicionaCampo(tab[1], "atrib23" , 'I', (sizeof(int))	,NPK, "","");
			tab[1] = adicionaCampo(tab[1], "atrib24" , 'S', 10				,NPK, "","");
			finalizaTabela(tab[1]);
		}
		if(!nTabela[2]){	//Se ainda não existe a tabela3, a mesma é criada
			tab[2] = iniciaTabela("tabela3"); 
			tab[2] = adicionaCampo(tab[2], "atrib31" , 'I', (sizeof(int))	,FK, "tabela1","atrib11");
            printf("tab: %s\n", tab[2]->esquema->tabelaApt);
            printf("apt: %s\n", tab[2]->esquema->attApt);

			tab[2] = adicionaCampo(tab[2], "atrib32" , 'S', 20				,NPK, "","");
			finalizaTabela(tab[2]);
		}
	}
	 
	//Inserção de tuplas na tabela1        
	colunas = insereValor(colunas, "atrib11", "10");
	colunas = insereValor(colunas, "atrib12", "35");
	colunas = insereValor(colunas, "atrib13", "teste13");
	colunas = insereValor(colunas, "atrib14", "1.4");
	erro = finalizaInsert("tabela1", colunas); 
	
	colunas = NULL;
	colunas = insereValor(colunas, "atrib11", "11");
	colunas = insereValor(colunas, "atrib12", "30");
	colunas = insereValor(colunas, "atrib13", "teste34"	);
	colunas = insereValor(colunas, "atrib14", "1.7");
	erro = finalizaInsert("tabela1", colunas); 

	colunas = NULL;
	colunas = insereValor(colunas, "atrib11", "12");
	colunas = insereValor(colunas, "atrib12", "33");
	colunas = insereValor(colunas, "atrib13", "teste340");
	colunas = insereValor(colunas, "atrib14", "1.9");
	erro = finalizaInsert("tabela1", colunas); 
	//Inserção de tuplas na tabela1        

	//Inserção de tuplas na tabela2	
	colunas = NULL;
	colunas = insereValor(colunas, "atrib21", "10");
	colunas = insereValor(colunas, "atrib22", "dez");
	colunas = insereValor(colunas, "atrib23", "10");
	colunas = insereValor(colunas, "atrib24", "dez");
	erro = finalizaInsert("tabela2", colunas);
	
	colunas = NULL;
	colunas = insereValor(colunas, "atrib21", "11");
	colunas = insereValor(colunas, "atrib22", "onze");
	colunas = insereValor(colunas, "atrib23", "11");
	colunas = insereValor(colunas, "atrib24", "onze");
	erro = finalizaInsert("tabela2", colunas);
	//Inserção de tuplas na tabela2    

	//Inserção de tupla na tabela3
	colunas = NULL;
	colunas = insereValor(colunas, "atrib31", "10");
	colunas = insereValor(colunas, "atrib32", "M");
	erro = finalizaInsert("tabela3", colunas);

    colunas = NULL;
    colunas = insereValor(colunas, "atrib31", "11");
    colunas = insereValor(colunas, "atrib32", "N");
    erro = finalizaInsert("tabela3", colunas);

    colunas = NULL;
    colunas = insereValor(colunas, "atrib31", "12");
    colunas = insereValor(colunas, "atrib32", "NO");
    erro = finalizaInsert("tabela3", colunas);
    //Inserção de tupla na tabela3
    
	if(erro == ERRO_CHAVE_PRIMARIA) {
		printf("ERRO DE CHAVE PRIMARIA !!! \n");
		return erro;
	}
	if(erro == ERRO_CHAVE_ESTRANGEIRA) {
		printf("ERRO DE CHAVE ESTRANGEIRA !!! \n");
		return erro;
	}
		
	//excluirArquivo("tabela8");
	imprime("tabela1");
    imprime("tabela2");
    imprime("tabela3");

    
    return 0;
}
