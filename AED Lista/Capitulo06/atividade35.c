#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// ====================================================================
// ESTRUTURAS BASES
// ====================================================================

typedef struct {
    int id;
    char nome[50];
    int pontuacao;
} Jogador;

// Estrutura unificada para rastrear métricas de todos os algoritmos
typedef struct {
    int comparacoes;
    int trocas;
    int deslocamentos;
    int passagens;
} Metricas;

// Função auxiliar para zerar as métricas antes de cada teste
void zerar_metricas(Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;
    m->deslocamentos = 0;
    m->passagens = 0;
}

// ====================================================================
// ALGORITMO 1: SELECTION SORT (Da primeira atividade)
// ====================================================================
int encontrar_menor_numerico(int arr[], int inicio, int n, Metricas *m) {
    int min_idx = inicio;
    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++;
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

void selection_sort_numerico(int arr[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = encontrar_menor_numerico(arr, i, n, m);
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            m->trocas++;
        }
    }
}

// ====================================================================
// ALGORITMO 2: INSERTION SORT (Da segunda atividade)
// ====================================================================
void inserir_no_prefixo_jogador_alfabetico(Jogador arr[], int i, Metricas *m) {
    Jogador chave = arr[i]; 
    int j = i - 1;

    while (j >= 0) {
        m->comparacoes++;
        bool deve_deslocar = false;
        
        int comparacao_nomes = strcmp(arr[j].nome, chave.nome);
        if (comparacao_nomes > 0) {
            deve_deslocar = true;
        } else if (comparacao_nomes == 0) {
            m->comparacoes++;
            if (arr[j].pontuacao < chave.pontuacao) deve_deslocar = true; // Desempate
        }

        if (deve_deslocar) {
            arr[j + 1] = arr[j]; 
            m->deslocamentos++;
            j--;
        } else break;
    }
    arr[j + 1] = chave;
}

void insertion_sort_jogadores_alfabetico(Jogador arr[], int n, Metricas *m) {
    for (int i = 1; i < n; i++) {
        inserir_no_prefixo_jogador_alfabetico(arr, i, m);
    }
}

// ====================================================================
// ALGORITMO 3: BUBBLE SORT OTIMIZADO (NOVO!)
// ====================================================================
void bubble_sort_otimizado(int arr[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        m->passagens++; // Registra a passagem atual
        bool houve_troca = false; // Indicador de troca para esta passagem
        
        // Reduz a faixa útil após cada passagem (n - i - 1)
        for (int j = 0; j < n - i - 1; j++) {
            m->comparacoes++; // Comparando elementos vizinhos
            
            if (arr[j] > arr[j + 1]) {
                // Realiza a troca
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                
                m->trocas++;
                houve_troca = true;
            }
        }
        
        // Se percorreu a faixa útil inteira e não trocou ninguém, já está ordenado!
        if (!houve_troca) {
            break; // Interrompe para evitar passagens desnecessárias
        }
    }
}

// ====================================================================
// FUNÇÕES DE EXIBIÇÃO
// ====================================================================
void imprimir_vetor_numerico(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// ====================================================================
// MAIN: A CENTRAL DE ORDENAÇÃO
// ====================================================================
int main() {
    Metricas metricas;

    printf("========================================================\n");
    printf("     CENTRAL DE ORDENACAO - BUBBLE SORT OTIMIZADO\n");
    printf("========================================================\n\n");

    // Preparando os 3 formatos de entrada exigidos
    int vetor_ordenado[]  = {10, 20, 30, 40, 50, 60};
    int vetor_inverso[]   = {60, 50, 40, 30, 20, 10};
    int vetor_aleatorio[] = {33, 12, 89, 45, 7, 10};
    
    int n = sizeof(vetor_ordenado) / sizeof(vetor_ordenado[0]);

    // ---------------------------------------------------------
    // TESTE 1: VETOR ORDENADO (Melhor Caso - O(N))
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- CENARIO 1: VETOR JA ORDENADO ---\n");
    printf("Original: ");
    imprimir_vetor_numerico(vetor_ordenado, n);
    
    bubble_sort_otimizado(vetor_ordenado, n, &metricas);
    
    printf("Ordenado: ");
    imprimir_vetor_numerico(vetor_ordenado, n);
    printf("Metricas -> Comparacoes: %d | Trocas: %d | Passagens: %d\n", metricas.comparacoes, metricas.trocas, metricas.passagens);
    printf("[Note que ele percebeu na %da. passagem e parou!]\n\n", metricas.passagens);

    // ---------------------------------------------------------
    // TESTE 2: VETOR INVERSO (Pior Caso - O(N^2))
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- CENARIO 2: VETOR EM ORDEM INVERSA ---\n");
    printf("Original: ");
    imprimir_vetor_numerico(vetor_inverso, n);
    
    bubble_sort_otimizado(vetor_inverso, n, &metricas);
    
    printf("Ordenado: ");
    imprimir_vetor_numerico(vetor_inverso, n);
    printf("Metricas -> Comparacoes: %d | Trocas: %d | Passagens: %d\n\n", metricas.comparacoes, metricas.trocas, metricas.passagens);

    // ---------------------------------------------------------
    // TESTE 3: VETOR ALEATÓRIO (Caso Médio)
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- CENARIO 3: VETOR ALEATORIO ---\n");
    printf("Original: ");
    imprimir_vetor_numerico(vetor_aleatorio, n);
    
    bubble_sort_otimizado(vetor_aleatorio, n, &metricas);
    
    printf("Ordenado: ");
    imprimir_vetor_numerico(vetor_aleatorio, n);
    printf("Metricas -> Comparacoes: %d | Trocas: %d | Passagens: %d\n\n", metricas.comparacoes, metricas.trocas, metricas.passagens);

    return 0;
}