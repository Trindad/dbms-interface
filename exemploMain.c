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
			tab[0] = iniciaTabela("tabela1");  										//Cria a tabela 
			tab[0] = adicionaCampo(tab[0], "atrib11" , 'I', (sizeof(int))	,PK);	//Cria os atributos
			tab[0] = adicionaCampo(tab[0], "atrib12" , 'I', (sizeof(int))	,NPK);
			tab[0] = adicionaCampo(tab[0], "atrib13" , 'S', 10				,NPK);
			tab[0] = adicionaCampo(tab[0], "atrib14" , 'D', (sizeof(double)),NPK);
			finalizaTabela(tab[0]);
		 }

		if(!nTabela[1]){	//Se ainda não existe a tabela2, a mesma é criada
			tab[1] = iniciaTabela("tabela2"); 
			tab[1] = adicionaCampo(tab[1], "atrib21" , 'I', (sizeof(int))	,PK);
			tab[1] = adicionaCampo(tab[1], "atrib22" , 'S', 20				,NPK);
			tab[1] = adicionaCampo(tab[1], "atrib23" , 'I', (sizeof(int))	,NPK);
			tab[1] = adicionaCampo(tab[1], "atrib24" , 'S', 10				,NPK);
			finalizaTabela(tab[1]);
		}
		if(!nTabela[2]){	//Se ainda não existe a tabela3, a mesma é criada
			tab[2] = iniciaTabela("tabela3"); 
			tab[2] = adicionaCampo(tab[2], "atrib11" , 'I', (sizeof(int))	,FK);
			tab[2] = adicionaCampo(tab[2], "atrib32" , 'C', 20				,NPK);
			finalizaTabela(tab[2]);
		}
	}
        
	//Inserção de tuplas na tabela1        
	colunas = insereValor(colunas, "atrib11", "10"	, "tabela1", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib12", "12"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib13", "teste13"	, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib14", "1.4"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela1", colunas); 
	
	colunas=NULL;
	colunas = insereValor(colunas, "atrib11", "11"	, "tabela1", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib12", "30"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib13", "teste34"	, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib14", "1.7"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela1", colunas); 

	colunas=NULL;
	colunas = insereValor(colunas, "atrib11", "12"		, "tabela1", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib12", "33"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib13", "teste340", "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib14", "1.9"		, "tabela1", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela1", colunas); 
	
	//Inserção de tuplas na tabela2	
	colunas=NULL;
	colunas = insereValor(colunas, "atrib21", "10"	, "tabela2", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib22", "dois", "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib23", "23"	, "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib24", "tres", "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela2", colunas);
	
	colunas=NULL;
	colunas = insereValor(colunas, "atrib21", "11"	, "tabela2", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib22", "dois", "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib23", "23"	, "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib24", "tres", "tabela2", NPK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela2", colunas);
	
	
	//Inserção de tupla na tabela3
	colunas=NULL;
	colunas = insereValor(colunas, "atrib11", "10"	, "tabela3", FK, "tabela1", "atrib11");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	colunas = insereValor(colunas, "atrib32", "M", "tabela3", PK, "", "");
	if(colunas->retorno<0){ printf("%d\n", colunas->retorno); return  colunas->retorno;}
	finalizaInsert("tabela3", colunas);
		
	
	//Chama função para imprimir as tabelas
	imprime("tabela1");
	imprime("tabela2");
	imprime("tabela3");
	return 0;
}


