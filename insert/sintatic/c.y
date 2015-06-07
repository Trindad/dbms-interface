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

typedef struct _datas 
{
	union TOKEN **insert; //dados para execução do insert
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
%token <str> CHAR
%token DATE
%token <str> STRING_LITERAL

%token-table
%nonassoc LOWERTHANCOMMA
%nonassoc COMMA

%glr-parser
%start	input 

%%
/**
 * REGRAS
 */
input:	exp 
	;
exp: INSERT INTO insert_in 
	;
insert_in : table_name 
	;

ident : ID {
	datas.insert[i][j].str = strdup($1);
	printf("nome tabela[%d][%d]: %s\n", i,j,datas.insert[i][j].str);
	i++;
	j = 0;
	datas.numberOfColumns[i]++;
	datas.numberOfRows++;
	}
	;
column: ID {
	datas.insert[i][j].str = strdup($1);
	printf("nome coluna[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	datas.numberOfColumns[i]++;
	countColumn++;
	j++;
	}
	;
table_name : ident columns
	;
columns: {
	count = 1;
	int *temp = realloc(datas.numberOfColumns, (i+1)*sizeof(int));
	if (temp != NULL)
		datas.numberOfColumns = temp;
	else
		fprintf(stderr, "Erro de allocação\n");
	datas.numberOfColumns[i] = countColumn = 0;
	datas.numberOfRows++;
	}insertOrder 
	| {
	int *temp = realloc(datas.numberOfColumns, (i+1)*sizeof(int));
	if (temp != NULL)
		datas.numberOfColumns = temp;
	else
		fprintf(stderr, "Erro de allocação\n");
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
		fprintf(stderr, "Número de campos %d não compatível com número de colunas %d.\n",countField,countColumn);
	else countField = 0;
	}
	|type values_plus %prec COMMA 
	;
values_plus: ',' insertValues
	; 

continue:  ',' {
	int *temp = realloc(datas.numberOfColumns, (i+2)*sizeof(int));

	if (temp != NULL)
		datas.numberOfColumns = temp;
	else
		fprintf(stderr, "Erro de allocação\n");
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

	char *c = str_replace("\"","", $1);
	datas.insert[i][j].str = strdup(c);
	//printf("STRING[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'S';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	} 
	| ID'.'ID {
	char *c = str_replace("\"","", $1);
	datas.insert[i][j].str = strdup(c);
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
	| CHAR {
	char *c = str_replace("\'","", $1);
	datas.insert[i][j].str = strdup(c);
	//printf("BOOL[%d][%d]: %s\n",i,j, datas.insert[i][j].str);
	j++;
	datas.insert[i][j].t_char = 'C';
	j++;
	countField++;
	datas.numberOfColumns[i]+=2;
	}
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
Datas execute(char *sql)
{
	datas.insert = (union TOKEN**) malloc (sizeof(union TOKEN*)*5000);

	if (datas.insert == NULL)
	{
		fprintf(stderr, "Erro na alocação de memória.\n");
	}
	int it = 0;

	for (it = 0; it < 5000; it++)
	{
		datas.insert[it] = (union TOKEN*) malloc (sizeof(union TOKEN)*250);

		if (datas.insert[it] == NULL)
		{
		  fprintf(stderr, "Erro na alocação de memória.\n");
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