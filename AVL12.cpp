#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 101

// ATENÇÃO: O cálculo da complexidade deve incluir todo o código, inclusive a leitura do arquivo.


char nome_arquivo[50], aux[MAX];								// proibido alterar esta linha
int opcao, tamanho, i, j, n, m, digrafo, matriz_adj[MAX][MAX];  // proibido alterar esta linha
// Declare variáveis globais a partir daqui.

int visitado [MAX]; // marca os vertices ja visitado
int visitado_ordem[MAX]; // guarda a ordem da visita
int quant_dfs; // conta quantos vertices foram visitados

void dfs(int linha){
	  
	visitado[linha] = 1;  // Marca o vertice atual como visitado
	
    visitado_ordem[quant_dfs] = linha; // Guarda o vertice atual 
    
    quant_dfs++; // Incrementa em 1 a quantidade de vertices visitados, avacando para a proxima posicao
    
    for (int coluna = 1; coluna <= n; coluna++) {
    	
        if (matriz_adj[linha][coluna] != 0 && visitado[coluna] == 0) { // Vai para a coluna se houver ligacao e  também se o vizinho nao foi visitado
        	
            dfs(coluna);  //Visita o vizinho
            
    	}
	}
}



//Struct referente a cada no da Arvore AVL
struct No{

	int vertice; //Variavel que recebera os vertices do grafo
	
	int altura; //Altura do no em especifico
	
	No *esquerda; //Ponteiro para o filho esquerdo (se nao existir, aponta para NULL)
	No *direita; //Ponteiro para o filho direito (se nao existir, aponta para NULL)
	
};

//Retorna a altura do no em questao
altura(No *no){
	
	if(!no){
		
		return 0; //Se o no nao existir, retorna valor 0 para a altura
		
	}
	
	return no->altura;
	
}

//Verifica qual numero eh maior. Util na funcao atualizarAltura para decidir entre qual altura (filho esquerdo ou direito) eh maior
int maior(int a, int b){
	
	if(a > b){
		
		return a;
		
	}
	
	return b;
	
}

//Atualiza o valor da altura do no
void atualizarAltura(No *no){
	
	if(no){
		
		no->altura = 1 + maior(altura(no->esquerda), altura(no->direita));
		
	}
	
}

//Percorre os filhos de um no ate encontrar o de menor valor (o mais a esquerda)
No* menorValor(No *no){
	
	while(no->esquerda){
		
		no = no->esquerda;
		
	}
	
	return no;
	
}

//Da teoria da AVL. Essa funcao calcula se a diferenca entre as alturas da subarvore esquerda e direita, indicando se a arvore "pesa" mais para um lado
fatorBalanceamento(No *no){
	
	if(!no){
		
		return 0;
		
	}
	
	return altura(no->esquerda) - altura(no->direita);
	
}

//Rotacao LL
No* rotacaoDireita(No *y){
	
	No *x = y->esquerda; //Copia o no esquerdo para *x
	No *T2 = x->direita; //Copia o no direito de x (y->esquerdo) para T2, uma variavel que vai armazenar o no de forma temporaria
	
	x->direita = y;
	y->esquerda = T2;
	
	atualizarAltura(y);
	atualizarAltura(x);
	
	return x;
	
}

//Rotacao RR
No* rotacaoEsquerda(No *x){
	
	No *y = x->direita;
	No *T2 = y->esquerda;
	
	y->esquerda = x;
	x->direita = T2;
	
	atualizarAltura(x);
	atualizarAltura(y);
	
	return y;
	
}

//Rotacao LR
No* rotacaoEsquerdaDireita(No *z){
	
	z->esquerda = rotacaoEsquerda(z->esquerda);
	
	return rotacaoDireita(z);
	
}

//Rotacao RL
No* rotacaoDireitaEsquerda(No *z){
	
	z->direita = rotacaoDireita(z->direita);
	
	return rotacaoEsquerda(z);
	
}

//Insere na AVL o vertice lido na DFS
No* inserir(No *no, int num_linha){
	
	if(!no){
		
		No *novo = (No*) malloc(sizeof(No));
		
		if(!novo){
			
			printf("\nAlocacao mal-sucedida!\n");
			exit(1);
			
		}
		
		novo->vertice = num_linha;
		
		novo->altura = 1;
		
		novo->esquerda = NULL;
		novo->direita = NULL;
		
		return novo;
		
	}
	
	if(num_linha < no->vertice){
		
		no->esquerda = inserir(no->esquerda, num_linha);
		
	} else if(num_linha > no->vertice){
	
		no->direita = inserir(no->direita, num_linha);
		
	} else{
	
		return no;	
		
	}
	
	atualizarAltura(no);
	
	int fb = fatorBalanceamento(no);
	
	//LL
	
	if(fb > 1 && num_linha < no->esquerda->vertice){
		
		return rotacaoDireita(no);
		
	}
	
	//RR
	
	if(fb < -1 && num_linha > no->direita->vertice){
		
		return rotacaoEsquerda(no);
		
	}
	
	//LR
	
	if(fb > 1 && num_linha > no->esquerda->vertice){
		
		return rotacaoEsquerdaDireita(no);
		
	}
	
	//RL
	
	if(fb < -1 && num_linha < no->direita->vertice){
		
		return rotacaoDireitaEsquerda(no);
		
	}
	
	return no;
	
}

No* remover(No *no, int num_linha){
	
	if(!no){
		
		return NULL;
		
	}
	
	if(num_linha < no->vertice){
		
		no->esquerda = remover(no->esquerda, num_linha);
		
	} else if(num_linha > no->vertice){
		
		no->direita = remover(no->direita, num_linha);
		
	} else{
		
		if(!no->esquerda && !no->direita){
			
			free(no);
			
			return NULL;
			
		}
		
		if(!no->esquerda){
			
			No *aux = no->direita;
			free(no);
			
			return aux;
			
		}
		
		if(!no->direita){
			
			No *aux = no->esquerda;
			free(no);
			
			return aux;
			
		}
		
		No *aux = menorValor(no->direita);
		
		no->vertice = aux->vertice;
		
		no->direita = remover(no->direita, aux->vertice);
		
	}
	
	atualizarAltura(no);
	
	int fb = fatorBalanceamento(no);
	
	if(fb > 1 && fatorBalanceamento(no->esquerda) >= 0){
		
		return rotacaoDireita(no);
		
	}
	
	if(fb < -1 && fatorBalanceamento(no->direita) <= 0){
		
		return rotacaoEsquerda(no);
		
	}
	
	if(fb > 1 && fatorBalanceamento(no->esquerda) < 0){
		
		return rotacaoEsquerdaDireita(no);
		
	}
	
	if(fb < -1 && fatorBalanceamento(no->direita) > 0){
		
		return rotacaoDireitaEsquerda(no);
		
	}
	
	return no;
	
}

void executafuncao (){   										// É dentro desta função que o aluno deverá escrever seu código
	
	int x = 0;
	if (!x) {
		
		printf ("deu certo");
		
	}
	
	quant_dfs=0;  // Zera o contador de vertices visitados
	
	for(int vertice=1; vertice <= n; vertice ++){
		
		visitado[vertice]=0; //Limpa a marca de visita da busca anterior
	
	}
	
	for(int vertice=1; vertice <= n; vertice ++){ // Visita os vertices que faltam
		
		if(visitado[vertice]==0){
			
			dfs(vertice);
			
		}
	}
	
	puts ("\n\nOrdem da busca em profundidade: "); // Mostrando a ordem da visita
	
	for(int pos=0; pos< quant_dfs; pos++){
		
		printf(" %d ", visitado_ordem[pos]);
		
	}
	
	puts("");
	
	int op;
	
	do{
		
		puts ("\n==========MENU AVL==========\n");
		
		puts ("1 - Inserir");
		puts ("2 - Remover linha");
		puts ("3 - Buscar linha");
		puts ("4 - Mostrar arvore equilibrada");
		puts ("0 - Voltar ao menu de arquivos");
		scanf("%d", &op);
		
		
	}while(op!=0);
    
}

int main() {
	opcao = 1;

    while(opcao != 0){
        inicio:
        tamanho = 0;
        m = 0;
        digrafo = 0;
        system("cls"); 
        fflush(stdin); 
        printf("Digite o nome do arquivo (sem o .txt): ");
        gets(nome_arquivo);
        FILE *arquivo; 
        strcat(nome_arquivo,".txt"); 
            
        if((arquivo = fopen(nome_arquivo,"r")) != NULL){
        	while(fgets(aux, MAX, arquivo)) { 
                   char *elemento; 
                   elemento = strtok(aux, " ");
                    while (elemento != NULL){
                        elemento = strtok(NULL, " "); 
                    }
                    tamanho = tamanho + 1; 
                }
                fclose(arquivo);
                
                printf ("\n --------------------------- MATRIZ DE ADJACÊNCIA ---------------------------\n");
                
                if((arquivo = fopen(nome_arquivo,"r")) != NULL){
                    printf("\n");
                    for(int i = 1; i <= tamanho; i++){
                        for(int j = 1; j <= tamanho; j++){
                            fscanf(arquivo, "%d", &matriz_adj[i][j]);
							printf("%3d ", matriz_adj[i][j]); 
                        }
                        printf("\n");
                    }
                }
                fclose(arquivo);
				n = tamanho;
				
                for(int i = 1; i <= tamanho; i++){
                    for(int j = 1; j <= tamanho; j++){
						if (matriz_adj[i][j] != matriz_adj[j][i]) {
							digrafo = 1;   // é dígrafo
						}
					}
				}
				
                for(int i = 1; i <= tamanho; i++){
                    for(int j = 1; j <= tamanho; j++){
						if (digrafo == 1 && matriz_adj[i][j] != 0) {
							m = m + 1;
						} 
						if (digrafo == 0 && matriz_adj[i][j] != 0 && i <= j) {
							m = m + 1;
						}
					}
				}
				
	          printf ("\nN = %d e M = %d \n \n",n,m);     
            } else {
                printf("\nErro ao abrir o arquivo/Erro ao localizar o arquivo\n\n");
                
            }
        	  
		executafuncao ();       
        printf("\nDigite 0 para sair ou qualquer outra coisa para continuar: ");
		scanf("%d", &opcao);
        
    }
    return 0;
}


