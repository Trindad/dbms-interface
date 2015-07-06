#include "../sintatic/c.tab.c"
#include "semantic.h"
int nColumns;
int database;

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
			// printf("%s\n",datas.insert[r][c].str );
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
				columns = insereValor(t,columns, nameColumns[it]," ");
			}
			else if (type == 'D')
			{
				columns = insereValor(t,columns, nameColumns[it],"0.0");
			}
			else if (type == 'I')
			{
				columns = insereValor(t,columns, nameColumns[it],"0");
			}

			free(nameColumns[it]);

			it++;
		}
	}
    
	it = finalizaInsert( table_name_cat(datas.insert[0][0].str,database), columns); 

	if (it == SUCCESS)
	{
		printf("Insert successful.\n");
	}
	else
	{
		printf("Insert failed.\n");
	}

}

/**
 * Inicializa esquema 
 */
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
void insert(char *sql,int index_database)
{
	database = index_database;

	Datas datas = execute_insert(sql);

	int rows = 0,columns = 0;

    char *file = table_name_cat(datas.insert[rows][columns].str,database);

    strcat(file,".dat");
	int exist  = existeArquivo(file);

	if (table_exist(datas.insert[0][0].str,database) == 0)
	{
		fprintf(stderr, "Table %s doesn't exist.\n",datas.insert[0][0].str);
		return;
	}
	rows++;
	/**
	 * Verifica possiveis erros semanticos
	 */

	table  *t = start(table_name_cat(datas.insert[0][0].str,database) );

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

	for (rows = 0; rows < datas.numberOfRows; rows++)
	{
		
		free(datas.insert[rows]);
	}
	
	free(datas.insert);

	free(file);
	datas.numberOfRows = 0;
	free(datas.numberOfColumns);

}

/**
 * Concatena nome da tabela com id do banco
 */
char *table_name_cat(char *name,int database)
{
	char *table_name = (char*) malloc (sizeof(char)*1000);

	if (table_name == NULL)
	{
		printf("Out of memory.\nAborting...\n");
		abort();
	}

	sprintf(table_name,"%d",database);
    strcat(table_name,"_");
    strcat(table_name,name);

    return table_name;
}

void selectTable(char *sql, int index_database)
{
	char *table_name = execute_select(sql);

	if (table_name == NULL)
	{
		printf("Invalid number of arguments. Type help to show de interface usage.\n");
		return;
	}

	if ( existeArquivo(table_name_cat(table_name,index_database)) == 1)
	{
		printf("Table %s already exists.\n",table_name);
		return;
	}

	imprime(table_name_cat(table_name,index_database));
}

void createTable(char *sql, int index_database)
{
	// printf("%s\n",sql);
	database = index_database;

	Datas datas = execute_create_table(sql);

	if (datas.name_table_create == NULL)
	{
		printf("Out of memory.\nAborting...\n");
		abort();
	}

	// for (i = 0; i < datas.number_columns; i++)
	// {
	// 	printf("\n------------------------------------------------------\n");
	// 	printf("nome %s\n",datas.create_new_table[i].name_column_table );
	// 	printf("tipo %c\n",datas.create_new_table[i].type_column );
	// 	printf("tamanho %d\n",datas.create_new_table[i].size );
	// 	printf("constraint %d\n",datas.create_new_table[i].constraint );
	// }
	// printf("\n------------------------------------------------------\n");

	//verifica se a tabela já não existe
	if ( existeArquivo(table_name_cat(datas.name_table_create,database)) == 1)
	{
		printf("Table %s already exists.\n",datas.name_table_create);
		return;
	}
	int object      = existeArquivo("fs_object.dat");
	int schema      = existeArquivo("fs_schema.dat");

	table  *tab[1];
	tab[0] = iniciaTabela(table_name_cat(datas.name_table_create,database));

	if (tab[0] == NULL)
	{
		for (i = 0; i < datas.number_columns; i++)
	    {
	    	free(datas.create_new_table[i].name_column_table);
	    }

	    free(datas.create_new_table);
	    
	    i = 0;

		printf("Out of memory.\nAborting...\n");
		abort();
	}

	for (i = 0; i < datas.number_columns; i++)
	{
		//chave estrangeira = 2
		if (datas.create_new_table[i].constraint == FK)
		{
			tab[0] = adicionaCampo(tab[0],datas.create_new_table[i].name_column_table,datas.create_new_table[i].type_column,datas.create_new_table[i].size,datas.create_new_table[i].constraint,table_name_cat(datas.create_new_table[i].table_foreign,database),datas.create_new_table[i].name_column_foreign);
		}
		else
		{
        	tab[0] = adicionaCampo(tab[0],datas.create_new_table[i].name_column_table,datas.create_new_table[i].type_column,datas.create_new_table[i].size,datas.create_new_table[i].constraint,"","");
		}
	}

	//limpa memória utilizada por datas
    for (i = 0; i < datas.number_columns; i++)
    {
    	free(datas.create_new_table[i].name_column_table);
    }

    free(datas.create_new_table);
    
    i  = finalizaTabela(tab[0],database);

    if (i == SUCCESS)
    {
    	printf("Table created successfully\n");
    }
}

/**
 * Verifica se a tabela existe 
 * Retorna 1 se sim e 0 do constrário
 */
int table_exist(char *tablename,int database)
{
    FILE *dictionary;
    i = 0;
    char *tb_name = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);

    if (tb_name == NULL)
    {
    	printf("Out of memory.\nAborting...\n");
    	abort();
    }

    if((dictionary = fopen("fs_object.dat","a+b")) == NULL){
        free(tb_name);
        printf("No table created.\n");
        return 0;
    }

    while(fgetc (dictionary) != EOF)
    {
        fseek(dictionary, -1, 1);

        fread(tb_name, sizeof(char), TAMANHO_NOME_TABELA, dictionary); //Lê somente o nome da tabela
        
        int n = 0;
        char **str = tokenize(tb_name,'_',&n);

        if (n >= 2) {
            n = atoi(str[0]);

            if(n == database){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
                char *temp = tb_name;

                for (i = 0; i < strlen(str[0]); i++)
                {
                    temp++;
                }

                temp++;
                if (strcmp(tablename,temp) == 0)
                {
                	return 1;
                }
            }
        }
        
        fseek(dictionary, 28, 1);
    }

    fclose(dictionary);

    return 0;
}

