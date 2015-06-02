%{
#include <stdio.h>
#include <stdlib.h>

extern FILE *fp;

%}

/**
 * DEFINIÇÕES
 */
%token INT FLOAT DOUBLE 
%token INSERT
%token INTO
%token NUM PONTOFLUTUANTE ID
%token VALUES
%token SELECT
%token FROM
%token WHERE
%token DEC
%token STRING_LITERAL
%%
/**
 * REGRAS
 */
start:	INSERT INTO ID options ';'
	;

options: insertOrder  
	| '('column_names')' insertOrder
	;
column_names: ID 
	| ID','column_names
	;

insertOrder: VALUES insertStart continue
	;
insertStart: '(' insertValues ')' continue

insertValues: tipo','insertValues
	| tipo 
	;

continue:  ',' insertStart
	|
	;

tipo: NUM 
	| STRING_LITERAL
	| PONTOFLUTUANTE
	;

%%
#include"lex.yy.c"
#include<ctype.h>
int count=0;

/**
 * Sequência básica operacional
 -- Arquivo que contêm gramática desejada no formato yacc (c.y)
 	-- Programa yacc
 -- Programa fonte C criado pelo yacc y.tab.c
 	-- Compilador C (gcc)
 -- Programa executável que faz a analise sintática da gramática descrita em parse.y (a.out)
 */
int main(int argc, char *argv[])
{
	struct eq_tree_node *result;
	printf("string de entrada : %s\n", argv[1]);
    yy_scan_string(argv[1]);
    yyparse();
    /* to avoid leakage */
    yylex_destroy();


    return 0;
}
         
yyerror(char *s) {
	printf("%d : %s %s\n", yylineno, s, yytext );
}         

