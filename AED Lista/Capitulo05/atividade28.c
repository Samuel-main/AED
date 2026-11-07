#include <stdio.h>
#include <stdlib.h>

// Função instrumentada para Acesso Direto - Complexidade O(1)
int acesso_direto(const int *vetor, int tamanho, int indice, long *op_contadas) {
    (*op_contadas)++;
    if (indice < 0 || indice >= tamanho) return -1;
    (*op_contadas)++;
    return vetor[indice];
}

// Função instrumentada para Busca Linear (Maior Valor) - Complexidade O(n)
int buscar_maior(const int *vetor, int tamanho, long *op_contadas) {
    if (tamanho <= 0) return -1;
    (*op_contadas)++;
    int maior = vetor[0];

    for (int i = 1; i < tamanho; i++) {
        (*op_contadas)++;
        if (vetor[i] > maior) {
            maior = vetor[i];
        }
    }
    return maior;
}

// Função instrumentada para Busca Quadrática de Pares - Complexidade O(n²)
int buscar_pares_soma(const int *vetor, int tamanho, int alvo, long *op_contadas) {
    int pares_encontrados = 0;
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            (*op_contadas)++;
            if (vetor[i] + vetor[j] == alvo) {
                pares_encontrados++;
            }
        }
    }
    return pares_encontrados;
}

// Verificação de Pré-condição: Garante que o vetor está devidamente ordenado
int verificar_ordenado(const int *vetor, int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        if (vetor[i] > vetor[i + 1]) return 0;
    }
    return 1;
}

// Nova Função: Busca Binária Iterativa - Complexidade O(log n)
int busca_binaria(const int *vetor, int tamanho, int chave, long *op_contadas, int modo_detalhado) {
    if (!verificar_ordenado(vetor, tamanho)) {
        printf("[ERRO] O vetor nao esta ordenado! Busca binaria cancelada.\n");
        return -1;
    }

    int inicio = 0;
    int fim = tamanho - 1;

    if (modo_detalhado) {
        printf("\n  --> Rastreando busca pela chave %d em vetor de N = %d:\n", chave, tamanho);
    }

    while (inicio <= fim) {
        // Cálculo seguro do meio evitando overflow de inteiros (inicio + fim)
        int meio = inicio + (fim - inicio) / 2;

        if (modo_detalhado) {
            printf("      [Passo] inicio: %-6d | meio: %-6d (valor: %-6d) | fim: %-6d\n", 
                   inicio, meio, vetor[meio], fim);
        }

        (*op_contadas)++;
        if (vetor[meio] == chave) {
            return meio; // Chave encontrada
        }

        (*op_contadas)++;
        if (vetor[meio] < chave) {
            inicio = meio + 1; // Reduz intervalo para a metade direita
        } else {
            fim = meio - 1;    // Reduz intervalo para a metade esquerda
        }
    }

    return -1; // Intervalo ficou vazio (inicio > fim): Chave ausente
}

// Preenche o vetor com dados estritamente ordenados (sequência de números pares)
void preencher_vetor_ordenado(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = i * 2;
    }
}

void executar_teste(int tamanho) {
    int *vetor = (int *)malloc((size_t)tamanho * sizeof(int));
    if (vetor == NULL) {
        printf("[ERRO] Falha de alocacao para N = %d.\n", tamanho);
        return;
    }

    preencher_vetor_ordenado(vetor, tamanho);

    long ops_binaria_presente = 0;
    long ops_binaria_ausente = 0;

    // Teste 1: Chave Presente (ex: elemento próximo do fim)
    int chave_presente = (tamanho - 5) * 2;
    // Ativa impressão detalhada de passos apenas em vetores pequenos
    int detalhar = (tamanho <= 100);
    
    int pos_encontrada = busca_binaria(vetor, tamanho, chave_presente, &ops_binaria_presente, detalhar);

    // Teste 2: Chave Ausente (ex: número ímpar que não está na sequência de pares)
    int chave_ausente = 9999999;
    busca_binaria(vetor, tamanho, chave_ausente, &ops_binaria_ausente, 0);

    printf("--------------------------------------------------------------------------------\n");
    printf("TAMANHO DO VETOR (N): %-8d\n", tamanho);
    printf("  [Chave Presente: %-8d] Posicao: %-8d | Comparacoes: %ld\n", 
           chave_presente, pos_encontrada, ops_binaria_presente);
    printf("  [Chave Ausente:  %-8d] Posicao: %-8d | Comparacoes: %ld\n", 
           chave_ausente, -1, ops_binaria_ausente);

    free(vetor);
}

int main(void) {
    int tamanhos[] = {16, 1024, 1048576}; // 16, ~1 mil, ~1 milhao
    int total_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("=== LABORATÓRIO DE COMPLEXIDADE - BUSCA BINÁRIA O(log n) ===\n");

    for (int i = 0; i < total_testes; i++) {
        executar_teste(tamanhos[i]);
    }

    printf("\n=== QUADRO COMPARATIVO DAS COMPLEXIDADES ===\n");
    printf("O(1)      - Acesso Direto:     Sempre ~2 operacoes (independente de N)\n");
    printf("O(log n)  - Busca Binaria:     N = 1.048.576 -> Apenas ~20 comparacoes!\n");
    printf("O(n)      - Busca Linear:      N = 1.048.576 -> ~1.048.576 operacoes\n");
    printf("O(n²)     - Busca de Pares:    N = 1.048.576 -> ~500 bilhoes de operacoes!\n");
    printf("\nConclui-se que log2(1.048.576) = 20. O espaço de busca cai pela metade a cada passo.\n");

    return 0;
}