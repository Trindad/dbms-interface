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
    nrTabelas = 4;
    table  *tab[nrTabelas];	
    
    //aqui cria as variáveis que receberão os retornos das chamadas de funções
    int tabela1, tabela2;
    object         = existeArquivo("fs_object.dat");
    schema         = existeArquivo("fs_schema.dat");
    tabela1        = existeArquivo("tabela1.dat");
    tabela2        = existeArquivo("tabela2.dat");
    
    if(!tabela1){ //se ainda não existe a tabela, a mesma é criada
            tab[0] = iniciaTabela("tabela1");   
            tab[0] = adicionaCampo(tab[0], "atrib1" , 'S', 20);
            tab[0] = adicionaCampo(tab[0], "atrib2" , 'I', (sizeof(int)));
            tab[0] = adicionaCampo(tab[0], "atrib3" , 'S', 10);
            tab[0] = adicionaCampo(tab[0], "atrib4" , 'D', (sizeof(double)));
            tab[0] = adicionaCampo(tab[0], "atrib5" , 'I', (sizeof(int)));
            tab[0] = adicionaCampo(tab[0], "atrib6" , 'S', 10);
            erro   = finalizaTabela(tab[0]);
     }

        if(!tabela2){	////se ainda não existe a tabela, a mesma é criada
            tab[1] = iniciaTabela("Tabela2"); 
            tab[1] = adicionaCampo(tab[1], "atrib1" , 'I', (sizeof(int)));
            tab[1] = adicionaCampo(tab[1], "atrib2" , 'S', 20);
            tab[1] = adicionaCampo(tab[1], "atrib3" , 'I', (sizeof(int)));
            erro   = finalizaTabela(tab[1]);
        }
    return 0;
}
