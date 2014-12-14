#include <stdlib.h>
#include <string.h>
#include "buffend.h"
#include "funcoesNRR.h"

int main(){
    int nrTabelas = 3;
    int nTabela[nrTabelas];
    table  *tab[nrTabelas];	
    column *colunas = NULL;
    int object, schema,erro;
    
    object      = existeArquivo("fs_object.dat");
    schema		= existeArquivo("fs_schema.dat");
    nTabela[0]  = existeArquivo("tabela1.dat");
    nTabela[1]  = existeArquivo("tabela2.dat");
    nTabela[2]  = existeArquivo("tabela3.dat");
     if(!object || !schema){
    
		if(!nTabela[0]){ 															//Se ainda não existe a tabela1, a mesma é criada
			tab[0] = iniciaTabela("tabela1");  												//Cria a tabela 
			tab[0] = adicionaCampo(tab[0], "atrib11" , 'I', (sizeof(int))	,PK,"","");		//Cria os atributos
			tab[0] = adicionaCampo(tab[0], "atrib12" , 'I', (sizeof(int))	,NPK,"","");
			tab[0] = adicionaCampo(tab[0], "atrib13" , 'S', 10				,NPK,"","");
			tab[0] = adicionaCampo(tab[0], "atrib14" , 'D', (sizeof(double)),NPK,"","");
			finalizaTabela(tab[0]);
		 }
	 }
	 
	 	//Inserção de tuplas na tabela1        
	colunas = insereValor(colunas, "atrib11", "10");
	colunas = insereValor(colunas, "atrib12", "12");
	colunas = insereValor(colunas, "atrib13", "teste13");
	colunas = insereValor(colunas, "atrib14", "1.4");
	erro = finalizaInsert("tabela1", colunas); 
	
	if(erro == ERRO_CHAVE_PRIMARIA) {
		printf("ERRO DE CHAVE PRIMARIA !!! \n");
		return erro;
	}
	if(erro == ERRO_CHAVE_ESTRANGEIRA) {
		printf("ERRO DE CHAVE ESTRANGEIRA !!! \n");
		return erro;
	}
		
	
	imprime("tabela1");

    
    return 0;
}
