/*#include <stdlib.h>
#include <string.h>
#include "buffend.h"

int main(){
    int nrTabelas = 3;
    int nTabela[nrTabelas];
    table  *tab[nrTabelas]; 
    column *colunas;
    int object, schema,erro;
    
    object      = existeArquivo("fs_object.dat");
    schema      = existeArquivo("fs_schema.dat");
    nTabela[0]  = existeArquivo("tabela1.dat");
    nTabela[1]  = existeArquivo("tabela2.dat");
    nTabela[2]  = existeArquivo("tabela3.dat");
    nTabela[3]  = existeArquivo("tabela4.dat");
     if(!object || !schema){
    
        if(!nTabela[0]){                                                            //Se ainda não existe a tabela1, a mesma é criada
            tab[0] = iniciaTabela("tabela1");                                               //Cria a tabela 
            tab[0] = adicionaCampo(tab[0], "atrib11" , 'I', (sizeof(int))   ,PK,"","");     //Cria os atributos
            tab[0] = adicionaCampo(tab[0], "atrib12" , 'I', (sizeof(int))   ,NPK,"","");
            tab[0] = adicionaCampo(tab[0], "atrib13" , 'S', 10              ,NPK,"","");
            tab[0] = adicionaCampo(tab[0], "atrib14" , 'D', (sizeof(double)),NPK,"","");
            finalizaTabela(tab[0]);
         }
         if(!nTabela[1]){   //Se ainda não existe a tabela2, a mesma é criada
            tab[1] = iniciaTabela("tabela2"); 
            tab[1] = adicionaCampo(tab[1], "atrib21" , 'I', (sizeof(int))   ,PK, "","");
            tab[1] = adicionaCampo(tab[1], "atrib22" , 'S', 20              ,NPK, "","");
            tab[1] = adicionaCampo(tab[1], "atrib23" , 'I', (sizeof(int))   ,NPK, "","");
            tab[1] = adicionaCampo(tab[1], "atrib24" , 'S', 10              ,NPK, "","");
            finalizaTabela(tab[1]);
        }
        if(!nTabela[2]){    //Se ainda não existe a tabela3, a mesma é criada
            tab[2] = iniciaTabela("tabela3"); 
            tab[2] = adicionaCampo(tab[2], "atrib31" , 'I', (sizeof(int))   ,PK, "","");
            tab[2] = adicionaCampo(tab[2], "atrib21" , 'I', (sizeof(int))   ,FK, "tabela2","atrib21");
            tab[2] = adicionaCampo(tab[2], "atrib11" , 'I', (sizeof(int))   ,FK, "tabela1","atrib11");
            finalizaTabela(tab[2]);
        }
        if(!nTabela[3]){    //Se ainda não existe a tabela3, a mesma é criada
            tab[3] = iniciaTabela("tabela4"); 
            tab[3] = adicionaCampo(tab[3], "atrib41" , 'I', (sizeof(int))   ,PK, "","");
            tab[3] = adicionaCampo(tab[3], "atrib42" , 'D', (sizeof(double)),NPK, " "," ");
            finalizaTabela(tab[3]);
        }
    }
     
     //Inserção de tuplas na tabela1   
    colunas = NULL;     
    colunas = insereValor(colunas, "atrib11", "10");
    colunas = insereValor(colunas, "atrib12", "35");
    colunas = insereValor(colunas, "atrib13", "teste13");
    colunas = insereValor(colunas, "atrib14", "1.4");
     finalizaInsert("tabela1", colunas); 
    
    colunas = NULL;
    colunas = insereValor(colunas, "atrib11", "11");
    colunas = insereValor(colunas, "atrib12", "30");
    colunas = insereValor(colunas, "atrib13", "teste34" );
    colunas = insereValor(colunas, "atrib14", "1.7");
     finalizaInsert("tabela1", colunas); 

    colunas = NULL;
    colunas = insereValor(colunas, "atrib11", "15");
    colunas = insereValor(colunas, "atrib12", "33");
    colunas = insereValor(colunas, "atrib13", "teste340");
    colunas = insereValor(colunas, "atrib14", "1.9");
     finalizaInsert("tabela1", colunas); 
    
    //Inserção de tuplas na tabela2 
    colunas = NULL;
    colunas = insereValor(colunas, "atrib21", "10");
    colunas = insereValor(colunas, "atrib22", "dois");
    colunas = insereValor(colunas, "atrib23", "23");
    colunas = insereValor(colunas, "atrib24", "tres");
     finalizaInsert("tabela2", colunas);
    
    
    colunas = NULL;
    colunas = insereValor(colunas, "atrib21", "11");
    colunas = insereValor(colunas, "atrib22", "dois");
    colunas = insereValor(colunas, "atrib23", "23");
    colunas = insereValor(colunas, "atrib24", "tres");
     finalizaInsert("tabela2", colunas);
    
    
    //Inserção de tupla na tabela3
    colunas = NULL;
    colunas = insereValor(colunas, "atrib31", "10");
    colunas = insereValor(colunas, "atrib21", "11");
    colunas = insereValor(colunas, "atrib11", "10");
     finalizaInsert("tabela3", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "atrib31", "11");
    colunas = insereValor(colunas, "atrib21", "10");
    colunas = insereValor(colunas, "atrib11", "15");
     finalizaInsert("tabela3", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "atrib31", "15");
    colunas = insereValor(colunas, "atrib21", "10");
    colunas = insereValor(colunas, "atrib11", "10");
     finalizaInsert("tabela3", colunas);

    
    //Inserção de tupla na tabela4
    colunas = NULL;
    colunas = insereValor(colunas, "atrib41", "100");
    colunas = insereValor(colunas, "atrib42", "1.1");
     finalizaInsert("tabela4", colunas);
    
    
    if(= ERRO_CHAVE_PRIMARIA) {
        printf("ERRO DE CHAVE PRIMARIA !!! \n");
        return erro;
    }
    if(= ERRO_CHAVE_ESTRANGEIRA) {
        printf("ERRO DE CHAVE ESTRANGEIRA !!! \n");
        return erro;
    }
        
    imprime("tabela1");    
    //excluirArquivo("tabela3");
    imprime("tabela2");
    imprime("tabela3");
    imprime("tabela4");
    
    return 0;
}*/

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
//    nTabela[3]  = existeArquivo("tabela4.dat");
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
    /*  if(!nTabela[3]){    //Se ainda não existe a tabela3, a mesma é criada
            tab[3] = iniciaTabela("tabela4"); 
            tab[3] = adicionaCampo(tab[3], "atrib41" , 'I', (sizeof(int))   ,PK, "","");
            tab[3] = adicionaCampo(tab[3], "atrib42" , 'D', (sizeof(double)),NPK, " "," ");
            finalizaTabela(tab[3]);
        }
    */}
    
     
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
//  imprime("tabela4");

    
    return 0;
}
