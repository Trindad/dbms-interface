#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sintatic/c.tab.c"
#include "../../buffend.h"
#include "semantic.h"


/**
 * Função para analisar a string de entrada para inserir no banco
 * Analise léxica,sintática e semântica, caso ocorra algum erro, 
 * o mesmo será lançado na tela e parado a execução;
 * segue a estrutura básica de inserção no banco em relação aos existentes
 */
void analyze(char *sql)
{
	union TOKEN **insert = execute(sql);

	char *file = strdup(insert[0][0].str);
	strcat(file,".dat");
	int exist  = existeArquivo(file);

	/**
	 * Verifica possiveis erros semanticos
	 */
	if (exist)
	{
		
	}
	else
	{
		fprintf(stderr, "Tabela %s não existe\n",insert[0][0]);
	}

}