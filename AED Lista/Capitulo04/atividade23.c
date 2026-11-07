#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_ORIGEM "historico_partidas.txt"
#define ARQUIVO_DESTINO "relatorio_temporada.txt"
#define TAM_NOME 50
#define FATOR_BONUS 1.20 // Aplicando 20% de bônus na pontuação

typedef struct {
    int id;
    char jogador[TAM_NOME];
    int pontuacao;
} Partida;

int processar_relatorio_temporada(const char *origem_path, const char *destino_path) {
    FILE *origem = fopen(origem_path, "r");
    if (origem == NULL) {
        perror("Erro ao abrir arquivo de origem");
        printf("[FALHA] Nao foi possivel acessar o arquivo de origem '%s'. Operacao cancelada.\n", origem_path);
        return 0;
    }

    FILE *destino = fopen(destino_path, "w");
    if (destino == NULL) {
        perror("Erro ao abrir arquivo de destino");
        printf("[FALHA] Nao foi possivel criar ou abrir o arquivo de destino '%s'.\n", destino_path);
        
        // Garante o fechamento do arquivo de origem já aberto antes de sair
        fclose(origem);
        return 0;
    }

    Partida p;
    int processados = 0;
    int resultado_scan;

    // Cabeçalho do arquivo gerado
    fprintf(destino, "ID;JOGADOR;PONTUACAO_BASE;PONTUACAO_FINAL\n");

    // Processa cada registro APENAS quando a leitura for completa (3 campos lidos com sucesso)
    while ((resultado_scan = fscanf(origem, "%d;%49[^;];%d\n", &p.id, p.jogador, &p.pontuacao)) == 3) {
        // Aplicação da regra de bonificação
        int pontuacao_final = (int)(p.pontuacao * FATOR_BONUS);

        // Gravação do dado transformado no arquivo de destino
        fprintf(destino, "%d;%s;%d;%d\n", p.id, p.jogador, p.pontuacao, pontuacao_final);
        processados++;
    }

    // Verificação de inconsistências ao parar a leitura
    if (resultado_scan != EOF) {
        printf("[AVISO] Leitura interrompida antes do fim: dado malformado encontrado no arquivo de origem.\n");
    }

    // Fechamento dos dois recursos no caminho de sucesso
    fclose(origem);
    fclose(destino);

    printf("\n=== PROCESSAMENTO CONCLUIDO ===\n");
    printf("Registros transformados e salvos com sucesso: %d\n", processados);
    printf("Relatorio final gravado em: '%s'\n", destino_path);

    return 1;
}

int main(void) {
    // Executa a transformação entre o arquivo bruto e o relatório processado
    processar_relatorio_temporada(ARQUIVO_ORIGEM, ARQUIVO_DESTINO);

    return 0;
}