#include <stdio.h>
#include <raylib.h>

#define MAX_PLATAFORMAS 10
#define VALOR_MIN 0
#define VALOR_MAX 100

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

/* COMO FUNCIONA O DESLOCAMENTO NA MEMÓRIA:
 * Quando fazemos (vetor + i), o C é esperto: ele não avança apenas "1 byte",
 * mas sim o tamanho exato do tipo da variável.
 * Como nosso vetor é de 'int' (que ocupa 4 bytes na memória), fazer (vetor + 1)
 * faz o ponteiro pular 4 bytes para a frente, caindo perfeitamente na próxima
 * posição do vetor.
 */

void ler_mapa(int *mapa, int tamanho)
{
    if (mapa == NULL || tamanho <= 0 || tamanho > MAX_PLATAFORMAS)
    {
        printf("Erro: Tamanho do mapa invalido!\n");
        return;
    }

    printf("\n=== LEITURA DO MAPA (PROJETISTA) ===\n");
    for (int i = 0; i < tamanho; i++)
    {
        int valor;
        do
        {
            printf("Informe o valor da plataforma %d (%d a %d): ", i, VALOR_MIN, VALOR_MAX);
            scanf("%d", &valor);

            if (valor < VALOR_MIN || valor > VALOR_MAX)
            {
                printf("Valor invalido! Tente novamente.\n");
            }
        } while (valor < VALOR_MIN || valor > VALOR_MAX);

        *(mapa + i) = valor;
    }
}

void mostrar_mapa(const int *mapa, int tamanho)
{
    if (mapa == NULL || tamanho <= 0 || tamanho > MAX_PLATAFORMAS)
    {
        printf("Erro: Tamanho do mapa invalido!\n");
        return;
    }

    printf("\n=== REVISAO DO MAPA (EXIBICAO) ===\n");
    for (int i = 0; i < tamanho; i++)
    {
        printf("Plataforma %d | Endereco: %p | Valor: %d\n", i, (void *)(mapa + i), *(mapa + i));
    }
}

int main()
{
    int vida = 100;
    int ponto = 250;
    int tamanho_mapa;

    printf("\n**ANTES**\n");
    printf("Vida: %d | Endereco da vida na main: %p\n", vida, (void *)&vida);
    printf("Pontuacao: %d | Endereco da pontuacao na main: %p\n\n", ponto, (void *)&ponto);

    printf("\n**FUNCOES**\n");
    aplicar_dano(&vida, 30);
    restauracao_vida(&vida);
    aplicar_ponto(&ponto);

    do
    {
        printf("\nProjetista, digite a quantidade de plataformas (1 a %d): ", MAX_PLATAFORMAS);
        scanf("%d", &tamanho_mapa);

        if (tamanho_mapa <= 0 || tamanho_mapa > MAX_PLATAFORMAS)
        {
            printf("Tamanho fora dos limites permitidos! Tente novamente.\n");
        }
    } while (tamanho_mapa <= 0 || tamanho_mapa > MAX_PLATAFORMAS);

    int plataformas[MAX_PLATAFORMAS];

    ler_mapa(plataformas, tamanho_mapa);
    mostrar_mapa(plataformas, tamanho_mapa);

    printf("\n=== EXPLORACAO COMPLETA DAS PLATAFORMAS ===\n");
    int altura_total = 0;

    for (int i = 0; i < tamanho_mapa; i++)
    {
        int valor_atual = *(plataformas + i);
        ponto += valor_atual;
        altura_total += valor_atual;

        printf("Indice: %d | Endereco: %p | Impulso/Ponto: %d\n",
               i, (void *)(plataformas + i), valor_atual);
    }

    printf("\n**ESTADO FINAL**\n");
    printf("Vida final: %d\n", vida);
    printf("Pontuacao final acumulada: %d\n", ponto);
    printf("Altura total percorrida: %d\n", altura_total);

    return 0;
}