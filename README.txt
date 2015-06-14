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

