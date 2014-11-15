#include "buffend.h"

int main(int rg, char *nomeTabela[]){
	
	int erro;

	table *t = NULL;
	column *c = NULL;

	t = iniciaTabela("Pessoa");

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}

	t = adicionaCampo(t, "Nome", 'S', 20);
	t = adicionaCampo(t, "Idade", 'I', (sizeof(int)));
	t = adicionaCampo(t, "Sexo", 'C', (sizeof(char)));
	t = adicionaCampo(t, "Obs", 'S', 40);
	t = adicionaCampo(t, "Media", 'D', (sizeof(double)));

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	
	c = insereValor(c, "Nome", "Um");
	c = insereValor(c, "Idade", "40");
	c = insereValor(c, "Sexo", "F");
	c = insereValor(c, "Obs", "Obs. Um");
	c = insereValor(c, "Media", "2.5");
	

	c = insereValor(c, "Nome", "Dois");
	c = insereValor(c, "Idade", "20");
	c = insereValor(c, "Sexo", "M");
	c = insereValor(c, "Obs", "Obs. Dois");
	c = insereValor(c, "Media", "1.67");

	c = insereValor(c, "Nome", "Três");
	c = insereValor(c, "Idade", "30");
	c = insereValor(c, "Sexo", "F");
	c = insereValor(c, "Obs", "Obs. Três");
	c = insereValor(c, "Media", "1.456");

	erro = finalizaInsert("Pessoa", c);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaInsert()\n", erro);
		return 0;
	}


	if(rg == 1){
		printf("Erro: nome de tabela nao inserido na linha de comando.\n");
		return 0;
	}

	struct fs_objects objeto = leObjeto(nomeTabela[1]);	
	
	tp_table *esquema = leSchema(objeto);

	if(esquema == ERRO_ABRIR_ESQUEMA){
		printf("Erro ao criar o esquema.\n");
		return 0;
	}

	tp_buffer *bufferpoll = initbuffer();

	if(bufferpoll == ERRO_DE_ALOCACAO){
		printf("Erro ao alocar memória para o buffer.\n");
		return 0;
	}

	erro = colocaTuplaBuffer(bufferpoll, 0, esquema, objeto);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}
	
	erro = colocaTuplaBuffer(bufferpoll, 1, esquema, objeto);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}
	
	erro = colocaTuplaBuffer(bufferpoll, 2, esquema, objeto);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}
	
	column *tuplaE = excluirTuplaBuffer(bufferpoll, esquema, objeto, 0, 2); //pg, tupla
	column *pagina = getPage(bufferpoll, esquema, objeto, 0);

	if(tuplaE == ERRO_PARAMETRO){
		printf("Erro, na função excluirTuplaBuffer(), problemas no parametro.\n");
		return 0;
	}
	if(pagina == ERRO_PARAMETRO){
		printf("Erro, na função getPage(), problemas no parametro.\n");
		return 0;
	}
	
	// PARA IMPRIMIR TUPLA EXCLUIDA -----------------------------
	//-------------------------------------------------------------
	printf("\nTupla excluída do Buffer.\n");
	int j = 0;
	for(j=0; j < objeto.qtdCampos; j++){
		
		if(tuplaE[j].tipoCampo == 'S')
			printf("%s: %s ", tuplaE[j].nomeCampo,tuplaE[j].valorCampo);
		else if(tuplaE[j].tipoCampo == 'I'){
			int *n = (int *)&tuplaE[j].valorCampo[0];
			printf("%s: %d ",tuplaE[j].nomeCampo, *n);
		}
		else if(tuplaE[j].tipoCampo == 'C'){
			printf("%s: %c ",tuplaE[j].nomeCampo, tuplaE[j].valorCampo[0]);
		}
		else if(tuplaE[j].tipoCampo == 'D'){
			double *n = (double *)&tuplaE[j].valorCampo[0];
			printf("%s: %f ",tuplaE[j].nomeCampo, *n);
		}
		printf("\n");
	}
	
	//-------------------------------------------------------------

	// PARA IMPRIMIR PÁGINA ---------------------------------------
	//-------------------------------------------------------------
	printf("\nPágina armazenada na estrutura column *pagina.\n");
	for(j=0; j < objeto.qtdCampos*bufferpoll[0].nrec; j++){
		
		if(pagina[j].tipoCampo == 'S')
			printf("%s: %s ", pagina[j].nomeCampo,pagina[j].valorCampo);
		else if(pagina[j].tipoCampo == 'I'){
			int *n = (int *)&pagina[j].valorCampo[0];
			printf("%s: %d ",pagina[j].nomeCampo, *n);
		}
		else if(pagina[j].tipoCampo == 'C'){
			printf("%s: %c ",pagina[j].nomeCampo, pagina[j].valorCampo[0]);
		}
		else if(pagina[j].tipoCampo == 'D'){
			double *n = (double *)&pagina[j].valorCampo[0];
			printf("%s: %f ",pagina[j].nomeCampo, *n);
		}
		printf("\n");
	}
	printf("\n\n");
	
	//-------------------------------------------------------------
	

	erro = printbufferpoll(bufferpoll, esquema, objeto, 0);

	if(erro != SUCCESS){
		printf("Erro %d: na função printbufferpoll().\n", erro);
		return 0;
	}

	return 0;
}
