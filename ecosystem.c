#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void initialize_board(BOARD* board, int R, int C) {
    OBJECT* object = malloc(sizeof(OBJECT));
    board->C = C;
    board->R = R;

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
            board->GEN_PROC_RABBITS = GEN_PROC_RABBITS;
        } else if (strcmp(type, "FOX") == 0) {
            board->map[x][y].object = object;
            board->GEN_FOOD_FOXES = GEN_FOOD_FOXES;
            board->GEN_PROC_FOXES = GEN_PROC_FOXES;

        } else if (strcmp(type, "ROCK") == 0) {
            board->map[x][y].object = object;
        }
    }
}
int check_north(BOARD* board, int x, int y) {
    if (strcmp(board->map[x - 1][y].object->type, ".") == 0) {
        return 1;
    }
    return 0;
}
int check_east(BOARD* board, int x, int y) {
    if (strcmp(board->map[x][y + 1].object->type, ".") == 0) {
        return 1;
    }
    return 0;
}

int check_south(BOARD* board, int x, int y) {
    if (strcmp(board->map[x + 1][y].object->type, ".") == 0) {
        return 1;
    }
    return 0;
}
int check_west(BOARD* board, int x, int y) {
    if (strcmp(board->map[x][y - 1].object->type, ".") == 0) {
        return 1;
    }
    return 0;
}
/**
 * @brief Get the cell for the next move
 *  1 North
 *  2 East
 *  3 South
 *  4 West
 * @param limitX
 * @param limitY
 * @param x
 * @param y
 * @return next cell to move
 */
int get_cells(int limitX, int limitY, int x, int y, BOARD* board) {
    // top left corner
    if (x == 0 && y == 0) {
        int next_move = -1;
        int count = 0;
        int p[2] = {2, 3};
        if (check_east(board, x, y) == 1) {
            count++;
        }
        if (check_south(board, x, y) == 1) {
            count++;
        }
        // if can move
        if (count != 0) {
            next_move = (board->generation + x + y) % count;
            return p[next_move];
        } else
            return next_move;
    }
    // top row
    if (x == 0 && y > 0 && y < limitY - 1) {
        int next_move = -1;
        int count = 0;
        int p[3] = {2, 3, 4};

        if (check_east(board, x, y) == 1) {
            count++;
        }
        if (check_south(board, x, y) == 1) {
            count++;
        }
        if (check_west(board, x, y) == 1) {
            count++;
        }
        // if can move
        if (count != 0) {
            next_move = (board->generation + x + y) % count;
            return p[next_move];
        } else
            return next_move;
    }
    // top right corner
    if (x == 0 && y == limitY - 1) {
        int next_move = -1;
        int count = 0;
        int p[2] = {3, 4};

        if (check_south(board, x, y) == 1) {
            count++;
        }
        if (check_west(board, x, y) == 1) {
            count++;
        }
        // if can move
        if (count != 0) {
            next_move = (board->generation + x + y) % count;
            return p[next_move];
        } else
            return next_move;
    }

    // bottom left corner
    if (x == limitX - 1 && y == 0) {
        int next_move = -1;
        int count = 0;
        int p[2] = {1, 2};

        if (check_north(board, x, y) == 1) {
            count++;
        }
        if (check_east(board, x, y) == 1) {
            count++;
        }
        // if can move
        if (count != 0) {
            next_move = (board->generation + x + y) % count;
            return p[next_move];
        } else
            return next_move;
    }

    // bottom row
    if (x == limitX - 1 && y > 0 && y < limitY - 1) {
        int next_move = -1;
        int count = 0;
        int p[3] = {1, 3, 4};

        if (check_north(board, x, y) == 1) {
            count++;
        }
        if (check_south(board, x, y) == 1) {
            count++;
        }
        if (check_west(board, x, y) == 1) {
            count++;
        }
        // if can move
        if (count != 0) {
            next_move = (board->generation + x + y) % count;
            return p[next_move];
        } else
            return next_move;
    }

    // bottom right corner 1 4
    if (x == limitX - 1 && y == limitY - 1) {
        int next_move = -1;
        int count = 0;
        int p[2] = {1, 4};

        if (check_north(board, x, y) == 1) {
            count++;
        }
        if (check_west(board, x, y) == 1) {
            count++;
        }
        // if can move
        if (count != 0) {
            next_move = (board->generation + x + y) % count;
            return p[next_move];
        } else
            return next_move;
    }
    // the rest
    int next_move = -1;
    int count = 0;
    int p[4] = {1, 2, 3, 4};

    if (check_north(board, x, y) == 1) {
        count++;
    }
    if (check_east(board, x, y) == 1) {
        count++;
    }
    if (check_south(board, x, y) == 1) {
        count++;
    }
    if (check_west(board, x, y) == 1) {
        count++;
    }
    // if can move
    if (count != 0) {
        next_move = (board->generation + x + y) % count;
        return p[next_move];
    } else
        return next_move;
}

void move(BOARD* board, BOARD* aux, OBJECT* object, int x, int y) {
    // ignore rocks
    if (strcmp(object->type, "ROCK") == 0) return;

    // get possible cells
    int move = get_cells(board->R, board->C, x, y, board);

    if (move != -1) {
        // switch position
        OBJECT* aux = malloc(sizeof(OBJECT));
        aux->type = board->map[x][y].object->type;

        // printf("MOVE TO: %d\n", move);
        switch (move) {
            // north
            case 1:
                board->map[x - 1][y].object = aux;
                board->map[x - 1][1].object->type = aux->type;
                board->map[x][y].object->type = ".";
                free(board->map[x][y].object);

                break;

            // east
            case 2:
                board->map[x][y + 1].object = aux;
                board->map[x][y + 1].object->type = aux->type;
                board->map[x][y].object->type = ".";
                free(board->map[x][y].object);
                break;

            // south
            case 3:
                board->map[x + 1][y].object = aux;
                board->map[x + 1][y].object->type = aux->type;
                board->map[x][y].object->type = ".";
                free(board->map[x][y].object);
                break;

            // west
            case 4:
                board->map[x][y - 1].object = aux;
                board->map[x][y - 1].object->type = aux->type;
                board->map[x][y].object->type = ".";
                free(board->map[x][y].object);
                break;

            default:
                printf("Erro");
                break;
        }
    }
}
void procriate(BOARD* board, int x, int y) {
    if (strcmp(board->map[x][y].object->type, "RABBIT") == 0) {
        if (board->map[x][y].object->GEN_PROC_RABBITS ==
            board->GEN_PROC_RABBITS) {
            // reset the rabbit gen_proc
            board->map[x][y].object->GEN_PROC_RABBITS = 0;

            // create a new rabbit
            OBJECT* new_rabbit = malloc(sizeof(OBJECT));
            new_rabbit->type = "RABBIT";

            // move the old one and replace with the newer
            // move(board, aux, board->map[x][y].object, x, y);
            board->map[x][y].object = new_rabbit;
        }
    } else if (strcmp(board->map[x][y].object->type, "FOX") == 0) {
    }
}
void transfer_rocks(BOARD* board, BOARD* aux) {
    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            if (strcmp(board->map[i][j].object->type, "ROCK") == 0) {
                OBJECT* rock = malloc(sizeof(OBJECT));
                rock->type = "ROCK";
                aux->map[i][j].object = rock;
            }
        }
    }
}
int main() {
    int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;
    scanf("%d %d %d %d %d %d %d ", &GEN_PROC_RABBITS, &GEN_PROC_FOXES,
          &GEN_FOOD_FOXES, &N_GEN, &R, &C, &N);

    /*
        make the 2 boards
    */
    BOARD* board = (BOARD*)malloc(sizeof(BOARD));
    BOARD* aux = (BOARD*)malloc(sizeof(BOARD));

    board->map = (CELL**)malloc(sizeof(CELL*) * R);
    aux->map = (CELL**)malloc(sizeof(CELL*) * R);

    for (int i = 0; i < R; i++) {
        board->map[i] = (CELL*)malloc(sizeof(CELL) * C);
        aux->map[i] = (CELL*)malloc(sizeof(CELL) * C);
    }

    initialize_board(board, R, C);
    initialize_board(aux, R, C);

    recieve_input(board, N, GEN_PROC_FOXES, GEN_FOOD_FOXES, GEN_PROC_FOXES);
    transfer_rocks(board, aux);
    // test move the rabbit
    move(board, aux, board->map[0][2].object, 0, 2);

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            printf("%s ", board->map[i][j].object->type);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            printf("%s ", aux->map[i][j].object->type);
        }
        printf("\n");
    }

    return 0;
}
