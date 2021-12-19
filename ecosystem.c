#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void initialize_board(BOARD* board, int R, int C) {
    OBJECT* object = malloc(sizeof(OBJECT));
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            board->map[i][j].object = object;
            board->map[i][j].object->type = ".";
        }
    }
}

void recieve_input(BOARD* board, int N, int GEN_PROC_RABBITS,
                   int GEN_FOOD_FOXES, int GEN_PROC_FOXES) {
    for (int i = 0; i < N; i++) {
        char* type = malloc(sizeof(char));
        int x, y;
        scanf("%s %d %d", type, &x, &y);
        OBJECT* object = malloc(sizeof(OBJECT));
        object->type = type;

        if (strcmp(type, "RABBIT") == 0) {
            board->map[x][y].object = object;
            object->GEN_PROC_RABBITS = GEN_PROC_RABBITS;
        } else if (strcmp(type, "FOX") == 0) {
            board->map[x][y].object = object;
            object->GEN_FOOD_FOXES = GEN_FOOD_FOXES;
            object->GEN_PROC_FOXES = GEN_PROC_FOXES;

        } else if (strcmp(type, "ROCK") == 0) {
            board->map[x][y].object = object;
        }
    }
}

int main() {
    int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;
    scanf("%d %d %d %d %d %d %d ", &GEN_PROC_RABBITS, &GEN_PROC_FOXES,
          &GEN_FOOD_FOXES, &N_GEN, &R, &C, &N);

    BOARD* board = (BOARD*)malloc(sizeof(BOARD));
    board->map = (CELL**)malloc(sizeof(CELL*) * R);

    for (int i = 0; i < C; i++) {
        board->map[i] = (CELL*)malloc(sizeof(CELL) * C);
    }

    initialize_board(board, R, C);
    recieve_input(board, N, GEN_PROC_FOXES, GEN_FOOD_FOXES, GEN_PROC_FOXES);

    // trocar de posiçao
    OBJECT* teste = malloc(sizeof(OBJECT));
    teste->type = board->map[4][0].object->type;
    board->map[4][1].object = teste;
    board->map[4][1].object->type = teste->type;
    board->map[4][0].object->type = ".";
    free(board->map[4][0].object);

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            printf("%s ", board->map[i][j].object->type);
        }
        printf("\n");
    }

    return 0;
}
