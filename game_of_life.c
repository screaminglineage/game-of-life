#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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

// Creates the glider on the board
void glider(int *board) {
    MAT_AT(board, 2, 1) = 1;
    MAT_AT(board, 3, 2) = 1;
    MAT_AT(board, 4, 0) = 1;
    MAT_AT(board, 4, 1) = 1;
    MAT_AT(board, 4, 2) = 1;
}

// Creates an explosion of cells on the board
void explosion(int *board) {
    int mid_x = state.cols/2;
    int mid_y = state.rows/2;
    MAT_AT(board, mid_y+1, mid_x+0) = 1;
    MAT_AT(board, mid_y+1, mid_x+1) = 1;
    MAT_AT(board, mid_y+1, mid_x+2) = 1;
    MAT_AT(board, mid_y+2, mid_x+2) = 1;
    MAT_AT(board, mid_y+0, mid_x+1) = 1;
}

int main() {
    InitWindow(1280, 720, "Game of Life");
    SetTargetFPS(12);
    state.paused = true;

    state.rows = GetScreenHeight() / (CELL_SIZE + PAD);
    state.cols = GetScreenWidth() / (CELL_SIZE + PAD);
    int *board = calloc(state.rows*state.cols, sizeof(int));
    int *next_board = calloc(state.rows*state.cols, sizeof(int));

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
            int *tmp = current;
            current = next;
            next = tmp;
        }
    }
    CloseWindow(); 


    putchar('\n');
    return 0;
}
