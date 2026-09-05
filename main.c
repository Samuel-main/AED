#include <stdio.h>
#include <ralyb.h>
int main(void)
{
    InitWindow(800, 450, "TESTE");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(
            "TESTE",
            250,
            200,
            30,
            BLUE
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
