alpha [a-zA-Z]
digit [0-9]

%%
[ \t]		;
[ \n]   { yylineno = yylineno + 1;}
int	return INT;
float return FLOAT;
double return DOUBLE;
insert	return INSERT;
into return INTO;
values return VALUES;
select return SELECT;
from return FROM;
where 	return WHERE;
{digit}+       return NUM;
{digit}+.{digit}+ return PONTOFLUTUANTE;

_({alpha}|{digit}|_)*|{alpha}({alpha}|{digit}|_)* return ID;

L?\"(\\.|[^\\"])*\"	return STRING_LITERAL;
.       return yytext[0];
%%
