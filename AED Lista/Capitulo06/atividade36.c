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
    m->comparacoes = 0; m->trocas = 0; m->deslocamentos = 0; m->passagens = 0;
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
// ATIVIDADE 1: SELECTION SORT 
// ====================================================================
int encontrar_menor_numerico(int arr[], int inicio, int n, Metricas *m) {
    int min_idx = inicio;
    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++;
        if (arr[i] < arr[min_idx]) min_idx = i;
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
// ATIVIDADE 2 e 3: INSERTION SORT 
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
            if (arr[j].pontuacao < chave.pontuacao) deve_deslocar = true; // Desempate por pontos
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
// ATIVIDADE 4 e 5: BUBBLE SORT OTIMIZADO (Números e Estruturas)
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
        if (!houve_troca) break; // Parada antecipada
    }
}

// Regra de comparação separada da lógica de ordenação
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
        if (!houve_troca) break; // Parada antecipada
    }
}

bool validar_bubble_jogadores(Jogador arr[], int n, bool crescente) {
    for (int i = 0; i < n - 1; i++) {
        if (crescente && arr[i].id > arr[i + 1].id) return false;
        if (!crescente && arr[i].id < arr[i + 1].id) return false;
    }
    return true;
}

// ====================================================================
// MAIN: A CENTRAL COMPLETA EXECUTANDO O ÚLTIMO TESTE
// ====================================================================
int main() {
    Metricas metricas;
    
    Jogador jogadores_originais[] = {
        {5, "Igor", 150},
        {2, "Samuel", 300},
        {1, "Leticia", 150},
        {4, "Gabriel", 80},
        {3, "Sanmilly", 150}
    };
    int n_jog = sizeof(jogadores_originais) / sizeof(jogadores_originais[0]);

    Jogador copia_crescente[n_jog];
    Jogador copia_decrescente[n_jog];
    memcpy(copia_crescente, jogadores_originais, n_jog * sizeof(Jogador));
    memcpy(copia_decrescente, jogadores_originais, n_jog * sizeof(Jogador));

    printf("==================================================================\n");
    printf(" CENTRAL DE ORDENACAO COMPLETA - AED \n");
    printf("==================================================================\n\n");
    
    printf("TODAS AS FUNCOES PRESERVADAS (Selection, Insertion, Bubble)\n\n");
    
    printf("--- TESTE BUBBLE SORT ESTRUTURAS (IDs EMBARALHADOS) ---\n");
    imprimir_jogadores(jogadores_originais, n_jog);
    printf("\n");

    // TESTE 1: ORDEM CRESCENTE
    zerar_metricas(&metricas);
    printf("-> EXECUTANDO BUBBLE SORT [ CAMPO: ID | DIRECAO: CRESCENTE ]\n");
    bubble_sort_jogadores(copia_crescente, n_jog, true, &metricas);
    imprimir_jogadores(copia_crescente, n_jog);
    printf("Metricas -> Comparacoes: %d | Trocas: %d | Passagens: %d\n", metricas.comparacoes, metricas.trocas, metricas.passagens);
    printf("Validacao: %s\n\n", validar_bubble_jogadores(copia_crescente, n_jog, true) ? "SUCESSO (Ordenado Corretamente)" : "FALHA");

    // TESTE 2: ORDEM DECRESCENTE
    zerar_metricas(&metricas);
    printf("-> EXECUTANDO BUBBLE SORT [ CAMPO: ID | DIRECAO: DECRESCENTE ]\n");
    bubble_sort_jogadores(copia_decrescente, n_jog, false, &metricas);
    imprimir_jogadores(copia_decrescente, n_jog);
    printf("Metricas -> Comparacoes: %d | Trocas: %d | Passagens: %d\n", metricas.comparacoes, metricas.trocas, metricas.passagens);
    printf("Validacao: %s\n", validar_bubble_jogadores(copia_decrescente, n_jog, false) ? "SUCESSO (Ordenado Corretamente)" : "FALHA");

    return 0;
}