#include <stdio.h>

void aplicar_dano(int *vida, int dano)
{
    if (vida != NULL)
    {
        *vida -= dano;
        printf("Endereco recebido: %p | vida com dano: %d\n", (void *)vida, *vida);
    }
}
void restauracao_vida(int *vida)
{
    if (vida != NULL)
    {
        printf("\nCura +30 de vida\n");
        *vida = 100;
        printf("Endereco recebido: %p | Nova vida: %d\n", (void *)vida, *vida);
    }
}
void aplicar_ponto(int *ponto)
{
    if (ponto != NULL)
    {
        *ponto *= 2;
        printf("Endereco recebido: %p | pontuacao dobrada: %d\n", (void *)ponto, *ponto);
    }
}
int main()
{
    int vida = 100;
    int ponto = 250;

    printf("\n**ANTES**\n");
    printf("Vida: %d | Endereco da vida na main: %p\n", vida, (void *)&vida);
    printf("Pontuacao: %d | Endereco da pontuacao na main: %p\n\n", ponto, (void *)&ponto);

    printf("\n**FUNCOES**\n");
    aplicar_dano(&vida, 30);
    restauracao_vida(&vida);
    aplicar_ponto(&ponto);

    printf("\n**ESTADO FINAL**\n");
    printf("Vida na main (atualizada): %d\n", vida);
    printf("Pontuacao na main (atualizada): %d\n", ponto);

    return 0;
}