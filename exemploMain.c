#include <stdlib.h>
#include <string.h>
#include "buffend.h"

int main(){
    int nrTabelas = 4;
    int nTabela[nrTabelas];
    table  *tab[nrTabelas]; 
    column *colunas;
    int object, schema;
    
    object      = existeArquivo("fs_object.dat");
    schema      = existeArquivo("fs_schema.dat");
    nTabela[0]  = existeArquivo("Aluno.dat");
    nTabela[1]  = existeArquivo("Inst.dat");
    nTabela[2]  = existeArquivo("Curso.dat");
    nTabela[3]  = existeArquivo("Inscri.dat");
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
            tab[2] = iniciaTabela("Curso"); 
            tab[2] = adicionaCampo(tab[2], "CodCurso"    , 'I', (sizeof(int))  ,PK, "","");
            tab[2] = adicionaCampo(tab[2], "Nome"        , 'S', 20  ,NPK, "","");
            tab[2] = adicionaCampo(tab[2], "CodInst"     , 'I', (sizeof(int))  ,FK, "Inst","CodInst");
            finalizaTabela(tab[2]);
        }
        if(!nTabela[3]){ 
            tab[3] = iniciaTabela("Inscri"); 
            tab[3] = adicionaCampo(tab[3], "CodMat"     , 'I', (sizeof(int))  ,PK, "","");
            tab[3] = adicionaCampo(tab[3], "CPF"        , 'I', (sizeof(int))  ,FK, "Aluno","CPF");
            tab[3] = adicionaCampo(tab[3], "CodInst"    , 'I', (sizeof(int))  ,FK , "Inst","CodInst");
            tab[3] = adicionaCampo(tab[3], "CodCurso"   , 'I',  (sizeof(int))  ,NPK, "","");
            finalizaTabela(tab[3]);
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
    colunas = insereValor(colunas, "Endereco", "RuaClevela");
    colunas = insereValor(colunas, "Peso", "88.9");
    finalizaInsert("Aluno", colunas); 

    colunas = NULL;     
    colunas = insereValor(colunas, "CPF", "1234567");
    colunas = insereValor(colunas, "Nome", "Natan");
    colunas = insereValor(colunas, "Endereco", "RuaDelmi");
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
    colunas = insereValor(colunas, "CodCurso", "001");
    colunas = insereValor(colunas, "Nome", "ADM");
    colunas = insereValor(colunas, "CodInst", "111");
    finalizaInsert("Curso", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "CodCurso", "002");
    colunas = insereValor(colunas, "Nome", "CC");
    colunas = insereValor(colunas, "CodInst", "111");
    finalizaInsert("Curso", colunas);
    
    
    //Inserção de tupla na tabela4
    colunas = NULL;
    colunas = insereValor(colunas, "CodMat", "1401");
    colunas = insereValor(colunas, "CPF", "123456");
    colunas = insereValor(colunas, "CodInst", "333");
    colunas = insereValor(colunas, "codCurso", "002");
    finalizaInsert("Inscri", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "CodMat", "1402");
    colunas = insereValor(colunas, "CPF", "654321");
    colunas = insereValor(colunas, "CodInst", "222");
    colunas = insereValor(colunas, "codCurso", "002");
    finalizaInsert("Inscri", colunas);
    
    colunas = NULL;
    colunas = insereValor(colunas, "CodMat", "1403");
    colunas = insereValor(colunas, "CPF", "1234567");
    colunas = insereValor(colunas, "CodInst", "111");
    colunas = insereValor(colunas, "codCurso", "001");
    finalizaInsert("Inscri", colunas);
    
    imprime("Aluno");
    imprime("Inst");
    excluirTabela("Inst");
    imprime("Inscri");
    imprime("Curso");
    excluirTabela("Inscri");
    excluirTabela("Aluno");
    
    return 0;
}
