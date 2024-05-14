#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "snake.h"

#define WIDTH 20
#define HEIGHT 15

int main()
{
    snake_game* snakeg = initialization(WIDTH, HEIGHT);
    int tact = 0;
    const int max_tact = 5;

    while (snakeg->state != game_exit) {
        new_game(snakeg);

        while (snakeg->state != game_exit && snakeg->state != gameover) {
            for (tact = 0; tact < max_tact; tact++) {
                update_screen(snakeg);
                input_screen(snakeg);
                Sleep(3);
            }
            snake_control(snakeg);
            move_snake(snakeg);
            eat_fruit(snakeg);
            collision_check(snakeg);
        }

        printf("\nGame over\n");
        Sleep(1000);
    }

    printf("\nExit\n");
    exit_game(snakeg);
    Sleep(500);

    return 0;
}