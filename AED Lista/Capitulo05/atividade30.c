#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 1. RECURSÃO NUMÉRICA SIMPLES: FATORIAL (N!)
// ============================================================================

// Versão Recursiva
long long fatorial_recursivo(int n, int profundidade, long *chamadas) {
    (*chamadas)++; // Contabiliza cada invocação da função

    // [CASO-BASE ALCANÇÁVEL]: Se n <= 1, interrompe a pilha de chamadas
    if (n <= 1) {
        return 1;
    }

    // [PROGRESSO]: Reduz o problema chamando (n - 1)
    return n * fatorial_recursivo(n - 1, profundidade + 1, chamadas);
}

// Versão Iterativa Equivalente
long long fatorial_iterativo(int n, long *op_contadas) {
    long long resultado = 1;
    for (int i = 2; i <= n; i++) {
        (*op_contadas)++; // Contabiliza a iteração do laço
        resultado *= i;
    }
    return resultado;
}

// ============================================================================
// 2. RECURSÃO SOBRE VETORES: SOMA DOS ELEMENTOS
// ============================================================================

// Versão Recursiva
long long soma_vetor_recursiva(const int *vetor, int tamanho, int indice, long *chamadas) {
    (*chamadas)++; // Contabiliza cada invocação da função

    // [CASO-BASE ALCANÇÁVEL]: Quando o índice atinge ou ultrapassa o tamanho do vetor
    if (indice >= tamanho || indice < 0) {
        return 0;
    }

    // [PROGRESSO]: Processa o elemento atual (indice) e avança para (indice + 1)
    return vetor[indice] + soma_vetor_recursiva(vetor, tamanho, indice + 1, chamadas);
}

// Versão Iterativa Equivalente
long long soma_vetor_iterativa(const int *vetor, int tamanho, long *op_contadas) {
    long long soma = 0;
    for (int i = 0; i < tamanho; i++) {
        (*op_contadas)++; // Contabiliza o acesso/iteração
        soma += vetor[i];
    }
    return soma;
}

// ============================================================================
// FUNÇÃO AUXILIAR DE TESTES
// ============================================================================
void preencher_vetor(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = i + 1; // Ex: [1, 2, 3, 4, 5, ...]
    }
}

int main(void) {
    printf("=== LABORATÓRIO DE RECURSÃO: CASO-BASE, PROGRESSO E CUSTO ===\n\n");

    // ------------------------------------------------------------------------
    // TESTE 1: FATORIAL (Operação Numérica)
    // ------------------------------------------------------------------------
    int n_fatorial = 10;
    long chamadas_fat_rec = 0;
    long ops_fat_iter = 0;

    long long res_fat_rec = fatorial_recursivo(n_fatorial, 1, &chamadas_fat_rec);
    long long res_fat_iter = fatorial_iterativo(n_fatorial, &ops_fat_iter);

    printf("1. TESTE NUMÉRICO: FATORIAL DE %d!\n", n_fatorial);
    printf("----------------------------------------------------------------------\n");
    printf("  [Recursivo] Resultado: %-10lld | Chamadas empilhadas: %ld\n", res_fat_rec, chamadas_fat_rec);
    printf("  [Iterativo] Resultado: %-10lld | Iteracoes de laço:   %ld\n", res_fat_iter, ops_fat_iter);
    printf("  Conferencia: %s\n\n", (res_fat_rec == res_fat_iter) ? "RESULTADOS IDENTICOS" : "ERRO");

    // ------------------------------------------------------------------------
    // TESTE 2: SOMA DE VETOR (Processamento de Estrutura)
    // ------------------------------------------------------------------------
    int tamanhos[] = {10, 100, 1000};
    int num_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("2. TESTE DE PROCESSAMENTO EM VETOR: SOMA RECURSIVA VS ITERATIVA\n");
    printf("======================================================================\n");
    printf("%-12s | %-15s | %-18s | %-18s\n", "Tamanho (N)", "Resultado", "Chamadas Rec. (Stack)", "Ops. Iterativas");
    printf("----------------------------------------------------------------------\n");

    for (int t = 0; t < num_testes; t++) {
        int N = tamanhos[t];
        int *vetor = (int *)malloc((size_t)N * sizeof(int));
        if (vetor == NULL) continue;

        preencher_vetor(vetor, N);

        long chamadas_soma_rec = 0;
        long ops_soma_iter = 0;

        // Inicia a recursão do índice 0 até N - 1
        long long res_soma_rec = soma_vetor_recursiva(vetor, N, 0, &chamadas_soma_rec);
        long long res_soma_iter = soma_vetor_iterativa(vetor, N, &ops_soma_iter);

        printf("%-12d | %-15lld | %-18ld | %-18ld\n", 
               N, res_soma_rec, chamadas_soma_rec, ops_soma_iter);

        free(vetor);
    }

    printf("\n======================================================================\n");
    printf("=== ANÁLISE COMPARATIVA: RECURSÃO VS ITERAÇÃO ===\n");
    printf("======================================================================\n");
    printf("1. CASO-BASE E PROGRESSO:\n");
    printf("   - Sem um caso-base bem definido (ex: 'n <= 1' ou 'indice >= tamanho'),\n");
    printf("     a função recursiva entra em loop infinito e estoura a pilha (Stack Overflow).\n");
    printf("   - O progresso é garantido aproximando os argumentos do caso-base a cada chamada (n-1 ou i+1).\n\n");
    printf("2. CUSTO DE MEMÓRIA E EXECUÇÃO:\n");
    printf("   - Abordagem Iterativa: Complexidade de Espaço O(1). Usa um unico frame de execucao.\n");
    printf("   - Abordagem Recursiva: Complexidade de Espaço O(n). Cada chamada aloca um novo\n");
    printf("     frame na pilha de memoria da JVM/C contendo parametros e enderecos de retorno.\n");

    return 0;
}