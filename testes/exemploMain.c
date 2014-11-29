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
|* EXEMPLO DE MAIN QUE CHAMARÁ AS FUNÇÕEs:
criaTabela(char nomeTabela);
* 

      */   

int main(){
    int nrTabelas = 2;
    table  *tab[nrTabelas];	
    column *colunas = NULL;
    
    //aqui cria as variáveis que receberão os retornos das chamadas de funções
    int tabela1, tabela2, erro, object, schema;
    object         = existeArquivo("fs_object.dat");
    schema         = existeArquivo("fs_schema.dat");
    tabela1        = existeArquivo("tabela1.dat");
    tabela2        = existeArquivo("tabela2.dat");
    
    if(!tabela1){ 															//se ainda não existe a tabela1, a mesma é criada
            tab[0] = iniciaTabela("tabela1");  								// cria a tabela 
            tab[0] = adicionaCampo(tab[0], "atrib1" , 'S', 20);				//cria os atributos
            tab[0] = adicionaCampo(tab[0], "atrib2" , 'I', (sizeof(int)));
            tab[0] = adicionaCampo(tab[0], "atrib3" , 'S', 10);
            tab[0] = adicionaCampo(tab[0], "atrib4" , 'D', (sizeof(double)));
            erro   = finalizaTabela(tab[0]);
     }

	if(!tabela2){	////se ainda não existe a tabela2, a mesma é criada
		tab[1] = iniciaTabela("tabela2"); 
		tab[1] = adicionaCampo(tab[1], "atrib21" , 'I', (sizeof(int)));
		tab[1] = adicionaCampo(tab[1], "atrib22" , 'S', 20);
		tab[1] = adicionaCampo(tab[1], "atrib23" , 'I', (sizeof(int)));
		erro   = finalizaTabela(tab[1]);
	}
        
//inserção de valores na tabela1        
	colunas = insereValor(colunas, "atrib1", "teste11"	, "tabela1", 1, "", "");
	if (!colunas) return ERRO_CHAVE_PRIMARIA;   
	colunas = insereValor(colunas, "atrib2", "12"		, "tabela1", 0, "", "");
	if (!colunas) return ERRO_CHAVE_PRIMARIA;   
	colunas = insereValor(colunas, "atrib2", "teste13"	, "tabela1", 0, "", "");
	if (!colunas) return ERRO_CHAVE_PRIMARIA;   
	colunas = insereValor(colunas, "atrib2", "1.4"		, "tabela1", 0, "", "");
	if (!colunas) return ERRO_CHAVE_PRIMARIA;            
	erro = finalizaInsert("tabela1", colunas);  
	
//inserção de valores na tabela2	
	colunas=NULL;
	colunas = insereValor(colunas, "atrib21", "10"	, "tabela2", 1, "", "");
	if (!colunas) return ERRO_CHAVE_PRIMARIA;   
	colunas = insereValor(colunas, "atrib22", "2.2"	, "tabela2", 0, "", "");
	if (!colunas) return ERRO_CHAVE_PRIMARIA;   
	colunas = insereValor(colunas, "atrib23", "23"	, "tabela2", 0, "", "");
	if (!colunas) return ERRO_CHAVE_PRIMARIA;           
	erro = finalizaInsert("tabela2", colunas);
	imprime("tabela1");
	imprime("tabela2");
	return 0;
}
