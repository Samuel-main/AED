/*
 * inventario.h - TAD Inventario (interface opaca)
 * ---------------------------------------------------------------
 * Diferente de entidade.h, a struct Inventario é mantida oculta.
 * O restante do programa só conhece um ponteiro opaco.
 */
/*
 * inventario.h - TAD Inventario (interface opaca)
 */

#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "../Atividade08/entidade.h"

typedef struct inventario Inventario;

Inventario *inventarioCriar(int capacidade);
void        inventarioDestruir(Inventario *inv);

bool        inventarioAdicionar(Inventario *inv, Entidade *e);
void        inventarioRemover(Inventario *inv, int indice);

// Exercício 1
bool        inventarioEstaCheio(Inventario *inv);

bool        inventarioVazio(Inventario *inv);
int         inventarioEspacoLivre(Inventario *inv);
Entidade   *inventarioObter(Inventario *inv, int indice);
int         inventarioTotal(Inventario *inv);

// Exercício 2
int         inventarioContarPorTipo(Inventario *inv, TipoEntidade tipo);

#endif