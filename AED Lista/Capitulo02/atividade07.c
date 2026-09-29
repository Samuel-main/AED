#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_CAPACIDADE 100
#define APELIDO_CAPACIDADE 32
#define SENHA_CAPACIDADE 64
#define APELIDO_TAMANHO_MINIMO 3
#define APELIDO_TAMANHO_MAXIMO 20

/* Retorna 0 e descarta o restante da linha quando o texto nao cabe no vetor. */
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

int main(void)
{
	char nome[NOME_CAPACIDADE];
	char apelido[APELIDO_CAPACIDADE];
	char senha[SENHA_CAPACIDADE];
	char confirmacao[SENHA_CAPACIDADE];

	while (1)
	{
		if (!ler_texto("Nome completo: ", nome, sizeof(nome)))
		{
			printf("Nome muito longo. Informe um nome com menos de %d caracteres.\n",
				   NOME_CAPACIDADE);
			continue;
		}
		if (strlen(nome) == 0)
		{
			printf("O nome precisa ser preenchido.\n");
			continue;
		}
		break;
	}

	while (1)
	{
		if (!ler_texto("Apelido (3 a 20 caracteres): ", apelido, sizeof(apelido)))
		{
			printf("Apelido muito longo. Informe um apelido com no maximo %d caracteres.\n",
				   APELIDO_CAPACIDADE - 1);
			continue;
		}

		size_t tamanho_apelido = strlen(apelido);
		if (tamanho_apelido < APELIDO_TAMANHO_MINIMO ||
			tamanho_apelido > APELIDO_TAMANHO_MAXIMO)
		{
			printf("Apelido invalido: use de %d a %d caracteres.\n",
				   APELIDO_TAMANHO_MINIMO, APELIDO_TAMANHO_MAXIMO);
			continue;
		}
		break;
	}

	while (1)
	{
		if (!ler_texto("Senha: ", senha, sizeof(senha)))
		{
			printf("Senha muito longa. Informe uma senha com menos de %d caracteres.\n",
				   SENHA_CAPACIDADE);
			continue;
		}
		if (strlen(senha) == 0)
		{
			printf("A senha precisa ser preenchida.\n");
			continue;
		}

		if (!ler_texto("Confirme a senha: ", confirmacao, sizeof(confirmacao)))
		{
			printf("Confirmacao muito longa. Digite novamente a senha e sua confirmacao.\n");
			continue;
		}

		if (strcmp(senha, confirmacao) != 0)
		{
			printf("As senhas nao correspondem. Corrija a senha e a confirmacao.\n");
			continue;
		}
		break;
	}

	printf("\nCadastro concluido!\n");
	printf("Nome: %s\nApelido: %s\n", nome, apelido);
	return 0;
}
