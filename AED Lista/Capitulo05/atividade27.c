#include <stdio.h>
#include <stdlib.h>

// Função instrumentada para Acesso Direto - Complexidade O(1)
int acesso_direto(const int *vetor, int tamanho, int indice, long *op_contadas) {
    (*op_contadas)++;
    if (indice < 0 || indice >= tamanho) return -1;

    (*op_contadas)++;
    return vetor[indice];
}

// Função instrumentada para Busca do Maior - Complexidade O(n)
int buscar_maior(const int *vetor, int tamanho, long *op_contadas) {
    if (tamanho <= 0) return -1;

    (*op_contadas)++;
    int maior = vetor[0];

    for (int i = 1; i < tamanho; i++) {
        (*op_contadas)++; // Comparação
        if (vetor[i] > maior) {
            maior = vetor[i];
        }
    }
    return maior;
}

// Nova Função: Busca de Pares com Soma Alvo - Complexidade O(n²)
// Para evitar duplicatas e auto-combinação (i == j), o laço interno começa em 'i + 1'
int buscar_pares_soma(const int *vetor, int tamanho, int alvo, long *op_contadas) {
    int pares_encontrados = 0;

    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            (*op_contadas)++; // Contabiliza a comparação no núcleo do algoritmo
            if (vetor[i] + vetor[j] == alvo) {
                pares_encontrados++;
            }
        }
    }

    return pares_encontrados;
}

void preencher_vetor(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = (i * 37) % 500; // Gera valores controlados para gerar algumas somas
    }
}

void executar_teste(int tamanho, int alvo) {
    int *vetor = (int *)malloc((size_t)tamanho * sizeof(int));
    if (vetor == NULL) {
        printf("[ERRO] Falha de alocacao para N = %d.\n", tamanho);
        return;
    }

    preencher_vetor(vetor, tamanho);

    long ops_acesso = 0;
    long ops_busca = 0;
    long ops_pares = 0;

    acesso_direto(vetor, tamanho, tamanho / 2, &ops_acesso);
    buscar_maior(vetor, tamanho, &ops_busca);
    int total_pares = buscar_pares_soma(vetor, tamanho, alvo, &ops_pares);

    printf("----------------------------------------------------------------------\n");
    printf("TAMANHO DO VETOR (N): %d | Alvo: %d\n", tamanho, alvo);
    printf("  [Acesso Direto - O(1)]   Ops: %-12ld | (Constante)\n", ops_acesso);
    printf("  [Busca Maior   - O(n)]   Ops: %-12ld | (Linear)\n", ops_busca);
    printf("  [Busca Pares   - O(n²)]  Ops: %-12ld | (Pares achados: %d)\n", ops_pares, total_pares);

    free(vetor);
}

int main(void) {
    // Testamos tamanhos menores devido ao crescimento rápido de N²
    int tamanhos[] = {100, 1000, 5000};
    int total_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);
    int alvo_teste = 450;

    printf("=== LABORATÓRIO DE COMPLEXIDADE - ANÁLISE QUADRÁTICA O(n²) ===\n\n");

    for (int i = 0; i < total_testes; i++) {
        executar_teste(tamanhos[i], alvo_teste);
    }

    printf("\n=== ANÁLISE COMPARATIVA DE CRESCIMENTO ===\n");
    printf("1. O(1):   Aumentar N nao altera o numero de operacoes (sempre 2).\n");
    printf("2. O(n):   Multiplicar N por 10 multiplica as operacoes por ~10.\n");
    printf("3. O(n²):  Multiplicar N por 10 multiplica as operacoes por ~100!\n");
    printf("   Formula exata de comparacoes da busca de pares: n * (n - 1) / 2\n");

    return 0;
}