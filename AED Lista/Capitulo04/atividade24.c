#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_ARQUIVO_BIN "partidas.bin"
#define TAM_NOME 50

// Estrutura serializável: apenas dados diretos (sem ponteiros)
typedef struct {
    int id;
    char jogador[TAM_NOME];
    int pontuacao;
} Partida;

// Grava o array de estruturas em modo binario ("wb")
int salvar_partidas_binario(const char *caminho, const Partida *partidas, size_t quantidade) {
    FILE *arquivo = fopen(caminho, "wb");
    if (arquivo == NULL) {
        perror("[ERRO] Falha ao abrir arquivo binario para escrita");
        return 0;
    }

    // fwrite retorna o número de elementos gravados com sucesso
    size_t gravados = fwrite(partidas, sizeof(Partida), quantidade, arquivo);
    
    fclose(arquivo);

    if (gravados != quantidade) {
        printf("[ERRO] Gravacao incompleta: esperava %zu elementos, gravou %zu.\n", quantidade, gravados);
        return 0;
    }

    printf("Sucesso: %zu registro(s) gravado(s) em '%s'.\n", gravados, caminho);
    return 1;
}

// Le e reconstroi os objetos em modo binario ("rb")
int carregar_e_exibir_binario(const char *caminho) {
    FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        perror("[ERRO] Falha ao abrir arquivo binario para leitura");
        return 0;
    }

    Partida p;
    size_t lidos;
    int contador = 0;

    printf("\n=== OBJETOS RECONSTRUIDOS DO BINARIO ===\n");

    // fread tenta ler 1 bloco de sizeof(Partida) por vez
    while ((lidos = fread(&p, sizeof(Partida), 1, arquivo)) == 1) {
        contador++;
        printf("[%d] ID: %-4d | Jogador: %-15s | Pontuacao: %d\n", 
               contador, p.id, p.jogador, p.pontuacao);
    }

    // Tratamento de interrupção: verifica se houve leitura incompleta/corrompida ou fim de arquivo
    if (ferror(arquivo)) {
        perror("[ERRO] Erro de leitura detectado durante o acesso ao arquivo");
    } else if (!feof(arquivo) && lidos > 0 && lidos < 1) {
        printf("[ERRO] Registro incompleto detectado no final do arquivo (tamanho corrompido).\n");
    } else if (contador == 0) {
        printf("[AVISO] O arquivo binario esta vazio.\n");
    } else {
        printf("----------------------------------------\n");
        printf("Total de registros restaurados com sucesso: %d\n", contador);
    }

    fclose(arquivo);
    return 1;
}

int main(void) {
    // Registros originais para teste
    Partida partidas_originais[] = {
        {501, "Samuel", 4500},
        {502, "Lucas", 3800},
        {503, "Beatriz", 5100}
    };
    size_t total = sizeof(partidas_originais) / sizeof(partidas_originais[0]);

    printf("--- GRAVANDO DADOS BINARIOS ---\n");
    salvar_partidas_binario(NOME_ARQUIVO_BIN, partidas_originais, total);

    printf("\n--- RESTAURANDO DADOS BINARIOS ---\n");
    carregar_e_exibir_binario(NOME_ARQUIVO_BIN);

    return 0;
}