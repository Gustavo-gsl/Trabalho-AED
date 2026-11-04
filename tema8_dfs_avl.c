#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 101

char nome_arquivo[50], aux[MAX];								// proibido alterar esta linha
int opcao, tamanho, i, j, n, m, digrafo, matriz_adj[MAX][MAX];  // proibido alterar esta linha
// Declare variáveis globais a partir daqui.

typedef struct no {
    int linha;
    int altura;
    struct no *esquerda, *direita;
} no;


int converter_inteiro(char *texto, int *valor) {
    char *fim;
    long long numero = strtoll(texto, &fim, 10);

    if (fim == texto) {
        return 0;
    }
    while (*fim == ' ' || *fim == '\t' || *fim == '\r' || *fim == '\n') {
        fim++;
    }
    if (*fim != '\0' || numero < -2147483648LL || numero > 2147483647LL) {
        return 0;
    }
    *valor = (int)numero;
    return 1;
}

int arquivo_valido(const char *nome) {
    char caminho[50], texto[102];
    int linhas = 0, colunas = 0;
    FILE *entrada;
    snprintf(caminho, sizeof caminho, "%s.txt", nome);
    entrada = fopen(caminho, "r");
    if (entrada == NULL) {
        puts("Arquivo nao encontrado.");
        return 0;
    }
    while (fgets(texto, sizeof texto, entrada)) {
        char *elemento;
        int quantidade = 0;
        if (strlen(texto) > 100 || ++linhas > 100) {
            puts("Limite do leitor original: 100 vertices e 100 caracteres por linha (incluindo a quebra).");
            fclose(entrada);
            return 0;
        }
        elemento = strtok(texto, " \t\r\n");
        while (elemento != NULL) {
            int valor;
            if (converter_inteiro(elemento, &valor) == 0) {
                puts("Use apenas inteiros separados por espacos.");
                fclose(entrada);
                return 0;
            }
            quantidade++;
            elemento = strtok(NULL, " \t\r\n");
        }
        if (linhas == 1) colunas = quantidade;
        if (!quantidade || quantidade != colunas) {
            puts("Todas as linhas devem ter a mesma quantidade de inteiros, sem linhas vazias.");
            fclose(entrada);
            return 0;
        }
    }
    if (ferror(entrada)) {
        fclose(entrada);
        puts("Erro de leitura.");
        return 0;
    }
    fclose(entrada);
    if (!linhas || linhas != colunas) {
        puts("A matriz precisa ser quadrada, com 1 a 100 vertices.");
        return 0;
    }
    return 1;
}

char *gets(char *destino) {
    char texto[256];
    for (;;) {
        int comprimento;
        int c;
        if (!fgets(texto, sizeof texto, stdin)) exit(0);
        comprimento = (int)strcspn(texto, "\r\n");
        if (!strchr(texto, '\n') && !feof(stdin)) {
            while ((c = getchar()) != '\n' && c != EOF) {}
            puts("Nome muito comprido. Digite novamente, sem .txt:");
            continue;
        }
        texto[comprimento] = '\0';
        if (!comprimento) continue;
        if (comprimento > 45) {
            puts("Use um nome de ate 45 caracteres, sem .txt:");
            continue;
        }
        if (!arquivo_valido(texto)) {
            puts("Digite outro nome, sem .txt:");
            continue;
        }
        strcpy(destino, texto);
        return destino;
    }
}

/* Alturas guardadas nos nos tornam cada atualizacao e rotacao O(1). */
int altura_no(const no *atual) {
    if (atual == NULL) {
        return 0;
    }
    return atual->altura;
}

void atualizar(no *atual) {
    int e = altura_no(atual->esquerda), d = altura_no(atual->direita);
    if (e > d) {
        atual->altura = 1 + e;
    } else {
        atual->altura = 1 + d;
    }
}

int fator(const no *atual) {
    return altura_no(atual->esquerda) - altura_no(atual->direita);
}

no *girar_direita(no *atual) {
    no *nova = atual->esquerda;
    atual->esquerda = nova->direita;
    nova->direita = atual;
    atualizar(atual);
    atualizar(nova);
    return nova;
}

no *girar_esquerda(no *atual) {
    no *nova = atual->direita;
    atual->direita = nova->esquerda;
    nova->esquerda = atual;
    atualizar(atual);
    atualizar(nova);
    return nova;
}

/* Trata LL, LR, RR e RL tanto na insercao quanto na remocao. O(1). */
no *equilibrar(no *atual) {
    if (atual == NULL) {
        return NULL;
    }
    atualizar(atual);
    if (fator(atual) > 1) {
        if (fator(atual->esquerda) < 0)
            atual->esquerda = girar_esquerda(atual->esquerda);
        return girar_direita(atual);
    }
    if (fator(atual) < -1) {
        if (fator(atual->direita) > 0)
            atual->direita = girar_direita(atual->direita);
        return girar_esquerda(atual);
    }
    return atual;
}

/* Desce por um unico caminho da AVL: O(log k), pilha O(log k).
 * Chaves repetidas sao ignoradas. k e o numero atual de nos.
 */
no *inserir_linha(no *atual, int linha) {
    if (atual == NULL) {
        no *novo = malloc(sizeof *novo);
        if (novo == NULL) {
            fputs("Memoria insuficiente.\n", stderr);
            exit(EXIT_FAILURE);
        }
        novo->linha = linha;
        novo->altura = 1;
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (linha < atual->linha)
        atual->esquerda = inserir_linha(atual->esquerda, linha);
    else if (linha > atual->linha)
        atual->direita = inserir_linha(atual->direita, linha);
    else {
        return atual;
    }
    return equilibrar(atual);
}

no *remover_linha(no *atual, int linha) {
    if (atual == NULL) {
        return NULL;
    }
    if (linha < atual->linha)
        atual->esquerda = remover_linha(atual->esquerda, linha);
    else if (linha > atual->linha)
        atual->direita = remover_linha(atual->direita, linha);
    else {
        if (atual->esquerda == NULL || atual->direita == NULL) {
            no *filho;
            if (atual->esquerda != NULL) {
                filho = atual->esquerda;
            } else {
                filho = atual->direita;
            }
            free(atual);
            return filho;
        }
        no *sucessor = atual->direita;
        while (sucessor->esquerda) sucessor = sucessor->esquerda;
        atual->linha = sucessor->linha;
        atual->direita = remover_linha(atual->direita, sucessor->linha);
    }
    return equilibrar(atual);
}

/* Busca iterativa: O(log k) tempo e O(1) espaco auxiliar. */
no *buscar_linha(no *atual, int linha) {
    while (atual && atual->linha != linha)
        if (linha < atual->linha) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    return atual;
}


char desenho[40][1200];

/* Soma o espaco necessario aos numeros, incluindo sinal e separacao. O(k). */
int largura_desenho(const no *atual) {
    char numero[12];
    if (atual == NULL) {
        return 0;
    }
    sprintf(numero, "%d", atual->linha);
    /* Basta saber se ultrapassa a largura da tela. */
    int largura = largura_desenho(atual->esquerda)
                + (int)strlen(numero) + 3
                + largura_desenho(atual->direita);
    if (largura > 1200) {
        return 1200;
    }
    return largura;
}


int desenhar_no(const no *atual, int nivel, int *coluna) {
    char numero[12];
    int esquerda = 0, direita = 0, centro;

    if (atual->esquerda != NULL) {
        esquerda = desenhar_no(atual->esquerda, nivel + 1, coluna);
    }
    sprintf(numero, "%d", atual->linha);
    centro = *coluna + (int)strlen(numero) / 2;
    for (int c = 0; numero[c] != '\0'; c++) {
        desenho[2 * nivel][*coluna + c] = numero[c];
    }
    *coluna = *coluna + (int)strlen(numero) + 3;
    if (atual->direita != NULL) {
        direita = desenhar_no(atual->direita, nivel + 1, coluna);
    }
    if (atual->esquerda != NULL || atual->direita != NULL) {
        desenho[2 * nivel + 1][centro] = '+';
        if (atual->esquerda != NULL) {
            desenho[2 * nivel + 1][esquerda] = '/';
            for (int c = esquerda + 1; c < centro; c++) {
                desenho[2 * nivel + 1][c] = '-';
            }
        }
        if (atual->direita != NULL) {
            for (int c = centro + 1; c < direita; c++) {
                desenho[2 * nivel + 1][c] = '-';
            }
            desenho[2 * nivel + 1][direita] = '\\';
        }
    }
    return centro;
}


void mostrar_arvore(const no *atual, int nivel) {
    int largura, linhas, coluna = 0;
    if (atual == NULL) {
        puts("Arvore vazia.");
        return;
    }
    largura = largura_desenho(atual);
    linhas = 2 * atual->altura - 1;
    if (largura >= 1200 || linhas > 40) {
        puts("A arvore ultrapassou o tamanho reservado para o desenho.");
        return;
    }
    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < largura; c++) {
            desenho[l][c] = ' ';
        }
    }
    desenhar_no(atual, nivel, &coluna);
    for (int l = 0; l < linhas; l++) {
        int fim = largura;
        while (fim > 0 && desenho[l][fim - 1] == ' ') {
            fim--;
        }
        desenho[l][fim] = '\0';
        puts(desenho[l]);
    }
}

void liberar_arvore(no *atual) {
    if (atual == NULL) {
        return;
    }
    liberar_arvore(atual->esquerda);
    liberar_arvore(atual->direita);
    free(atual);
}


void profundidade(int linha, int visitado[], no **raiz) {
    visitado[linha] = 1;
    printf("%d ", linha);
    *raiz = inserir_linha(*raiz, linha);
    for (int coluna = 1; coluna <= n; ++coluna)
        if (matriz_adj[linha][coluna] != 0 && !visitado[coluna])
            profundidade(coluna, visitado, raiz);
}


int ler_inteiro(const char *mensagem, int *valor) {
    char texto[128];
    int c;
    while (1) {
        printf("%s", mensagem);
        if (fgets(texto, sizeof texto, stdin) == NULL) {
            return 0;
        }
        if (strchr(texto, '\n') == NULL && feof(stdin) == 0) {
            while ((c = getchar()) != '\n' && c != EOF) {}
            puts("Entrada muito longa.");
        } else if (converter_inteiro(texto, valor) == 1) {
            return 1;
        } else {
            puts("Digite um numero inteiro valido.");
        }
    }
}

void executafuncao (){   										// É dentro desta função que o aluno deverá escrever seu código
    no *raiz = NULL;
    int visitado[MAX] = {0};
    int escolha, linha;

    FILE *conferencia = fopen(nome_arquivo, "r");
    if (!conferencia) return;
    fclose(conferencia);
    if (n < 1 || n >= MAX) return;

    printf("\nOrdem da busca em profundidade: ");
   
    for (int linha_inicial = 1; linha_inicial <= n; ++linha_inicial)
        if (!visitado[linha_inicial])
            profundidade(linha_inicial, visitado, &raiz);
    puts("\n\nArvore AVL equilibrada:");
    mostrar_arvore(raiz, 0);

    for (;;) {
        puts("\n----- Menu AVL -----\n1 - Inserir linha\n2 - Remover linha\n3 - Buscar linha\n4 - Mostrar arvore equilibrada\n0 - Voltar ao menu de arquivos");
        if (!ler_inteiro("Escolha: ", &escolha)) {
            liberar_arvore(raiz);
            exit(0);
        }
        if (escolha == 0) break;
        if (escolha == 4) {
            if (raiz) mostrar_arvore(raiz, 0);
            else puts("Arvore vazia.");
            continue;
        }
        if (escolha < 1 || escolha > 3) {
            puts("Opcao invalida.");
            continue;
        }
        if (!ler_inteiro("Digite a linha: ", &linha)) {
            liberar_arvore(raiz);
            exit(0);
        }
        no *encontrado = buscar_linha(raiz, linha);
        if (escolha == 1) {
            if (encontrado) puts("Linha ja existe.");
            else {
                raiz = inserir_linha(raiz, linha);
                puts("Linha inserida; AVL equilibrada.");
            }
        } else if (escolha == 2) {
            if (!encontrado) puts("Linha nao encontrada.");
            else {
                raiz = remover_linha(raiz, linha);
                puts("Linha removida; AVL equilibrada.");
            }
        } else {
            if (encontrado != NULL) {
                puts("Linha encontrada.");
            } else {
                puts("Linha nao encontrada.");
            }
        }
    }
    liberar_arvore(raiz);

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

