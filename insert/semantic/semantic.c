#include "../sintatic/c.tab.c"
#include "semantic.h"
int nColumns;

/**
 * Insere campos correspondente a uma tupla
 * Verica se o tipo do valor é igual
 * Se sim insere
 * Do contrário lança erro
 */
void insertFields(table *t,Datas datas,char *type , int r) {

	
	int  c = 0, it = 0; //indice da coluna com o nome do campo a ser inserido
	column *columns = NULL;
	char *nameColumns[nColumns]; //vetor com nome das colunas para controle de nulos
	/**
	 * Insere um conjunto de dados de cada vez
	 */
	
	while(c < datas.numberOfColumns[r])
	{
		if (datas.insert[r][c+1].t_char == type[it])
		{
			columns = insereValor(t,columns,datas.insert[1][it].str,datas.insert[r][c].str);
		}
		else
		{
			fprintf(stderr, "Value %s doesn't match the column %s's type.\n",datas.insert[r][c].str,datas.insert[1][c].str);
			return;
		}
		it++;
		c += 2;
	}

	/**
	 * Insere nulo nos campos que não foram passados 
	 * como parâmetro no insert.
	 */
	if (datas.numberOfColumns[1] < nColumns)
	{
		int index = 0;

		tp_table *temp = t->esquema;
		int count = 0;//adiciona 1 se já foi inserido valor
		
		while(temp != NULL && temp->nome != NULL)
		{
			for(it = 0; it < datas.numberOfColumns[1]; it++)
			{
				if (strcmp(temp->nome,datas.insert[1][it].str) == 0)
				{
					count = 1;
					break;
				}
			}
			

			if (count == 0)
			{
				nameColumns[index] = strdup(temp->nome);
				index++;
			}
			count = 0;
			temp = temp->next;
		}
		it = 0;

		while(it < index)
		{
			char type = retornaTipoDoCampo(nameColumns[it],t);

			if (type == 'S')
			{
				columns = insereValor(t,columns,nameColumns[it]," ");
			}
			else if (type == 'D')
			{
				columns = insereValor(t,columns,nameColumns[it],"0.0");
			}
			else if (type == 'I')
			{
				columns = insereValor(t,columns,nameColumns[it],"0");
			}

			free(nameColumns[it]);

			it++;
		}
	}
    
	it = finalizaInsert(datas.insert[0][0].str, columns); 

	if (it == SUCCESS)
	{
		printf("Insert successful.\n");
	}
	else
	{
		printf("Insert failed.\n");
	}

}

table *start(char *name) 
{
	table *t = (table *)malloc(sizeof(table));

    if (t == NULL)
    {
        fprintf(stderr, "Out of memory.\n");
    }

    int n = strlen(name);

    if (n > TAMANHO_NOME_TABELA)
    {
        n = TAMANHO_NOME_TABELA;
    }

    strncpy(t->nome,name,n+1); // Inicia a estrutura de tabela com o nome da tabela.
   
	struct fs_objects  object = leObjeto(t->nome);    
    
	nColumns = object.qtdCampos; //recebe o número de colunas da tabela, para saber quais campos adicionar nulo

    tp_table *schema = leSchema( object);

    if (schema == ERRO_ABRIR_ESQUEMA)
    {
    	fprintf(stderr, "Error while reading database schema %s\n",t->nome);
    }

    t->esquema = schema;

    if (t->esquema == NULL)
    {
    	fprintf(stderr, "Error while reading database schema %s\n",t->nome);
    }
    return t;
}


/**
 * Função para analisar a string de entrada para inserir no banco
 * Analise léxica,sintática e semântica, caso ocorra algum erro, 
 * o mesmo será lançado na tela e parado a execução;
 * segue a estrutura básica de inserção no banco em relação aos existentes
 */
void insert(char *sql,int current_database)
{

	Datas datas = execute(sql);
	int rows = 0,columns = 0;

	char *table_name = (char*) malloc (sizeof(char)*1000);

    if (!table_name)
    {
    	printf("Out of memory.\n");
    	exit(1);
    }

    sprintf(table_name,"%d",current_database);

    strcat(table_name,"_");
    strcat(table_name,datas.insert[rows][columns].str);

	char *file = strdup(table_name);
	
	strcat(file,".dat");

	int exist  = existeArquivo(file);

	rows++;

	/**
	 * Verifica possiveis erros semanticos
	 */
	if (exist)
	{
		table  *t = start(datas.insert[0][0].str);

		if (datas.numberOfColumns[rows] >= 1)
		{
			/**
			 * Insere na ordem do insert os campos
			 */
			char *type = (char*) malloc (sizeof(char)*datas.numberOfColumns[rows]);

			if (type == NULL)
			{
				fprintf(stderr, "Out of memory.\n");
			}

			int it = 0;
			
			while(it < datas.numberOfColumns[rows])
			{
				type[it] = retornaTipoDoCampo(datas.insert[rows][it].str,t);
				
				it++;
			}

			rows++;
			
			for (it = rows; it < datas.numberOfRows; it++)
			{
				insertFields(t,datas,type,it);
			}
			free(type);
		}
		else
		{
			tp_table *e = t->esquema;
			int it = 0;

			//insere campos na ordem do banco
			while(e != NULL)
			{
				datas.insert[rows][it].str = strdup(e->nome);
				e = e->next;
				it++;
			}

			datas.numberOfColumns[rows] = it;

			char *type = (char*) malloc (sizeof(char)*datas.numberOfColumns[rows]);

			if (type == NULL)
			{
				fprintf(stderr, "Out of memory.\n");
			}

			it = 0;

			while(it < datas.numberOfColumns[rows])
			{
				type[it] = retornaTipoDoCampo(datas.insert[rows][it].str,t);
				
				it++;
			}

			rows++;

			for (it = rows; it < datas.numberOfRows; it++)
			{
				insertFields(t,datas,type,it);
			}
			free(type);
		}
		
	}
	else
	{
		fprintf(stderr, "Table %s doesn't exist.\n",datas.insert[0][0]);
	}

	for (rows = 0; rows < datas.numberOfRows; rows++)
	{
		
		free(datas.insert[rows]);
	}

	free(file);
	free(table_name);
	free(datas.insert);
}