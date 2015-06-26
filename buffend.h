#ifndef BUFFEND_H
#define BUFFEND_H

#include "table.h"
#include "tuple.h"


/************************************************************************************************
 ************************************************************************************************/

tp_buffer * initbuffer();
/*
	Esta função tem por objetivo criar e inicializar uma estrutura do tipo tp_buffer
	que será usada para carregar tuplas na memória

*/

int printbufferpoll(tp_buffer *buffpoll, tp_table *s,struct fs_objects objeto, int num_page);
/*
    Esta função imprime todos os dados carregados numa determinada página do buffer
    *buffer - Estrutura para armazenar tuplas na memória
    *s - Estrutura que armazena esquema da tabela para ler os dados do buffer
    *objeto - Estrutura que armazena dados sobre a tabela que está no buffer
    *num_page - Número da página a ser impressa
*/

/************************************************************************************************
 ************************************************************************************************/

int colocaTuplaBuffer(tp_buffer *buffer, int from, tp_table *campos, struct fs_objects objeto);
/*
    Esta função insere uma tupla em uma página do buffer em que haja espaço suficiente. 
    Retorna ERRO_BUFFER_CHEIO caso não haja espeço para a tupla

    *buffer - Estrutura para armazenar tuplas na meméria
    *from   - Número da tupla a ser posta no buffer. Este número é relativo a ordem de inserção da 
              tupla na tabela em disco.
    *campos - Estrutura que armazena esquema da tabela para ler os dados do buffer
    *objeto - Estrutura que armazena dados sobre a tabela que está no buffer
*/

column *getPage(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page);

/*
    Esta função recupera uma página do buffer e retorna a mesma em uma estrutura do tipo column
    A estrutura column possui informações de como manipular os dados
    *buffer - Estrutura para armazenar tuplas na meméria
    *campos - Estrutura que armazena esquema da tabela para ler os dados do buffer
    *objeto - Estrutura que armazena dados sobre a tabela que está no buffer
    *page - Número da página a ser recuperada (0 a PAGES)
*/

/************************************************************************************************
 ************************************************************************************************/

column *excluirTuplaBuffer(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, int nTupla);
/*
    Esta função uma determinada tupla do buffer e retorna a mesma em uma estrutura do tipo column; 
    A estrutura column possui informações de como manipular os dados
    *buffer - Estrutura para armazenar tuplas na meméria
    *campos - Estrutura que armazena esquema da tabela para ler os dados do buffer
    *objeto - Estrutura que armazena dados sobre a tabela que está no buffer
    *page   - Número da página a ser recuperada uma tupla (0 a PAGES)
    *nTupla - Número da tupla a ser excluida, este número é relativo a página do buffer e não a 
              todos os registros carregados
*/

#endif