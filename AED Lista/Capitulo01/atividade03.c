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

    printf("\n\n=== EXPLORACAO DAS PLATAFORMAS ===\n");

    /* COMO FUNCIONA O DESLOCAMENTO NA MEMÓRIA:
     * Quando fazemos (vetor + i), o C é esperto: ele não avança apenas "1 byte",
     * mas sim o tamanho exato do tipo da variável.
     * Como nosso vetor é de 'int' (que ocupa 4 bytes na memória), fazer (vetor + 1)
     * faz o ponteiro pular 4 bytes para a frente, caindo perfeitamente na próxima
     * posição do vetor.
     */

    int plataformas[5] = {10, 20, 30, 40, 50};
    int altura_total = 0;
    int i;
    int valor_atual, ;

    for (i = 0; i < 5; i++)
    {

        valor_atual = *(plataformas + i);

        ponto += valor_atual;
        altura_total += valor_atual;

        printf("Indice: %d | Endereco: %p | Impulso/Ponto: %d\n", i, (void *)(plataformas + i), *(plataformas + i));
    }

    printf("\n**ESTADO FINAL**\n");
    printf("Vida final: %d\n", vida);
    printf("Pontuacao final acumulada: %d\n", ponto);
    printf("Altura total percorrida: %d\n", altura_total);

    return 0;
}