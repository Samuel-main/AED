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
// ATIVIDADE 6: PREPARAÇÃO MERGE SORT (Preservado)
// ====================================================================
void intercalar_dois_vetores(int A[], int tamA, int B[], int tamB, int C[], Metricas *m) {
    int i = 0, j = 0, k = 0;
    while (i < tamA && j < tamB) {
        m->comparacoes++;
        if (A[i] <= B[j]) { C[k++] = A[i++]; } 
        else { C[k++] = B[j++]; }
    }
    while (i < tamA) C[k++] = A[i++];
    while (j < tamB) C[k++] = B[j++];
}

// ====================================================================
// ATIVIDADE 7: MERGE SORT COMPLETO (NOVO!)
// ====================================================================

// --- VERSÃO PARA NÚMEROS ---
void intercalar_numerico(int arr[], int inicio, int meio, int fim, Metricas *m) {
    int tam_esq = meio - inicio + 1;
    int tam_dir = fim - meio;
    
    // 5. Liberar memória auxiliar dinâmica: Primeiro alocamos (malloc)
    int *esq = (int*) malloc(tam_esq * sizeof(int));
    int *dir = (int*) malloc(tam_dir * sizeof(int));

    // Copiando os dados para os vetores auxiliares
    for (int i = 0; i < tam_esq; i++) esq[i] = arr[inicio + i];
    for (int j = 0; j < tam_dir; j++) dir[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = inicio;
    
    // 3. Não perder elementos repetidos: o sinal '<=' garante a estabilidade
    while (i < tam_esq && j < tam_dir) {
        m->comparacoes++;
        if (esq[i] <= dir[j]) {
            arr[k++] = esq[i++];
        } else {
            arr[k++] = dir[j++];
        }
    }
    
    while (i < tam_esq) arr[k++] = esq[i++];
    while (j < tam_dir) arr[k++] = dir[j++];

    // 5. Liberar memória auxiliar dinâmica (CRÍTICO em C!)
    free(esq);
    free(dir);
}

void merge_sort_numerico(int arr[], int inicio, int fim, Metricas *m) {
    // 1. Definir caso-base para intervalos com zero ou um elemento
    if (inicio >= fim) return;
    
    int meio = inicio + (fim - inicio) / 2;
    
    // 2. Ordenar e combinar as duas metades
    merge_sort_numerico(arr, inicio, meio, m);
    merge_sort_numerico(arr, meio + 1, fim, m);
    
    intercalar_numerico(arr, inicio, meio, fim, m);
}

// --- VERSÃO PARA ESTRUTURAS (JOGADORES) ---
void intercalar_jogadores(Jogador arr[], int inicio, int meio, int fim, bool crescente, Metricas *m) {
    int tam_esq = meio - inicio + 1;
    int tam_dir = fim - meio;
    
    // Alocação dinâmica de Estruturas
    Jogador *esq = (Jogador*) malloc(tam_esq * sizeof(Jogador));
    Jogador *dir = (Jogador*) malloc(tam_dir * sizeof(Jogador));

    for (int i = 0; i < tam_esq; i++) esq[i] = arr[inicio + i];
    for (int j = 0; j < tam_dir; j++) dir[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = inicio;
    
    while (i < tam_esq && j < tam_dir) {
        m->comparacoes++;
        bool copiar_esq = false;
        
        // 4. Aplicar critério explícito às estruturas (Pontuação, com desempate por Nome)
        if (crescente) {
            if (esq[i].pontuacao < dir[j].pontuacao) copiar_esq = true;
            else if (esq[i].pontuacao == dir[j].pontuacao && strcmp(esq[i].nome, dir[j].nome) <= 0) copiar_esq = true;
        } else {
            if (esq[i].pontuacao > dir[j].pontuacao) copiar_esq = true;
            else if (esq[i].pontuacao == dir[j].pontuacao && strcmp(esq[i].nome, dir[j].nome) <= 0) copiar_esq = true;
        }

        if (copiar_esq) {
            arr[k++] = esq[i++];
        } else {
            arr[k++] = dir[j++];
        }
    }
    
    while (i < tam_esq) arr[k++] = esq[i++];
    while (j < tam_dir) arr[k++] = dir[j++];

    // 5. Liberar memória auxiliar
    free(esq);
    free(dir);
}

void merge_sort_jogadores(Jogador arr[], int inicio, int fim, bool crescente, Metricas *m) {
    // 1. Caso base
    if (inicio >= fim) return;
    
    int meio = inicio + (fim - inicio) / 2;
    
    // 2. Recursão e intercalação
    merge_sort_jogadores(arr, inicio, meio, crescente, m);
    merge_sort_jogadores(arr, meio + 1, fim, crescente, m);
    
    intercalar_jogadores(arr, inicio, meio, fim, crescente, m);
}

// ====================================================================
// MAIN: CENTRAL DE ORDENAÇÃO COMPLETA
// ====================================================================
int main() {
    Metricas metricas;
    
    printf("==================================================================\n");
    printf(" CENTRAL DE ORDENACAO COMPLETA - AED \n");
    printf(" ARQUIVO DEFINITIVO: TODAS AS ETAPAS ACUMULADAS \n");
    printf("==================================================================\n\n");
    
    // Dados Originais da Central para conferência
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

    // Criando cópias para não destruir as originais
    int numeros_copia[n_num];
    memcpy(numeros_copia, numeros_originais, n_num * sizeof(int));
    
    Jogador jogadores_copia[n_jog];
    memcpy(jogadores_copia, jogadores_originais, n_jog * sizeof(Jogador));

    // ---------------------------------------------------------
    // TESTE 1: MERGE SORT NUMÉRICO
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- TESTE MERGE SORT (NUMERICO) ---\n");
    printf("Entrada Original : ");
    imprimir_vetor_numerico(numeros_originais, n_num);
    
    merge_sort_numerico(numeros_copia, 0, n_num - 1, &metricas);
    
    printf("Saida Ordenada   : ");
    imprimir_vetor_numerico(numeros_copia, n_num);
    printf("Comparacoes feitas: %d\n\n", metricas.comparacoes);

    // ---------------------------------------------------------
    // TESTE 2: MERGE SORT JOGADORES (ESTRUTURAS)
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- TESTE MERGE SORT (JOGADORES - CRESCENTE POR PONTOS) ---\n");
    printf("Entrada Original:\n");
    imprimir_jogadores(jogadores_originais, n_jog);
    
    // 6. Conferir a saída contra as entradas da central
    merge_sort_jogadores(jogadores_copia, 0, n_jog - 1, true, &metricas);
    
    printf("\nSaida Ordenada (Pontos Crescente, Desempate Nome):\n");
    imprimir_jogadores(jogadores_copia, n_jog);
    printf("Comparacoes feitas: %d\n\n", metricas.comparacoes);
    
    printf(">>> SUCESSO! Memoria alocada e liberada corretamente.\n");

    return 0;
}