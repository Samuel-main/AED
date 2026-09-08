#include <stdio.h>

int main()
{
    int vida = 100;
    int tesouro = 0;
    int *pont_vida, *pont_tesouro;

    pont_vida = &vida;
    pont_tesouro = &tesouro;

    printf("\n**ANTES**\n");
    printf("Vida: %d | Endereco: %p | Via Ponteiro: %d\n", vida, (void *)&vida, *pont_vida);
    printf("Tesouro: %d | Endereco: %p | Via Ponteiro: %d\n\n", tesouro, (void *)&tesouro, *pont_tesouro);

    vida += -20;
    printf("\nDano recebido | Estado de Vida: %d\n\n", vida);

    if (vida < 100)
    {
        *pont_vida += 20;
    }

    *pont_tesouro = 1;
    printf("\n**DEPOIS**\n");
    printf("Vida Restaurada: %d | Tesouro: %d\n", *pont_vida, *pont_tesouro);

    return 0;
}