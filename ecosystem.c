#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void print_board(BOARD* board) {
    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            printf("%s ", board->map[i][j].object->type);
        }
        printf("\n");
    }
    printf("\n");
}
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
 *  0 North
 *  1 East
 *  2 South
 *  3 West
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
        int p[2] = {1, 2};
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
        int p[3] = {1, 2, 3};

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
        int p[2] = {2, 3};

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
        int p[2] = {0, 1};

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
        int p[3] = {0, 1, 3};

        if (check_north(board, x, y) == 1) {
            count++;
        }
        if (check_east(board, x, y) == 1) {
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

    // bottom right corner
    if (x == limitX - 1 && y == limitY - 1) {
        int next_move = -1;
        int count = 0;
        int p[2] = {0, 3};

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
    // left border
    if (x > 0 && x <= limitX - 1 && y == 0) {
        int next_move = -1;
        int count = 0;
        int p[3] = {0, 1, 2};

        if (check_north(board, x, y) == 1) {
            count++;
        }
        if (check_south(board, x, y) == 1) {
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

    // right border
    if (x == limitX - 1 && y > 0 && y < limitY - 1) {
        int next_move = -1;
        int count = 0;
        int p[3] = {0, 1, 3};

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

void resolve_conflit(BOARD* b, OBJECT* obj, int x, int y) {
    if (strcmp(obj->type, "RABBIT") == 0) {
        if (obj->GEN_PROC_RABBITS > b->map[x][y].object->GEN_PROC_RABBITS) {
            b->map[x][y].object = obj;
        }
    } else if (strcmp(obj->type, "FOX") == 0) {
        if (obj->GEN_PROC_FOXES >= b->map[x][y].object->GEN_PROC_FOXES) {
            // if the same compare the food

            if (obj->GEN_PROC_FOXES == b->map[x][y].object->GEN_PROC_FOXES) {
                if (obj->GEN_FOOD_FOXES < b->map[x][y].object->GEN_FOOD_FOXES) {
                    b->map[x][y].object = obj;
                }
            } else {
                b->map[x][y].object = obj;
            }
        }
    }
}
void move(BOARD* board, BOARD* board_aux, OBJECT* object, int x, int y) {
    // ignore rocks
    if (strcmp(object->type, "ROCK") == 0) return;

    // get possible cells
    int move = get_cells(board->R, board->C, x, y, board);
    if (move != -1) {
        // switch position
        // OBJECT* aux = malloc(sizeof(OBJECT));
        // aux->type = board->map[x][y].object->type;

        printf("MOVE TO: %d\n", move);
        switch (move) {
            // north
            case 0:
                if (strcmp(board_aux->map[x - 1][y].object->type, ".") != 0) {
                    resolve_conflit(board_aux, object, x - 1, y);
                } else {
                    board_aux->map[x - 1][y].object = object;
                }

                break;

            // east
            case 1:
                if (strcmp(board_aux->map[x][y + 1].object->type, ".") != 0) {
                    resolve_conflit(board_aux, object, x, y + 1);
                } else {
                    board_aux->map[x][y + 1].object = object;
                }
                break;

            // south
            case 2:
                if (strcmp(board_aux->map[x + 1][y].object->type, ".") != 0) {
                    resolve_conflit(board_aux, object, x + 1, y);
                } else {
                    board_aux->map[x + 1][y].object = object;
                }
                break;

            // west
            case 3:
                if (strcmp(board_aux->map[x][y - 1].object->type, ".") != 0) {
                    resolve_conflit(board_aux, object, x, y - 1);
                } else {
                    board_aux->map[x][y - 1].object = object;
                }
                break;

            default:
                printf("Erro");
                break;
        }
    }
}
void procriate(BOARD* board, BOARD* board_aux, int x, int y) {
    if (strcmp(board->map[x][y].object->type, "RABBIT") == 0) {
        if (board->map[x][y].object->GEN_PROC_RABBITS ==
            board->GEN_PROC_RABBITS) {
            // reset the rabbit gen_proc
            board->map[x][y].object->GEN_PROC_RABBITS = 0;

            // create a new rabbit
            OBJECT* new_rabbit = malloc(sizeof(OBJECT));
            new_rabbit->type = "RABBIT";

            // move the old one and replace with the newer
            move(board, board_aux, board->map[x][y].object, x, y);
            board->map[x][y].object = new_rabbit;
        }

    } else if (strcmp(board->map[x][y].object->type, "FOX") == 0) {
    }
    move(board, board_aux, board->map[x][y].object, x, y);
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
void swap_board(BOARD* board, BOARD* aux) {
    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            board->map[i][j] = aux->map[i][j];
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

    recieve_input(board, N, GEN_PROC_FOXES, GEN_FOOD_FOXES, GEN_PROC_FOXES);
    // move(board, aux, board->map[3][0].object, 3, 0);
    while (N_GEN >= 0) {
        initialize_board(aux, R, C);
        transfer_rocks(board, aux);
        // criar
        move_rabbits(board);
        swap_board(board, aux);
        // criar func pra isto
        // clear_world()
        initialize_board(aux, R, C);
        transfer_rocks(board, aux);

        // criar
        transfer_rabbits(board, aux);
        // criar
        move_foxes(board);
        swap_board(board, aux);

        board->generation++;
        N_GEN--;
    }

    print_board(board);
    print_board(aux);

    return 0;
}
