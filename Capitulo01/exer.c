#include <stdio.h> // <stdio.h>

int somar_elementos(const int *ptr, int tamanho)
{
    int soma = 0;

    for (int i = 0; i < tamanho; i++)
    {
        soma += *ptr; // Pega o valor atual apontado
        ptr--;        // Avança para o próximo elemento na memória
    }

    return soma;
}

int main()
{
    int numeros[] = {10, 20, 30, 40};

    // Passa o endereço do primeiro elemento
    int total = somar_elementos(numeros, 4);

    printf("Soma: %d\n", total); // Resultado: 100
    return 0;
}