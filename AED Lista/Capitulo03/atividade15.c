#include <stdio.h>
#include <stdlib.h>

struct Personagem
{
    int id;
    char nome[30];
    int vida;
    int pontuacao;
    int posicao;
};

int main(void)
{
    size_t capacidade = 3;
    size_t nova_capacidade;
    int *catalogo = NULL;
    int *temporario = NULL;
    struct Personagem equipe[3];

    equipe[0] = (struct Personagem){1, "Aria", 100, 0, 1};
    equipe[1] = (struct Personagem){2, "Breno", 80, 50, 2};
    equipe[2] = (struct Personagem){3, "Cleo", 90, 120, 3};

    catalogo = (int *)calloc(capacidade, sizeof(*catalogo));
    if (catalogo == NULL)
    {
        printf("Erro ao reservar a capacidade inicial.\n");
        return 1;
    }

    printf("Capacidade inicial: %zu\n", capacidade);
    puts("Posicoes do vetor apos a alocacao com calloc:");
    for (size_t i = 0; i < capacidade; i++)
    {
        printf("catalogo[%zu] = %d\n", i, catalogo[i]);
    }

    printf("\nAntes das alteracoes:\n");
    for (size_t i = 0; i < 3; i++)
    {
        printf("ID: %d | Nome: %s | Vida: %d | Pontuacao: %d | Posicao: %d\n",
               equipe[i].id,
               equipe[i].nome,
               equipe[i].vida,
               equipe[i].pontuacao,
               equipe[i].posicao);
    }

    printf("\nDigite a nova capacidade desejada: ");
    scanf("%zu", &nova_capacidade);

    if (nova_capacidade <= 0)
    {
        printf("Nova capacidade invalida.\n");
        free(catalogo);
        return 1;
    }

    printf("\nCapacidade anterior: %zu\n", capacidade);
    printf("Nova capacidade solicitada: %zu\n", nova_capacidade);

    temporario = (int *)realloc(catalogo, nova_capacidade * sizeof(*catalogo));
    if (temporario == NULL)
    {
        printf("Erro ao redimensionar o vetor. O bloco original foi preservado.\n");
    }
    else
    {
        catalogo = temporario;

        if (nova_capacidade > capacidade)
        {
            for (size_t i = capacidade; i < nova_capacidade; i++)
            {
                catalogo[i] = 0;
            }
            puts("Posicoes acrescentadas inicializadas com zero.");
        }
        else
        {
            puts("Capacidade reduzida com sucesso.");
        }

        capacidade = nova_capacidade;
        printf("Nova capacidade: %zu\n", capacidade);
    }

    for (size_t i = 0; i < 3; i++)
    {
        if (equipe[i].vida > 0)
            equipe[i].vida -= 10;

        if (equipe[i].pontuacao >= 0)
            equipe[i].pontuacao += 25;

        if (equipe[i].posicao >= 1 && equipe[i].posicao <= 10)
            equipe[i].posicao += 1;
        else
            equipe[i].posicao = 1;
    }

    printf("\nDepois das alteracoes:\n");
    for (size_t i = 0; i < 3; i++)
    {
        printf("ID: %d | Nome: %s | Vida: %d | Pontuacao: %d | Posicao: %d\n",
               equipe[i].id,
               equipe[i].nome,
               equipe[i].vida,
               equipe[i].pontuacao,
               equipe[i].posicao);
    }

    free(catalogo);
    puts("\nVetor liberado antes de encerrar o programa.");

    return 0;
}
