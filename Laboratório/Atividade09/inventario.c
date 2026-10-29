#include <stdlib.h>
#include "inventario.h"

struct inventario {
    Entidade **vetor;
    int total;
    int capacidade;
};

Inventario *inventarioCriar(int capacidade) {
    if (capacidade <= 0) return NULL;

    Inventario *inv = (Inventario *)malloc(sizeof(Inventario));
    if (inv == NULL) return NULL;

    inv->vetor = (Entidade **)calloc((size_t)capacidade, sizeof(Entidade *));
    if (inv->vetor == NULL) {
        free(inv);
        return NULL;
    }

    inv->total = 0;
    inv->capacidade = capacidade;
    return inv;
}

void inventarioDestruir(Inventario *inv) {
    if (inv == NULL) return;

    for (int i = 0; i < inv->total; i++) {
        if (inv->vetor[i] != NULL) {
            free(inv->vetor[i]);
            inv->vetor[i] = NULL;
        }
    }
    free(inv->vetor);
    free(inv);
}

bool inventarioAdicionar(Inventario *inv, Entidade *e) {
    if (inv == NULL || e == NULL || inventarioEstaCheio(inv)) return false;

    inv->vetor[inv->total] = e;
    inv->total++;
    return true;
}

void inventarioRemover(Inventario *inv, int indice) {
    if (inv == NULL || indice < 0 || indice >= inv->total) return;

    if (inv->vetor[indice] != NULL) {
        free(inv->vetor[indice]);
        inv->vetor[indice] = NULL;
    }

    inv->vetor[indice] = inv->vetor[inv->total - 1];
    inv->vetor[inv->total - 1] = NULL;
    inv->total--;
}

// Exercício 1: Verifica se o inventário atingiu a capacidade máxima
bool inventarioEstaCheio(Inventario *inv) {
    return inv != NULL && inv->total >= inv->capacidade;
}

bool inventarioVazio(Inventario *inv) {
    return inv == NULL || inv->total <= 0;
}

int inventarioEspacoLivre(Inventario *inv) {
    if (inv == NULL) return 0;
    return inv->capacidade - inv->total;
}

Entidade *inventarioObter(Inventario *inv, int indice) {
    if (inv == NULL || indice < 0 || indice >= inv->total) return NULL;
    return inv->vetor[indice];
}

int inventarioTotal(Inventario *inv) {
    if (inv == NULL) return 0;
    return inv->total;
}

// Exercício 2: Contagem de entidades por tipo percorrendo o vetor internamente no TAD
int inventarioContarPorTipo(Inventario *inv, TipoEntidade tipo) {
    if (inv == NULL) return 0;

    int contador = 0;
    for (int i = 0; i < inv->total; i++) {
        if (inv->vetor[i] != NULL && inv->vetor[i]->tipo == tipo) {
            contador++;
        }
    }
    return contador;
}