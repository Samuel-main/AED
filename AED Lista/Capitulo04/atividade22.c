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

// Adiciona uma nova partida ao final do arquivo existente (Modo de anexação "a")
int anexar_partida(const char *caminho, const Partida *p) {
    FILE *arquivo = fopen(caminho, "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para anexacao");
        return 0;
    }

    fprintf(arquivo, "%d;%s;%d\n", p->id, p->jogador, p->pontuacao);

    fclose(arquivo);
    return 1;
}

// Percorre todo o historico, contando os registros e tratando fim de arquivo / erros
int listar_historico(const char *caminho) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("\n[AVISO] Historico nao encontrado ou ainda nao criado.\n");
        return 0;
    }

    Partida p;
    int contador = 0;
    int resultado_scan;

    printf("\n=== HISTORICO DE PARTIDAS ===\n");

    // O laço avança BASEADO no retorno da leitura, evitando duplicar o ultimo registro
    while ((resultado_scan = fscanf(arquivo, "%d;%49[^;];%d\n", &p.id, p.jogador, &p.pontuacao)) == 3) {
        contador++;
        printf("[%d] ID: %-4d | Jogador: %-15s | Pontuacao: %d\n", contador, p.id, p.jogador, p.pontuacao);
    }

    // Diferenciação entre fim normal de arquivo (EOF) e dado malformado
    if (resultado_scan != EOF) {
        printf("\n[ERRO] Leitura interrompida por dados malformatados no arquivo.\n");
    } else if (contador == 0) {
        printf("O historico esta vazio.\n");
    } else {
        printf("------------------------------------------\n");
        printf("Total de registros recuperados: %d\n", contador);
    }

    fclose(arquivo);
    return 1;
}

int main(void) {
    // Exemplo de inclusao incremental de partidas
    Partida nova_partida1 = {101, "Samuel", 3200};
    Partida nova_partida2 = {102, "Lucas", 2750};

    printf("Anexando nova partida...\n");
    anexar_partida(NOME_ARQUIVO, &nova_partida1);

    printf("Anexando outra partida...\n");
    anexar_partida(NOME_ARQUIVO, &nova_partida2);

    // Exibe todo o historico acumulado
    listar_historico(NOME_ARQUIVO);

    return 0;
}