#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Posicao
{
    int x;
    int y;
} Posicao;

typedef enum
{
    CLASSE_GUERREIRO,
    CLASSE_MAGO,
    CLASSE_ARQUEIRO,
    CLASSE_CURANDEIRO
} Classe;

typedef struct Personagem
{
    int id;
    char nome[30];
    Posicao posicao;
    int vida;
    int pontuacao;
    Classe classe;
} Personagem;

typedef struct Equipe
{
    char nome[30];
    Personagem integrantes[10];
    int total;
} Equipe;

typedef struct Catalogo
{
    Equipe equipes[5];
    int total;
} Catalogo;

const char *classeParaTexto(Classe classe)
{
    switch (classe)
    {
        case CLASSE_GUERREIRO:
            return "Guerreiro";
        case CLASSE_MAGO:
            return "Mago";
        case CLASSE_ARQUEIRO:
            return "Arqueiro";
        case CLASSE_CURANDEIRO:
            return "Curandeiro";
        default:
            return "Desconhecida";
    }
}

Personagem criarPersonagem(int id, const char *nome, int vida, int pontuacao, int x, int y, Classe classe)
{
    Personagem p;

    p.id = id;
    snprintf(p.nome, sizeof(p.nome), "%s", nome);
    p.posicao.x = x;
    p.posicao.y = y;
    p.vida = vida;
    p.pontuacao = pontuacao;
    p.classe = classe;

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
    return p.vida + p.pontuacao + (p.posicao.x * 10) + (p.posicao.y * 5);
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

    p->posicao.x += passos;
    printf("Posicao atual apos avancar: (%d, %d)\n", p->posicao.x, p->posicao.y);
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

    Personagem heroi = {1, "Aria", {3, 4}, 100, 0, CLASSE_GUERREIRO};
    Personagem outro = {.id = 2, .nome = "Breno", .posicao = {5, 6}, .vida = 80, .pontuacao = 50, .classe = CLASSE_ARQUEIRO};
    Personagem personagem = criarPersonagem(3, "Cleo", 90, 120, 2, 3, CLASSE_MAGO);
    char nomeNovo[30];
    Catalogo catalogoEquipe = {0};
    Equipe equipe = {"Guardioes", {{0}}, 0};
    int opcao;

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

    printf("\nInicializacao posicional: %d, %s, vida=%d, pontos=%d, posicao=(%d,%d), classe=%s\n",
           heroi.id, heroi.nome, heroi.vida, heroi.pontuacao, heroi.posicao.x, heroi.posicao.y, classeParaTexto(heroi.classe));
    printf("Inicializacao designada: %d, %s, vida=%d, pontos=%d, posicao=(%d,%d), classe=%s\n",
           outro.id, outro.nome, outro.vida, outro.pontuacao, outro.posicao.x, outro.posicao.y, classeParaTexto(outro.classe));
    printf("Construtor: %d, %s, vida=%d, pontos=%d, posicao=(%d,%d), classe=%s\n\n",
           personagem.id, personagem.nome, personagem.vida, personagem.pontuacao, personagem.posicao.x, personagem.posicao.y, classeParaTexto(personagem.classe));

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
    printf("Posicao: (%d, %d)\n", personagem.posicao.x, personagem.posicao.y);

    receberDano(&personagem, 15);
    avancarNoMapa(&personagem, 3);
    adicionarPontos(&personagem, 25);

    printf("\nDepois das alteracoes no personagem principal:\n");
    printf("Vida: %d\n", personagem.vida);
    printf("Pontuacao: %d\n", personagem.pontuacao);
    printf("Posicao: (%d, %d)\n", personagem.posicao.x, personagem.posicao.y);

    printf("\nEquivalencia com (*p).membro: (*(&personagem)).vida = %d\n", (*(&personagem)).vida);

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
    printf("Posicao: (%d, %d)\n", personagem.posicao.x, personagem.posicao.y);
    printf("Classe: %s\n", classeParaTexto(personagem.classe));

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

    printf("\n--- Menu do catálogo e da equipe ---\n");
    strcpy(equipe.nome, "Guardioes");
    equipe.total = 0;
    equipe.integrantes[equipe.total++] = heroi;
    equipe.integrantes[equipe.total++] = personagem;
    catalogoEquipe.equipes[catalogoEquipe.total++] = equipe;

    do
    {
        printf("1 - Cadastrar personagem\n");
        printf("2 - Buscar personagem\n");
        printf("3 - Alterar personagem\n");
        printf("4 - Listar equipe\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
            {
                Personagem novo = criarPersonagem(4, "Nyra", 95, 200, 8, 9, CLASSE_CURANDEIRO);
                equipe.integrantes[equipe.total++] = novo;
                printf("Cadastro realizado: %s\n", novo.nome);
                break;
            }
            case 2:
            {
                int idBusca;
                printf("Digite o ID para buscar: ");
                scanf("%d", &idBusca);
                for (int i = 0; i < equipe.total; i++)
                {
                    if (equipe.integrantes[i].id == idBusca)
                    {
                        printf("Encontrado: %s, classe=%s\n", equipe.integrantes[i].nome, classeParaTexto(equipe.integrantes[i].classe));
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                int idAlterar;
                printf("Digite o ID do personagem para alterar: ");
                scanf("%d", &idAlterar);
                for (int i = 0; i < equipe.total; i++)
                {
                    if (equipe.integrantes[i].id == idAlterar)
                    {
                        snprintf(equipe.integrantes[i].nome, sizeof(equipe.integrantes[i].nome), "%s", "NovaAria");
                        equipe.integrantes[i].vida = 110;
                        equipe.integrantes[i].pontuacao += 25;
                        equipe.integrantes[i].posicao.x += 2;
                        equipe.integrantes[i].posicao.y += 1;
                        printf("Alterado com sucesso: %s\n", equipe.integrantes[i].nome);
                        break;
                    }
                }
                break;
            }
            case 4:
            {
                printf("Equipe: %s\n", equipe.nome);
                for (int i = 0; i < equipe.total; i++)
                {
                    printf("- %s | vida=%d | pontuacao=%d | posicao=(%d,%d) | classe=%s\n",
                           equipe.integrantes[i].nome,
                           equipe.integrantes[i].vida,
                           equipe.integrantes[i].pontuacao,
                           equipe.integrantes[i].posicao.x,
                           equipe.integrantes[i].posicao.y,
                           classeParaTexto(equipe.integrantes[i].classe));
                }
                break;
            }
            case 0:
                printf("Saindo do menu.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    free(catalogo);
    puts("\nVetor liberado antes de encerrar o programa.");

    return 0;
}
