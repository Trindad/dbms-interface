/*/////////////////////////////////////////////////////////////////////////////////*/
//  Alunos: Natan J. Mai, Ricardo Zanuzzo e Rogério Torchelsen                    //
//  Disciplina: Banco de Dados II                                                 //      
//  Curso: Ciência da Computação                                                  //      
//  Universidade Federal da Fronteira Sul                                         //      
//                                                                                //      
//                                                                                //      
//                                                                                //      
//////////////////////////////////////////////////////////////////////////////////*/



//imprime: Utilizada na impressão das tabelas, conforme o nomeTabela 
void imprime(char nomeTabela[] );

//existeArquivo: Verifica a existência do arquivo da tabela 'filename'
int existeArquivo(const char* filename);

//excluir: Exclui a tabela com 'nomeTabela'    
int excluirArquivo(char *nomeTabela );

// Essa função verifica as condições para a chave estrangeira(FK). E retorna erros caso ocorra.
int verificaChaveFK(char *nomeTabela,column *c, char *nomeCampo, char *valorCampo, char *tabelaApt, char *attApt);

// Essa função verifica as condições para a chave primaria(PK). E retorna erros caso ocorra.
int verificaChavePK(char *nomeTabela, column *c, char *nomeCampo, char *valorCampo);

//função que recebe o nome de uma tabela e engloba as funções leObjeto() e leSchema()
int abreTabela(char *nomeTabela, struct fs_objects *objeto, tp_table **tabela);