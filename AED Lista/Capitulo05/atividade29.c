#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// VERIFICAÇÃO DE PRÉ-CONDIÇÃO
// ============================================================================
int verificar_ordenado(const int *vetor, int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        if (vetor[i] > vetor[i + 1]) return 0;
    }
    return 1;
}

// ============================================================================
// BUSCA LINEAR INSTRUMENTADA - Complexidade O(n)
// Pre-condição: Nenhuma (funciona em vetores ordenados ou desordenados)
// ============================================================================
int busca_linear(const int *vetor, int tamanho, int chave, long *op_contadas) {
    for (int i = 0; i < tamanho; i++) {
        (*op_contadas)++; // Contabiliza a comparação a cada elemento
        if (vetor[i] == chave) {
            return i; // Retorna a primeira ocorrência encontrada
        }
    }
    return -1; // Valor ausente
}

// ============================================================================
// BUSCA BINÁRIA INSTRUMENTADA - Complexidade O(log n)
// Pre-condição: EXIGE que o vetor esteja estritamente ORDENADO
// ============================================================================
int busca_binaria(const int *vetor, int tamanho, int chave, long *op_contadas) {
    // Garante que o algoritmo não rode se a pré-condição for violada
    if (!verificar_ordenado(vetor, tamanho)) {
        return -2; // Código de erro: Pré-condição violada
    }

    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        (*op_contadas)++;
        if (vetor[meio] == chave) {
            return meio;
        }

        (*op_contadas)++;
        if (vetor[meio] < chave) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    return -1; // Valor ausente
}

// Versão "Ingênua" da Busca Binária sem verificação de pré-condição (para teste de falha)
int busca_binaria_ingenua(const int *vetor, int tamanho, int chave, long *op_contadas) {
    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        (*op_contadas)++;
        if (vetor[meio] == chave) return meio;

        (*op_contadas)++;
        if (vetor[meio] < chave) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// ============================================================================
// POPULAÇÃO DE DADOS
// ============================================================================
void preencher_ordenado(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = (i + 1) * 10; // Ex: 10, 20, 30, 40...
    }
}

void preencher_desordenado(int *vetor, int tamanho) {
    // Sequência arbitrária e não ordenada
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = ((i * 127) + 33) % 1000;
    }
    // Garante presença de um valor específico em posição conhecida para o teste
    if (tamanho > 5) vetor[5] = 42; 
}

// ============================================================================
// TESTE 1: CONFRONTO DIRETO EM VETOR ORDENADO (Início, Meio, Fim e Ausente)
// ============================================================================
void executar_confronto_ordenado(int tamanho) {
    int *vetor = (int *)malloc((size_t)tamanho * sizeof(int));
    if (vetor == NULL) return;

    preencher_ordenado(vetor, tamanho);

    // Casos de teste especificados nos requisitos
    int chaves_teste[] = {
        vetor[0],                // Início
        vetor[tamanho / 2],      // Meio
        vetor[tamanho - 1],      // Fim
        9999999                  // Ausente
    };
    const char *rotulos[] = {"Inicio", "Meio", "Fim", "Ausente"};

    printf("======================================================================\n");
    printf("1. CONFRONTO EM VETOR ORDENADO (N = %d)\n", tamanho);
    printf("======================================================================\n");
    printf("%-10s | %-12s | %-15s | %-15s\n", "Cenario", "Chave", "Ops Linear O(n)", "Ops Binaria O(log n)");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < 4; i++) {
        long ops_lin = 0;
        long ops_bin = 0;

        int pos_lin = busca_linear(vetor, tamanho, chaves_teste[i], &ops_lin);
        int pos_bin = busca_binaria(vetor, tamanho, chaves_teste[i], &ops_bin);

        printf("%-10s | %-12d | %-15ld | %-15ld\n", 
               rotulos[i], chaves_teste[i], ops_lin, ops_bin);
    }

    free(vetor);
}

// ============================================================================
// TESTE 2: DEMONSTRAÇÃO DE FALHA DA BUSCA BINÁRIA EM VETOR DESORDENADO
// ============================================================================
void demonstrar_vetor_desordenado(void) {
    int tamanho = 10;
    int vetor_desordenado[] = {85, 12, 42, 4, 99, 15, 73, 50, 61, 23};
    int chave_alvo = 42; // O valor 42 está presente no índice 2

    printf("\n======================================================================\n");
    printf("2. DEMONSTRAÇÃO DE PRÉ-CONDIÇÃO: VETOR DESORDENADO\n");
    printf("======================================================================\n");
    printf("Vetor: [85, 12, 42, 4, 99, 15, 73, 50, 61, 23]\n");
    printf("Procurando o valor: %d (que está no índice 2)\n\n", chave_alvo);

    long ops_lin = 0;
    long ops_bin_segura = 0;
    long ops_bin_ingenua = 0;

    int pos_lin = busca_linear(vetor_desordenado, tamanho, chave_alvo, &ops_lin);
    int pos_bin_segura = busca_binaria(vetor_desordenado, tamanho, chave_alvo, &ops_bin_segura);
    int pos_bin_ingenua = busca_binaria_ingenua(vetor_desordenado, tamanho, chave_alvo, &ops_bin_ingenua);

    printf("  [Busca Linear]\n");
    printf("    Status: SUCESSO | Posicao: %d | Comparacoes: %ld\n\n", pos_lin, ops_lin);

    printf("  [Busca Binaria Segura (Verifica Ordenacao)]\n");
    printf("    Status: RECUSADA (Cod %d) | O algoritmo detectou falta de ordenacao.\n\n", pos_bin_segura);

    printf("  [Busca Binaria Ingenua (Sem Checar Ordenacao)]\n");
    printf("    Status: %s | Posicao retornada: %d | Comparacoes: %ld\n",
           pos_bin_ingenua == pos_lin ? "SUCESSO" : "FALHA/FALSO NEGATIVO",
           pos_bin_ingenua, ops_bin_ingenua);
    printf("    -> A busca binaria descartou metadades erradas devido a falta de ordenacao!\n");
}

int main(void) {
    executar_confronto_ordenado(100000);
    demonstrar_vetor_desordenado();

    printf("\n======================================================================\n");
    printf("=== ANÁLISE COMPARATIVA DE PRÉ-CONDIÇÕES E CUSTOS ===\n");
    printf("======================================================================\n");
    printf("1. BUSCA LINEAR:\n");
    printf("   - Pre-condicao: NENHUMA. Funciona em qualquer estrutura/vetor.\n");
    printf("   - Custo: O(n). No pior caso (ausente ou fim), realiza N comparacoes.\n\n");
    printf("2. BUSCA BINÁRIA:\n");
    printf("   - Pre-condicao: OBRIGATORIAMENTE ORDENADO.\n");
    printf("   - Custo: O(log n). Para N = 100.000, realiza no maximo ~34 comparacoes.\n");
    printf("   - Risco: Se aplicada em vetor desordenado, gera FALSOS NEGATIVOS,\n");
    printf("     pois ignora metades do vetor baseando-se em premissas falsas.\n");

    return 0;
}