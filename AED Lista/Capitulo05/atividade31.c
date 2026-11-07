#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// ESTRUTURAS E VARIÁVEIS GLOBAIS DE INSTRUMENTAÇÃO
// ============================================================================
typedef struct {
    long ops_acesso;      // O(1)
    long ops_linear;      // O(n)
    long ops_quadratica;  // O(n²)
    long ops_binaria;     // O(log n)
    long chamadas_rec;    // Medição de pilha/recursão
} Contadores;

// ============================================================================
// 1. RECURSÃO RAMIFICADA vs DIVISÃO E CONQUISTA (ETAPA ATUAL)
// ============================================================================

// Fibonacci Ingênuo (Recursão Ramificada: 2 chamadas por nível) -> O(2^n)
// NOTA: Entradas limitadas a n <= 30 para evitar travamento da CPU.
long long fibonacci_ramificado(int n, long *chamadas) {
    (*chamadas)++; // Contabiliza cada nó da árvore de chamadas

    // [CASOS-BASE]
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // [RAMIFICAÇÃO]: Cada chamada subdivide-se em DUAS novas chamadas
    return fibonacci_ramificado(n - 1, chamadas) + fibonacci_ramificado(n - 2, chamadas);
}

// Busca Binária Recursiva (Divisão e Conquista: 1 subproblema por nível) -> O(log n)
int busca_binaria_recursiva(const int *vetor, int inicio, int fim, int chave, long *chamadas) {
    (*chamadas)++; // Contabiliza o nível atual de profundidade

    // [CASO-BASE]: Intervalo esgotado (Elemento ausente)
    if (inicio > fim) {
        return -1;
    }

    int meio = inicio + (fim - inicio) / 2;

    // [CASO-BASE]: Elemento encontrado no meio
    if (vetor[meio] == chave) {
        return meio;
    }

    // [DIVISÃO E CONQUISTA]: Escolhe apenas UMA das metades para progredir
    if (vetor[meio] > chave) {
        return busca_binaria_recursiva(vetor, inicio, meio - 1, chave, chamadas);
    } else {
        return busca_binaria_recursiva(vetor, meio + 1, fim, chave, chamadas);
    }
}

// ============================================================================
// 2. MÓDULOS DE EXPERIMENTOS ANTERIORES (MANTIDOS E INTEGRADOS)
// ============================================================================

int acesso_direto(const int *vetor, int tamanho, int indice, long *ops) {
    (*ops)++;
    if (indice < 0 || indice >= tamanho) return -1;
    (*ops)++;
    return vetor[indice];
}

int buscar_maior(const int *vetor, int tamanho, long *ops) {
    if (tamanho <= 0) return -1;
    (*ops)++;
    int maior = vetor[0];
    for (int i = 1; i < tamanho; i++) {
        (*ops)++;
        if (vetor[i] > maior) maior = vetor[i];
    }
    return maior;
}

int buscar_pares_soma(const int *vetor, int tamanho, int alvo, long *ops) {
    int pares = 0;
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            (*ops)++;
            if (vetor[i] + vetor[j] == alvo) pares++;
        }
    }
    return pares;
}

int busca_binaria_iterativa(const int *vetor, int tamanho, int chave, long *ops) {
    int inicio = 0, fim = tamanho - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*ops)++;
        if (vetor[meio] == chave) return meio;
        (*ops)++;
        if (vetor[meio] < chave) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

long long soma_recursiva(const int *vetor, int tamanho, int indice, long *chamadas) {
    (*chamadas)++;
    if (indice >= tamanho || indice < 0) return 0;
    return vetor[indice] + soma_recursiva(vetor, tamanho, indice + 1, chamadas);
}

// ============================================================================
// ROTINAS DE MONTAGEM DE TESTES E MENU
// ============================================================================

void rodar_experimento_ramificacao(void) {
    printf("\n=== EXPERIMENTO: RECURSÃO RAMIFICADA vs DIVISÃO E CONQUISTA ===\n");

    // 1. Teste do Fibonacci (Ramificação Dupla O(2^n))
    int n_fib = 25; // Entrada limitada para execução rápida e segura
    long chamadas_fib = 0;
    long long res_fib = fibonacci_ramificado(n_fib, &chamadas_fib);

    printf("\n1. FIBONACCI RAMIFICADO (Entrada N = %d):\n", n_fib);
    printf("   - Resultado obtido: %lld\n", res_fib);
    printf("   - Total de chamadas a funcao: %ld\n", chamadas_fib);
    printf("   -> Observe a explosao de chamadas decorrente de duas ramificacoes por nivel.\n");

    // 2. Teste da Busca Binária Recursiva (Divisão e Conquista O(log n))
    int tamanho_bb = 1048576; // N = ~1 milhão de elementos
    int *vetor = (int *)malloc((size_t)tamanho_bb * sizeof(int));
    if (vetor == NULL) return;

    for (int i = 0; i < tamanho_bb; i++) vetor[i] = i * 2;

    int chave = 2097150; // Elemento no final do vetor
    long chamadas_bb = 0;
    int pos = busca_binaria_recursiva(vetor, 0, tamanho_bb - 1, chave, &chamadas_bb);

    printf("\n2. BUSCA BINÁRIA RECURSIVA (Vetor ordenado N = %d):\n", tamanho_bb);
    printf("   - Posicao encontrada: %d\n", pos);
    printf("   - Total de chamadas a funcao: %ld\n", chamadas_bb);
    printf("   -> Apesar do N gigantesco, descartar metade a cada passo exige pouquissimas chamadas.\n");

    free(vetor);
}

void rodar_experimentos_complexidade(void) {
    int tamanhos[] = {100, 1000, 5000};
    printf("\n=== COMPARAÇÃO GERAL DE COMPLEXIDADES ===\n");
    printf("%-8s | %-12s | %-12s | %-14s | %-12s\n", "N", "O(1)", "O(n)", "O(n²)", "O(log n)");
    printf("----------------------------------------------------------------------\n");

    for (int t = 0; t < 3; t++) {
        int N = tamanhos[t];
        int *v = (int *)malloc((size_t)N * sizeof(int));
        for (int i = 0; i < N; i++) v[i] = i * 2;

        long c1 = 0, cn = 0, cn2 = 0, clog = 0;
        acesso_direto(v, N, N / 2, &c1);
        buscar_maior(v, N, &cn);
        buscar_pares_soma(v, N, 450, &cn2);
        busca_binaria_iterativa(v, N, (N - 2) * 2, &clog);

        printf("%-8d | %-12ld | %-12ld | %-14ld | %-12ld\n", N, c1, cn, cn2, clog);
        free(v);
    }
}

int main(void) {
    int opcao = 0;

    do {
        printf("\n========================================================\n");
        printf("         LABORATÓRIO COMPLETO DE ALGORITMOS            \n");
        printf("========================================================\n");
        printf("1. Comparar Complexidades Básicas [O(1), O(n), O(n²), O(log n)]\n");
        printf("2. Teste Busca Linear vs Busca Binária e Pré-condições\n");
        printf("3. Teste de Recursão Simples (Fatorial e Soma de Vetor)\n");
        printf("4. Teste de Recursão Ramificada (Fibonacci) vs Divisão e Conquista\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                rodar_experimentos_complexidade();
                break;
            case 2: {
                int N = 10000;
                int *v = (int *)malloc((size_t)N * sizeof(int));
                for (int i = 0; i < N; i++) v[i] = i * 2;
                long lin = 0, bin = 0;
                busca_binaria_iterativa(v, N, (N - 1) * 2, &bin);
                for (int i = 0; i < N; i++) { lin++; if (v[i] == (N - 1) * 2) break; }
                printf("\nBusca no fim de N=%d -> Linear: %ld ops | Binaria: %ld ops\n", N, lin, bin);
                free(v);
                break;
            }
            case 3: {
                long c_rec = 0;
                int v[] = {1, 2, 3, 4, 5};
                long long s = soma_recursiva(v, 5, 0, &c_rec);
                printf("\nSoma de vetor N=5 -> Resultado: %lld | Chamadas empilhadas: %ld\n", s, c_rec);
                break;
            }
            case 4:
                rodar_experimento_ramificacao();
                break;
            case 0:
                printf("\nEncerrando o laboratório...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}