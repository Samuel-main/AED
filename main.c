#include <stdio.h>
#include <raylib.h>
int main(void)
{
    InitWindow(800, 450, "TESTE");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(
            "TESTE",
            900,
            500,
            30,
            BLACK
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
