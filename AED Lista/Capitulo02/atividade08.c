#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_JOGADORES 5
#define APELIDO_CAPACIDADE 32
#define EQUIPE_CAPACIDADE 48
#define NOME_EXIBICAO_CAPACIDADE 96
#define SEPARADOR " - "

int ler_texto(const char *rotulo, char *destino, size_t capacidade)
{
	printf("%s", rotulo);
	if (fgets(destino, (int)capacidade, stdin) == NULL)
	{
		printf("\nEntrada encerrada antes da conclusao do cadastro.\n");
		exit(EXIT_FAILURE);
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

int main(void)
{
	char equipe[EQUIPE_CAPACIDADE];
	char jogadores[TOTAL_JOGADORES][NOME_EXIBICAO_CAPACIDADE];

	while (!ler_texto("Nome da equipe: ", equipe, sizeof(equipe)))
	{
		printf("Nome da equipe muito longo. Use no maximo %d caracteres.\n",
			   EQUIPE_CAPACIDADE - 1);
	}
	if (equipe[0] == '\0')
	{
		printf("O nome da equipe nao pode ficar vazio.\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < TOTAL_JOGADORES; i++)
	{
		char apelido[APELIDO_CAPACIDADE];
		char nome_exibicao[NOME_EXIBICAO_CAPACIDADE];
		char rotulo[64];

		snprintf(rotulo, sizeof(rotulo), "Apelido do jogador %d: ", i + 1);
		while (!ler_texto(rotulo, apelido, sizeof(apelido)))
		{
			printf("Apelido muito longo. Use no maximo %d caracteres.\n",
				   APELIDO_CAPACIDADE - 1);
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
	for (int i = 0; i < TOTAL_JOGADORES; i++)
	{
		printf("[%d] %s\n", i, jogadores[i]);
	}

	char consulta[NOME_EXIBICAO_CAPACIDADE];
	if (!ler_texto("\nNome exato para buscar: ", consulta, sizeof(consulta)))
	{
		printf("Busca muito longa. O nome excede a capacidade permitida.\n");
		return EXIT_FAILURE;
	}

	int encontrado = 0;
	for (int i = 0; i < TOTAL_JOGADORES; i++)
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

	return 0;
}
