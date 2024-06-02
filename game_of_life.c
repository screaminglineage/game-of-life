#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>

#define MAT_AT(mat, y, x) (mat)[(y) * state.cols + (x)]

#define CELL_SIZE 20
#define PAD 2

enum MODES {
    MODE_PLAY,
    MODE_PAUSE,
    MODE_EDIT,
    MODE_STEP,
    MODE_COUNT // total no. of modes
};

const char *MODES[MODE_COUNT] = {
    [MODE_PLAY] = "Play",
    [MODE_PAUSE] = "Paused",
    [MODE_EDIT] = "Edit",
    [MODE_STEP] = "Step",
};

typedef struct {
    int rows;
    int cols;
    int mode;
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

void swap_pointers(int **a, int **b) {
    int *tmp = *a;
    *a = *b;
    *b = tmp;
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
    // For centering the grid
    int start_x = (GetScreenWidth() - (state.cols * (CELL_SIZE + PAD) - PAD)) / 2;
    int start_y = (GetScreenHeight() - (state.rows * (CELL_SIZE + PAD) - PAD)) / 2;;

    for (int j = 0; j < state.rows; j++) {                                                   
        for (int i = 0; i < state.cols; i++) {                                               
            int x = start_x + i*(CELL_SIZE + PAD);
            int y = start_y + j*(CELL_SIZE + PAD);
            
            if (state.mode == MODE_EDIT) {
                Vector2 mouse = GetMousePosition();
                Rectangle box = {x, y, CELL_SIZE, CELL_SIZE};
                if (CheckCollisionPointRec(mouse, box)) {                                                     
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, RED);
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        MAT_AT(current, j, i) = 1;
                        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, WHITE);
                    }
                    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                        MAT_AT(current, j, i) = 0;
                        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, BLACK);
                    }
                } else {
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, MAT_AT(current, j, i) ? WHITE : BLACK);
                }                
            } else {
                DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, MAT_AT(current, j, i) ? WHITE : BLACK);
            }
        }                                                                                    
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
    InitWindow(1600, 900, "Game of Life");
    SetTargetFPS(60);
    state.mode = MODE_EDIT;

    state.rows = GetScreenHeight() / (CELL_SIZE + PAD) - PAD;
    state.cols = GetScreenWidth() / (CELL_SIZE + PAD) - PAD;
    int *board = calloc(state.rows*state.cols, sizeof(int));
    int *next_board = calloc(state.rows*state.cols, sizeof(int));

    int *current = board;
    int *next = next_board;
    
    while (!WindowShouldClose()) {
        BeginDrawing();                                                                          
        print_board_gui(current);
        DrawText(MODES[state.mode], 0, 0, 30, WHITE); 
        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) {
            switch (state.mode) {
                case MODE_PLAY:
                    state.mode = MODE_PAUSE;
                    break;
                default:
                    state.mode = MODE_PLAY;
                    break;
            }
        }

        if (state.mode != MODE_PAUSE) {
            if (IsKeyPressed(KEY_E)) {
                state.mode = MODE_EDIT;     
            }

            if (IsKeyPressed(KEY_S)) {
                switch (state.mode) {
                    case MODE_STEP:
                        game_of_life(current, next);
                        swap_pointers(&current, &next);
                        break;
                    case MODE_PLAY:
                        state.mode = MODE_STEP;
                    default:
                        state.mode = MODE_STEP;
                        game_of_life(current, next);
                        swap_pointers(&current, &next);
                        break;
                }
            }
        }

        if (state.mode == MODE_PLAY) {
            game_of_life(current, next);
            swap_pointers(&current, &next);
            WaitTime(0.07);
        }
    }
    CloseWindow(); 


    putchar('\n');
    return 0;
}
