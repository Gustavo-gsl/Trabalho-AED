#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 101

// ATENÇÃO: O cálculo da complexidade deve incluir todo o ccódigo, inclusive a leitura do arquivo.


char nome_arquivo[50], aux[MAX];								// proibido alterar esta linha
int opcao, tamanho, i, j, n, m, digrafo, matriz_adj[MAX][MAX];  // proibido alterar esta linha
// Declare variáveis globais a partir daqui.

struct Aluno{
	
	char nome[100];
	float nota1;
	float nota2;
	int idade;
	
	int altura;
	
	Aluno *esquerda;
	Aluno *direita;
	
};

Aluno* menorValor(Aluno *raiz){
	
	while(raiz->esquerda){
		
		raiz = raiz->esquerda;
		
	}
	
	return raiz;
	
}

Aluno* buscar(Aluno *raiz, int idade){
	
	if(!raiz){
		
		return NULL;
		
	}
	
	if(idade == raiz->idade){
		
		return raiz;
		
	}
	
	if(idade < raiz->idade){
		
		return buscar(raiz->esquerda, idade);
		
	}
	
	return buscar(raiz->direita, idade);
	
}

altura(Aluno *no){
	
	if(!no){
		
		return 0;
		
	}
	
	return no->altura;
	
}

int maior(int a, int b){
	
	if(a > b){
		
		return a;
		
	}
	
	return b;
	
}

void atualizarAltura(Aluno *no){
	
	if(no){
		
		no->altura = 1 + maior(altura(no->esquerda), altura(no->direita));
		
	}
	
}

fatorBalanceamento(Aluno *no){
	
	if(!no){
		
		return 0;
		
	}
	
	return altura(no->esquerda) - altura(no->direita);
	
}

Aluno* rotacaoDireita(Aluno *y){
	
	Aluno *x = y->esquerda;
	Aluno *T2 = x->direita;
	
	x->direita = y;
	y->esquerda = T2;
	
	atualizarAltura(y);
	atualizarAltura(x);
	
	return x;
	
}

Aluno* rotacaoEsquerda(Aluno *x){
	
	Aluno *y = x->direita;
	Aluno *T2 = y->esquerda;
	
	y->esquerda = x;
	x->direita = T2;
	
	atualizarAltura(x);
	atualizarAltura(y);
	
	return y;
	
}

Aluno* rotacaoEsquerdaDireita(Aluno *z){
	
	z->esquerda = rotacaoEsquerda(z->esquerda);
	
	return rotacaoDireita(z);
	
}

Aluno* rotacaoDireitaEsquerda(Aluno *z){
	
	z->direita = rotacaoDireita(z->direita);
	
	return rotacaoEsquerda(z);
	
}

Aluno* inserir(Aluno *raiz){
	
	if(!raiz){
		
		Aluno *novo = (Aluno*) malloc(sizeof(Aluno));
		
		printf("\nDigite o nome: ");
		fflush(stdin);
		scanf("%[^\n]", novo->nome);
		
		printf("\nDigite a nota 1: ");
		fflush(stdin);
		scanf("%f", &novo->nota1);

		printf("\nDigite a nota 2: ");
		fflush(stdin);
		scanf("%f", &novo->nota2);
		
		printf("\nDigite a idade: ");
		fflush(stdin);
		scanf("%d", &novo->idade);
		
		novo->altura = 1;
		
		novo->esquerda = NULL;
		novo->direita = NULL;
		
		return novo;
		
	}
	
	int idade;
	
	printf("\nDigite a idade: ");
	fflush(stdin);
	scanf("%d", &idade);
	
	if(idade < raiz->idade){
		
		raiz->esquerda = inserir(raiz->esquerda);
		
	} else{
	
		raiz->direita = inserir(raiz->direita);
		
	}
	
	atualizarAltura(raiz);
	
	int fb = fatorBalanceamento(raiz);
	
	//LL
	
	if(fb > 1 && idade < raiz->esquerda->idade){
		
		return rotacaoDireita(raiz);
		
	}
	
	//RR
	
	if(fb < -1 && idade > raiz->direita->idade){
		
		return rotacaoEsquerda(raiz);
		
	}
	
	//LR
	
	if(fb > 1 && idade > raiz->esquerda->idade){
		
		return rotacaoEsquerdaDireita(raiz);
		
	}
	
	//RL
	
	if(fb < -1 && idade < raiz->direita->idade){
		
		return rotacaoDireitaEsquerda(raiz);
		
	}
	
	return raiz;
	
}

Aluno* remover(Aluno *raiz, int idade){
	
	if(!raiz){
		
		return NULL;
		
	}
	
	if(idade < raiz->idade){
		
		raiz->esquerda = remover(raiz->esquerda, idade);
		
	} else if(idade > raiz->idade){
		
		raiz->direita = remover(raiz->direita, idade);
		
	} else{
		
		if(!raiz->esquerda && !raiz->direita){
			
			free(raiz);
			
			return NULL;
			
		}
		
		if(!raiz->esquerda){
			
			Aluno *aux = raiz->direita;
			free(raiz);
			
			return aux;
			
		}
		
		if(!raiz->direita){
			
			Aluno *aux = raiz->esquerda;
			free(raiz);
			
			return aux;
			
		}
		
		Aluno *aux = menorValor(raiz->direita);
		
		strcpy(raiz->nome, aux->nome);
		raiz->nota1 = aux->nota1;
		raiz->nota2 = aux->nota2;
		raiz->idade = aux->idade;
		
		raiz->direita = remover(raiz->direita, aux->idade);
		
	}
	
	atualizarAltura(raiz);
	
	int fb = fatorBalanceamento(raiz);
	
	if(fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0){
		
		return rotacaoDireita(raiz);
		
	}
	
	if(fb < -1 && fatorBalanceamento(raiz->direita) <= 0){
		
		return rotacaoEsquerda(raiz);
		
	}
	
	if(fb > 1 && fatorBalanceamento(raiz->esquerda) < 0){
		
		return rotacaoEsquerdaDireita(raiz);
		
	}
	
	if(fb < -1 && fatorBalanceamento(raiz->direita) > 0){
		
		return rotacaoDireitaEsquerda(raiz);
		
	}
	
	return raiz;
	
}


void executafuncao (){   										// É dentro desta função que o aluno deverá escrever seu código
int x = 0;
if (!x) {
printf ("deu certo");
}



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

