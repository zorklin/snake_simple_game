#ifndef SNAKE_H
#define SNAKE_H

enum snake_objects {
    s_free_space,
    s_block,
    s_head,
    s_body,
    s_apple
};

enum direction_move {
    up,
    down,
    left,
    right
};

enum game_state {
    gameover,
    game_exit,
    game_continues
};

typedef struct snake_screen {
    int width;
    int height;
    int* pixel;
} snake_screen;

typedef struct snake_head {
    enum direction_move direction;
    int snake_head_x;
    int snake_head_y;
} snake_head;

typedef struct snake_body_segment {
    int snake_length;
    int* body_segments_y;
    int* body_segments_x;
} snake_body_segment;

typedef struct snake_apple {
    int apple_x;
    int apple_y;
} snake_apple;

typedef struct snake_game {
    enum game_state state;
    int score;
    int max_score;
    int deaths;
    snake_screen screen;
    snake_head head;
    snake_body_segment body;
    snake_apple apple;
} snake_game;

snake_game* initialization(int width, int height);
void new_game(snake_game* snakeg);
void exit_game(snake_game* snakeg);
int* create_field(int size_width, int size_height);
int* create_body(int size_width, int size_height);
void update_screen(snake_game* snakeg);
void input_screen(snake_game* snakeg);
void snake_control(snake_game* snakeg);
void move_snake(snake_game* snakeg);
void eat_fruit(snake_game* snakeg);
int collision_check(snake_game* snakeg);

#endif