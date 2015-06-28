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
	int size;//tamanho limite do valor
	
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
%token SELECT
%token FROM
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
	;

create_next_name: ID create_columns {
		datas.name_table_create = strdup($1);

		if (datas.name_table_create == NULL)
		{
			printf("Out of memory.\nAborting...\n");
			exit(1);
		}
	}
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
	else
		fprintf(stderr, "Out of memory.\nAborting...\n");exit(1);
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
	else
		fprintf(stderr, "Out of memory.\nAborting...\n");exit(1);
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
create_columns: '(' values_table ')'';'

values_table: ID{
	
	CreateTB *temp = (CreateTB*) realloc (datas.create_new_table, (datas.number_columns+2)*sizeof(int));

	if (temp == NULL)
	{
		printf("Out of memory.\nAborting...\n");
		exit(1);
	}

	datas.create_new_table = temp;
	int auxiliar_ = datas.number_columns;
	datas.create_new_table[auxiliar_].name_column_table = strdup($1);

} type_value plus_columns
	;

plus_columns: ',' values_table %prec COMMA
	| %prec LOWERTHANCOMMA
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

parenthesis: '(' NUM ')' %prec PARENTHESIS {datas.create_new_table[datas.number_columns].size = atoi($2); }
	| %prec LOWERPARENTHESIS {datas.create_new_table[datas.number_columns].size = -1;}
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
	}
	int it = 0;

	for (it = 0; it < 5000; it++)
	{
		datas.insert[it] = (union TOKEN*) malloc (sizeof(union TOKEN)*250);

		if (datas.insert[it] == NULL)
		{
		  fprintf(stderr, "Out of memory.\nAborting...\n");
		}
	}

	datas.numberOfColumns = (int*) malloc (sizeof(int));
	datas.numberOfColumns[0] = 0;

	datas.numberOfRows = 0;

	// printf("string de entrada : %s\n", argv[1]);
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