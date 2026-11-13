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
        if (comparacao_nomes > 0) deve_deslocar = true;
        else if (comparacao_nomes == 0) {
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
// ATIVIDADE 6 e 7: MERGE SORT (Preservado)
// ====================================================================
void intercalar_numerico(int arr[], int inicio, int meio, int fim, Metricas *m) {
    int tam_esq = meio - inicio + 1, tam_dir = fim - meio;
    int *esq = (int*) malloc(tam_esq * sizeof(int));
    int *dir = (int*) malloc(tam_dir * sizeof(int));
    for (int i = 0; i < tam_esq; i++) esq[i] = arr[inicio + i];
    for (int j = 0; j < tam_dir; j++) dir[j] = arr[meio + 1 + j];
    int i = 0, j = 0, k = inicio;
    while (i < tam_esq && j < tam_dir) {
        m->comparacoes++;
        if (esq[i] <= dir[j]) arr[k++] = esq[i++];
        else arr[k++] = dir[j++];
    }
    while (i < tam_esq) arr[k++] = esq[i++];
    while (j < tam_dir) arr[k++] = dir[j++];
    free(esq); free(dir);
}

void merge_sort_numerico(int arr[], int inicio, int fim, Metricas *m) {
    if (inicio >= fim) return;
    int meio = inicio + (fim - inicio) / 2;
    merge_sort_numerico(arr, inicio, meio, m);
    merge_sort_numerico(arr, meio + 1, fim, m);
    intercalar_numerico(arr, inicio, meio, fim, m);
}

// ====================================================================
// ATIVIDADE 8: QUICK SORT E PARTIÇÕES (NOVO!)
// ====================================================================

// 1. Partição COM memória auxiliar (Para compreensão do processo)
int particao_auxiliar_numerica(int arr[], int inicio, int fim, Metricas *m) {
    int pivo = arr[fim]; // Escolhendo o último elemento como pivô
    int n = fim - inicio + 1;
    
    // Alocando memória para separar os valores
    int *menores = (int*) malloc(n * sizeof(int));
    int *maiores = (int*) malloc(n * sizeof(int));
    int cont_menor = 0, cont_maior = 0;

    // Regra clara: agrupa os menores ou iguais (<=) de um lado, e maiores (>) de outro
    for (int i = inicio; i < fim; i++) {
        m->comparacoes++;
        if (arr[i] <= pivo) menores[cont_menor++] = arr[i];
        else maiores[cont_maior++] = arr[i];
    }

    // Remontando o vetor e preservando todos os elementos
    int idx = inicio;
    for (int i = 0; i < cont_menor; i++) arr[idx++] = menores[i];
    
    int pos_pivo = idx;
    arr[idx++] = pivo; // O pivô ganha a sua posição final exata
    
    for (int i = 0; i < cont_maior; i++) arr[idx++] = maiores[i];

    free(menores);
    free(maiores);

    return pos_pivo; // Devolve a posição final
}

// 2. Partição IN-PLACE (No próprio vetor - Otimizada - Partição de Lomuto)
int particao_in_place_numerica(int arr[], int inicio, int fim, Metricas *m) {
    int pivo = arr[fim];
    int i = inicio - 1; // Índice que demarca o limite dos elementos menores que o pivô

    // Mantendo os índices válidos dentro do próprio vetor
    for (int j = inicio; j < fim; j++) {
        m->comparacoes++;
        
        if (arr[j] <= pivo) { // Regra do pivô
            i++;
            // Faz a troca
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            m->trocas++;
        }
    }
    
    // Colocando o pivô na posição exata (logo após o último menor)
    int pos_pivo = i + 1;
    int temp = arr[pos_pivo];
    arr[pos_pivo] = arr[fim];
    arr[fim] = temp;
    m->trocas++;
    
    return pos_pivo;
}

// 3. Quick Sort Recursivo
void quick_sort_numerico(int arr[], int inicio, int fim, Metricas *m) {
    if (inicio < fim) {
        // Encontra o pivô usando a partição In-Place
        int pos_pivo = particao_in_place_numerica(arr, inicio, fim, m);
        
        // Aplica o Quick Sort SOMENTE aos subintervalos necessários
        // Nota: O pivô (pos_pivo) não entra na recursão, pois ele já está na posição final!
        quick_sort_numerico(arr, inicio, pos_pivo - 1, m);
        quick_sort_numerico(arr, pos_pivo + 1, fim, m);
    }
}

// ====================================================================
// MAIN: CENTRAL DE ORDENAÇÃO COMPLETA
// ====================================================================
int main() {
    Metricas metricas;
    
    printf("==================================================================\n");
    printf(" CENTRAL DE ORDENACAO COMPLETA - AED \n");
    printf(" ARQUIVO DEFINITIVO: TODAS AS 8 ETAPAS ACUMULADAS \n");
    printf("==================================================================\n\n");
    
    // Teste para o Quick Sort: Caso cheio de valores repetidos!
    int vetor_teste[] = {45, 12, 89, 12, 45, 7, 33, 12};
    int n = sizeof(vetor_teste) / sizeof(vetor_teste[0]);

    // Criando cópias
    int copia_auxiliar[n];
    int copia_inplace[n];
    int copia_quick[n];
    memcpy(copia_auxiliar, vetor_teste, n * sizeof(int));
    memcpy(copia_inplace, vetor_teste, n * sizeof(int));
    memcpy(copia_quick, vetor_teste, n * sizeof(int));

    // ---------------------------------------------------------
    // TESTE 1: PARTIÇÃO AUXILIAR (Demonstração Isolada)
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- 1. DEMONSTRACAO ISOLADA: PARTICAO COM MEMORIA AUXILIAR ---\n");
    printf("Original : "); imprimir_vetor_numerico(copia_auxiliar, n);
    
    int pos_pivo_aux = particao_auxiliar_numerica(copia_auxiliar, 0, n - 1, &metricas);
    
    printf("Apos Particao (Pivo: %d) -> ", copia_auxiliar[pos_pivo_aux]);
    imprimir_vetor_numerico(copia_auxiliar, n);
    printf("Posicao final do pivo: Indice [%d]\n\n", pos_pivo_aux);

    // ---------------------------------------------------------
    // TESTE 2: PARTIÇÃO IN-PLACE (Demonstração Isolada)
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- 2. DEMONSTRACAO ISOLADA: PARTICAO IN-PLACE ---\n");
    printf("Original : "); imprimir_vetor_numerico(copia_inplace, n);
    
    int pos_pivo_in = particao_in_place_numerica(copia_inplace, 0, n - 1, &metricas);
    
    printf("Apos Particao (Pivo: %d) -> ", copia_inplace[pos_pivo_in]);
    imprimir_vetor_numerico(copia_inplace, n);
    printf("Posicao final do pivo: Indice [%d]\n", pos_pivo_in);
    printf("Metricas da Particao -> Comparacoes: %d | Trocas: %d\n\n", metricas.comparacoes, metricas.trocas);

    // ---------------------------------------------------------
    // TESTE 3: QUICK SORT COMPLETO
    // ---------------------------------------------------------
    zerar_metricas(&metricas);
    printf("--- 3. QUICK SORT COMPLETO (RECURSIVO + IN-PLACE) ---\n");
    printf("Original (Com Repetidos): "); imprimir_vetor_numerico(copia_quick, n);
    
    quick_sort_numerico(copia_quick, 0, n - 1, &metricas);
    
    printf("Vetor Totalmente Ordenado: "); imprimir_vetor_numerico(copia_quick, n);
    printf("Metricas Totais -> Comparacoes: %d | Trocas: %d\n\n", metricas.comparacoes, metricas.trocas);

    return 0;
}