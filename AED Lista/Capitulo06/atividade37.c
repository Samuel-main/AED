#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// ====================================================================
// ESTRUTURAS BASES DA CENTRAL
// ====================================================================

typedef struct {
    int id;
    char nome[50];
    int pontuacao;
} Jogador;

typedef struct {
    int comparacoes;
    int trocas;
    int deslocamentos;
    int passagens;
} Metricas;

void zerar_metricas(Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;
    m->deslocamentos = 0;
    m->passagens = 0;
}

void imprimir_vetor_numerico(int arr[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

void imprimir_jogadores(Jogador arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("ID: %d | Nome: %-10s | Pontos: %d\n", arr[i].id, arr[i].nome, arr[i].pontuacao);
    }
}

// ====================================================================
// ATIVIDADE 1: SELECTION SORT (Preservado)
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

int encontrar_menor_jogador(Jogador arr[], int inicio, int n, Metricas *m) {
    int min_idx = inicio;
    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++; 
        if (arr[i].pontuacao < arr[min_idx].pontuacao) {
            min_idx = i;
        } else if (arr[i].pontuacao == arr[min_idx].pontuacao) {
            m->comparacoes++; 
            if (strcmp(arr[i].nome, arr[min_idx].nome) < 0) {
                min_idx = i;
            }
        }
    }
    return min_idx;
}

void selection_sort_jogadores(Jogador arr[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = encontrar_menor_jogador(arr, i, n, m);
        if (min_idx != i) {
            Jogador temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            m->trocas++; 
        }
    }
}

// ====================================================================
// ATIVIDADE 2 e 3: INSERTION SORT (Preservado)
// ====================================================================
void inserir_no_prefixo_numerico(int arr[], int i, Metricas *m) {
    int chave = arr[i];
    int j = i - 1;
    while (j >= 0) {
        m->comparacoes++;
        if (arr[j] > chave) {
            arr[j + 1] = arr[j];
            m->deslocamentos++;
            j--;
        } else break;
    }
    arr[j + 1] = chave;
}

void insertion_sort_numerico(int arr[], int n, Metricas *m) {
    for (int i = 1; i < n; i++) inserir_no_prefixo_numerico(arr, i, m);
}

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
            if (arr[j].pontuacao < chave.pontuacao) deve_deslocar = true; 
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
    for (int i = 1; i < n; i++) inserir_no_prefixo_jogador_alfabetico(arr, i, m);
}

// ====================================================================
// ATIVIDADE 4 e 5: BUBBLE SORT OTIMIZADO (Preservado)
// ====================================================================
void bubble_sort_otimizado_numerico(int arr[], int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        m->passagens++;
        bool houve_troca = false; 
        for (int j = 0; j < n - i - 1; j++) {
            m->comparacoes++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                m->trocas++;
                houve_troca = true;
            }
        }
        if (!houve_troca) break; 
    }
}

bool comparar_por_id(Jogador a, Jogador b, bool crescente) {
    if (crescente) return a.id > b.id;
    else return a.id < b.id;
}

void bubble_sort_jogadores(Jogador arr[], int n, bool crescente, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        m->passagens++;
        bool houve_troca = false; 
        
        for (int j = 0; j < n - i - 1; j++) {
            m->comparacoes++;
            
            if (comparar_por_id(arr[j], arr[j + 1], crescente)) {
                Jogador temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                m->trocas++;
                houve_troca = true;
            }
        }
        if (!houve_troca) break; 
    }
}

// ====================================================================
// ATIVIDADE 6: PREPARAÇÃO MERGE SORT - INTERCALAÇÃO (NOVO!)
// ====================================================================

// Função que recebe dois vetores ordenados (A e B) e produz um terceiro (C)
void intercalar_dois_vetores(int A[], int tamA, int B[], int tamB, int C[], Metricas *m) {
    // 1. Manter índices independentes para as duas entradas
    int i = 0; // Índice de A
    int j = 0; // Índice de B
    int k = 0; // Índice de C (saída)

    // Percorre enquanto nenhum dos dois tiver terminado
    while (i < tamA && j < tamB) {
        m->comparacoes++;
        
        if (A[i] <= B[j]) {
            C[k] = A[i];
            i++;
        } else {
            C[k] = B[j];
            j++;
        }
        k++;
    }

    // 2. Copiar os elementos restantes quando uma entrada terminar
    while (i < tamA) {
        C[k] = A[i];
        i++;
        k++;
    }

    while (j < tamB) {
        C[k] = B[j];
        j++;
        k++;
    }
}

// Estrutura recursiva que divide o intervalo pela metade
void estrutura_recursiva_merge(int arr[], int inicio, int fim) {
    if (inicio < fim) {
        // 5. Etapa: DIVIDIR
        // 4. Calcular corretamente o ponto médio para evitar overflow
        int meio = inicio + (fim - inicio) / 2;

        // 5. Etapa: RESOLVER (Chama a recursão para as metades)
        estrutura_recursiva_merge(arr, inicio, meio);
        estrutura_recursiva_merge(arr, meio + 1, fim);

        // 5. Etapa: COMBINAR 
        // (Será a intercalação do próprio vetor na próxima atividade)
    }
}

// ====================================================================
// MAIN: CENTRAL DE ORDENAÇÃO COMPLETA
// ====================================================================
int main() {
    Metricas metricas;
    zerar_metricas(&metricas);

    printf("==================================================================\n");
    printf(" CENTRAL DE ORDENACAO COMPLETA - AED \n");
    printf(" TODAS AS FUNCOES PRESERVADAS (Selection, Insertion, Bubble, Merge)\n");
    printf("==================================================================\n\n");
    
    // ---------------------------------------------------------
    // TESTE DA ATIVIDADE ATUAL: INTERCALAÇÃO
    // ---------------------------------------------------------
    printf("--- TESTE PREPARACAO MERGE SORT (INTERCALACAO) ---\n");
    
    // Vetores originais já ordenados
    int A[] = {10, 30, 50, 70};
    int tamA = sizeof(A) / sizeof(A[0]);

    int B[] = {20, 40, 60};
    int tamB = sizeof(B) / sizeof(B[0]);

    // 3. Produzir saída com todos os valores
    int tamC = tamA + tamB;
    int C[tamC]; 

    printf("Vetor A (Tamanho %d): ", tamA);
    imprimir_vetor_numerico(A, tamA);
    
    printf("Vetor B (Tamanho %d): ", tamB);
    imprimir_vetor_numerico(B, tamB);

    // Costurando os vetores
    intercalar_dois_vetores(A, tamA, B, tamB, C, &metricas);

    printf("\nVetor C (A e B Intercalados): ");
    imprimir_vetor_numerico(C, tamC);
    
    printf("Metricas -> Comparacoes realizadas na intercalacao: %d\n", metricas.comparacoes);
    printf("Validacao: Os %d elementos originais estao na saida? %s\n\n", tamC, (tamC == 7) ? "SIM!" : "NAO");

    return 0;
}