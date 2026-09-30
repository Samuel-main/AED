#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t capacidade = 5;
    size_t nova_capacidade;
    int *catalogo = NULL;
    int *temporario = NULL;

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

    free(catalogo);
    puts("\nVetor liberado antes de encerrar o programa.");

    return 0;
}
