#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Estrutura para os jogadores
typedef struct {
    int id;
    char nome[50];
    int pontuacao;
} Jogador;

// Estrutura para armazenar as métricas de desempenho do algoritmo
typedef struct {
    int comparacoes;
    int trocas;
} Metricas;

// ====================================================================
// PARTE 1: VETOR NUMÉRICO
// ====================================================================

// Função auxiliar: Encontra o índice do menor elemento no vetor numérico
int encontrar_menor_numerico(int arr[], int inicio, int n, Metricas *m) {
    int min_idx = inicio;
    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++; // Contabiliza a comparação
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

// Algoritmo Selection Sort para o vetor numérico
void selection_sort_numerico(int arr[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        // Usa a função auxiliar para achar o menor da parte não ordenada
        int min_idx = encontrar_menor_numerico(arr, i, n, m);
        
        if (min_idx != i) {
            // Realiza a troca
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            m->trocas++; // Contabiliza a troca
        }
    }
}

// ====================================================================
// PARTE 2: VETOR DE JOGADORES (COM DESEMPATE)
// ====================================================================

// Função auxiliar: Encontra o índice do menor jogador (por pontuação e nome)
int encontrar_menor_jogador(Jogador arr[], int inicio, int n, Metricas *m) {
    int min_idx = inicio;
    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++; // Comparação principal (pontuação)
        
        if (arr[i].pontuacao < arr[min_idx].pontuacao) {
            min_idx = i;
        } 
        // Lógica de desempate: se as pontuações forem iguais, desempata pelo Nome (ordem alfabética)
        else if (arr[i].pontuacao == arr[min_idx].pontuacao) {
            m->comparacoes++; // Contabiliza a comparação do desempate
            if (strcmp(arr[i].nome, arr[min_idx].nome) < 0) {
                min_idx = i;
            }
        }
    }
    return min_idx;
}

// Algoritmo Selection Sort adaptado para a estrutura inteira de Jogadores
void selection_sort_jogadores(Jogador arr[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = encontrar_menor_jogador(arr, i, n, m);
        
        if (min_idx != i) {
            // Troca a estrutura INTEIRA de uma vez
            Jogador temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            m->trocas++; // Contabiliza a troca
        }
    }
}

// ====================================================================
// FUNÇÕES DE EXIBIÇÃO E MAIN
// ====================================================================

void imprimir_vetor_numerico(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void imprimir_jogadores(Jogador arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] %-10s - Pontos: %d\n", arr[i].id, arr[i].nome, arr[i].pontuacao);
    }
}

int main() {
    // 1. Preservando a entrada original: Criando bases originais
    int numeros_originais[] = {89, 45, 12, 33, 12, 7};
    int n_num = sizeof(numeros_originais) / sizeof(numeros_originais[0]);
    
    Jogador jogadores_originais[] = {
        {1, "Igor", 150},
        {2, "Samuel", 300},
        {3, "Leticia", 150},  // Empate triplo de 150 pontos para testar o desempate
        {4, "Gabriel", 80},
        {5, "Sanmilly", 150}
    };
    int n_jog = sizeof(jogadores_originais) / sizeof(jogadores_originais[0]);

    // Criando cópias de trabalho (para não alterar os originais)
    int numeros_copia[n_num];
    memcpy(numeros_copia, numeros_originais, n_num * sizeof(int));

    Jogador jogadores_copia[n_jog];
    memcpy(jogadores_copia, jogadores_originais, n_jog * sizeof(Jogador));

    // Inicializando métricas
    Metricas metricas_num = {0, 0};
    Metricas metricas_jog = {0, 0};

    // Executando as ordenações nas cópias
    printf("--- ORDENACAO NUMERICA ---\n");
    printf("Vetor Original: ");
    imprimir_vetor_numerico(numeros_originais, n_num);
    
    selection_sort_numerico(numeros_copia, n_num, &metricas_num);
    
    printf("Vetor Ordenado: ");
    imprimir_vetor_numerico(numeros_copia, n_num);
    printf("Metricas: %d comparacoes, %d trocas\n\n", metricas_num.comparacoes, metricas_num.trocas);

    printf("--- ORDENACAO DE JOGADORES ---\n");
    printf("Jogadores Originais:\n");
    imprimir_jogadores(jogadores_originais, n_jog);
    
    selection_sort_jogadores(jogadores_copia, n_jog, &metricas_jog);
    
    printf("\nJogadores Ordenados (Crescente com desempate alfabetico):\n");
    imprimir_jogadores(jogadores_copia, n_jog);
    printf("Metricas: %d comparacoes, %d trocas\n", metricas_jog.comparacoes, metricas_jog.trocas);

    return 0;
}