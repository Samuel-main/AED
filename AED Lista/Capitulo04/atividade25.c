#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_ARQUIVO_VETOR "temporada_completa.bin"
#define TAM_NOME 50

typedef struct {
    int id;
    char jogador[TAM_NOME];
    int pontuacao;
} Partida;

// Salva o número total de partidas no início e o vetor completo em seguida
int salvar_temporada(const char *caminho, const Partida *partidas, int quantidade) {
    if (partidas == NULL && quantidade > 0) {
        printf("[ERRO] Ponteiro de partidas invalido.\n");
        return 0;
    }

    FILE *arquivo = fopen(caminho, "wb");
    if (arquivo == NULL) {
        perror("[ERRO] Nao foi possivel abrir o arquivo para escrita");
        return 0;
    }

    // 1. Grava os metadados (quantidade de registros)
    if (fwrite(&quantidade, sizeof(int), 1, arquivo) != 1) {
        perror("[ERRO] Falha ao gravar a quantidade de partidas");
        fclose(arquivo);
        return 0;
    }

    // 2. Grava todo o vetor em uma única operação
    if (quantidade > 0) {
        size_t gravados = fwrite(partidas, sizeof(Partida), (size_t)quantidade, arquivo);
        if (gravados != (size_t)quantidade) {
            printf("[ERRO] Gravacao incompleta: esperava %d registros, gravou %zu.\n", quantidade, gravados);
            fclose(arquivo);
            return 0;
        }
    }

    fclose(arquivo);
    printf("-> Temporada salva com sucesso em '%s' (%d partidas).\n", caminho, quantidade);
    return 1;
}

// Carrega a temporada alocando memória dinâmica de forma segura
Partida *carregar_temporada(const char *caminho, int *out_quantidade) {
    *out_quantidade = 0;

    FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        perror("[ERRO] Nao foi possivel abrir o arquivo para leitura");
        return NULL;
    }

    int qtd_lida = 0;
    // 1. Leitura e validação dos metadados
    if (fread(&qtd_lida, sizeof(int), 1, arquivo) != 1) {
        printf("[ERRO] Falha ao ler o cabecalho ou arquivo vazio.\n");
        fclose(arquivo);
        return NULL;
    }

    if (qtd_lida < 0) {
        printf("[ERRO] Metadado invalido no arquivo (quantidade negativa: %d).\n", qtd_lida);
        fclose(arquivo);
        return NULL;
    }

    if (qtd_lida == 0) {
        printf("[AVISO] O arquivo indica uma temporada sem partidas.\n");
        fclose(arquivo);
        return NULL;
    }

    // 2. Alocação de memória temporária
    Partida *buffer_temp = (Partida *)malloc((size_t)qtd_lida * sizeof(Partida));
    if (buffer_temp == NULL) {
        printf("[ERRO] Falha de alocacao de memoria para %d registros.\n", qtd_lida);
        fclose(arquivo);
        return NULL;
    }

    // 3. Leitura em bloco de todas as estruturas
    size_t lidos = fread(buffer_temp, sizeof(Partida), (size_t)qtd_lida, arquivo);
    if (lidos != (size_t)qtd_lida) {
        printf("[ERRO] Leitura incompleta: esperava %d registros, mas leu apenas %zu.\n", qtd_lida, lidos);
        free(buffer_temp); // Libera a memória alocada antes de sair
        fclose(arquivo);   // Fecha o recurso de arquivo
        return NULL;
    }

    fclose(arquivo);
    *out_quantidade = qtd_lida;
    printf("-> Temporada carregada com sucesso (%d partidas).\n", qtd_lida);
    return buffer_temp;
}

void listar_partidas_memoria(const Partida *partidas, int quantidade) {
    if (partidas == NULL || quantidade == 0) {
        printf("\nNenhuma partida carregada na memoria atual.\n");
        return;
    }

    printf("\n=== PARTIDAS NA MEMORIA ATUAL (%d) ===\n", quantidade);
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] ID: %-4d | Jogador: %-15s | Pontuacao: %d\n",
               i + 1, partidas[i].id, partidas[i].jogador, partidas[i].pontuacao);
    }
    printf("---------------------------------------\n");
}

int main(void) {
    Partida *vetor_partidas = NULL;
    int total_partidas = 0;
    int opcao = 0;

    do {
        printf("\n========== MENU TEMPORADA ==========\n");
        printf("1. Cadastrar partida\n");
        printf("2. Listar partidas na memoria\n");
        printf("3. Salvar temporada completa\n");
        printf("4. Carregar temporada completa\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpa buffer em caso de entrada invalida
            continue;
        }

        switch (opcao) {
            case 1: {
                Partida nova;
                printf("\n--- Cadastrar Nova Partida ---\n");
                printf("ID da partida: ");
                scanf("%d", &nova.id);
                printf("Nome do jogador: ");
                scanf(" %49[^\n]", nova.jogador);
                printf("Pontuacao: ");
                scanf("%d", &nova.pontuacao);

                // Realocação do vetor dinâmico local
                Partida *temp = (Partida *)realloc(vetor_partidas, (size_t)(total_partidas + 1) * sizeof(Partida));
                if (temp == NULL) {
                    printf("[ERRO] Memoria insuficiente para cadastrar nova partida.\n");
                } else {
                    vetor_partidas = temp;
                    vetor_partidas[total_partidas] = nova;
                    total_partidas++;
                    printf("Partida adicionada a memoria com sucesso!\n");
                }
                break;
            }
            case 2:
                listar_partidas_memoria(vetor_partidas, total_partidas);
                break;

            case 3:
                salvar_temporada(NOME_ARQUIVO_VETOR, vetor_partidas, total_partidas);
                break;

            case 4: {
                int qtd_nova = 0;
                Partida *carregado = carregar_temporada(NOME_ARQUIVO_VETOR, &qtd_nova);
                
                // Só substitui a memória local se a operação inteira obteve sucesso
                if (carregado != NULL) {
                    free(vetor_partidas); // Libera o vetor antigo
                    vetor_partidas = carregado;
                    total_partidas = qtd_nova;
                }
                break;
            }
            case 0:
                printf("\nEncerrando o gerenciador...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    // Limpeza final de memória ao sair do programa
    free(vetor_partidas);
    return 0;
}