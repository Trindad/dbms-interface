#include <stdlib.h>
#include <string.h>
#include "buffend.h"

int main(){
    int nrTabelas = 3;
    int nTabela[nrTabelas];
    table  *tab[nrTabelas]; 
    column *colunas;
    int object, schema;
    
    object      = existeArquivo("fs_object.dat");
    schema      = existeArquivo("fs_schema.dat");
    nTabela[0]  = existeArquivo("Aluno.dat");
    nTabela[1]  = existeArquivo("Inst.dat");
    nTabela[2]  = existeArquivo("Insc.dat");
     if(!object || !schema){
    
        if(!nTabela[0]){                                                            //Se ainda não existe a tabela1, a mesma é criada
            tab[0] = iniciaTabela("Aluno");                                         //Cria a tabela 
            tab[0] = adicionaCampo(tab[0], "CPF"     , 'I', (sizeof(int))   ,PK,"","");
            tab[0] = adicionaCampo(tab[0], "Nome"    , 'S', 20              ,NPK,"","");        //Cria os atributos
            tab[0] = adicionaCampo(tab[0], "Endereco", 'S', 20              ,NPK,"","");
            tab[0] = adicionaCampo(tab[0], "Peso"    , 'D', (sizeof(double)),NPK,"","");
            finalizaTabela(tab[0]);
         }
         if(!nTabela[1]){   //Se ainda não existe a tabela2, a mesma é criada
            tab[1] = iniciaTabela("Inst"); 
            tab[1] = adicionaCampo(tab[1], "CodInst" , 'I', (sizeof(int))   ,PK , "","");
            tab[1] = adicionaCampo(tab[1], "Nome"    , 'S', 20              ,NPK, "","");
            tab[1] = adicionaCampo(tab[1], "Endereco", 'S', 20              ,NPK, "","");
            tab[1] = adicionaCampo(tab[1], "Reitor"  , 'S', 10              ,NPK, "","");
            finalizaTabela(tab[1]);
        }
        if(!nTabela[2]){    //Se ainda não existe a tabela3, a mesma é criada
            tab[2] = iniciaTabela("Insc"); 
            tab[2] = adicionaCampo(tab[2], "ID"      , 'I', (sizeof(int))   ,PK, "","");
            tab[2] = adicionaCampo(tab[2], "CPF"      , 'I', (sizeof(int))  ,FK, "Aluno","CPF");
            tab[2] = adicionaCampo(tab[2], "Matricula", 'I', (sizeof(int))  ,FK , "Inst","CodInst");
            tab[2] = adicionaCampo(tab[2], "CodInst"  , 'I', (sizeof(int))  ,NPK, " ","");
            finalizaTabela(tab[2]);
        }
    }
    
     
     //Inserção de tuplas na tabela1   
    colunas = NULL;     
    colunas = insereValor(colunas, "CPF", "123456");
    colunas = insereValor(colunas, "Nome", "Rogerio");
    colunas = insereValor(colunas, "Endereco", "Rua Marechal");
    colunas = insereValor(colunas, "Peso", "81.4");
    finalizaInsert("Aluno", colunas); 
    
    colunas = NULL;     
    colunas = insereValor(colunas, "CPF", "654321");
    colunas = insereValor(colunas, "Nome", "Ricardo");
    colunas = insereValor(colunas, "Endereco", "Rua Clevela");
    colunas = insereValor(colunas, "Peso", "88.9");
    finalizaInsert("Aluno", colunas); 

    colunas = NULL;     
    colunas = insereValor(colunas, "CPF", "1234567");
    colunas = insereValor(colunas, "Nome", "Natan");
    colunas = insereValor(colunas, "Endereco", "Rua De lmi");
    colunas = insereValor(colunas, "Peso", "58.9");
    finalizaInsert("Aluno", colunas); 
        
    
    //Inserção de tuplas na tabela2 
    colunas = NULL;
    colunas = insereValor(colunas, "CodInst", "111");
    colunas = insereValor(colunas, "Nome", "UFFS");
    colunas = insereValor(colunas, "Endereco", "RuadeTerra");
    colunas = insereValor(colunas, "Reitor", "MandaChuva");
    finalizaInsert("Inst", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "CodInst", "222");
    colunas = insereValor(colunas, "Nome", "CEFET");
    colunas = insereValor(colunas, "Endereco", "RuadePedra");
    colunas = insereValor(colunas, "Reitor", "MandaVento");
    finalizaInsert("Inst", colunas);

    colunas = NULL;
    colunas = insereValor(colunas, "CodInst", "333");
    colunas = insereValor(colunas, "Nome", "UNOESC");
    colunas = insereValor(colunas, "Endereco", "RuadeAsfal");
    colunas = insereValor(colunas, "Reitor", "MandaAgua");
    finalizaInsert("Inst", colunas);
    
    //Inserção de tupla na tabela3
    colunas = NULL;
    colunas = insereValor(colunas, "ID", "1");
    colunas = insereValor(colunas, "CPF", "123456");
    colunas = insereValor(colunas, "Matricula", "1221");
    colunas = insereValor(colunas, "CodInst", "111");
    finalizaInsert("Insc", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "ID", "2");
    colunas = insereValor(colunas, "CPF", "654321");
    colunas = insereValor(colunas, "Matricula", "1222");
    colunas = insereValor(colunas, "CodInst", "222");
    finalizaInsert("Insc", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "ID", "3");
    colunas = insereValor(colunas, "CPF", "1234567");
    colunas = insereValor(colunas, "Matricula", "1223");
    colunas = insereValor(colunas, "CodInst", "111");
    finalizaInsert("Insc", colunas);
 
    
    imprime("Aluno");
    imprime("Inst");
    excluirArquivo("Inst");
    imprime("Insc");
    excluirArquivo("Insc");

    
    return 0;
}
