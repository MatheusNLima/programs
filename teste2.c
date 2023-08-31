#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definição da estrutura Token
typedef struct {
    char lexema[100];
    int linha;
} Token;

// Definição da estrutura Simbolo
typedef struct {
    char lexema[100];
    bool declarado;
} Simbolo;

// Variáveis globais
Token *tokens;
int numTokens = 0;
int posToken = 0;
Simbolo *tabelaSimbolos;
int numSimbolos = 0;

// Protótipos das funções
void adicionarToken(const char *lexema, int linha);
void imprimirTokens();
Token obterProximoToken();
void erroLexico(const char *lexema, int linha);
void erroSintatico(const char *mensagem);
bool expressao();
bool expressao2();
void imprimirArvoreSintatica(int nivel);
bool termo();
bool termo2();
bool fator();
void adicionarSimbolo(const char *lexema);
void imprimirTabelaSimbolos();
void iniciarAnalise();
void finalizarAnalise();
void processarCodigo(char *codigo);

void processarCodigo(char *codigo) {
    numTokens = 0;
    posToken = 0;

    char *token = strtok(codigo, " ");

    while (token != NULL) {
        adicionarToken(token, numTokens + 1);
        token = strtok(NULL, " ");
    }

    imprimirTokens();

    // Reiniciar a análise para cada código inserido
    posToken = 0;
    bool resultado = expressao();

    if (resultado) {
        printf("Análise sintática concluída. Código válido.\n");
    }

    imprimirArvoreSintatica(0);
    imprimirTabelaSimbolos(); 
}



int main() {
    iniciarAnalise();

    while (1) {
        char codigo[100];

        printf("Digite um código (ou digite 'fim' para encerrar):\n");
        fgets(codigo, sizeof(codigo), stdin);
        codigo[strlen(codigo) - 1] = '\0';

        if (strcmp(codigo, "fim") == 0) {
            break;
        }

        processarCodigo(codigo);
    }

    finalizarAnalise();

    return 0;
}

void adicionarToken(const char *lexema, int linha) {
    tokens = realloc(tokens, sizeof(Token) * (numTokens + 1));
    Token token;
    strcpy(token.lexema, lexema);
    token.linha = linha;
    tokens[numTokens++] = token;
}

void imprimirTokens() {
    printf("\nTokens encontrados:\n");
    printf("-----------------\n");

    for (int i = 0; i < numTokens; i++) {
        printf("Token: %s, Linha: %d\n", tokens[i].lexema, tokens[i].linha);
    }

    printf("-----------------\n");
}

Token obterProximoToken() {
    if (posToken < numTokens) {
        return tokens[posToken++];
    }

    Token token;
    strcpy(token.lexema, "");
    token.linha = -1;
    return token;
}

void erroLexico(const char *lexema, int linha) {
    printf("Erro léxico: Lexema inválido '%s' na linha %d\n", lexema, linha);
}

void erroSintatico(const char *mensagem) {
    printf("Erro sintático: %s\n", mensagem);
}

bool expressao() {
    Token token = obterProximoToken();

    if (strcmp(token.lexema, "(") == 0) {
        if (termo()) {
            return expressao2();
        }

        erroSintatico("Esperado '(' ou número");
        return false;
    } else {
        // Verificar se é um número
        char *p;
        strtol(token.lexema, &p, 10);

        if (*p == '\0') {
            return true;
        }

        // Verificar se é um identificador
        adicionarSimbolo(token.lexema);

        return true;
    }
}


bool expressao2() {
    Token token = obterProximoToken();

    if (strcmp(token.lexema, "+") == 0 || strcmp(token.lexema, "-") == 0) {
        if (termo()) {
            return expressao2();
        }

        erroSintatico("Esperado '(' ou número");
        return false;
    }

    // Voltar o token para a análise de outros não terminais
    posToken--;

    return true;
}

void imprimirArvoreSintatica(int nivel) {
    printf("\nÁrvore sintática:\n");
    printf("-----------------\n");
    printf("expressao\n");

    for (int i = 0; i < nivel; i++) {
        printf("+- ");
    }

    printf("termo\n");

    for (int i = 0; i < nivel + 1; i++) {
        printf("|  ");
    }

    printf("+- fator\n");

    printf("-----------------\n");
}


bool termo() {
    if (fator()) {
        return termo2();
    }

    erroSintatico("Esperado '(' ou número");
    return false;
}

bool termo2() {
    Token token = obterProximoToken();

    if (strcmp(token.lexema, "*") == 0 || strcmp(token.lexema, "/") == 0) {
        if (fator()) {
            return termo2();
        }

        erroSintatico("Esperado '(' ou número");
        return false;
    }

    // Voltar o token para a análise de outros não terminais
    posToken--;

    return true;
}

bool fator() {
    Token token = obterProximoToken();

    if (strcmp(token.lexema, "(") == 0) {
        bool resultado = expressao();

        if (!resultado) {
            erroSintatico("Esperado ')' ou número");
            return false;
        }

        token = obterProximoToken();

        if (strcmp(token.lexema, ")") != 0) {
            erroSintatico("Esperado ')'");
            return false;
        }

        return true;
    } else {
        // Verificar se é um número
        char *p;
        strtol(token.lexema, &p, 10);

        if (*p == '\0') {
            return true;
        }

        // Verificar se é um identificador
        adicionarSimbolo(token.lexema);  // Adicionar identificador à tabela de símbolos

        return true;
    }
}


void imprimirTabelaSimbolos() {
    printf("\nTabela de Símbolos:\n");
    printf("------------------\n");

    for (int i = 0; i < numSimbolos; i++) {
        printf("|--- %s\n", tabelaSimbolos[i].lexema);

        if (!tabelaSimbolos[i].declarado) {
            printf("Erro semântico: Identificador '%s' não declarado\n", tabelaSimbolos[i].lexema);
        }
    }

    printf("------------------\n");
}


void adicionarSimbolo(const char *lexema) {
    for (int i = 0; i < numSimbolos; i++) {
        if (strcmp(tabelaSimbolos[i].lexema, lexema) == 0) {
            tabelaSimbolos[i].declarado = true;  // Marcar o identificador como declarado
            return;
        }
    }

    tabelaSimbolos = realloc(tabelaSimbolos, sizeof(Simbolo) * (numSimbolos + 1));
    Simbolo simbolo;
    strcpy(simbolo.lexema, lexema);
    simbolo.declarado = true;  // Marcar o identificador como declarado
    tabelaSimbolos[numSimbolos++] = simbolo;
}

void iniciarAnalise() {
    tokens = NULL;
    numTokens = 0;
    posToken = 0;

    tabelaSimbolos = NULL;
    numSimbolos = 0;
}

void finalizarAnalise() {
    free(tokens);
    free(tabelaSimbolos);
}
