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

	// printf("INSERRE CAMPOSHD %d\n",r);
	int  c = 0, it = 0; //indice da coluna com o nome do campo a ser inserido
	column *columns = NULL;
	char *nameColumns[nColumns]; //vetor com nome das colunas para controle de nulos
	/**
	 * Insere um conjunto de dados de cada vez
	 */
	// printf("numberOfColumns %d R %d\n", datas.numberOfColumns[r],r);
	while(c < datas.numberOfColumns[r])
	{
		// printf("TIPO AQUI  %c %c\n",datas.insert[r][c+1].t_char,type[it]);
		if (datas.insert[r][c+1].t_char == type[it])
		{
			printf("IMPRIME %s = %s\n",datas.insert[1][it].str,datas.insert[r][c].str );
			columns = insereValor(t,columns,datas.insert[1][it].str,datas.insert[r][c].str);
		}
		else
		{
			fprintf(stderr, "Campo com valor %s não corresponde ao tipo da coluna %s\n",datas.insert[r][c].str,datas.insert[1][c].str);
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
		printf("Insere valores nulos %d %d\n", datas.numberOfColumns[1],nColumns);
		int index = 0;

			// printf("aqui?\n");
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
				printf("VALOR do esquema %s\n",temp->nome );
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
			printf("INSERE VALORES NULOS %s %c\n",nameColumns[it],type);

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

	printf("\n-----------------------------------------\n");
	
	// columns = NULL;     
    // columns = insereValor(t,columns, "CPF", "22222");
    // columns = insereValor(t,columns, "Nome", "Carol df");
    // columns = insereValor(t,columns, "Endereco", " ");
    // columns = insereValor(t,columns, "Peso", "0.0");
    
	it = finalizaInsert(datas.insert[0][0].str, columns); 

	if (it == SUCCESS)
	{
		printf("INSERIU COM SUCESSO...\n");
	}
	else
	{
		printf("NÃO INSERIU COM SUCESSO...%d\n", it);
	}

}

table *start(char *name) 
{
	table *t = (table *)malloc(sizeof(table));

    if (t == NULL)
    {
        fprintf(stderr, "Erro na alocação de memória.\n");
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
    	fprintf(stderr, "Erro ao ler esquema da tabela %s\n",t->nome);
    }

    t->esquema = schema;

    if (t->esquema == NULL)
    {
    	fprintf(stderr, "Erro ao ler esquema da tabela %s\n",t->nome);
    }
    return t;
}


/**
 * Função para analisar a string de entrada para inserir no banco
 * Analise léxica,sintática e semântica, caso ocorra algum erro, 
 * o mesmo será lançado na tela e parado a execução;
 * segue a estrutura básica de inserção no banco em relação aos existentes
 */
void insert(char *sql)
{

	Datas datas = execute(sql);
	int rows = 0,columns = 0;

	char *file = strdup(datas.insert[rows][columns].str);
	
	strcat(file,".dat");
	// printf("Arquivo %s\n",file);

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
				fprintf(stderr, "Erro de alocação de memória.\n");
			}

			int it = 0;
			// printf("Columns %d %d %s\n",columns, datas.numberOfColumns[rows],datas.insert[0][0].str );
			
			while(it < datas.numberOfColumns[rows])
			{
				// printf("%s\n",datas.insert[rows][it].str );
				type[it] = retornaTipoDoCampo(datas.insert[rows][it].str,t);
				// printf("Tipo campo %c\n",type[it] );
				it++;
			}
			// printf("numberOfColumns CCCC %d\n", datas.numberOfColumns[rows]);

			rows++;
			
			for (it = rows; it < datas.numberOfRows; it++)
			{
				// printf(" numberOfRows RRRR %d\n",datas.numberOfRows );
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
				fprintf(stderr, "Erro de alocação de memória.\n");
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
			printf("Insere na ordem do banco\n");
		}
		
	}
	else
	{
		fprintf(stderr, "Tabela %s não existe\n",datas.insert[0][0]);
	}

	for (rows = 0; rows < datas.numberOfRows; rows++)
	{
		
		free(datas.insert[rows]);
	}


	free(datas.insert);
}