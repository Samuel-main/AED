#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_ARQUIVO "relatorio_partidas.txt"
#define TAM_NOME 50

typedef struct {
    int id;
    char jogador[TAM_NOME];
    int pontuacao;
} Partida;

int salvar_partidas(const char *caminho, const Partida *partidas, int quantidade) {
    FILE *arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para escrita");
        return 0;
    }

    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d;%s;%d\n", partidas[i].id, partidas[i].jogador, partidas[i].pontuacao);
    }

    fclose(arquivo);
    return 1;
}

int carregar_e_exibir_partidas(const char *caminho) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para leitura");
        return 0;
    }

    Partida p;
    printf("\n=== PARTIDAS RECUPERADAS ===\n");
    
    // Leitura formatada retornando a quantidade esperada de itens lidos (3 campos)
    while (fscanf(arquivo, "%d;%49[^;];%d\n", &p.id, p.jogador, &p.pontuacao) == 3) {
        printf("ID: %-4d | Jogador: %-15s | Pontuacao: %d\n", p.id, p.jogador, p.pontuacao);
    }

    fclose(arquivo);
    return 1;
}

int main(void) {
    Partida lista_partidas[] = {
        {1, "Samuel", 1500},
        {2, "Alex", 2350},
        {3, "Beatriz", 1890}
    };
    int total = sizeof(lista_partidas) / sizeof(lista_partidas[0]);

    if (salvar_partidas(NOME_ARQUIVO, lista_partidas, total)) {
        printf("Dados salvos com sucesso em '%s'.\n", NOME_ARQUIVO);
    }

    carregar_e_exibir_partidas(NOME_ARQUIVO);

    return 0;
}