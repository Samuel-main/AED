#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura para os jogadores (Mesma da atividade anterior)
typedef struct {
    int id;
    char nome[50];
    int pontuacao;
} Jogador;

// Estrutura para as métricas (agora focada em deslocamentos)
typedef struct {
    int comparacoes;
    int deslocamentos;
} Metricas;

// ====================================================================
// PARTE 1: VETOR NUMÉRICO
// ====================================================================

// Função que insere um elemento no prefixo que já está ordenado
void inserir_no_prefixo_numerico(int arr[], int i, Metricas *m) {
    int chave = arr[i]; // 1. Guardar temporariamente o valor inserido
    int j = i - 1;

    // 2. Respeitar o limite inferior (j >= 0) durante a busca
    while (j >= 0) {
        m->comparacoes++;
        if (arr[j] > chave) {
            arr[j + 1] = arr[j]; // 3. Deslocar o elemento sem sobrescrever o original (que está na 'chave')
            m->deslocamentos++;
            j--;
        } else {
            break; // Achou a posição
        }
    }
    arr[j + 1] = chave; // Encaixa a chave no espaço que foi aberto
}

// Generalizando o processo: Insertion Sort
void insertion_sort_numerico(int arr[], int n, Metricas *m) {
    // Começa do índice 1, pois o índice 0 já é um "prefixo de 1 elemento ordenado"
    for (int i = 1; i < n; i++) {
        inserir_no_prefixo_numerico(arr, i, m); // 4. Completar a ordenação do vetor
    }
}

// Função para validar se o vetor numérico foi ordenado corretamente
bool validar_ordenacao_numerica(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

// ====================================================================
// PARTE 2: VETOR DE JOGADORES (COM DESEMPATE)
// ====================================================================

// Função que insere um jogador no prefixo ordenado
void inserir_no_prefixo_jogador(Jogador arr[], int i, Metricas *m) {
    Jogador chave = arr[i]; // Guarda a estrutura inteira temporariamente
    int j = i - 1;

    while (j >= 0) {
        m->comparacoes++;
        bool deve_deslocar = false;

        // Verifica a pontuação
        if (arr[j].pontuacao > chave.pontuacao) {
            deve_deslocar = true;
        } 
        // Desempate por nome (ordem alfabética)
        else if (arr[j].pontuacao == chave.pontuacao) {
            m->comparacoes++; // Contabiliza a comparação extra do desempate
            if (strcmp(arr[j].nome, chave.nome) > 0) {
                deve_deslocar = true;
            }
        }

        if (deve_deslocar) {
            arr[j + 1] = arr[j]; // Desloca a estrutura inteira para a direita
            m->deslocamentos++;
            j--;
        } else {
            break;
        }
    }
    arr[j + 1] = chave; // Insere o jogador na posição correta
}

// Insertion Sort para a estrutura de Jogadores
void insertion_sort_jogadores(Jogador arr[], int n, Metricas *m) {
    for (int i = 1; i < n; i++) {
        inserir_no_prefixo_jogador(arr, i, m);
    }
}

// Função para validar se os jogadores foram ordenados corretamente
bool validar_ordenacao_jogadores(Jogador arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i].pontuacao > arr[i + 1].pontuacao) return false;
        if (arr[i].pontuacao == arr[i + 1].pontuacao && strcmp(arr[i].nome, arr[i + 1].nome) > 0) return false;
    }
    return true;
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
    // Mesma entrada da atividade anterior!
    int numeros_originais[] = {89, 45, 12, 33, 12, 7};
    int n_num = sizeof(numeros_originais) / sizeof(numeros_originais[0]);
    
    Jogador jogadores_originais[] = {
        {1, "Igor", 150},
        {2, "Samuel", 300},
        {3, "Leticia", 150},
        {4, "Gabriel", 80},
        {5, "Sanmilly", 150}
    };
    int n_jog = sizeof(jogadores_originais) / sizeof(jogadores_originais[0]);

    // Criando cópias de trabalho
    int numeros_copia[n_num];
    memcpy(numeros_copia, numeros_originais, n_num * sizeof(int));

    Jogador jogadores_copia[n_jog];
    memcpy(jogadores_copia, jogadores_originais, n_jog * sizeof(Jogador));

    // Inicializando métricas
    Metricas metricas_num = {0, 0};
    Metricas metricas_jog = {0, 0};

    // --- EXECUÇÃO NUMÉRICA ---
    printf("--- ORDENACAO NUMERICA (INSERTION SORT) ---\n");
    printf("Original: ");
    imprimir_vetor_numerico(numeros_originais, n_num);
    
    insertion_sort_numerico(numeros_copia, n_num, &metricas_num);
    
    printf("Ordenado: ");
    imprimir_vetor_numerico(numeros_copia, n_num);
    printf("Metricas: %d comparacoes, %d deslocamentos\n", metricas_num.comparacoes, metricas_num.deslocamentos);
    printf("Validacao: %s\n\n", validar_ordenacao_numerica(numeros_copia, n_num) ? "SUCESSO (Ordenado corretamente)" : "FALHA");

    // --- EXECUÇÃO JOGADORES ---
    printf("--- ORDENACAO DE JOGADORES (INSERTION SORT) ---\n");
    printf("Original:\n");
    imprimir_jogadores(jogadores_originais, n_jog);
    
    insertion_sort_jogadores(jogadores_copia, n_jog, &metricas_jog);
    
    printf("\nOrdenado (Crescente com desempate alfabetico):\n");
    imprimir_jogadores(jogadores_copia, n_jog);
    printf("Metricas: %d comparacoes, %d deslocamentos\n", metricas_jog.comparacoes, metricas_jog.deslocamentos);
    printf("Validacao: %s\n", validar_ordenacao_jogadores(jogadores_copia, n_jog) ? "SUCESSO (Ordenado corretamente)" : "FALHA");

    return 0;
}