#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define MAPA_LINHAS 5
#define MAPA_COLUNAS 5
#define MAX_JOGADORES (MAPA_LINHAS * MAPA_COLUNAS)
#define POSICAO_LIVRE -1
#define APELIDO_CAPACIDADE 32
#define EQUIPE_CAPACIDADE 48
#define NOME_EXIBICAO_CAPACIDADE 96
#define SEPARADOR " - "

typedef enum
{
	TIPO_INTEIRO,
	TIPO_TEXTO
} TipoDado;

void exibir_valor(void *valor, TipoDado tipo)
{
	if (valor == NULL)
	{
		printf("Valor indisponivel (ponteiro nulo).\n");
		return;
	}

	switch (tipo)
	{
	case TIPO_INTEIRO:
		printf("Valor inteiro: %d\n", *(int *)valor);
		break;
	case TIPO_TEXTO:
		printf("Texto: %s\n", (char *)valor);
		break;
	default:
		printf("Tipo de dado desconhecido.\n");
	}
}

int ler_texto(const char *rotulo, char *destino, size_t capacidade)
{
	printf("%s", rotulo);
	if (fgets(destino, (int)capacidade, stdin) == NULL)
	{
		printf("\nEntrada encerrada antes da conclusao do cadastro.\n");
		return -1;
	}

	size_t tamanho = strlen(destino);
	if (tamanho > 0 && destino[tamanho - 1] == '\n')
	{
		destino[tamanho - 1] = '\0';
		return 1;
	}

	int caractere = getchar();
	if (caractere == '\n' || caractere == EOF)
	{
		return 1;
	}

	while (caractere != '\n' && caractere != EOF)
	{
		caractere = getchar();
	}
	return 0;
}

int criar_nome_exibicao(char *destino, size_t capacidade,
						const char *apelido, const char *equipe)
{
	size_t tamanho_apelido = strlen(apelido);
	size_t tamanho_separador = strlen(SEPARADOR);
	size_t tamanho_equipe = strlen(equipe);

	if (tamanho_apelido + 1 > capacidade ||
		tamanho_apelido + tamanho_separador + tamanho_equipe + 1 > capacidade)
	{
		return 0;
	}

	strcpy(destino, apelido);
	strcat(destino, SEPARADOR);
	strcat(destino, equipe);
	return 1;
}

int ler_inteiro(const char *rotulo, int *valor)
{
	char linha[32];
	char *fim;

	printf("%s", rotulo);
	if (fgets(linha, sizeof(linha), stdin) == NULL)
	{
		return 0;
	}

	errno = 0;
	long convertido = strtol(linha, &fim, 10);
	if (fim == linha || errno == ERANGE || convertido < INT_MIN || convertido > INT_MAX ||
		(*fim != '\n' && *fim != '\0'))
	{
		printf("Entrada invalida: informe um numero inteiro.\n");
		return -1;
	}

	*valor = (int)convertido;
	return 1;
}

void inicializar_mapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS])
{
	for (int linha = 0; linha < MAPA_LINHAS; linha++)
	{
		for (int coluna = 0; coluna < MAPA_COLUNAS; coluna++)
		{
			mapa[linha][coluna] = POSICAO_LIVRE;
		}
	}
}

int coordenada_valida(int linha, int coluna)
{
	return linha >= 0 && linha < MAPA_LINHAS &&
		   coluna >= 0 && coluna < MAPA_COLUNAS;
}

int encontrar_jogador(int mapa[MAPA_LINHAS][MAPA_COLUNAS], int jogador,
					  int *linha_encontrada, int *coluna_encontrada)
{
	for (int linha = 0; linha < MAPA_LINHAS; linha++)
	{
		for (int coluna = 0; coluna < MAPA_COLUNAS; coluna++)
		{
			if (mapa[linha][coluna] == jogador)
			{
				*linha_encontrada = linha;
				*coluna_encontrada = coluna;
				return 1;
			}
		}
	}
	return 0;
}

int posicionar_jogador(int mapa[MAPA_LINHAS][MAPA_COLUNAS], int jogador,
					   int total_jogadores, int linha, int columna)
{
	if (jogador < 0 || jogador >= total_jogadores)
	{
		printf("Jogador invalido.\n");
		return 0;
	}
	if (!coordenada_valida(linha, columna))
	{
		printf("Coordenada invalida. Linhas e colunas devem estar entre 0 e %d.\n",
			   MAPA_LINHAS - 1);
		return 0;
	}
	if (mapa[linha][columna] != POSICAO_LIVRE)
	{
		printf("Essa celula ja esta ocupada.\n");
		return 0;
	}

	int linha_atual;
	int coluna_atual;
	if (encontrar_jogador(mapa, jogador, &linha_atual, &coluna_atual))
	{
		printf("Esse jogador ja esta no mapa; use reposicionar.\n");
		return 0;
	}

	mapa[linha][columna] = jogador;
	printf("Jogador posicionado.\n");
	return 1;
}

int remover_posicao(int mapa[MAPA_LINHAS][MAPA_COLUNAS], int linha, int coluna)
{
	if (!coordenada_valida(linha, coluna))
	{
		printf("Coordenada invalida. Linhas e colunas devem estar entre 0 e %d.\n",
			   MAPA_LINHAS - 1);
		return 0;
	}
	if (mapa[linha][coluna] == POSICAO_LIVRE)
	{
		printf("Essa celula ja esta livre.\n");
		return 0;
	}

	mapa[linha][coluna] = POSICAO_LIVRE;
	printf("Posicao removida.\n");
	return 1;
}

int reposicionar_jogador(int mapa[MAPA_LINHAS][MAPA_COLUNAS], int jogador,
						 int total_jogadores, int nova_linha, int nova_coluna)
{
	if (jogador < 0 || jogador >= total_jogadores)
	{
		printf("Jogador invalido.\n");
		return 0;
	}
	if (!coordenada_valida(nova_linha, nova_coluna))
	{
		printf("Coordenada invalida. Linhas e colunas devem estar entre 0 e %d.\n",
			   MAPA_LINHAS - 1);
		return 0;
	}

	int linha_atual;
	int coluna_atual;
	if (!encontrar_jogador(mapa, jogador, &linha_atual, &coluna_atual))
	{
		printf("Esse jogador ainda nao esta posicionado.\n");
		return 0;
	}
	if (mapa[nova_linha][nova_coluna] != POSICAO_LIVRE &&
		(linha_atual != nova_linha || coluna_atual != nova_coluna))
	{
		printf("A nova celula ja esta ocupada; a posicao atual foi mantida.\n");
		return 0;
	}

	mapa[linha_atual][coluna_atual] = POSICAO_LIVRE;
	mapa[nova_linha][nova_coluna] = jogador;
	printf("Jogador reposicionado.\n");
	return 1;
}

void exibir_mapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS],
				 char (*jogadores)[NOME_EXIBICAO_CAPACIDADE], int total_jogadores)
{
	printf("\n=== MAPA DA EQUIPE ===\n");
	printf("     ");
	for (int coluna = 0; coluna < MAPA_COLUNAS; coluna++)
	{
		printf("| C%-2d ", coluna);
	}
	printf("|\n");

	for (int linha = 0; linha < MAPA_LINHAS; linha++)
	{
		printf("L%-3d ", linha);
		for (int coluna = 0; coluna < MAPA_COLUNAS; coluna++)
		{
			int jogador = mapa[linha][coluna];
			if (jogador == POSICAO_LIVRE)
			{
				printf("| Livre");
			}
			else
			{
				printf("| J%-4d", jogador + 1);
			}
		}
		printf("|\n");
	}

	printf("\nJogadores: J1-J%d correspondem a ordem da lista abaixo.\n", total_jogadores);
	for (int i = 0; i < total_jogadores; i++)
	{
		printf("J%d: %s\n", i + 1, jogadores[i]);
	}
}

int ler_coordenadas(int *linha, int *coluna)
{
	int resultado = ler_inteiro("Linha: ", linha);
	if (resultado != 1)
	{
		return resultado;
	}
	resultado = ler_inteiro("Coluna: ", coluna);
	if (resultado != 1)
	{
		return resultado;
	}
	return 1;
}

int main(void)
{
	int quantidade_jogadores;
	char rotulo_quantidade[64];
	snprintf(rotulo_quantidade, sizeof(rotulo_quantidade),
			 "Quantidade de jogadores (1 a %d): ", MAX_JOGADORES);
	while (1)
	{
		int resultado = ler_inteiro(rotulo_quantidade, &quantidade_jogadores);
		if (resultado == 0)
		{
			return EXIT_FAILURE;
		}
		if (resultado < 0)
		{
			continue;
		}
		if (quantidade_jogadores < 1 || quantidade_jogadores > MAX_JOGADORES)
		{
			printf("Informe uma quantidade entre 1 e %d.\n", MAX_JOGADORES);
			continue;
		}
		break;
	}
	/* sizeof(*jogadores) acompanha o tamanho de cada registro se o tipo mudar. */
	char (*jogadores)[NOME_EXIBICAO_CAPACIDADE] =
		malloc((size_t)quantidade_jogadores * sizeof(*jogadores));
	if (jogadores == NULL)
	{
		printf("Nao foi possivel alocar memoria para os jogadores.\n");
		return EXIT_FAILURE;
	}

	char equipe[EQUIPE_CAPACIDADE];

	int resultado_texto;
	while ((resultado_texto = ler_texto("Nome da equipe: ", equipe, sizeof(equipe))) == 0)
	{
		printf("Nome da equipe muito longo. Use no maximo %d caracteres.\n",
			   EQUIPE_CAPACIDADE - 1);
	}
	if (resultado_texto < 0)
	{
		free(jogadores);
		jogadores = NULL;
		return EXIT_FAILURE;
	}
	if (equipe[0] == '\0')
	{
		printf("O nome da equipe nao pode ficar vazio.\n");
		free(jogadores);
		jogadores = NULL;
		return EXIT_FAILURE;
	}

	exibir_valor(&quantidade_jogadores, TIPO_INTEIRO);
	exibir_valor(equipe, TIPO_TEXTO);

	for (int i = 0; i < quantidade_jogadores; i++)
	{
		char apelido[APELIDO_CAPACIDADE];
		char nome_exibicao[NOME_EXIBICAO_CAPACIDADE];
		char rotulo[64];

		snprintf(rotulo, sizeof(rotulo), "Apelido do jogador %d: ", i + 1);
		while ((resultado_texto = ler_texto(rotulo, apelido, sizeof(apelido))) == 0)
		{
			printf("Apelido muito longo. Use no maximo %d caracteres.\n",
				   APELIDO_CAPACIDADE - 1);
		}
		if (resultado_texto < 0)
		{
			free(jogadores);
			jogadores = NULL;
			return EXIT_FAILURE;
		}
		if (apelido[0] == '\0')
		{
			printf("O apelido nao pode ficar vazio. Digite novamente.\n");
			i--;
			continue;
		}

		if (!criar_nome_exibicao(nome_exibicao, sizeof(nome_exibicao), apelido, equipe))
		{
			printf("Nao foi possivel criar a identificacao: o texto excede a capacidade.\n");
			i--;
			continue;
		}

		size_t tamanho_nome = strlen(nome_exibicao);
		if (tamanho_nome + 1 > sizeof(jogadores[i]))
		{
			printf("A identificacao do jogador excede o espaco reservado.\n");
			i--;
			continue;
		}
		strcpy(jogadores[i], nome_exibicao);
	}

	printf("\n=== JOGADORES CADASTRADOS ===\n");
	for (int i = 0; i < quantidade_jogadores; i++)
	{
		printf("[%d] %s\n", i, jogadores[i]);
	}

	char consulta[NOME_EXIBICAO_CAPACIDADE];
	resultado_texto = ler_texto("\nNome exato para buscar: ", consulta, sizeof(consulta));
	if (resultado_texto != 1)
	{
		if (resultado_texto == 0)
		{
			printf("Busca muito longa. O nome excede a capacidade permitida.\n");
		}
		free(jogadores);
		jogadores = NULL;
		return EXIT_FAILURE;
	}

	int encontrado = 0;
	for (int i = 0; i < quantidade_jogadores; i++)
	{
		if (strcmp(jogadores[i], consulta) == 0)
		{
			printf("Jogador encontrado na posicao %d: %s\n", i, jogadores[i]);
			encontrado = 1;
			break;
		}
	}
	if (!encontrado)
	{
		printf("Nenhum jogador corresponde exatamente a '%s'.\n", consulta);
	}

	int mapa[MAPA_LINHAS][MAPA_COLUNAS];
	inicializar_mapa(mapa);

	int opcao = -1;
	while (opcao != 0)
	{
		printf("\n=== FORMACAO DA EQUIPE ===\n");
		printf("1. Exibir mapa\n2. Posicionar jogador\n3. Remover posicao\n"
			   "4. Reposicionar jogador\n0. Sair\n");
		int resultado = ler_inteiro("\nEscolha: ", &opcao);
		if (resultado == 0)
		{
			break;
		}
		if (resultado < 0)
		{
			continue;
		}

		if (opcao == 1)
		{
			exibir_mapa(mapa, jogadores, quantidade_jogadores);
		}
		else if (opcao == 2 || opcao == 4)
		{
			int jogador;
			printf("Jogadores cadastrados:\n");
			for (int i = 0; i < quantidade_jogadores; i++)
			{
				printf("%d. %s\n", i + 1, jogadores[i]);
			}
			resultado = ler_inteiro("Numero do jogador: ", &jogador);
			if (resultado != 1)
			{
				continue;
			}
			jogador--;

			int linha;
			int coluna;
			resultado = ler_coordenadas(&linha, &coluna);
			if (resultado != 1)
			{
				continue;
			}

			if (opcao == 2)
			{
				posicionar_jogador(mapa, jogador, quantidade_jogadores, linha, coluna);
			}
			else
			{
				reposicionar_jogador(mapa, jogador, quantidade_jogadores, linha, coluna);
			}
		}
		else if (opcao == 3)
		{
			int linha;
			int coluna;
			resultado = ler_coordenadas(&linha, &coluna);
			if (resultado == 1)
			{
				remover_posicao(mapa, linha, coluna);
			}
		}
		else if (opcao != 0)
		{
			printf("Opcao invalida.\n");
		}
	}

	free(jogadores);
	jogadores = NULL;
	return 0;
}
