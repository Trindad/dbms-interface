#include <stdlib.h>
#include <string.h>
#include "buffend.h"
#include "funcoesNRR.h"

int main(){
    int nrTabelas = 3;
    int nTabela[nrTabelas];
    table  *tab[nrTabelas];	
    column *colunas = NULL;
    int object, schema;
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
//	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib12", "12");
//	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib13", "teste13");
//	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib14", "1.4");
//	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela1", colunas); 
	imprime("tabela1");

    
    return 0;
}
