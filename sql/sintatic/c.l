alpha [a-zA-Z]
digit [0-9]
yylineno = 1;
%%
[ \t]*		;
[ \n]   { yylineno = yylineno + 1;}
INSERT|insert	{yylval.str = yytext; return INSERT;}
INTO|into {yylval.str = yytext; return INTO;}
VALUES|values {yylval.str = yytext; return VALUES;}
SELECT|select {yylval.str = yytext; return SELECT;}
CREATE|create {yylval.str = yytext; return CREATE;}
TABLE|table {yylval.str = yytext; return TABLE;}
INTEGER|integer {yylval.str = yytext; return INTEGER;}
CHAR|char {yylval.str = yytext; return CHAR;}
DOUBLE|double {yylval.str = yytext; return DOUBLE;}
FROM|from {yylval.str = yytext; return FROM;}
WHERE|where 	{yylval.str = yytext; return WHERE;}
{digit}+     {yylval.str = yytext;  return NUM ;}
{digit}+\.{digit}+ {yylval.str = yytext;return PONTOFLUTUANTE;}
true|TRUE { yylval.str = "1"; return BOOL; }
false|FALSE { yylval.str = "0"; return BOOL; }
_({alpha}|{digit}|_)*|{alpha}({alpha}|{digit}|_)* {yylval.str = yytext; return ID;}
\'(\\.)?\'	{yylval.t_char = yytext[1] == '\'' ? ' ':yytext[1]; return CHAR;}
L?\"(\\.|[^\\"])*\"	{yylval.str = yytext; return STRING_LITERAL;}
.       return yytext[0];
%%
