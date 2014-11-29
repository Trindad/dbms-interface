/*/////////////////////////////////////////////////////////////////////////////////*/
//  Alunos: Natan J. Mai, Ricardo Zanuzzo e Rogério Torchelsen                    //
//  Disciplina: Banco de Dados II                                                 //      
//  Curso: Ciência da Computação                                                  //      
//  Universidade Federal da Fronteira Sul                                         //      
//                                                                                //      
//                                                                                //      
//                                                                                //      
//////////////////////////////////////////////////////////////////////////////////*/

//menu: Utilizada no controle do 'menu', com todas as opcoes.
void menu();

//imprime: Utilizada na impressão das tabelas, conforme o nomeTabela 
void imprime(char nomeTabela[] );

//excluir: Exclui a tabela com 'nomeTabela'    
void excluirArquivo(char nomeTabela[] );

//insere: Inserir tuplas na tabela selecionada conforme 'menu'
int insere(int menu);

//existeArquivo: Verifica a existência do arquivo da tabela 'filename'
int existeArquivo(const char* filename);

// Essa função verifica as condições para a chave estrangeira(FK). E retorna erros caso ocorra.
int verificaChaveFK(char *nomeTabela, char *nomeCampo, char *valorCampo, char *tabelaApt, char *attApt);

// Essa função verifica as condições para a chave primaria(PK). E retorna erros caso ocorra.
int verificaChavePK(char *nomeTabela, char *nomeCampo, char *valorCampo);