#ifndef SNAKE_C
#define SNAKE_C

#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

snake_game* initialization(int width, int height)
{
    snake_game* snakeg = (snake_game*)malloc(sizeof(snake_game));
    if (snakeg) {
        snakeg->body.body_segments_y = create_body(width, height);
        snakeg->body.body_segments_x = create_body(width, height);
        snakeg->screen.pixel = create_field(width, height);
        snakeg->screen.width = width;
        snakeg->screen.height = height;
        snakeg->head.snake_head_x = (int)width / 2.0;
        snakeg->head.snake_head_y = (int)height / 2.0;
        snakeg->score = 0;
        snakeg->max_score = 0;
        snakeg->deaths = 0;
        snakeg->body.snake_length = 0;
        snakeg->apple.apple_x = (int)width / 2.0 + 2.0;
        snakeg->apple.apple_y = (int)height / 2.0;
        snakeg->head.direction = right;
        snakeg->state = game_continues;

        return snakeg;
    }
    return 0;
}

int* create_field(int size_width, int size_height)
{
    int* screen = (int*)malloc(size_width * size_height * sizeof(int));
    if (screen) {
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                if (i == 0 || i == (size_height - 1) || j == 0 || j == (size_width - 1))
                    screen[i * size_width + j] = 1;
                else
                    screen[i * size_width + j] = 0;
            }
        }
        return screen;
    }
    return 0;
}

int* create_body(int size_width, int size_height)
{
    int* body = (int*)malloc(size_width * size_height * sizeof(int));
    if (body) {
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                body[i * size_width + j] = 0;
            }
        }
        return body;
    }
    return 0;
}

void new_game(snake_game* snakeg)
{
    snakeg->head.snake_head_x = snakeg->screen.width / 2;
    snakeg->head.snake_head_y = snakeg->screen.height / 2;
    snakeg->score = 0;
    snakeg->body.snake_length = 0;
    snakeg->apple.apple_x = (int)snakeg->screen.width / 2.0 + 2.0;
    snakeg->apple.apple_y = snakeg->screen.height / 2;
    snakeg->head.direction = right;
    snakeg->state = game_continues;
    for (int i = 0; i < snakeg->screen.width * snakeg->screen.height; i++) {
        snakeg->body.body_segments_y[i] = 0;
        snakeg->body.body_segments_x[i] = 0;
    }
}

void exit_game(snake_game* snakeg)
{
    if (snakeg) {
        free(snakeg->screen.pixel);
        free(snakeg->body.body_segments_y);
        free(snakeg->body.body_segments_x);
    }
    free(snakeg);
}

void update_screen(snake_game* snakeg) {
    for (int i = 1; i < snakeg->screen.height - 1; i++) {
        for (int j = 1; j < snakeg->screen.width - 1; j++) {
            if (i == snakeg->apple.apple_y && j == snakeg->apple.apple_x)
                snakeg->screen.pixel[i * snakeg->screen.width + j] = s_apple;
            else if (i == snakeg->head.snake_head_y && j == snakeg->head.snake_head_x)
                snakeg->screen.pixel[i * snakeg->screen.width + j] = s_head;
            else
                snakeg->screen.pixel[i * snakeg->screen.width + j] = s_free_space;
        }
    }
    for (int i = 0; i < snakeg->body.snake_length; i++) {
        snakeg->screen.pixel[snakeg->body.body_segments_y[i] * snakeg->screen.width + snakeg->body.body_segments_x[i]] = s_body;
    }
}

void input_screen(snake_game* snakeg)
{
    const char block[] = { '[', ']', '\0' };
    const char apple[] = { '{', '}', '\0' };
    const char snake_h[] = { '$', '$', '\0' };
    const char snake_b[] = { '(', ')', '\0' };
    const char space[] = { ' ', ' ', '\0' };

    system("cls");

    /*for (int i = 0; i < 7; i++) {
        printf("\n");
    }*/

    printf("Snake game\nIf you want to quit the program, press Escape\nControl the snake with keys: A S D W\n\n");

    for (int i = 0; i < snakeg->screen.height; i++) {
        for (int j = 0; j < snakeg->screen.width; j++) {
            if (snakeg->screen.pixel[i * snakeg->screen.width + j] == s_apple)
                printf("%s", apple);
            else if (snakeg->screen.pixel[i * snakeg->screen.width + j] == s_head)
                printf("%s", snake_h);
            else if (snakeg->screen.pixel[i * snakeg->screen.width + j] == s_block)
                printf("%s", block);
            else if (snakeg->screen.pixel[i * snakeg->screen.width + j] == s_body)
                printf("%s", snake_b);
            else
                printf("%s", space);

        }
        printf("\n");
    }

    printf("\nScore: %d\n", snakeg->score);
    printf("Max score: %d\n", snakeg->max_score);
    printf("Length of snake: %d\n", snakeg->body.snake_length);
    printf("Deaths: %d", snakeg->deaths);
}

void snake_control(snake_game* snakeg)
{
    if ((GetAsyncKeyState('W') & 0x8001) && (snakeg->head.direction != down))
        snakeg->head.direction = up;
    else if ((GetAsyncKeyState('S') & 0x8001) && (snakeg->head.direction != up))
        snakeg->head.direction = down;
    else if ((GetAsyncKeyState('D') & 0x8001) && (snakeg->head.direction != left))
        snakeg->head.direction = right;
    else if ((GetAsyncKeyState('A') & 0x8001) && (snakeg->head.direction != right))
        snakeg->head.direction = left;
    else if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
        snakeg->state = game_exit;
}

void move_snake(snake_game* snakeg)
{
    for (int i = snakeg->body.snake_length; i > 0; i--) {
        snakeg->body.body_segments_x[i] = snakeg->body.body_segments_x[i - 1];
        snakeg->body.body_segments_y[i] = snakeg->body.body_segments_y[i - 1];
    }

    snakeg->body.body_segments_x[0] = snakeg->head.snake_head_x;
    snakeg->body.body_segments_y[0] = snakeg->head.snake_head_y;

    if (snakeg->head.direction == up) {
        snakeg->head.snake_head_y--;
    }
    else if (snakeg->head.direction == down) {
        snakeg->head.snake_head_y++;
    }
    else if (snakeg->head.direction == right) {
        snakeg->head.snake_head_x++;
    }
    else if (snakeg->head.direction == left) {
        snakeg->head.snake_head_x--;
    }
}

void eat_fruit(snake_game* snakeg)
{
    if (snakeg->head.snake_head_y == snakeg->apple.apple_y && snakeg->head.snake_head_x == snakeg->apple.apple_x) {
        do {
            snakeg->apple.apple_x = rand() % (snakeg->screen.width - 2) + 1;
            snakeg->apple.apple_y = rand() % (snakeg->screen.height - 2) + 1;
        } while (snakeg->screen.pixel[snakeg->apple.apple_y * snakeg->screen.width + snakeg->apple.apple_x] != s_free_space);

        snakeg->body.snake_length += 1;
        snakeg->score += 10;
        if (snakeg->score > snakeg->max_score) {
            snakeg->max_score = snakeg->score;
        }
    }
}

int collision_check(snake_game* snakeg)
{
    if (snakeg->screen.pixel[snakeg->head.snake_head_y * snakeg->screen.width + snakeg->head.snake_head_x] == s_block ||
        snakeg->screen.pixel[snakeg->head.snake_head_y * snakeg->screen.width + snakeg->head.snake_head_x] == s_body) {
        snakeg->deaths++;
        snakeg->state = gameover;
        return 1;
    }
    return 0;
}

#endif