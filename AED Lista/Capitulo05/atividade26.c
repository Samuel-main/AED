#include <stdio.h>
#include <stdlib.h>

// Função instrumentada para Acesso Direto - Complexidade O(1)
int acesso_direto(const int *vetor, int tamanho, int indice, long *op_contadas) {
    (*op_contadas)++; // Contabiliza a validação dos limites
    if (indice < 0 || indice >= tamanho) {
        printf("[ERRO] Indice %d fora dos limites (0 a %d).\n", indice, tamanho - 1);
        return -1;
    }

    (*op_contadas)++; // Contabiliza o acesso direto à memória
    return vetor[indice];
}

// Função instrumentada para Busca do Maior Elemento - Complexidade O(n)
int buscar_maior(const int *vetor, int tamanho, long *op_contadas) {
    if (tamanho <= 0) return -1;

    (*op_contadas)++; // Contabiliza a atribuição inicial
    int maior = vetor[0];

    // Percorre todos os N elementos
    for (int i = 1; i < tamanho; i++) {
        (*op_contadas)++; // Contabiliza a comparação a cada iteração
        if (vetor[i] > maior) {
            maior = vetor[i];
            (*op_contadas)++; // Contabiliza a atualização do maior valor
        }
    }

    return maior;
}

// Função auxiliar para preencher o vetor com dados de teste
void preencher_vetor(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = (i * 37) % 1000; // Gera números pseudo-aleatórios
    }
}

void executar_teste(int tamanho) {
    int *vetor = (int *)malloc((size_t)tamanho * sizeof(int));
    if (vetor == NULL) {
        printf("[ERRO] Falha de alocacao de memoria para tamanho %d.\n", tamanho);
        return;
    }

    preencher_vetor(vetor, tamanho);

    long ops_acesso = 0;
    long ops_busca = 0;

    // 1. Executa Acesso Direto (índice central)
    int valor_acesso = acesso_direto(vetor, tamanho, tamanho / 2, &ops_acesso);

    // 2. Executa Busca do Maior Elemento
    int maior_valor = buscar_maior(vetor, tamanho, &ops_busca);

    // Exibição dos resultados e contadores
    printf("--------------------------------------------------\n");
    printf("TAMANHO DO VETOR (N): %d\n", tamanho);
    printf("  [Acesso Direto] Valor lido: %d | Operacoes: %ld\n", valor_acesso, ops_acesso);
    printf("  [Busca Maior]   Maior valor: %d | Operacoes: %ld\n", maior_valor, ops_busca);

    free(vetor);
}

int main(void) {
    // Três tamanhos de entrada distintos para comparação de escala
    int tamanhos[] = {100, 10000, 1000000};
    int total_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("=== LABORATÓRIO DE COMPLEXIDADE DE ALGORITMOS ===\n\n");

    for (int i = 0; i < total_testes; i++) {
        executar_teste(tamanhos[i]);
    }

    printf("\n=== ANÁLISE DOS RESULTADOS ===\n");
    printf("1. O(1) - Tempo Constante (Acesso Direto):\n");
    printf("   O numero de operacoes permaneceu FIXO (2 ops) independentemente do aumento de N.\n\n");
    printf("2. O(n) - Tempo Linear (Busca do Maior):\n");
    printf("   O numero de operacoes cresceu PROPORCIONALMENTE ao tamanho N da entrada.\n");

    return 0;
}