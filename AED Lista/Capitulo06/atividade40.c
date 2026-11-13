#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Necessário para o srand() e rand()

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

int encontrar_menor_jogador(Jogador arr[], int inicio, int n, Metricas *m) {
    int min_idx = inicio;
    for (int i = inicio + 1; i < n; i++) {
        m->comparacoes++; 
        if (arr[i].pontuacao < arr[min_idx].pontuacao) {
            min_idx = i;
        } else if (arr[i].pontuacao == arr[min_idx].pontuacao) {
            m->comparacoes++; 
            if (strcmp(arr[i].nome, arr[min_idx].nome) < 0) min_idx = i;
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

void insertion_sort_jogadores(Jogador arr[], int n, Metricas *m) {
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

void intercalar_jogadores(Jogador arr[], int inicio, int meio, int fim, bool crescente, Metricas *m) {
    int tam_esq = meio - inicio + 1, tam_dir = fim - meio;
    Jogador *esq = (Jogador*) malloc(tam_esq * sizeof(Jogador));
    Jogador *dir = (Jogador*) malloc(tam_dir * sizeof(Jogador));
    for (int i = 0; i < tam_esq; i++) esq[i] = arr[inicio + i];
    for (int j = 0; j < tam_dir; j++) dir[j] = arr[meio + 1 + j];
    int i = 0, j = 0, k = inicio;
    while (i < tam_esq && j < tam_dir) {
        m->comparacoes++;
        bool copiar_esq = false;
        if (crescente) {
            if (esq[i].pontuacao < dir[j].pontuacao) copiar_esq = true;
            else if (esq[i].pontuacao == dir[j].pontuacao && strcmp(esq[i].nome, dir[j].nome) <= 0) copiar_esq = true;
        } else {
            if (esq[i].pontuacao > dir[j].pontuacao) copiar_esq = true;
            else if (esq[i].pontuacao == dir[j].pontuacao && strcmp(esq[i].nome, dir[j].nome) <= 0) copiar_esq = true;
        }
        if (copiar_esq) arr[k++] = esq[i++];
        else arr[k++] = dir[j++];
    }
    while (i < tam_esq) arr[k++] = esq[i++];
    while (j < tam_dir) arr[k++] = dir[j++];
    free(esq); free(dir);
}

void merge_sort_jogadores(Jogador arr[], int inicio, int fim, bool crescente, Metricas *m) {
    if (inicio >= fim) return;
    int meio = inicio + (fim - inicio) / 2;
    merge_sort_jogadores(arr, inicio, meio, crescente, m);
    merge_sort_jogadores(arr, meio + 1, fim, crescente, m);
    intercalar_jogadores(arr, inicio, meio, fim, crescente, m);
}

// ====================================================================
// ATIVIDADE 8: QUICK SORT IN-PLACE NUMERICO (Preservado)
// ====================================================================
int particao_in_place_numerica(int arr[], int inicio, int fim, Metricas *m) {
    int pivo = arr[fim];
    int i = inicio - 1;
    for (int j = inicio; j < fim; j++) {
        m->comparacoes++;
        if (arr[j] <= pivo) {
            i++;
            int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            m->trocas++;
        }
    }
    int pos_pivo = i + 1;
    int temp = arr[pos_pivo]; arr[pos_pivo] = arr[fim]; arr[fim] = temp;
    m->trocas++;
    return pos_pivo;
}

void quick_sort_numerico(int arr[], int inicio, int fim, Metricas *m) {
    if (inicio < fim) {
        int pos_pivo = particao_in_place_numerica(arr, inicio, fim, m);
        quick_sort_numerico(arr, inicio, pos_pivo - 1, m);
        quick_sort_numerico(arr, pos_pivo + 1, fim, m);
    }
}

// ====================================================================
// ATIVIDADE 9: QUICK SORT ALEATÓRIO PARA JOGADORES (NOVO!)
// ====================================================================

// Partição In-Place com Pivô Aleatório e Critérios Completos (Pontuação e Desempate por Nome)
int particao_aleatoria_jogadores(Jogador arr[], int inicio, int fim, bool crescente, Metricas *m) {
    // 2. Escolher o pivô apenas dentro do subintervalo
    int pivo_idx = inicio + rand() % (fim - inicio + 1);
    
    // Move o pivô aleatório para a última posição para usar a lógica de Lomuto
    Jogador temp_pivo = arr[pivo_idx];
    arr[pivo_idx] = arr[fim];
    arr[fim] = temp_pivo;
    m->trocas++;

    Jogador pivo = arr[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        m->comparacoes++;
        bool deve_trocar = false;
        
        // 4. Aplicar critério e desempate consistentes
        if (crescente) {
            if (arr[j].pontuacao < pivo.pontuacao) deve_trocar = true;
            else if (arr[j].pontuacao == pivo.pontuacao && strcmp(arr[j].nome, pivo.nome) <= 0) deve_trocar = true;
        } else {
            if (arr[j].pontuacao > pivo.pontuacao) deve_trocar = true;
            else if (arr[j].pontuacao == pivo.pontuacao && strcmp(arr[j].nome, pivo.nome) <= 0) deve_trocar = true;
        }

        if (deve_trocar) {
            i++;
            // 3. Adaptar troca para a estrutura inteira de jogadores
            Jogador temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            m->trocas++;
        }
    }
    
    // Posicionando o pivô no local final correto
    int pos_pivo = i + 1;
    Jogador temp_final = arr[pos_pivo];
    arr[pos_pivo] = arr[fim];
    arr[fim] = temp_final;
    m->trocas++;
    
    return pos_pivo;
}

void quick_sort_aleatorio_jogadores(Jogador arr[], int inicio, int fim, bool crescente, Metricas *m) {
    if (inicio < fim) {
        int pos_pivo = particao_aleatoria_jogadores(arr, inicio, fim, crescente, m);
        quick_sort_aleatorio_jogadores(arr, inicio, pos_pivo - 1, crescente, m);
        quick_sort_aleatorio_jogadores(arr, pos_pivo + 1, fim, crescente, m);
    }
}

// ====================================================================
// MAIN: O RELATÓRIO FINAL E COMPARATIVO DA CENTRAL
// ====================================================================
int main() {
    // 1. Inicializar o gerador pseudoaleatório uma ÚNICA vez
    srand(time(NULL));

    printf("==================================================================\n");
    printf("        CENTRAL DE ORDENACAO COMPLETA - RELATORIO FINAL           \n");
    printf("==================================================================\n\n");
    
    // Dados Originais (Sempre preservados)
    Jogador originais[] = {
        {1, "Igor", 150},
        {2, "Samuel", 300},
        {3, "Leticia", 150},
        {4, "Gabriel", 80},
        {5, "Sanmilly", 150}
    };
    int n = sizeof(originais) / sizeof(originais[0]);

    // Criando cópias independentes para garantir que todos rodem sobre a MESMA entrada
    Jogador copia_sel[n], copia_ins[n], copia_bub[n], copia_mer[n], copia_qui[n];
    memcpy(copia_sel, originais, n * sizeof(Jogador));
    memcpy(copia_ins, originais, n * sizeof(Jogador));
    memcpy(copia_bub, originais, n * sizeof(Jogador));
    memcpy(copia_mer, originais, n * sizeof(Jogador));
    memcpy(copia_qui, originais, n * sizeof(Jogador));

    Metricas m_sel, m_ins, m_bub, m_mer, m_qui;
    
    // 5. Executar cada algoritmo sobre a MESMA entrada original
    printf("=> Processando Selection Sort...\n");
    zerar_metricas(&m_sel);
    selection_sort_jogadores(copia_sel, n, &m_sel);

    printf("=> Processando Insertion Sort...\n");
    zerar_metricas(&m_ins);
    insertion_sort_jogadores(copia_ins, n, &m_ins);

    printf("=> Processando Bubble Sort Otimizado...\n");
    zerar_metricas(&m_bub);
    bubble_sort_jogadores(copia_bub, n, true, &m_bub);

    printf("=> Processando Merge Sort...\n");
    zerar_metricas(&m_mer);
    merge_sort_jogadores(copia_mer, 0, n - 1, true, &m_mer);

    printf("=> Processando Quick Sort (Pivo Aleatorio)...\n");
    zerar_metricas(&m_qui);
    quick_sort_aleatorio_jogadores(copia_qui, 0, n - 1, true, &m_qui);

    // 6. Produzir um relatório final de comparações, trocas e deslocamentos
    printf("\n==================================================================\n");
    printf("                    RELATORIO DE DESEMPENHO                       \n");
    printf("==================================================================\n");
    printf("%-18s | %-12s | %-8s | %-14s\n", "ALGORITMO", "COMPARACOES", "TROCAS", "DESLOCAMENTOS");
    printf("-------------------|--------------|----------|--------------------\n");
    printf("%-18s | %-12d | %-8d | %-14d\n", "Selection Sort", m_sel.comparacoes, m_sel.trocas, m_sel.deslocamentos);
    printf("%-18s | %-12d | %-8d | %-14d\n", "Insertion Sort", m_ins.comparacoes, m_ins.trocas, m_ins.deslocamentos);
    printf("%-18s | %-12d | %-8d | %-14d\n", "Bubble Sort Opt.", m_bub.comparacoes, m_bub.trocas, m_bub.deslocamentos);
    printf("%-18s | %-12d | %-8d | %-14d\n", "Merge Sort", m_mer.comparacoes, m_mer.trocas, m_mer.deslocamentos);
    printf("%-18s | %-12d | %-8d | %-14d\n", "Quick Sort Rand.", m_qui.comparacoes, m_qui.trocas, m_qui.deslocamentos);
    printf("==================================================================\n\n");

    printf("Vetor Final (Demonstracao Quick Sort Aleatorio):\n");
    imprimir_jogadores(copia_qui, n);

    return 0;
}