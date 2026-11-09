#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 101

// ATEN��O: O c�lculo da complexidade deve incluir todo o cc�digo, inclusive a leitura do arquivo.


char nome_arquivo[50], aux[MAX];								// proibido alterar esta linha
int opcao, tamanho, i, j, n, m, digrafo, matriz_adj[MAX][MAX];  // proibido alterar esta linha
// Declare vari�veis globais a partir daqui.

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

	No *esquerda; //Ponteiro para o filho esquerdo (se nao existir, apontara para NULL -> Processo feito durante o metodo inserir())
	
	No *direita; //Ponteiro para o filho direito (se nao existir, apontara para NULL -> Processo feito durante o metodo inserir())

};

No *raiz = NULL; // raiz da arvore AVL

//Retorna a altura do no em questao
int altura(No *no){

	if(!no){

		return 0; //Se o no nao existir, retorna valor 0 para a altura

	}

	return no->altura; //Retorna a informacao da altura presente na estrutura

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

		no->altura = 1 + maior(altura(no->esquerda), altura(no->direita)); //Um no, por existir, tem altura 1. Se possuir filhos, devera ser somada a maior altura entre os filhos

	}

}

//Percorre os filhos de um no ate encontrar o de menor valor (o mais a esquerda)
No* menorValor(No *no){

	while(no->esquerda){

		no = no->esquerda; //Adentra o filho esquerdo enquanto existir

	}

	return no; //Retorna o no mais a esquerda encontrado

}

//Da teoria da AVL. Essa funcao calcula se a diferenca entre as alturas da subarvore esquerda e direita, indicando se a arvore "pesa" mais para um lado
int fatorBalanceamento(No *no){

	if(!no){

		return 0; //Caso o no esteja vazio, seu fator de balanceamento eh 0

	}

	return altura(no->esquerda) - altura(no->direita); //Calculo do Fator de Balanceamento (altura da subarvore esquerda - altura da subarvore direita)

}

//Rotacao LL - Arvore desbalanceada para a esquerda
No* rotacaoDireita(No *y){

	No *x = y->esquerda; //Copia o no esquerdo para *x
	
	No *T2 = x->direita; //Copia o no direito de x (y->esquerda) para T2, uma variavel que vai armazenar o no de forma temporaria

	x->direita = y; //No direito de *x passa a ser y (o no-raiz inicial)
	
	y->esquerda = T2; //No esquerdo de y passa a ser o mesmo que T2 apontava (antigo x->direita)
	
	//Atualizando alturas após alterações
	
	atualizarAltura(y); //Primeiro se atualiza a altura do no y, pois ele se tornou um filho de x
	
	atualizarAltura(x); //Entao se atualiza a altura de x que diz respeito a altura da subarvore completa

	return x; //Retorna a nova raiz da subarvore

}

//Rotacao RR - Arvore desbalanceada para a direita
No* rotacaoEsquerda(No *x){

	No *y = x->direita; //Copia o no direito para *y
	
	No *T2 = y->esquerda; //Copia o no esquerdo de y (x->direita) para T2, uma variavel que vai armazenar o no de forma temporaria

	y->esquerda = x; //No esquerdo de *y passa a ser x (o no-raiz inicial)
	
	x->direita = T2; //No direito de x passa a ser o mesmo que T2 apontava (antigo y->esquerda)

	//Atualizando alturas após alterações
	atualizarAltura(x); //Primeiro se atualiza a altura do no x, pois ele se tornou um filho de y
	
	atualizarAltura(y); //Entao se atualiza a altura de y que diz respeito a altura da subarvore completa

	return y; //Retorna a nova raiz da subarvore

}

//Rotacao LR - Arvore desbalanceada na direita da subarvore esquerda
No* rotacaoEsquerdaDireita(No *z){

	z->esquerda = rotacaoEsquerda(z->esquerda); //Realiza a rotacao a esquerda na subarvore esquerda

	return rotacaoDireita(z); //Realiza a rotacao a direita na raiz

}

//Rotacao RL - Arvore desbalanceada na esquerda da subarvore direita
No* rotacaoDireitaEsquerda(No *z){

	z->direita = rotacaoDireita(z->direita); //Realiza a rotacao a direita na subarvore direita

	return rotacaoEsquerda(z); //Realiza a rotacao a esquerda na raiz

}

//Insere de forma recursiva na AVL o vertice lido na DFS
No* inserir(No *no, int num_linha){
	
	//Inicializacao do no ocorre somente se este estiver vazio
	if(!no){

		No *novo = (No*) malloc(sizeof(No)); //Alocacao dinamica do no a ser adicionado

		if(!novo){

			printf("\nAlocacao mal-sucedida!\n");
			exit(1); //Em caso de erro na alocacao, encerra o programa

		}

		novo->vertice = num_linha; //Guarda o numero do vertice lido na linha

		novo->altura = 1; //Como o no acabou de ser criado, sua altura é definida diretamente como 1

		novo->esquerda = NULL; //Inicializacao do ponteiro para o filho esquerdo (atualmente inexistente)
		
		novo->direita = NULL; //Inicializacao do ponteiro para o filho direito (atualmente inexistente)

		return novo; //Retorna o no criado

	}

	//Busca de onde deve ser inserido o novo vertice lido da matriz de adjacência
	//Necessaria para manter a arvore ordenada
	if(num_linha < no->vertice){ //Se o vertice tiver indice menor que o presente na arvore sendo analisado:

		no->esquerda = inserir(no->esquerda, num_linha); //A insercao sera feita no filho esquerdo do no analisado

	} else if(num_linha > no->vertice){ //Se o vertice tiver indice maior que o presente na arvore sendo analisado:

		no->direita = inserir(no->direita, num_linha); //A insercao sera feita no filho direito do no analisado

	} else{ //Se o vertice tiver indice igual ao presente na arvore sendo analisado:

		return no; //Ele nao sera inserido. Sera encerrada o metodo inserir() retornando o no analisado

	}

	atualizarAltura(no); //Apos a insercao, atualiza-se a altura do no inicial

	int fb = fatorBalanceamento(no); //Calculo do Fator de Balanceamento para rotacao posterior

	//Verifica desbalaceamento na subarvore esquerda
	//E verifica se o valor foi adicionado a esquerda na subarvore esquerda
	if(fb > 1 && num_linha < no->esquerda->vertice){

		return rotacaoDireita(no); //Realiza a rotacao LL no no

	}

	//Verifica desbalaceamento na subarvore direita
	//E verifica se o valor foi adicionado a direita na subarvore direita
	if(fb < -1 && num_linha > no->direita->vertice){

		return rotacaoEsquerda(no); //Realiza a rotacao RR no no

	}

	//Verifica desbalaceamento na subarvore esquerda
	//E verifica se o valor foi adicionado a direita na subarvore esquerda
	if(fb > 1 && num_linha > no->esquerda->vertice){

		return rotacaoEsquerdaDireita(no); //Realiza a rotacao LR no no

	}

	//Verifica desbalaceamento na subarvore direita
	//E verifica se o valor foi adicionado a esquerda na subarvore direita
	if(fb < -1 && num_linha < no->direita->vertice){

		return rotacaoDireitaEsquerda(no); //Realiza a rotacao RL no no

	}

	return no; //Retorna a subarvore balanceada

}

//Remove de forma recursiva o vertice desejado
No* remover(No *no, int num_linha){

	//Se o no estiver vazio, retorna NULL
	if(!no){

		return NULL;

	}

	//Busca de onde deve ser removido o vertice desejado
	if(num_linha < no->vertice){ //Se o valor desejado for menor que o valor do vertice guardado no no analisado:

		no->esquerda = remover(no->esquerda, num_linha); //A remocao sera feita no filho esquerdo do no analisado

	} else if(num_linha > no->vertice){ //Se o valor desejado for maior que o valor do vertice guardado no no analisado:

		no->direita = remover(no->direita, num_linha); //A remocao sera feita no filho direito do no analisado

	} else{ //Caso o valor seja igual ao do vertice guardado no no analisado, entao o vertice a ser removido foi encontrado
		
		//Caso 1: No sem filhos
		if(!no->esquerda && !no->direita){

			free(no); //O no eh simplesmente liberado

			return NULL; //Retorna o valor Vazio

		}

		//Caso 2: No sem filho esquerdo
		if(!no->esquerda){

			No *aux = no->direita; //Guarda o filho direito
			free(no); //Libera o no

			return aux; //Retorna o filho direito para ocupar a posicao na arvore

		}

		//Caso 3: No sem filho direito
		if(!no->direita){

			No *aux = no->esquerda; //Guarda o filho esquerdo
			free(no); //Libera o no

			return aux; //Retorna o filho esquerdo para ocupar a posicao na arvore

		}

		//Caso 4: No com ambos filhos esquerdo e direito
		No *aux = menorValor(no->direita); //Busca e guarda o filho mais a esquerda na subarvore direita

		no->vertice = aux->vertice; //Substitui o valor do vertice atual pelo vertice do filho mais a esquerda encontrado

		no->direita = remover(no->direita, aux->vertice); //Remocao do filho mais a esquerda da subarvore direita

	}

	atualizarAltura(no); //Apos a remocao, atualiza-se a altura do no inicial

	int fb = fatorBalanceamento(no); //Calculo do Fator de Balanceamento para rotacao posterior

	//Verifica desbalaceamento na subarvore esquerda
	//E verifica o desbalanceamento na subarvore esquerda (Caso LL)
	if(fb > 1 && fatorBalanceamento(no->esquerda) >= 0){

		return rotacaoDireita(no); //Realiza a rotacao LL no no

	}
	
	//Verifica desbalaceamento na subarvore direita
	//E verifica o desbalanceamento na subarvore direita (Caso RR)
	if(fb < -1 && fatorBalanceamento(no->direita) <= 0){

		return rotacaoEsquerda(no); //Realiza a rotacao RR no no

	}
	
	//Verifica desbalaceamento na subarvore esquerda
	//E verifica o desbalanceamento na subarvore esquerda (Caso LR)
	if(fb > 1 && fatorBalanceamento(no->esquerda) < 0){

		return rotacaoEsquerdaDireita(no); //Realiza a rotacao LR no no

	}

	//Verifica desbalaceamento na subarvore direita
	//E verifica o desbalanceamento na subarvore direita (Caso RL)
	if(fb < -1 && fatorBalanceamento(no->direita) > 0){

		return rotacaoDireitaEsquerda(no); //Realiza a rotacao RL no no

	}

	return no; //Retorna o no balanceado

}


// Mostra os vertices da arvore em ordem crescente (esquerda, raiz, direita)
// Complexidade: O(n), onde n eh o numero de nos. A recursao visita cada no exatamente uma vez.
void mostrarEmOrdem(No *no){

	if(!no){
		return; // Caso base: subarvore vazia
	}

	mostrarEmOrdem(no->esquerda); // Recursao: visita a subarvore esquerda primeiro

	printf(" %d ", no->vertice); // Visita: mostra o vertice atual

	mostrarEmOrdem(no->direita); // Recursao: visita a subarvore direita por ultimo

}


char desenho[12][260];

// Coloca os numeros e os galhos no desenho
void desenharNo(No *no, int nivel, int coluna, int distancia){

    if(no == NULL){
        return;
    }

    char numero[12];
    sprintf(numero, "%d", no->vertice);

    int inicio = coluna - strlen(numero) / 2;

    for(int i = 0; numero[i] != '\0'; i++){
        desenho[2 * nivel][inicio + i] = numero[i];
    }

    // Desenha o filho esquerdo
    if(no->esquerda != NULL){

        desenho[2 * nivel + 1][coluna - distancia] = '/';

        for(int i = coluna - distancia + 1; i < coluna; i++){
            desenho[2 * nivel + 1][i] = '-';
        }

        desenharNo(no->esquerda, nivel + 1,
                  coluna - distancia, distancia / 2);
    }

    // Desenha o filho direito
    if(no->direita != NULL){

        for(int i = coluna + 1; i < coluna + distancia; i++){
            desenho[2 * nivel + 1][i] = '-';
        }

        desenho[2 * nivel + 1][coluna + distancia] = '\\';

        desenharNo(no->direita, nivel + 1,
                  coluna + distancia, distancia / 2);
    }

    if(no->esquerda != NULL || no->direita != NULL){
        desenho[2 * nivel + 1][coluna] = '+';
    }
}

// Mostra a arvore no terminal
void mostrarArvore(No *raiz, int nivel){

    if(raiz == NULL){
        printf("Arvore vazia!\n");
        return;
    }

    int h = altura(raiz);

    if(h > 6){
        printf("Arvore muito grande para desenhar.\n");
        return;
    }

    // Limpa o desenho
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < 259; j++){
            desenho[i][j] = ' ';
        }
        desenho[i][259] = '\0';
    }

    int distancia = 1;

    for(int i = 0; i < h; i++){
        distancia = distancia * 2;
    }

    desenharNo(raiz, 0, 2 * distancia, distancia);

    // Mostra o desenho no terminal
    for(int i = 0; i < 2 * h - 1; i++){

        int fim = 258;

        while(fim >= 0 && desenho[i][fim] == ' '){
            fim--;
        }

        desenho[i][fim + 1] = '\0';
        printf("%s\n", desenho[i]);
    }
}

// Libera toda a memoria da arvore (pos-ordem: filhos antes do pai)
// Complexidade: O(n), libera cada no uma unica vez
No* liberarArvore(No *no){

	if(!no){

		return NULL; // Caso base

	}

	liberarArvore(no->esquerda); // Libera subarvore esquerda
	liberarArvore(no->direita); // Libera subarvore direita

	free(no); // Libera o no atual

	return NULL; // Permite fazer raiz = liberarArvore(raiz)

}


// Busca um vertice na AVL. Complexidade: O(log n)
int buscar(No *no, int valor){

	if(!no){

		return 0; // Nao encontrado

	}

	if(valor < no->vertice){

		return buscar(no->esquerda, valor); // Procura a esquerda

	}

	if(valor > no->vertice){

		return buscar(no->direita, valor); // Procura a direita

	}

	return 1; // Encontrado

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

	raiz = liberarArvore(raiz); // Limpa arvore de execucao anterior

	for(int pos = 0; pos < quant_dfs; pos++){ // Transforma a saida da DFS em AVL

		raiz = inserir(raiz, visitado_ordem[pos]);

	}
	
	printf("\nArvore AVL construida: \n");
	
	mostrarArvore(raiz, 0);

	int op, valor;

	do{

		puts("\n===================================");
		puts("          MENU ARVORE AVL");
		puts("===================================");
		puts ("1 - Inserir");
		puts ("2 - Remover linha");
		puts ("3 - Buscar linha");
		puts ("4 - Mostrar arvore equilibrada");
		puts ("0 - Voltar ao menu de arquivos");
		puts ("Digite a opcao desejada: ");
		scanf("%d", &op);

		switch(op){
					
				case 1:
				    printf("Vertice a inserir: ");
				    
				    scanf("%d", &valor);
				
				    if(buscar(raiz, valor)){
				    	
				        printf("\nO vertice %d ja existe na arvore!\n", valor);
				    }
				    else{
				    	
				        raiz = inserir(raiz, valor);
				        
				        printf("\nVertice %d inserido com sucesso!\n", valor);
				    }
			    	break;


				
				case 2:
				    printf("Vertice a remover: ");
				    
				    scanf("%d", &valor);
				
				    if(buscar(raiz, valor)){
				        raiz = remover(raiz, valor);
				        
				        printf("\nVertice %d removido com sucesso!\n", valor);
				    }
				    else{
				    	
				        printf("\nVertice %d nao encontrado na arvore!\n", valor);
				    }
				    
				    break;


				case 3:
					printf("Vertice a buscar: ");
					
					scanf("%d", &valor);
					
					if(buscar(raiz, valor)){
						
						printf("Vertice %d encontrado.\n", valor);
						
					} else {
						
						printf("Vertice %d nao encontrado.\n", valor);
					}
					
					break;

				case 4:
					puts("\nArvore AVL:\n");
					if(raiz == NULL) puts("A arvore esta vazia!"); else mostrarArvore(raiz, 0);
					puts("\nEm ordem:");
					if(raiz == NULL) puts("A arvore esta vazia!"); else mostrarEmOrdem(raiz);
					puts("");
					break;
				
				case 0:
    				puts("\nVoltando ao menu de arquivos...");
   					break;
    
				default:
					puts("Insira um numero entre os listados");
					break;
		}

	}while(op != 0);

	raiz = liberarArvore(raiz); // Libera memoria ao sair

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
