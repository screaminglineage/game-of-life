#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>

#define MAT_AT(mat, y, x) (mat)[(y) * state.cols + (x)]

#define CELL_SIZE 20
#define PAD 2

typedef struct {
    int rows;
    int cols;
    bool paused;
} GameState;

GameState state = {0};

// modulo function where negative values wrap around
int modulo(int a, int b) {
    return (a % b + b) % b;
}

int count_neigbours(int x, int y, int *board) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int new_x = modulo(x + dx, state.cols);
            int new_y = modulo(y + dy, state.rows);
            if (!(dx == 0 && dy == 0) && MAT_AT(board, new_y, new_x)) {
                count++;
            }
        }
    }
    return count;
}

void game_of_life(int *current, int *next) {
    for (int y = 0; y < state.rows; y++) {
        for (int x = 0; x < state.cols; x++) {
            int neighbours = count_neigbours(x, y, current);
            if (MAT_AT(current, y, x)) {
                MAT_AT(next, y, x) = (neighbours == 2 || neighbours == 3);
            } else {
                MAT_AT(next, y, x) = (neighbours == 3);
            }
        }
    }
}

void print_board_cli(int *current) {
    for (int y = 0; y < state.rows; y++) {
        for (int x = 0; x < state.cols; x++) {
            if (MAT_AT(current, y, x)) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}


void cli(int *current, int *next, int tick_ms) {
    while (1) {
        print_board_cli(current);
        game_of_life(current, next);
        printf("\033[%dA\033[%dD", state.rows, state.cols);
        usleep(tick_ms*1000);

        int *tmp = current;
        current = next;
        next = tmp;
    }
}

void print_board_gui(int *current) {
    ClearBackground(GRAY);                                                                   
    int y = 0;
    for (int j = 0; j < state.rows; j++) {                                                   
        int x = 0;                                                                           
        for (int i = 0; i < state.cols; i++) {                                               
            int current_x = x + i*CELL_SIZE;
            int current_y = y + j*CELL_SIZE;
            
            if (state.paused) {
                Vector2 mouse = GetMousePosition();
                Rectangle box = {current_x, current_y, CELL_SIZE, CELL_SIZE};
                if (CheckCollisionPointRec(mouse, box)) {                                                     
                    DrawRectangle(current_x, current_y, CELL_SIZE, CELL_SIZE, RED);
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        MAT_AT(current, j, i) = 1;
                        DrawRectangle(current_x, current_y, CELL_SIZE, CELL_SIZE, WHITE);
                    }
                    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                        MAT_AT(current, j, i) = 0;
                        DrawRectangle(current_x, current_y, CELL_SIZE, CELL_SIZE, BLACK);
                    }
                } else {
                    DrawRectangle(current_x, current_y, CELL_SIZE, CELL_SIZE, MAT_AT(current, j, i) ? WHITE : BLACK);
                }                
            } else {
                DrawRectangle(current_x, current_y, CELL_SIZE, CELL_SIZE, MAT_AT(current, j, i) ? WHITE : BLACK);
            }
            x += PAD;                                                                        
        }                                                                                    
        y += PAD;                                                                            
    }    
}

int main() {
    InitWindow(1280, 720, "Game of Life");
    SetTargetFPS(60);

    state.rows = GetScreenHeight() / (CELL_SIZE + PAD);
    state.cols = GetScreenWidth() / (CELL_SIZE + PAD);
    int *board = calloc(state.rows*state.cols, sizeof(int));
    int *next_board = calloc(state.rows*state.cols, sizeof(int));

    // MAT_AT(board, 2, 1) = 1;
    // MAT_AT(board, 3, 2) = 1;
    // MAT_AT(board, 4, 0) = 1;
    // MAT_AT(board, 4, 1) = 1;
    // MAT_AT(board, 4, 2) = 1;

    int *current = board;
    int *next = next_board;

    while (!WindowShouldClose()) {
        BeginDrawing();                                                                          
        print_board_gui(current);
        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) {
            state.paused = !state.paused;
        }

        if (!state.paused) {
            game_of_life(current, next);
            usleep(100*1000);

            int *tmp = current;
            current = next;
            next = tmp;
        }
    }
    CloseWindow(); 


    putchar('\n');
    return 0;
}