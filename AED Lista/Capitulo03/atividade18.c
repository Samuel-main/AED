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

    free(catalogo);
    puts("\nVetor liberado antes de encerrar o programa.");

    return 0;
}
