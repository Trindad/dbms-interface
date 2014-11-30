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

//insere: Inserir tuplas na tabela selecionada conforme 'menu'
void insere(int menu);

//imprime: Utilizada na impressão das tabelas, conforme o nomeTabela 
void imprime(char nomeTabela[] );

//existeArquivo: Verifica a existência do arquivo da tabela 'filename'
int existeArquivo(const char* filename);

//excluir: Exclui a tabela com 'nomeTabela'    
void excluirArquivo(char nomeTabela[] );
