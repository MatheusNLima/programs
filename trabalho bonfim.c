#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_MAXIMO_PILHA 100
#define NUM_MAX_TOKENS 50

typedef struct {
    char *pilha[TAMANHO_MAXIMO_PILHA];
    int topo;
} Pilha;

typedef struct {
    char *tokens[NUM_MAX_TOKENS];
    int numTokens;
} Tokens;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

void empilhar(Pilha *p, char *c) {
    if (p->topo < TAMANHO_MAXIMO_PILHA - 1) {
        p->topo++;
        p->pilha[p->topo] = c;
    } else {
        printf("Estouro de Pilha\n");
        exit(1);
    }
}

char *desempilhar(Pilha *p) {
    if (p->topo >= 0) {
        char *c = p->pilha[p->topo];
        p->topo--;
        return c;
    } else {
        printf("Subdesbordamento de Pilha\n");
        exit(1);
    }
}

void imprimirPilha(Pilha *p) {
    int i;
    for (i = p->topo; i >= 0; i--) {
        printf("%s", p->pilha[i]);
    }
}

void tratarErro() {
    printf("Erro: Regra inválida\n");
    exit(1);
}

void dividirEmTokens(char *entrada, Tokens *tokens) {
    char *token = strtok(entrada, " ");
    while (token != NULL) {
        tokens->tokens[tokens->numTokens] = token;
        tokens->numTokens++;
        token = strtok(NULL, " ");
    }
}

int main() {
    Pilha pilha;
    Tokens tokens;
    char entrada[50];
    int i;

    // Inicializar pilha
    inicializarPilha(&pilha);

    // Inicializar tokens
    tokens.numTokens = 0;

    // Obter entrada do usuário
    printf("Digite a entrada: ");
    fgets(entrada, 50, stdin);

    // Remover caractere de nova linha da entrada
    entrada[strcspn(entrada, "\n")] = '\0';

    // Dividir entrada em tokens
    dividirEmTokens(entrada, &tokens);

    // Empilhar símbolo inicial da pilha
    empilhar(&pilha, "$");
    empilhar(&pilha, "S");

    printf("Analisando a entrada '%s':\n", entrada);
    
    // Imprimir cabeçalho da tabela
    printf("| %-20s | %-20s | %-20s |\n", "Pilha", "Cadeia", "Regra");

    // Analisar entrada
    for (i = 0; i < tokens.numTokens; i++) {
        char *simbolo = tokens.tokens[i];

        char *topoPilha = desempilhar(&pilha);

        if (strcmp(simbolo, topoPilha) == 0) {
            printf("| ");
            imprimirPilha(&pilha);
            printf("%-20s | %-20s | %-20s |\n", simbolo, "", "");
        } else {
            if (strcmp(topoPilha, "S") == 0) {
                if (strcmp(simbolo, "if") == 0) {
                    empilhar(&pilha, "C");
                    empilhar(&pilha, "then");
                    empilhar(&pilha, "E");
                    empilhar(&pilha, "if");
                    printf("| ");
                    imprimirPilha(&pilha);
                    printf("%-20s | %-20s | %-20s |\n", simbolo, "", "S -> if E then C");
                } else if (strcmp(simbolo, "b") == 0) {
                    empilhar(&pilha, "C");
                    printf("| ");
                    imprimirPilha(&pilha);
                    printf("%-20s | %-20s | %-20s |\n", simbolo, "", "S -> C");
                } else {
                    tratarErro();
                }
            } else if (strcmp(topoPilha, "E") == 0) {
                if (strcmp(simbolo, "a") == 0) {
                    printf("| ");
                    imprimirPilha(&pilha);
                    printf("%-20s | %-20s | %-20s |\n", simbolo, "", "E -> a");
                } else {
                    tratarErro();
                }
            } else if (strcmp(topoPilha, "C") == 0) {
                if (strcmp(simbolo, "b") == 0) {
                    printf("| ");
                    imprimirPilha(&pilha);
                    printf("%-20s | %-20s | %-20s |\n", simbolo, "", "C -> b");
                } else {
                    tratarErro();
                }
            } else {
                tratarErro();
            }
        }
    }

    printf("\nAnálise bem-sucedida\n");

    return 0;
}
