#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// 1. Estrutura para os jogadores (campos movem-se juntos)
typedef struct {
    int id;
    char nome[50];
    int pontuacao;
} Jogador;

typedef struct {
    int comparacoes;
    int deslocamentos;
} Metricas;

// ====================================================================
// OPÇÃO 1: INSERTION SORT - VETOR NUMÉRICO (Mantido da Central)
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

// ====================================================================
// OPÇÃO 2: INSERTION SORT - JOGADORES POR PONTUAÇÃO (Mantido da Central)
// ====================================================================
void inserir_no_prefixo_jogador_pontos(Jogador arr[], int i, Metricas *m) {
    Jogador chave = arr[i];
    int j = i - 1;
    while (j >= 0) {
        m->comparacoes++;
        bool deve_deslocar = false;
        if (arr[j].pontuacao > chave.pontuacao) deve_deslocar = true;
        else if (arr[j].pontuacao == chave.pontuacao) {
            m->comparacoes++;
            if (strcmp(arr[j].nome, chave.nome) > 0) deve_deslocar = true;
        }
        if (deve_deslocar) {
            arr[j + 1] = arr[j];
            m->deslocamentos++;
            j--;
        } else break;
    }
    arr[j + 1] = chave;
}

void insertion_sort_jogadores_pontos(Jogador arr[], int n, Metricas *m) {
    for (int i = 1; i < n; i++) inserir_no_prefixo_jogador_pontos(arr, i, m);
}

// ====================================================================
// OPÇÃO 3: INSERTION SORT - JOGADORES POR ORDEM ALFABÉTICA (NOVO!)
// ====================================================================
void inserir_no_prefixo_jogador_alfabetico(Jogador arr[], int i, Metricas *m) {
    // 1. Usar uma estrutura temporária completa
    Jogador chave = arr[i]; 
    int j = i - 1;

    while (j >= 0) {
        m->comparacoes++;
        bool deve_deslocar = false;
        
        // 2. Comparar nomes com strcmp
        int comparacao_nomes = strcmp(arr[j].nome, chave.nome);

        if (comparacao_nomes > 0) {
            deve_deslocar = true; // Nome atual vem depois no alfabeto, desloca pra direita
        } 
        // 3. Definir critério secundário para nomes iguais (Desempate)
        else if (comparacao_nomes == 0) {
            m->comparacoes++;
            // Critério: Se o nome é igual, quem tem MENOS pontos vai pra direita (Maior pontuação ganha)
            if (arr[j].pontuacao < chave.pontuacao) {
                deve_deslocar = true;
            }
        }

        if (deve_deslocar) {
            // 4. Mover todos os campos juntos (C copia a struct inteira aqui)
            arr[j + 1] = arr[j]; 
            m->deslocamentos++;
            j--;
        } else {
            break;
        }
    }
    arr[j + 1] = chave; // Encaixa a estrutura inteira na posição correta
}

void insertion_sort_jogadores_alfabetico(Jogador arr[], int n, Metricas *m) {
    for (int i = 1; i < n; i++) {
        inserir_no_prefixo_jogador_alfabetico(arr, i, m);
    }
}

// Função de validação da nova ordenação alfabética
bool validar_ordenacao_alfabetica(Jogador arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int cmp = strcmp(arr[i].nome, arr[i + 1].nome);
        if (cmp > 0) return false;
        // Valida o desempate
        if (cmp == 0 && arr[i].pontuacao < arr[i + 1].pontuacao) return false; 
    }
    return true;
}

// ====================================================================
// FUNÇÕES DE EXIBIÇÃO E MAIN (A CENTRAL)
// ====================================================================
void imprimir_jogadores(Jogador arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("ID: %d | Nome: %-10s | Pontos: %d\n", arr[i].id, arr[i].nome, arr[i].pontuacao);
    }
}

int main() {
    // Tabela original incrementada com um nome repetido para testar o desempate
    Jogador jogadores_originais[] = {
        {1, "Igor", 150},
        {2, "Samuel", 300},
        {3, "Leticia", 150},
        {4, "Gabriel", 80},
        {5, "Sanmilly", 150},
        {6, "Igor", 250} // Novo registro com nome repetido e pontuação diferente!
    };
    int n_jog = sizeof(jogadores_originais) / sizeof(jogadores_originais[0]);

    // Criando a cópia de trabalho para a nova funcionalidade
    Jogador jogadores_copia_alfabetica[n_jog];
    memcpy(jogadores_copia_alfabetica, jogadores_originais, n_jog * sizeof(Jogador));
    Metricas metricas_alfabetica = {0, 0};

    // 5 e 6. Incluir nova opção na central e exibir antes/depois
    printf("====================================================\n");
    printf(" CENTRAL DE ORDENACAO - OPCAO 3 (ORDEM ALFABETICA)\n");
    printf("====================================================\n\n");
    
    printf("--- ANTES DA ORDENACAO ---\n");
    imprimir_jogadores(jogadores_originais, n_jog);
    
    // Executa a ordenação alfabética
    insertion_sort_jogadores_alfabetico(jogadores_copia_alfabetica, n_jog, &metricas_alfabetica);
    
    printf("\n--- DEPOIS DA ORDENACAO (Ordem Alfabetica + Desempate por Pontos) ---\n");
    imprimir_jogadores(jogadores_copia_alfabetica, n_jog);
    
    printf("\nMetricas: %d comparacoes, %d deslocamentos\n", metricas_alfabetica.comparacoes, metricas_alfabetica.deslocamentos);
    
    if (validar_ordenacao_alfabetica(jogadores_copia_alfabetica, n_jog)) {
        printf("Validacao: SUCESSO! A estrutura e os dados permaneceram integros.\n");
    } else {
        printf("Validacao: FALHA!\n");
    }

    return 0;
}