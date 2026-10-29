#include "raylib.h"
#include "entidade.h"
#include <stdlib.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 8B - Modulo Entidade");
    SetTargetFPS(60);

    Entidade *entidades[] = {
        entidadeCriar(ENTIDADE_JOGADOR, (Vector2){200.0f, 300.0f}),
        entidadeCriar(ENTIDADE_INIMIGO, (Vector2){400.0f, 300.0f}),
        entidadeCriar(ENTIDADE_ITEM, (Vector2){600.0f, 300.0f})
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (int i = 0; i < 3; i++) {
                entidadeDesenhar(entidades[i]);
            }
        EndDrawing();
    }

    for (int i = 0; i < 3; i++) free(entidades[i]);
    CloseWindow();
    return 0;
}