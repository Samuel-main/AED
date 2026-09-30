#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Personagem
{
    int id;
    char nome[30];
    int vida;
    int pontuacao;
    int posicao;
} Personagem;

Personagem criarPersonagem(int id, const char *nome, int vida, int pontuacao, int posicao)
{
    Personagem p;

    p.id = id;
    snprintf(p.nome, sizeof(p.nome), "%s", nome);
    p.vida = vida;
    p.pontuacao = pontuacao;
    p.posicao = posicao;

    return p;
}

void trocarNome(Personagem *p, const char *novoNome)
{
    if (p == NULL || novoNome == NULL)
        return;

    snprintf(p->nome, sizeof(p->nome), "%s", novoNome);
}

int calcularForcaTotal(Personagem p)
{
    return p.vida + p.pontuacao + (p.posicao * 10);
}

void aumentarPontuacao(Personagem p)
{
    p.pontuacao += 50;
    printf("Dentro da funcao por valor: pontuacao = %d\n", p.pontuacao);
}

void imprimirVetor(const int *v, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int intercalarVetoresOrdenados(const int *v1, int n1, const int *v2, int n2, int *saida)
{
    int i = 0, j = 0, k = 0;
    int comparacoes = 0;

    while (i < n1 && j < n2)
    {
        comparacoes++;
        if (v1[i] <= v2[j])
        {
            saida[k++] = v1[i++];
        }
        else
        {
            saida[k++] = v2[j++];
        }
    }

    while (i < n1)
    {
        saida[k++] = v1[i++];
    }

    while (j < n2)
    {
        saida[k++] = v2[j++];
    }

    printf("Comparacoes da intercalacao: %d\n", comparacoes);
    return k;
}

void mergeSortRecursivo(int *v, int inicio, int fim, int *comparacoes)
{
    /* dividir: separa o intervalo em duas metades */
    if (inicio >= fim)
    {
        return;
    }

    int meio = inicio + (fim - inicio) / 2;

    /* resolver: ordena cada metade recursivamente */
    mergeSortRecursivo(v, inicio, meio, comparacoes);
    mergeSortRecursivo(v, meio + 1, fim, comparacoes);

    /* combinar: nesta etapa a uniao das metades seria feita por intercalacao */
    printf("Dividir intervalo [%d..%d] em [%d..%d] e [%d..%d]\n",
           inicio, fim, inicio, meio, meio + 1, fim);
    printf("Ponto medio calculado: %d\n", meio);
    printf("Etapa de combinacao: a ordenacao completa sera concluida na proxima atividade.\n");
}

void receberDano(Personagem *p, int dano)
{
    if (p == NULL)
    {
        printf("Erro: ponteiro nulo.\n");
        return;
    }

    if (dano < 0)
    {
        printf("Erro: dano invalido.\n");
        return;
    }

    if (dano > p->vida)
        dano = p->vida;

    p->vida -= dano;
    printf("Vida atual apos receber dano: %d\n", p->vida);
}

void avancarNoMapa(Personagem *p, int passos)
{
    if (p == NULL)
    {
        printf("Erro: ponteiro nulo.\n");
        return;
    }

    if (passos < 0)
    {
        printf("Erro: numero de passos invalido.\n");
        return;
    }

    p->posicao += passos;
    printf("Posicao atual apos avancar: %d\n", p->posicao);
}

void adicionarPontos(Personagem *p, int pontos)
{
    if (p == NULL)
    {
        printf("Erro: ponteiro nulo.\n");
        return;
    }

    if (pontos < 0)
    {
        printf("Erro: quantidade de pontos invalida.\n");
        return;
    }

    p->pontuacao += pontos;
    printf("Pontuacao atual apos adicionar pontos: %d\n", p->pontuacao);
}

int main(void)
{
    size_t capacidade = 5;
    size_t nova_capacidade;
    int *catalogo = NULL;
    int *temporario = NULL;

    Personagem heroi = {1, "Aria", 100, 0, 1};
    Personagem outro = {.id = 2, .nome = "Breno", .vida = 80, .pontuacao = 50, .posicao = 2};
    Personagem personagem = criarPersonagem(3, "Cleo", 90, 120, 3);
    char nomeNovo[30];

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

    printf("\nInicializacao posicional: %d, %s, %d, %d, %d\n", heroi.id, heroi.nome, heroi.vida, heroi.pontuacao, heroi.posicao);
    printf("Inicializacao designada: %d, %s, %d, %d, %d\n", outro.id, outro.nome, outro.vida, outro.pontuacao, outro.posicao);
    printf("Construtor: %d, %s, %d, %d, %d\n\n", personagem.id, personagem.nome, personagem.vida, personagem.pontuacao, personagem.posicao);

    printf("Forca total do personagem: %d\n", calcularForcaTotal(personagem));
    printf("Pontuacao antes da copia local: %d\n", personagem.pontuacao);
    aumentarPontuacao(personagem);
    printf("Pontuacao apos a alteracao na copia local: %d\n", personagem.pontuacao);

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

    printf("\nAntes das alteracoes no personagem principal:\n");
    printf("Vida: %d\n", personagem.vida);
    printf("Pontuacao: %d\n", personagem.pontuacao);
    printf("Posicao: %d\n", personagem.posicao);

    receberDano(&personagem, 15);
    avancarNoMapa(&personagem, 3);
    adicionarPontos(&personagem, 25);

    printf("\nDepois das alteracoes no personagem principal:\n");
    printf("Vida: %d\n", personagem.vida);
    printf("Pontuacao: %d\n", personagem.pontuacao);
    printf("Posicao: %d\n", personagem.posicao);

    printf("\nEquivalencia com (*p).membro: (*personagem).vida = %d\n", (*(&personagem)).vida);

    printf("\nDigite o novo nome para o personagem: ");
    if (scanf("%29s", nomeNovo) != 1)
    {
        printf("Entrada invalida.\n");
        free(catalogo);
        return 1;
    }

    trocarNome(&personagem, nomeNovo);

    printf("\nRegistro completo apos a alteracao:\n");
    printf("ID: %d\n", personagem.id);
    printf("Nome: %s\n", personagem.nome);
    printf("Vida: %d\n", personagem.vida);
    printf("Pontuacao: %d\n", personagem.pontuacao);
    printf("Posicao: %d\n", personagem.posicao);

    printf("\nComentario: as funcoes receberDano, avancarNoMapa e adicionarPontos recebem ponteiro para Personagem e alteram o registro principal.\n");
    printf("A sintaxe p->campo e equivalente a (*p).campo; o uso com ponteiro permite mudar o valor original sem copiar a estrutura inteira.\n");

    printf("\n--- Demonstração de intercalação de vetores ordenados ---\n");
    {
        int v1[] = {1, 3, 5, 7, 9};
        int v2[] = {2, 4, 6, 8};
        int saida[20];
        int total = intercalarVetoresOrdenados(v1, 5, v2, 4, saida);

        printf("Vetor 1: ");
        imprimirVetor(v1, 5);
        printf("Vetor 2: ");
        imprimirVetor(v2, 4);
        printf("Resultado intercalado: ");
        imprimirVetor(saida, total);
    }

    printf("\n--- Estrutura recursiva do merge sort ---\n");
    {
        int v3[] = {9, 4, 7, 1, 3, 8, 5, 2};
        int comparacoes = 0;

        printf("Vetor original: ");
        imprimirVetor(v3, 8);
        mergeSortRecursivo(v3, 0, 7, &comparacoes);
        printf("Ponto medio calculado: %d\n", 0 + (7 - 0) / 2);
        printf("Todos os elementos do vetor: ");
        imprimirVetor(v3, 8);
        printf("Contador de comparacoes da estrutura recursiva: %d\n", comparacoes);
    }

    free(catalogo);
    puts("\nVetor liberado antes de encerrar o programa.");

    return 0;
}
