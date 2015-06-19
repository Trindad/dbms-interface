########## LEIA-ME PARTE 1 ##############################################################################################################

=> 15 Julho 2014 <=

Universidade Federal da Fronteira Sul – UFFS 
Curso: Ciência da Computação
Componente Curricular: Banco de Dados II
Professor: Dr. Denio Duarte
Alunos: Gabrielle Almeida de Souza e Lais Borin

Introdução
	Trabalho desenvolvido para o componente curricular de Banco de Dados II, sua finalidade é implementar a criação de tabelas, bem como a inclusão de tuplas em disco e remoção de tuplas do buffer complementando o que ja havia sido feito anteriormente.

Sobre o dicionário de dados e o esquema:
	Por definição, o dicionário de dados está no aquivo fs_object.dat e o esquema em fs_schema.dat, os mesmos serão criados no diretório raiz da pasta dos fontes.
	São carregados na memória pelas funçõe leObjeto(nTabela) e leSchema(objeto) e tem a seguinte estrutura:

	struct fs_objects { // Estrutura usada para carregar fs_object.dat
		char nome[20];
		int cod;
		char nArquivo[20];
		int qtdCampos;
	};

	typedef struct tp_table{ // Estrutura usada para carregar fs_schema.dat
		char nome[TAMANHO_NOME_CAMPO];	
		char tipo;						
		int tam;						
		struct tp_table *next;	
	}tp_table;

	O arquivo fs_object.dat organiza-se da seguinte forma:
		20B que representam o nome da tabela; 
		1B que representam o código da tabela;
		20B que representa o nome do arquivo onde estão as tuplas da tabela;
		4B que representa a quantidade de campos da tabela.


	O arquivo fs_schema.dat organiza-se da seguinte forma:
		4B que representam o codigo da tabela; (Código que representa a tabela em fs_objects.dat)
		40B que representam o nome do campo;
		1B que representa o tipo do campo;
		4B que representa o tamanho do campo.

	Caso os arquivos fs_object.dat e fs_schema.dat não existam ainda, quando chamada, a função finalizaTabela(...) criará os mesmos adicionando os dados da tabela criada.

Sobre a criação e inserção em tabelas:
	Para criar uma tabela é necessário uma estrutura do tipo 'table'. Esta estrutura armazenará os dados da tabela a ser criada.
	Após a criação de uma istância de 'table' é necessário iniciá-la com o nome da tabla a ser criada usando a função iniciaTabela(nomeTabela):

		table *t = NULL;
		t = iniciaTabela("Pessoa");

	A partir da iniciação da tabela, pode-se adicionar os campos utilizando a função adicionaCampo(t, nomeCampo, tipoCampo, tamanhoCampo):

		t = adicionaCampo(t, "Nome", 'S', 20); // Cria um campo de nome 'Nome' do tipo string e de tamanho 20
		t = adicionaCampo(t, "Idade", 'I', 4); // Cria um campo de nome 'Idade' do tipo inteiro e de tamanho 4

	Agora, para a tabela ser finalmente adicionada ao dicionário de dados e ao esquema, chama-se a função finalizaTabela(t);

	Criada a tabela, pode-se inserir os dados.
	Quanto da inserção, é necessário criar uma estrutura do tipo 'column' que criará uma lista encadeada de valores a serem adicionados a tabela.
	Para adicionar uma tupla na tabela, é preciso inserir valor por valor de cada campo chamando a função insereValor(c, nomeCampo, valorCampo):

		column *c = NULL;
		c = insereValor(c, "Nome", "Joana");
		c = insereValor(c, "Idade", "40");

	OBS: A inserção dos dados deve ocorrer na sequência de criação de campo, sendo que não é possível realizar a inserção incompleta de uma tupla

	Assim, com os dados já adicionados na tabela, basta chamar a função finalizaInsert(nomeTabela, c) para gravar as tuplas no arquivo nomeTabela.dat, que armazena os dados referente a esta tabela:

		finalizaInsert("Pessoa", c);

Sobre inserir uma tupla no buffer:
	Para adicionar uma tupla no buffer, realize o seguinte tendo já o bufferpoll, o esquema e o objeto devidamete inicializados:

		colocaTuplaBuffer(bufferpoll, 0, esquema, objeto); // Onde '0' é tupla a ser adicionada, relativa a ordem de inserção em sua tabela


Sobre excluir uma tupla do buffer e recuperar uma página do buffer:
	Para excluir uma tupla e obter a mesma para impressão, basta chamar a função com o buffer,  esquema, a página a excluir uma tupla e a tupla a ser excluida:

		column *tuplaE = excluirTuplaBuffer(buffer, esquema, objeto, 0, 2); // '2' é a posição da tupla relativa a página do buffer

	Para recuperar uma página do buffer, bem como obter esta página, basta chamar a função com o buffer, o esquema e o número da página a ser recuperada, como segue:

		column *pagina = getPage(bufferpoll, esquema, objeto, 0);

	A estrutura do tipo 'column' foi criada com o objetivo de armazenar os dados das tabelas, bem como para promover possíveis impressões de dados.



Da compilação:

	$ gcc *.c -o prog -Wall
	$ ./prog <Nome da Tabela>




########## LEIA-ME PARTE 2 ##############################################################################################################



=> 16 outubro 2014 <=

Universidade Federal da Fronteira Sul – UFFS 
Curso: Ciência da Computação
Componente Curricular: Banco de Dados II
Professor: Dr. Denio Duarte
Alunos: Natan J. Mai, Ricardo Zanuzzo, Rogerio T. Schmidt

Introdução
	Trabalho desenvolvido para o componente curricular de Banco de Dados II, sua finalidade é implementar a criação de tabelas, bem como a inclusão de tuplas em disco e remoção de tuplas do buffer complementando o que ja havia sido feito anteriormente.

Sobre o dicionário de dados e o esquema:
	Por definição, o dicionário de dados está no aquivo fs_object.dat e o esquema em fs_schema.dat, os mesmos serão criados no diretório raiz da pasta dos fontes.
	São carregados na memória pelas funçõe leObjeto(nTabela) e leSchema(objeto) e tem a seguinte estrutura:

	struct fs_objects { // Estrutura usada para carregar fs_object.dat
		char nome[20];
		int cod;
		char nArquivo[20];
		int qtdCampos;
	};

	typedef struct tp_table{ // Estrutura usada para carregar fs_schema.dat
		char nome[TAMANHO_NOME_CAMPO];	
		char tipo;						
		int tam;						
		struct tp_table *next;	
	}tp_table;

	O arquivo fs_object.dat organiza-se da seguinte forma:
		20B que representam o nome da tabela; 
		1B que representam o código da tabela;
		20B que representa o nome do arquivo onde estão as tuplas da tabela;
		4B que representa a quantidade de campos da tabela.


	O arquivo fs_schema.dat organiza-se da seguinte forma:
		4B que representam o codigo da tabela; (Código que representa a tabela em fs_objects.dat)
		40B que representam o nome do campo;
		1B que representa o tipo do campo;
		4B que representa o tamanho do campo.

	Caso os arquivos fs_object.dat e fs_schema.dat não existam ainda, quando chamada, a função finalizaTabela(...) criará os mesmos adicionando os dados da tabela criada.

Sobre a criação e inserção em tabelas:
	Para criar uma tabela é necessário uma estrutura do tipo 'table'. Esta estrutura armazenará os dados da tabela a ser criada.
	Após a criação de uma istância de 'table' é necessário iniciá-la com o nome da tabla a ser criada usando a função iniciaTabela(nomeTabela):

		table *t = NULL;
		t = iniciaTabela("Pessoa");

	A partir da iniciação da tabela, pode-se adicionar os campos utilizando a função adicionaCampo(t, nomeCampo, tipoCampo, tamanhoCampo):

		t = adicionaCampo(t, "Nome", 'S', 20); // Cria um campo de nome 'Nome' do tipo string e de tamanho 20
		t = adicionaCampo(t, "Idade", 'I', 4); // Cria um campo de nome 'Idade' do tipo inteiro e de tamanho 4

	Agora, para a tabela ser finalmente adicionada ao dicionário de dados e ao esquema, chama-se a função finalizaTabela(t);

	Criada a tabela, pode-se inserir os dados.
	Quanto da inserção, é necessário criar uma estrutura do tipo 'column' que criará uma lista encadeada de valores a serem adicionados a tabela.
	Para adicionar uma tupla na tabela, é preciso inserir valor por valor de cada campo chamando a função insereValor(c, nomeCampo, valorCampo):

		column *c = NULL;
		c = insereValor(c, "Nome", "Joana");
		c = insereValor(c, "Idade", "40");

	OBS: A inserção dos dados deve ocorrer na sequência de criação de campo, sendo que não é possível realizar a inserção incompleta de uma tupla

	Assim, com os dados já adicionados na tabela, basta chamar a função finalizaInsert(nomeTabela, c) para gravar as tuplas no arquivo nomeTabela.dat, que armazena os dados referente a esta tabela:

		finalizaInsert("Pessoa", c);

Sobre inserir uma tupla no buffer:
	Para adicionar uma tupla no buffer, realize o seguinte tendo já o bufferpoll, o esquema e o objeto devidamete inicializados:

		colocaTuplaBuffer(bufferpoll, 0, esquema, objeto); // Onde '0' é tupla a ser adicionada, relativa a ordem de inserção em sua tabela


Sobre excluir uma tupla do buffer e recuperar uma página do buffer:
	Para excluir uma tupla e obter a mesma para impressão, basta chamar a função com o buffer,  esquema, a página a excluir uma tupla e a tupla a ser excluida:

		column *tuplaE = excluirTuplaBuffer(buffer, esquema, objeto, 0, 2); // '2' é a posição da tupla relativa a página do buffer

	Para recuperar uma página do buffer, bem como obter esta página, basta chamar a função com o buffer, o esquema e o número da página a ser recuperada, como segue:

		column *pagina = getPage(bufferpoll, esquema, objeto, 0);

	A estrutura do tipo 'column' foi criada com o objetivo de armazenar os dados das tabelas, bem como para promover possíveis impressões de dados.



Da compilação:

	$ gcc *.c -o prog -Wall
	$ ./prog <Nome da Tabela>



##############LEIA-ME PARTE 3 #############################################################################################################

=> 20 dezembro 2014 <=

Universidade Federal da Fronteira Sul – UFFS 
Curso: Ciência da Computação
Componente Curricular: Banco de Dados II
Professor: Dr. Denio Duarte
Alunos: Natan J. Mai, Ricardo Zanuzzo, Rogerio T. Schmidt
=============================================================================================================================================

Introdução:

- Ao criar uma tabela, ter a possibilidade de definir chaves primárias e estrangeiras.
- Fazer uma funcionalidade para abrir uma tabela para trabalhar (uma função que englobe leObjeto e leSchema)
- Fazer uma funcionalidade que exclua uma tabela (um drop table). A tabela deve ser excluída do disco e do dicionário.
- Atualizar o txt com as instruções e comentar o código para facilitar as futuras manutenções.
	
A definição de chave primária ou chave estrangeira de um atributo é representado por um "flag" que é passado por parametro na criação da tabela. Exemplo abaixo

- NPK (Sem chave);
- PK (Chave primária);
- FK (Chave estrangeira)

Da criação das tabelas:
	iniciaTabela("tabela"); 							
	adicionaCampo(tabela, "atrib11" , 'I', (sizeof(int))	,PK	," "		," ");
	adicionaCampo(tabela, "atrib12" , 'I', (sizeof(int))	,NPK	," "		," ");
	adicionaCampo(tabela, "atrib13" , 'S', 10		,NPK	," "		," ");
	adicionaCampo(tabela, "atrib14" , 'D', (sizeof(double))	,NPK	," "		," ");
	adicionaCampo(tabela, "atrib11" , 'I', (sizeof(int))	,FK	,"tabRefer"	,"atribRefer");
	finalizaTabela(tabela);

Foi adicionado mais um parâmetro, que é passado para a função adicionaCampo. Trata-se do "flag" que indica o tipo de atributo.
No caso de se tratar de uma chave estrangeira, é necessário apontar a tabela referenciada (tabRefer) e o atributo referenciado (atribRefer).

Obsevação Inserção: Se tabela tiver chave estrangeira FK, o primeiro atributo deve ser PK.

Da inserção de Tuplas:
	insereValor(colunas, "nomeAtrib1", "value1");
	insereValor(colunas, "nomeAtrib2", "value2");
	erro = finalizaInsert("tabela", colunas); 

-> Variável colunas do tipo column,
-> Nome do atributo "nomeAtrib" a ser adicionado o valor,
-> Valor adicionado,


Dos retornos - Tratamento de PK e FK:
	ERRO_CHAVE_PRIMARIA -> -14
	ERRO_CHAVE_ESTRANGEIRA -> -15
	ERRO_CRIACAO_TABELA -> -16

Estes podem ser tratados na main.

É possível adicionar apenas UM atributo como chave PK, ou seja, não é tratada a forma de chave composta.

Foi alterado a struct tp_table para podermos trabalhar com as chaves:

typedef struct tp_table{ // Estrutura usada para carregar fs_schema.dat
	char nome[TAMANHO_NOME_CAMPO];		// Nome do Campo.
	char tipo;				// Tipo do Campo.
	int tam;				// Tamanho do Campo.
	int chave;				// Tipo da chave
	char tabelaApt[TAMANHO_NOME_TABELA]; 	//Nome da Tabela Apontada
	char attApt[TAMANHO_NOME_CAMPO];	//Nome do Atributo Apontado
	struct tp_table *next;			// Encadeamento para o próximo campo.
}tp_table;


Foi também criado as seguintes funções:
-> IniciaAtributos,
-> ExisteAtributo,
-> VerificaChavePK,
-> VerificaChaveFK,
-> ExcluirArquivo,
-> AbreTabela.

Juntamente com os arquivos do trabalho, foi criado também o "exemploMain.c". Trata-se de um modelo (exemplo) de uma maneira
de criar a main(). Nessa exemplo de main() é necessário se atentar ao seguinte:
-int nrTabelas;	->deve ser setado para a quantidades de tabelas a ser criadas;

-   nTabela[0]  = existeArquivo("tabela1.dat"); 	// -> deve ser colocado aqui os nomes das tabelas
    nTabela[1]  = existeArquivo("tabela2.dat");
    nTabela[2]  = existeArquivo("tabela3.dat");

- ao final, foi chamado a função imprime("tabela1") que realiza a impressão das tabelas.

Também pode ser utilizado a função excluirArquivo. A mesma apaga a fisicamente tabela e os registros do dicionário de dados



Da compilação:

	$ gcc *.c -o prog -Wall
	$ ./prog
	
	
	
########## LEIA-ME PARTE 4 ##############################################################################################################

===============================================================================	
Universidade Federal da Fronteira Sul – UFFS 
Curso: Ciência da Computação
Componente Curricular: Banco de Dados II
Professor: Dr. Denio Duarte
Alunos: EDIMAR ROQUE MARTELLO JUNIOR, GEOVANI KOSTULSKI, LEONARDO BELINSKI, SILVANA TRINDADE
===============================================================================

Link github
	https://github.com/Trindad/PK_FK

RESUMO:

	A seguinte parte do trabalho é destinada a criação de uma interface para a manipulação de banco de dados e inserir em tabelas.
	Implementamos o shell.c e o shell.h, o database.c e o database.h, bem como o semantic.c e c.y e c.l (funções para o insert). 
	O shell.c e o shell.h possuem funções relacionadas a interface. 
	O arquivos do database são funções relacionadas ao banco de dados, como criação do banco, buscar bancos entre outras. 
	O semantic.c faz a análise semântica e chama o arquivo gerado pelo .c gerado pelo bison.
	Além disso, dividimos o arquivo buffend nos arquivos table,tuple,common e buffend, todos com .c e .h.

FUNCIONAMENTO:

	A interface foi escrita no arquivo shell.c . Inicialmente nenhum banco estará logado. Guando o usuário entrar no shell poderá digita os comandos desejados e aceitos neste momento. O comando de insert é tratado com o BISON na análise sintática.
	A partir dessa versão existe a possibilidade da criação de vários bancos de dados, listagem dos bancos, de tabelas do sgbd e de inserção de dados via interface.
	A interface é inicializada no começo do arquivo exemploMain.c com a chamada da função shell(), antes de chamar está função é executado um comando de exemplo que gera, insere tabelas, dados nas tabelas no banco de dados, denominado UFFS. Em seguida executa a interface, ela é responsável por chamar as funções que interpretam os comandos digitados pelo usuário.  
	Para seu desenvolvimento foi utilizado como referência a comandos SQL padrão. Os comandos interpretados pelo SGBD podem ser minúsculos ou maiúsculos, portanto pode ter 'CREATE DATABASE my_db' ou 'create datebase my_db', para isso foi desenvolvido função para converter os comandos de entrada para minúsculo (strtolower() -- que recebe como entrada a string que deseja ser convertida, retornando a string convertida para minúsculo, esta função esta implementada em shell.c).

Comandos do Shell:


SHELL
Todos os comandos aceitam ';' no final com exceção dos \l e \d.

	Conectar no banco
		\c <database_name>
	Criar banco de dados
		create database <database_name>
	Listar bancos existentes
		\l 
	Listar tabelas existentes
		\d
	Mostrar dados de uma tabela
		show <table_name>
	Inserções 
		Segue sintaxe SQL normal (abaixo melhor descritas cada umas das maneiras de inserção interpretadas pelo nosso shell)
		Exemplos
			insert INTO Aluno values (4571, "Joca", "Rua dos martins", 258.0);
			insert INTO Aluno (CPF,Nome,Endereco,Peso) values (0444, "Joca", "Rua dos martins", 258.0);
			insert into Aluno values (8079, "Joca", "Rua dos martins", 258.0),(100000, "Joao", "Rua dos capitaes", 70.0);
			insert into Aluno (CPF,Nome,Endereco,Peso) values (2138, "Joca", "Rua dos martins", 258.0),(6778, "Joao", "Rua dos capitaes", 70.0);

	Sair do Shell (interface)
		exit



Implementação das funções:

A implementação das funções está no arquivo database.c e seus protótipos em database.h. 

 - create database name_my_db
Nessa nova versão cada tabela criada no banco recebe o id do banco ao qual ela pertence. 
As tabelas são guardadas no dicionário com os ids e são escritas em arquivo com o id também. 
O id é controlado por uma varíavel que é atualizada a cada conexão ao banco, assim sendo necessário que o usuário crie e se conecte ao banco para depois inserir as tabelas e os dados no respectivo banco. 
Para criar um banco utiliza-se o comando create database nome_do_banco, sendo que o nome do banco pode conter letras maiúsculas e minúsculas. 
Pode se criar um novo banco estando ou não conectado a  um banco.

 - \c name_my_db: 
Para realizar conexão a um banco é utilizado o comando \c nome_banco.
Conecta ao banco que já foi criado. Não existe a possíbilidade de existirem dois bancos com o mesmo nome. Faz uso da função checkCreateDB do arquivo database.c para testar. 
É possível se conectar a um banco a qualquer momento, ou seja, posso estar em um banco de dados e utilizar o comando \c nome_banco e me conectar a um outro banco (saindo do atual).

 - \d
Lista as tabelas do banco, para isso é acionado a função listaTabelas.

 - show name_table
Mostra os dados dentro das tabelas, para isso faz uso da função imprime.

 - \l
Lista o nomes dos bancos existentes no SGBD. Podendo ser utilizada, estando ou não conectado a um banco. 
Mesmo que o programa seja recompilado por completo os bancos criados permanecem lá. A listagem é feita através da função listaBancos, do arquivo database.c.

-help
	ajuda ao usuário

 - INSERT:
	Para realizar um insert em uma tabela é utilizado o comando insert, este possuindo quatro sintaxes diferentes, mas todas obedecendo as mesmas restrições: 
	As palavras insert, into e values podem estar escritas em letras maiúsculas ou minúsculas, mas nunca misturadas (ex: Insert, inTo, Values).  
	A nomenclatura da tabela e os seus atributos devem ser respeitados.
	Para valores cujo o atributo da tabela é do tipo String (S) e char (C) é necessário que o valor passado no insert esteja entra aspas duplas (ex: “valor”, “A”).
	Para os valores cujo o atributo da tabela é do tipo Double (D) é necessário que o valor passado no insert esteja no formato valor.casaDecimal, 
	onde um ponto separa o valor da sua casa decimal (ex: 100.0, 246.273).
	Para o comando insert existem quatro sintaxes diferentes: Insert indicando nome dos atributos e seus respectivos valores. 
	No final do comando sempre deve ser inserido ';'. A seguir podemos ver a sintaxe:
	*Sintaxe onde usuário insere somente uma tupla para n colunas, podendo n ser de 1 até o número de colunas do banco:
		insert into nome_tabela(<colunas>) values (<valores>);
		insert INTO Aluno (CPF,Nome,Endereco,Peso) values (0444, "Joca", "Rua dos martins", 258.0);
		
	*Insert indicando somente os valores dos atributos, estes devem estar na ordem das tabelas no banco:
		Sintaxe: insert into nome_tabela values (<valores_na_ordem_do_bd>);
		Ex: insert INTO Aluno values (4571, "Joca", "Rua dos martins", 258.0);
		
	Insert com inserção de múltiplas tuplas:
		Sintaxe: insert into nome_tabela values (<valores_na_ordem_do_bd>), (<valores_na_ordem_do_bd>);
		Ex: insert into Aluno values (8079, "Joca", "Rua dos martins", 258.0),(100000, "Joao", "Rua dos capitaes", 70.0);
		
	Insert com inserção de múltiplas tuplas indicando os atributos e valores:
		Sintaxe: insert into nome_tabela(<colunas>) values (<valores>),(<valores>);
		Ex: insert into Aluno (CPF,Nome,Endereco,Peso) values (2138, "Joca", "Rua dos martins", 258.0),(6778, "Joao", "Rua dos capitaes", 70.0);
	
	Para o desenvolvimento do insert foi feito a análise léxica, semântica e sintática do comando sql. O código pode ser encontrado no diretório insert
	onde contêm dois diretórios, o sintátic e semantic. O código desenvolvido básicamente recebe uma string com o comando sql, verifica a questão léxica, em seguida a análise semântica utiliza de expressões para verificar a string passada que deve seguir o padrão dos exemplos anteriores, caso algum erro ocorra então é lançado uma mensagem de erro, as mensagens são geradas com o padrão do bison. 
	Na análise semântica, é verificado se os tipos estão corretos, se a tabela existe para o banco logado, estando tudo certo ele faz a inserção das tuplas na tabela.  
 - exit

PASSOS PARA UTILIZAR A INTERFACE

Comando para saída do shell, simplesmente verifica se o token de entrada é exit, dá um break e encerra o programa.

COMPILAR:
	Além do gcc é necessário ter na máquina instalado o bison e o lex.

OPÇÃO 1: Manual
	1º Passo
		Estar no diretório root do projeto
	2º Passo
		Entrar no diretório sintatic, por exemplo
			cd insert/sintatic
		Em seguida 
			lex c.l
			bison c.y
			gcc c.tab.c -o bf -g -lfl
	3° Passo
		Voltar ao diretório root
	4º Passo
		Compilar com GCC
			gcc *.c insert/semantic/semantic.c -o <nome_executavel> -g -ll -ly
	5º Passo 
		Executar
			./<nome_executavel>

OPÇÃO 2: usando Make
	1º Passo 
		make
	2º Passo
		./sgbd
	Para remover os dados usar 'make clean'

	
