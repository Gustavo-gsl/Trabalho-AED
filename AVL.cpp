#include <stdio.h>
#include <stdlib.h>

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