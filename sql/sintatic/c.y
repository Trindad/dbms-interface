%union {
	char *str;
	int  num;
	char t_char;
	double t_double;
};

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *fp;
extern char *yytext;

union TOKEN{
	char *str;
	int  num;
	char t_char;
	double t_double;
};

typedef struct _datas_create
{
	char type_column;//tipo da coluna
	char *name_column_table;//nome da coluna
	char *table_foreign;//armazena a tabela da chave estrangeira
	char *name_column_foreign;//armazena nome da chave estrangeira
	int size;//tamanho limite do valor
	int constraint;
	
}CreateTB;

typedef struct _datas 
{
	union TOKEN **insert; //dados para execução do insert
	CreateTB *create_new_table;//dados para a criação da nova tabela
	char *name_table_create;
	int number_columns;	//número de colunas da tabela para o crate table
	int *numberOfColumns; //número de colunas de cada  linha
	int numberOfRows; //número de linhas da matriz insert
}Datas;

Datas datas;
char *table_select;

int i = 0;
int j = 0;
int countColumn = 0;//controle para o número de campos inseridos
int countField = 0;
int count = 0;
char *str_replace(char *search , char *replace , char *subject);
char *remove_quotes(char *str);
char *remove_single_quotes(char *str);
%}


/**
 * DEFINIÇÕES
 */
%token <str> INT 
%token INSERT
%token INTO
%token <str>NUM
%token <str> PONTOFLUTUANTE
%token VALUES
%token <str>SELECT
%token <str>FROM
%token <str> BOOL
%token WHERE
%token <str> ID
%token <str> CHAR_
%token DATE
%token <str> STRING_LITERAL
%token <str>CREATE 
%token <str>TABLE 
%token <str>INTEGER 
%token <str>CHAR 
%token <str>DOUBLE 
%token <str>STRING
%token <str>PRIMARY
%token <str>KEY
%token <str>FOREIGN
%token <str>CONSTRAINT 
%token <str>REFERENCES 

%token-table
%nonassoc LOWERTHANCOMMA
%nonassoc COMMA

%nonassoc LOWERPARENTHESIS
%nonassoc PARENTHESIS

%glr-parser
%start	input 

%%
/**
 * REGRAS
 */
input:	exp 
	;
exp: INSERT INTO insert_in 
	|CREATE TABLE create_next_name
	|SELECT next_select
	;

next_select: '*' FROM table_name_select
	;
table_name_select: ID{table_select = strdup($1);} ';'
	;
create_next_name: ID{
		datas.name_table_create = strdup($1);

		if (datas.name_table_create == NULL)
		{
			printf("Out of memory.\nAborting...\n");
			exit(1);
		}
	}create_columns
	;
insert_in : table_name 
	;

ident : ID {
	datas.insert[i][j].str = strdup($1);
	//printf("nome tabela[%d][%d]: %s\n", i,j,datas.insert[i][j].str);
	i++;
	j = 0;
	datas.numberOfColumns[i]++;
	datas.numberOfRows++;
	}
	;
column: ID {
	datas.insert[i][j].str = strdup($1);
	//printf("nome coluna[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	datas.numberOfColumns[i]++;
	countColumn++;
	j++;
	}
	;
table_name : ident columns
	;
columns: {
	count = 1;
	int *temp = realloc(datas.numberOfColumns, (i+2)*sizeof(int));
	if (temp != NULL)
		datas.numberOfColumns = temp;
	else
		fprintf(stderr, "Out of memory.\n");
	datas.numberOfColumns[i] = countColumn = 0;
	datas.numberOfRows++;
	}insertOrder 
	| {
	int *temp = realloc(datas.numberOfColumns, (i+2)*sizeof(int));
	if (temp != NULL)
		datas.numberOfColumns = temp;
	else {
		fprintf(stderr, "Out of memory.\nAborting...\n");exit(1);
	}
	datas.numberOfColumns[i] = countColumn = 0;
	datas.numberOfRows++;
	}'('column_names')' insertOrder 
	;
column_names: column %prec LOWERTHANCOMMA  
	| column column_plus %prec COMMA 
	;
column_plus: ','column_names 
	;

insertOrder: VALUES {i++;j = countField = 0; datas.numberOfColumns[i] = 0;} insertStart
	;

insertStart: '(' insertValues ')' {i++; j =0;} continue 

insertValues: type %prec LOWERTHANCOMMA {
	if( (countField > countColumn || countField < countColumn) && count == 0)
		fprintf(stderr, "Number of fields %d specified doesn't match number of columns %d.\n",countField,countColumn);
	else countField = 0;
	}
	|type values_plus %prec COMMA 
	;
values_plus: ',' insertValues
	; 

continue:  ',' {
	int *temp = (int*)realloc(datas.numberOfColumns, (i+2)*sizeof(int));

	if (temp != NULL)
		datas.numberOfColumns = temp;
	else {
		fprintf(stderr, "Out of memory.\nAborting...\n");exit(1);
	}
	datas.numberOfRows++;
	datas.numberOfColumns[i] = 0;
	} insertStart {
	// i++;
	j = 0;
	}
	|';' { datas.numberOfRows++; }
	;
/**
 * Tipos de retorno
 * Inteiro
 * string
 * nome da tabela
 * ponto flutuante
 * booleano
 * data
 */
type: NUM {
	datas.insert[i][j].str = strdup($1);
	//printf("INT[%d][%d]: %s\n", i,j,datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'I';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	} 
	| STRING_LITERAL {

	char *c = remove_quotes($1);
	datas.insert[i][j].str = c;
	//printf("STRING[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'S';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	} 
	| ID'.'ID {
	char *c = remove_quotes($1);
	datas.insert[i][j].str = c;
	//printf("FIELD[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'S';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	} 
	| PONTOFLUTUANTE {
	datas.insert[i][j].str = strdup($1);
	//printf("DOUBLE[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'D';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	} 
	| BOOL {
	datas.insert[i][j].str = strdup($1);
	//printf("BOOL[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'I';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	}
	| CHAR_ {
	char *c = remove_single_quotes($1);
	datas.insert[i][j].str = c;
	//printf("BOOL[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'C';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	}
	;

/**
 * Expressões que fazem tratamento para a criação de tabelas
 */
create_columns: '(' values_table 

values_table: ID{datas.create_new_table[datas.number_columns].name_column_table = strdup($1);} type_value plus_columns
	;

plus_columns: ',' {
	
	CreateTB *temp = (CreateTB*) realloc (datas.create_new_table, (datas.number_columns+2)*sizeof(CreateTB));

	if (temp == NULL)
	{
		printf("Out of memory.\nAborting...\n");
		exit(1);
	}

	datas.create_new_table = temp;

} values_table %prec COMMA
	|init_constraints %prec LOWERTHANCOMMA
	;

type_value: INTEGER parenthesis {
	datas.create_new_table[datas.number_columns].type_column = 'I';
	if (datas.create_new_table[datas.number_columns].size == -1)
	{
		datas.create_new_table[datas.number_columns].size = sizeof(int);
	}
	datas.number_columns = datas.number_columns + 1;

}
	|CHAR parenthesis{
	datas.create_new_table[datas.number_columns].type_column = 'C';
	if (datas.create_new_table[datas.number_columns].size == -1)
	{
		datas.create_new_table[datas.number_columns].size = sizeof(char);
	}
	datas.number_columns = datas.number_columns + 1;

}
	|STRING parenthesis {
	datas.create_new_table[datas.number_columns].type_column = 'S';
	if (datas.create_new_table[datas.number_columns].size == -1)
	{
		datas.create_new_table[datas.number_columns].size = 255;
	}
	datas.number_columns = datas.number_columns + 1;

}
	|DOUBLE parenthesis{
	datas.create_new_table[datas.number_columns].type_column = 'D';

	if (datas.create_new_table[datas.number_columns].size == -1)
	{
		datas.create_new_table[datas.number_columns].size = sizeof(double);
	}
	datas.number_columns = datas.number_columns + 1;
}
	;

parenthesis: '(' NUM ')' isprimarykey %prec PARENTHESIS {datas.create_new_table[datas.number_columns].size = atoi($2); }
	| isprimarykey %prec LOWERPARENTHESIS {datas.create_new_table[datas.number_columns].size = -1;}
	;
isprimarykey: PRIMARY KEY {datas.create_new_table[datas.number_columns].constraint = 1;}
	|{datas.create_new_table[datas.number_columns].constraint = 0;}
	;

init_constraints: end_create
	| ',' constraints
	;
constraints: CONSTRAINT PRIMARY KEY '(' ID  {
	int ok = 0;
	printf("%s\n",$5);
	for (i = 0; i < datas.number_columns; i++)
	{
		if (strcmp(datas.create_new_table[i].name_column_table,$5) == 0)
		{
			datas.create_new_table[i].constraint = 1;
			ok = 1;
		}
	}

	if (ok == 0)
	{
		printf("Column %s doesn't exist.\n",$5);
		return;
	}
}')' next_constraints
	|CONSTRAINT FOREIGN KEY '('ID {
		int ok = 0;
		for (i = 0; i < datas.number_columns; i++)
		{
			printf("%s\n",$5);
			if (strcmp(datas.create_new_table[i].name_column_table,$5) == 0)
			{
				ok = 1;
			}
		}

		if (ok == 0)
		{
			printf("Column %s doesn't exist.\n",$5);
			return;
		}
	}')' REFERENCES ID{
		datas.create_new_table[i].table_foreign = strdup($3);
		if (datas.create_new_table[i].table_foreign == NULL)
		{
			printf("Out of memory.\nAborting...\n");
			exit(1);
		}
}'('ID 
	{
		datas.create_new_table[i].constraint = 2;
		datas.create_new_table[i].name_column_foreign = strdup($2);
		if (datas.create_new_table[i].name_column_foreign == NULL)
		{
			printf("Out of memory.\nAborting...\n");
			exit(1);
		}
	}')'next_constraints 
	;
next_constraints: ','constraints
	| end_create
	;
end_create:')'';'
	;

%%
#include"lex.yy.c"
#include<ctype.h>


union TOKEN **output(void);
/**
 * Sequência básica operacional
 -- Arquivo que contêm gramática desejada no formato yacc (c.y)
 	-- Programa yacc
 -- Programa fonte C criado pelo yacc y.tab.c
 	-- Compilador C (gcc)
 -- Programa executável que faz a analise sintática da gramática descrita em parse.y (a.out)
 */
Datas execute_create_table(char *sql)
{
	datas.create_new_table = (CreateTB*) malloc (sizeof(CreateTB));
	datas.number_columns = 0;

	if (datas.create_new_table == NULL)
	{
		printf("Out of memory.\nAborting...\n");
		exit(1);
	}

	yy_scan_string(sql);
    yyparse();

    /* to avoid leakage */
    yylex_destroy();

    return datas;
}

char *execute_select(char *sql)
{
	table_select = NULL;

	yy_scan_string(sql);
    
    yyparse();

    /* to avoid leakage */
    yylex_destroy();

    return table_select;//retorna o nome da tabela 
}

Datas execute_insert(char *sql)
{
	i = 0;
	j = 0;
	countColumn = 0;//controle para o número de campos inseridos
	countField = 0;
	count = 0;
	datas.insert = (union TOKEN**) malloc (sizeof(union TOKEN*)*5000);
	
	if (datas.insert == NULL)
	{
		fprintf(stderr, "Out of memory.\nAborting...\n");
		exit(1);
	}
	int it = 0;

	for (it = 0; it < 5000; it++)
	{
		datas.insert[it] = (union TOKEN*) malloc (sizeof(union TOKEN)*250);

		if (datas.insert[it] == NULL)
		{
		  fprintf(stderr, "Out of memory.\nAborting...\n");
		  exit(1);
		}
	}

	datas.numberOfColumns = (int*) malloc (sizeof(int));
	if (datas.numberOfColumns == NULL)
	{
		printf("Out of memory.\nAborting...\n");
		exit(1);
	}
	datas.numberOfColumns[0] = 0;

	datas.numberOfRows = 0;

    yy_scan_string(sql);
    yyparse();

    /* to avoid leakage */
    yylex_destroy();

    return datas;
}
         
yyerror(char *s) {
	printf("%d : %s %s\n", yylineno, s, yytext );
}         

union TOKEN **output(void)
{
	return datas.insert;
}

char *str_replace(char *search , char *replace , char *subject)
{
    char  *p = NULL , *old = NULL , *new_subject = NULL ;
    int c = 0 , search_size;
     
    search_size = strlen(search);
     
    for(p = strstr(subject , search) ; p != NULL ; p = strstr(p + search_size , search) )
    {
        c++;
    }
     
    c = ( strlen(replace) - search_size )*c + strlen(subject);
     
    new_subject = malloc( c );
     
    strcpy(new_subject , "");
    
    old = subject;
     
    for(p = strstr(subject , search) ; p != NULL ; p = strstr(p + search_size , search))
    {
        strncpy(new_subject + strlen(new_subject) , old , p - old);
         
        strcpy(new_subject + strlen(new_subject) , replace);
         
        old = p + search_size;
    }
     
    strcpy(new_subject + strlen(new_subject) , old);
     
    return new_subject;
}

char *remove_quotes(char *str)
{
  char *temp = (char*) malloc (sizeof(char) * strlen(str));

  int i, pos = 0;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] != '\"') {
      temp[pos++] = str[i];
    }
  }

  temp[pos] = '\0';

  return temp;
}

char *remove_single_quotes(char *str)
{
  char *temp = (char*) malloc (sizeof(char) * strlen(str));

  int i, pos = 0;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] != '\'') {
      temp[pos++] = str[i];
    }
  }

  temp[pos] = '\0';

  return temp;
}