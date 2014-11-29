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


int main(){
    int nrTabelas = 2;
    table  *tab[nrTabelas];	
    column *colunas = NULL;
    
    //aqui cria as variáveis que receberão os retornos das chamadas de funções
    int tabela1, tabela2, object, schema;
    object         = existeArquivo("fs_object.dat");
    schema         = existeArquivo("fs_schema.dat");
    tabela1        = existeArquivo("tabela1.dat");
    tabela2        = existeArquivo("tabela2.dat");
    if(!object || !schema){
    
		if(!tabela1){ 															//se ainda não existe a tabela1, a mesma é criada
				tab[0] = iniciaTabela("tabela1");  								// cria a tabela 
				tab[0] = adicionaCampo(tab[0], "atrib1" , 'I', (sizeof(int))	,PK);				//cria os atributos
				tab[0] = adicionaCampo(tab[0], "atrib2" , 'I', (sizeof(int))	,NPK);
				tab[0] = adicionaCampo(tab[0], "atrib3" , 'S', 10				,NPK);
				tab[0] = adicionaCampo(tab[0], "atrib4" , 'D', (sizeof(double))	,NPK);
				finalizaTabela(tab[0]);
		 }

		if(!tabela2){	////se ainda não existe a tabela2, a mesma é criada
			tab[1] = iniciaTabela("tabela2"); 
			tab[1] = adicionaCampo(tab[1], "atrib1" , 'I', (sizeof(int))	,FK);
			tab[1] = adicionaCampo(tab[1], "atrib22" , 'S', 20				,NPK);
			tab[1] = adicionaCampo(tab[1], "atrib23" , 'I', (sizeof(int))	,NPK);
			finalizaTabela(tab[1]);
		}
	}
        
//inserção de tupla na tabela1        
	colunas = insereValor(colunas, "atrib1", "10"	, "tabela1", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib2", "12"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib3", "teste13"	, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib4", "1.4"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela1", colunas); 
	
	colunas=NULL;
	colunas = insereValor(colunas, "atrib1", "11"	, "tabela1", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib2", "30"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib3", "teste34"	, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib4", "1.7"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela1", colunas); 
	
//inserção de tupla na tabela2	
	colunas=NULL;
	colunas = insereValor(colunas, "atrib1", "10"	, "tabela2", FK, "tabela1", "atrib1");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib22", "2.2"	, "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib23", "23"	, "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela2", colunas);
	

	imprime("tabela1");
	imprime("tabela2");
	return 0;
}



