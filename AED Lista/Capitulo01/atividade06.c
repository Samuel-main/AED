#include <stdio.h>
#include <raylib.h>

#define MAX_PLATAFORMAS 10
#define VALOR_MIN 0
#define VALOR_MAX 100
#define TOTAL_ITENS 3

typedef struct
{
    char nome[40];
    int quantidade;
} Item;

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

void mostrar_item_indice(Item *inventario[], int indice)
{
    if (inventario == NULL || indice < 0 || indice >= TOTAL_ITENS || inventario[indice] == NULL)
    {
        printf("Item invalido.\n");
        return;
    }

    printf("Item %d: %s | Quantidade: %d | Endereco: %p\n",
           indice, inventario[indice]->nome, inventario[indice]->quantidade,
           (void *)inventario[indice]);
}

void mostrar_item_ponteiro(Item *inventario[], int indice)
{
    if (inventario == NULL || indice < 0 || indice >= TOTAL_ITENS || *(inventario + indice) == NULL)
    {
        printf("Item invalido.\n");
        return;
    }

    Item *item = *(inventario + indice);
    printf("Item %d: %s | Quantidade: %d | Endereco: %p\n",
           indice, item->nome, item->quantidade, (void *)item);
}

void alterar_item_indice(Item *inventario[], int indice)
{
    if (inventario == NULL || indice < 0 || indice >= TOTAL_ITENS || inventario[indice] == NULL)
    {
        printf("Item invalido.\n");
        return;
    }

    printf("Nova quantidade de %s: ", inventario[indice]->nome);
    if (scanf("%d", &inventario[indice]->quantidade) != 1 || inventario[indice]->quantidade < 0)
    {
        printf("Quantidade invalida.\n");
        inventario[indice]->quantidade = 0;
    }
}

void alterar_item_ponteiro(Item *inventario[], int indice)
{
    if (inventario == NULL || indice < 0 || indice >= TOTAL_ITENS || *(inventario + indice) == NULL)
    {
        printf("Item invalido.\n");
        return;
    }

    Item *item = *(inventario + indice);
    printf("Nova quantidade de %s: ", item->nome);
    if (scanf("%d", &item->quantidade) != 1 || item->quantidade < 0)
    {
        printf("Quantidade invalida.\n");
        item->quantidade = 0;
    }
}

void mostrar_inventario(Item *inventario[])
{
    printf("\n=== INVENTARIO ===\n");
    for (int i = 0; i < TOTAL_ITENS; i++)
    {
        mostrar_item_indice(inventario, i);
    }
}

int ler_indice_item(void)
{
    int indice;
    printf("Indice do item (0 a %d): ", TOTAL_ITENS - 1);
    if (scanf("%d", &indice) != 1 || indice < 0 || indice >= TOTAL_ITENS)
    {
        printf("Indice invalido.\n");
        return -1;
    }
    return indice;
}

void explorar_mapa(const int *mapa, int tamanho, int *ponto)
{
    if (mapa == NULL || tamanho <= 0 || tamanho > MAX_PLATAFORMAS || ponto == NULL)
    {
        printf("Mapa ou pontuacao invalido.\n");
        return;
    }

    int altura_total = 0;
    int posicao_logica = 0;
    const int *cursor = mapa;
    const int *fim_mapa = mapa + tamanho;

    printf("\n=== EXPLORACAO DO MAPA ===\n");
    while (cursor < fim_mapa)
    {
        int valor_atual = *cursor;
        *ponto += valor_atual;
        altura_total += valor_atual;
        printf("Posicao %d | Valor: %d | Pontuacao: %d\n",
               posicao_logica, valor_atual, *ponto);
        cursor++;
        posicao_logica++;
    }

    printf("Percurso concluido: %d plataformas | Altura total: %d | Pontuacao: %d\n",
           posicao_logica, altura_total, *ponto);
}

int main()
{
    int vida = 100;
    int ponto = 250;
    int tamanho_mapa;
    Item pocao = {"Pocao de cura", 2};
    Item chave = {"Chave antiga", 1};
    Item moeda = {"Moeda de ouro", 5};
    Item *inventario[TOTAL_ITENS] = {&pocao, &chave, &moeda};

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

    int opcao = -1;
    while (opcao != 0)
    {
        printf("\n=== SIMULADOR ===\n");
        printf("1. Vida\n2. Pontuacao\n3. Mapa\n4. Itens\n0. Sair\nEscolha: ");
        if (scanf("%d", &opcao) != 1)
        {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }

        if (opcao == 1)
        {
            int acao;
            printf("Vida atual: %d\n1. Receber dano\n2. Restaurar vida\nEscolha: ", vida);
            if (scanf("%d", &acao) != 1)
            {
                printf("Acao invalida.\n");
                break;
            }
            if (acao == 1)
            {
                int dano;
                printf("Dano recebido: ");
                if (scanf("%d", &dano) == 1 && dano >= 0)
                    aplicar_dano(&vida, dano);
                else
                    printf("Dano invalido.\n");
            }
            else if (acao == 2)
                restauracao_vida(&vida);
            else
                printf("Acao invalida.\n");
        }
        else if (opcao == 2)
        {
            int acao;
            printf("Pontuacao atual: %d\n1. Adicionar pontos\n2. Dobrar pontuacao\nEscolha: ", ponto);
            if (scanf("%d", &acao) != 1)
            {
                printf("Acao invalida.\n");
                break;
            }
            if (acao == 1)
            {
                int valor;
                printf("Pontos a adicionar: ");
                if (scanf("%d", &valor) == 1 && valor >= 0)
                    ponto += valor;
                else
                    printf("Valor invalido.\n");
            }
            else if (acao == 2)
                aplicar_ponto(&ponto);
            else
                printf("Acao invalida.\n");
            printf("Pontuacao: %d\n", ponto);
        }
        else if (opcao == 3)
        {
            int acao;
            printf("1. Mostrar mapa\n2. Explorar mapa\nEscolha: ");
            if (scanf("%d", &acao) != 1)
            {
                printf("Acao invalida.\n");
                break;
            }
            if (acao == 1)
                mostrar_mapa(plataformas, tamanho_mapa);
            else if (acao == 2)
                explorar_mapa(plataformas, tamanho_mapa, &ponto);
            else
                printf("Acao invalida.\n");
        }
        else if (opcao == 4)
        {
            int acao;
            mostrar_inventario(inventario);
            printf("1. Consultar por indice\n2. Consultar por ponteiro\n"
                   "3. Alterar por indice\n4. Alterar por ponteiro\n0. Voltar\nEscolha: ");
            if (scanf("%d", &acao) != 1)
            {
                printf("Acao invalida.\n");
                break;
            }
            if (acao >= 1 && acao <= 4)
            {
                int indice = ler_indice_item();
                if (indice >= 0)
                {
                    if (acao == 1)
                        mostrar_item_indice(inventario, indice);
                    else if (acao == 2)
                        mostrar_item_ponteiro(inventario, indice);
                    else if (acao == 3)
                        alterar_item_indice(inventario, indice);
                    else
                        alterar_item_ponteiro(inventario, indice);
                }
            }
            else if (acao != 0)
                printf("Acao invalida.\n");
        }
        else if (opcao != 0)
            printf("Opcao invalida.\n");
    }

    printf("\nEstado final | Vida: %d | Pontuacao: %d\n", vida, ponto);

    return 0;
}