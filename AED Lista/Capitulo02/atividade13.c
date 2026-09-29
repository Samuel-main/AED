#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

#define MAPA_LINHAS 5
#define MAPA_COLUNAS 5
#define MAX_JOGADORES (MAPA_LINHAS * MAPA_COLUNAS)
#define MAX_PARTIDAS 1000
#define MAX_DIMENSAO 1000
#define MAX_CELULAS 1000000
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

int *criar_historico(int quantidade)
{
	if (quantidade <= 0 || quantidade > MAX_PARTIDAS)
	{
		return NULL;
	}

	return malloc((size_t)quantidade * sizeof(int));
}

int preencher_historico(int *historico, int quantidade)
{
	if (historico == NULL || quantidade <= 0 || quantidade > MAX_PARTIDAS)
	{
		return 0;
	}

	for (int i = 0; i < quantidade; i++)
	{
		char rotulo[64];
		int resultado;
		snprintf(rotulo, sizeof(rotulo), "Pontuacao da partida %d: ", i + 1);
		resultado = ler_inteiro(rotulo, &historico[i]);
		while (resultado < 0)
		{
			printf("Informe uma pontuacao inteira valida.\n");
			resultado = ler_inteiro(rotulo, &historico[i]);
		}
		if (resultado == 0)
		{
			return 0;
		}
	}
	return 1;
}

void exibir_historico(const int *historico, int quantidade)
{
	if (historico == NULL || quantidade <= 0 || quantidade > MAX_PARTIDAS)
	{
		printf("Historico indisponivel.\n");
		return;
	}

	printf("\n=== HISTORICO DE PONTUACOES ===\n");
	for (int i = 0; i < quantidade; i++)
	{
		printf("Partida %d: %d pontos\n", i + 1, historico[i]);
	}
}

double calcular_media(const int *historico, int quantidade)
{
	if (historico == NULL || quantidade <= 0 || quantidade > MAX_PARTIDAS)
	{
		return 0.0;
	}

	double soma = 0.0;
	for (int i = 0; i < quantidade; i++)
	{
		soma += historico[i];
	}
	return soma / (double)quantidade;
}

int localizar_maior(const int *historico, int quantidade, int *posicao_maior)
{
	if (historico == NULL || quantidade <= 0 || quantidade > MAX_PARTIDAS ||
		posicao_maior == NULL)
	{
		return 0;
	}

	*posicao_maior = 0;
	for (int i = 1; i < quantidade; i++)
	{
		if (historico[i] > historico[*posicao_maior])
		{
			*posicao_maior = i;
		}
	}
	return 1;
}

int dimensoes_validas(int linhas, int colunas)
{
	if (linhas <= 0 || colunas <= 0 || linhas > MAX_DIMENSAO || colunas > MAX_DIMENSAO)
	{
		return 0;
	}

	if ((size_t)linhas > SIZE_MAX / (size_t)colunas)
	{
		return 0;
	}
	size_t celulas = (size_t)linhas * (size_t)colunas;
	if (celulas > MAX_CELULAS || celulas > SIZE_MAX / sizeof(int))
	{
		return 0;
	}
	return 1;
}

/* Uma reserva continua tem uma alocacao; linha e coluna viram um deslocamento. */
int *criar_matriz_linear(int linhas, int colunas)
{
	if (!dimensoes_validas(linhas, colunas))
	{
		return NULL;
	}

	size_t celulas = (size_t)linhas * (size_t)colunas;
	int *matriz = malloc(celulas * sizeof(*matriz));
	if (matriz == NULL)
	{
		return NULL;
	}

	for (size_t i = 0; i < celulas; i++)
	{
		matriz[i] = POSICAO_LIVRE;
	}
	return matriz;
}

/* Ponteiro de ponteiros usa reservas separadas e permite linhas independentes. */
int **criar_matriz_por_linhas(int linhas, int colunas)
{
	if (!dimensoes_validas(linhas, colunas) ||
		(size_t)linhas > SIZE_MAX / sizeof(int *))
	{
		return NULL;
	}

	int **matriz = malloc((size_t)linhas * sizeof(*matriz));
	if (matriz == NULL)
	{
		return NULL;
	}

	for (int linha = 0; linha < linhas; linha++)
	{
		matriz[linha] = malloc((size_t)colunas * sizeof(*matriz[linha]));
		if (matriz[linha] == NULL)
		{
			while (linha > 0)
			{
				free(matriz[--linha]);
			}
			free(matriz);
			return NULL;
		}
		for (int coluna = 0; coluna < colunas; coluna++)
		{
			matriz[linha][coluna] = POSICAO_LIVRE;
		}
	}
	return matriz;
}

int preencher_matriz_linear(int *matriz, int linhas, int colunas)
{
	if (matriz == NULL || !dimensoes_validas(linhas, colunas))
	{
		return 0;
	}

	printf("Informe os valores para a matriz linear (use %d para livre):\n", POSICAO_LIVRE);
	for (int linha = 0; linha < linhas; linha++)
	{
		for (int coluna = 0; coluna < colunas; coluna++)
		{
			char rotulo[80];
			snprintf(rotulo, sizeof(rotulo), "Celula [%d][%d]: ", linha, coluna);
			int resultado = ler_inteiro(rotulo, &matriz[linha * colunas + coluna]);
			while (resultado < 0)
			{
				printf("Informe um valor inteiro.\n");
				resultado = ler_inteiro(rotulo, &matriz[linha * colunas + coluna]);
			}
			if (resultado == 0)
			{
				return 0;
			}
		}
	}
	return 1;
}

int preencher_matriz_por_linhas(int **matriz, const int *matriz_linear,
								int linhas, int colunas)
{
	if (matriz == NULL || matriz_linear == NULL || !dimensoes_validas(linhas, colunas))
	{
		return 0;
	}

	for (int linha = 0; linha < linhas; linha++)
	{
		for (int coluna = 0; coluna < colunas; coluna++)
		{
			matriz[linha][coluna] = matriz_linear[linha * colunas + coluna];
		}
	}
	return 1;
}

void exibir_matriz_linear(const int *matriz, int linhas, int colunas)
{
	if (matriz == NULL || !dimensoes_validas(linhas, colunas))
	{
		printf("Matriz linear indisponivel.\n");
		return;
	}

	printf("\n=== MATRIZ EM BLOCO CONTINUO ===\n");
	for (int linha = 0; linha < linhas; linha++)
	{
		for (int coluna = 0; coluna < colunas; coluna++)
		{
			printf("%d\t", matriz[linha * colunas + coluna]);
		}
		printf("\n");
	}
}

void exibir_matriz_por_linhas(int *const *matriz, int linhas, int colunas)
{
	if (matriz == NULL || !dimensoes_validas(linhas, colunas))
	{
		printf("Matriz por linhas indisponivel.\n");
		return;
	}

	printf("\n=== MATRIZ DE LINHAS INDEPENDENTES ===\n");
	for (int linha = 0; linha < linhas; linha++)
	{
		for (int coluna = 0; coluna < colunas; coluna++)
		{
			printf("%d\t", matriz[linha][coluna]);
		}
		printf("\n");
	}
}

void liberar_matriz_linear(int *matriz)
{
	free(matriz);
}

void liberar_matriz_por_linhas(int **matriz, int linhas)
{
	if (matriz == NULL)
	{
		return;
	}
	for (int linha = 0; linha < linhas; linha++)
	{
		free(matriz[linha]);
	}
	free(matriz);
}

void executar_matrizes_dinamicas(void)
{
	int linhas;
	int colunas;
	while (1)
	{
		int resultado = ler_inteiro("Numero de linhas (1 a 1000): ", &linhas);
		if (resultado == 0)
		{
			return;
		}
		if (resultado < 0)
		{
			continue;
		}
		resultado = ler_inteiro("Numero de colunas (1 a 1000): ", &colunas);
		if (resultado == 0)
		{
			return;
		}
		if (resultado < 0)
		{
			continue;
		}
		if (!dimensoes_validas(linhas, colunas))
		{
			printf("Dimensoes invalidas ou matriz acima de %d celulas.\n", MAX_CELULAS);
			continue;
		}
		break;
	}

	int *linear = criar_matriz_linear(linhas, colunas);
	if (linear == NULL)
	{
		printf("Falha ao alocar a matriz linear.\n");
		return;
	}

	int **por_linhas = criar_matriz_por_linhas(linhas, colunas);
	if (por_linhas == NULL)
	{
		printf("Falha ao alocar a matriz por linhas.\n");
		liberar_matriz_linear(linear);
		linear = NULL;
		return;
	}

	if (!preencher_matriz_linear(linear, linhas, colunas))
	{
		printf("Preenchimento interrompido.\n");
		liberar_matriz_linear(linear);
		linear = NULL;
		liberar_matriz_por_linhas(por_linhas, linhas);
		por_linhas = NULL;
		return;
	}
	preencher_matriz_por_linhas(por_linhas, linear, linhas, colunas);

	exibir_matriz_linear(linear, linhas, colunas);
	exibir_matriz_por_linhas(por_linhas, linhas, colunas);

	liberar_matriz_linear(linear);
	linear = NULL;
	liberar_matriz_por_linhas(por_linhas, linhas);
	por_linhas = NULL;
}

void executar_historico(void)
{
	int quantidade_partidas;
	char rotulo_partidas[64];
	snprintf(rotulo_partidas, sizeof(rotulo_partidas),
			 "Quantidade de partidas no historico (1 a %d): ", MAX_PARTIDAS);
	while (1)
	{
		int resultado = ler_inteiro(rotulo_partidas, &quantidade_partidas);
		if (resultado == 0)
		{
			return;
		}
		if (resultado < 0)
		{
			continue;
		}
		if (quantidade_partidas <= 0 || quantidade_partidas > MAX_PARTIDAS)
		{
			printf("A quantidade deve estar entre 1 e %d.\n", MAX_PARTIDAS);
			continue;
		}
		break;
	}

	int *historico = criar_historico(quantidade_partidas);
	if (historico == NULL)
	{
		printf("Nao foi possivel criar o historico de pontuacoes.\n");
		return;
	}
	if (!preencher_historico(historico, quantidade_partidas))
	{
		printf("Historico incompleto; liberando memoria.\n");
		free(historico);
		historico = NULL;
		return;
	}

	exibir_historico(historico, quantidade_partidas);
	int posicao_maior;
	if (localizar_maior(historico, quantidade_partidas, &posicao_maior))
	{
		printf("Media das pontuacoes: %.2f\n", calcular_media(historico, quantidade_partidas));
		printf("Maior resultado: %d pontos (partida %d, posicao %d).\n",
			   historico[posicao_maior], posicao_maior + 1, posicao_maior);
	}

	free(historico);
	historico = NULL;
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
			   "4. Reposicionar jogador\n5. Comparar matrizes dinamicas\n"
			   "6. Consultar historico de partidas\n0. Sair\n");
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
		else if (opcao == 5)
		{
			executar_matrizes_dinamicas();
		}
		else if (opcao == 6)
		{
			executar_historico();
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
