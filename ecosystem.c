#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void print_board(BOARD* board) {
    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            printf("%c ", board->map[i][j].object->type);
        }
        printf("\n");
    }
    printf("\n");
}
void empty_board(BOARD* board) {
    OBJECT* object = malloc(sizeof(OBJECT));

    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            board->map[i][j].object = object;
            board->map[i][j].object->type = '.';
        }
    }
}
/**
 * @brief Copies b2 to b1
 *
 * @param b1
 * @param b2
 */
void copy_board(BOARD* b1, BOARD* b2) {
    for (int i = 0; i < b1->R; i++) {
        for (int j = 0; j < b1->C; j++) {
            b1->map[i][j].object = b2->map[i][j].object;
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

        if (strcmp(type, "RABBIT") == 0) {
            object->type = 'R';
            board->map[x][y].object = object;
            board->GEN_PROC_RABBITS = GEN_PROC_RABBITS;
        } else if (strcmp(type, "FOX") == 0) {
            object->type = 'F';
            board->map[x][y].object = object;
            board->GEN_FOOD_FOXES = GEN_FOOD_FOXES;
            board->GEN_PROC_FOXES = GEN_PROC_FOXES;

        } else if (strcmp(type, "ROCK") == 0) {
            object->type = '*';
            board->map[x][y].object = object;
        }
    }
}

int check_empty(BOARD* board, int x, int y) {
    if (board->map[x][y].object->type == '.') {
        return 1;
    }
    return 0;
}
int check_rabbit(BOARD* board, int x, int y) {
    if (board->map[x][y].object->type == 'R') {
        return 1;
    }
    return 0;
}
/**
 * @brief Get the cell for the next move
 *
 * @param board
 * @param x
 * @param y
 * @param f function to see if is empty(for rabbits) / there is a rabbit (for
 * foxes)
 * @return POS
 */
POS get_cell(BOARD* board, int x, int y, int (*f)(BOARD*, int, int)) {
    int count = 0;
    POS p[4];

    // top left corner
    if (x == 0 && y == 0) {
        if ((*f)(board, x, y + 1) == 1) {
            p[count].x = x;
            p[count].y = y + 1;
            count++;
        }
        if ((*f)(board, x + 1, y) == 1) {
            p[count].x = x + 1;
            p[count].y = y;
            count++;
        }
    }
    // top row
    else if (x == 0 && y > 0 && y < board->C - 1) {
        if ((*f)(board, x, y + 1) == 1) {
            p[count].x = x;
            p[count].y = y + 1;
            count++;
        }
        if ((*f)(board, x + 1, y) == 1) {
            p[count].x = x + 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y - 1) == 1) {
            p[count].x = x;
            p[count].y = y - 1;
            count++;
        }
    }
    // top right corner
    else if (x == 0 && y == board->C - 1) {
        if ((*f)(board, x + 1, y) == 1) {
            p[count].x = x + 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y - 1) == 1) {
            p[count].x = x;
            p[count].y = y - 1;
            count++;
        }
    }

    // bottom left corner
    else if (x == board->R - 1 && y == 0) {
        if ((*f)(board, x - 1, y) == 1) {
            p[count].x = x - 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y + 1) == 1) {
            p[count].x = x;
            p[count].y = y + 1;
            count++;
        }
    }

    // bottom row
    else if (x == board->R - 1 && y > 0 && y < board->C - 1) {
        if ((*f)(board, x - 1, y) == 1) {
            p[count].x = x - 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y + 1) == 1) {
            p[count].x = x;
            p[count].y = y + 1;
            count++;
        }
        if ((*f)(board, x, y - 1) == 1) {
            p[count].x = x;
            p[count].y = y - 1;
            count++;
        }
    }

    // bottom right corner
    else if (x == board->R - 1 && y == board->C - 1) {
        if ((*f)(board, x - 1, y) == 1) {
            p[count].x = x - 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y - 1) == 1) {
            p[count].x = x;
            p[count].y = y - 1;
            count++;
        }
    }
    // left border
    else if (x > 0 && x < board->R - 1 && y == 0) {
        if ((*f)(board, x - 1, y) == 1) {
            p[count].x = x - 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y + 1) == 1) {
            p[count].x = x;
            p[count].y = y + 1;
            count++;
        }
        if ((*f)(board, x + 1, y) == 1) {
            p[count].x = x + 1;
            p[count].y = y;
            count++;
        }

    }

    // right border
    else if (x > 0 && x < board->R - 1 && y == board->C - 1) {
        if ((*f)(board, x - 1, y) == 1) {
            p[count].x = x - 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x + 1, y) == 1) {
            p[count].x = x + 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y - 1) == 1) {
            p[count].x = x;
            p[count].y = y - 1;
            count++;
        }
    }
    // the rest
    else {
        if ((*f)(board, x - 1, y) == 1) {
            p[count].x = x - 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y + 1) == 1) {
            p[count].x = x;
            p[count].y = y + 1;
            count++;
        }
        if ((*f)(board, x + 1, y) == 1) {
            p[count].x = x + 1;
            p[count].y = y;
            count++;
        }
        if ((*f)(board, x, y - 1) == 1) {
            p[count].x = x;
            p[count].y = y - 1;
            count++;
        }
    }

    // if can move
    if (count != 0) {
        return p[(board->generation + x + y) % count];
    }

    if (count == 0) {
        POS er;
        er.x = er.y = -1;
        return er;
    }
}

void resolve_conflit(BOARD* b, OBJECT* obj, int x, int y) {
    if (obj->type == 'R') {
        if (obj->GEN_PROC_RABBITS > b->map[x][y].object->GEN_PROC_RABBITS) {
            b->map[x][y].object = obj;
        }
    } else if (obj->type == 'F') {
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
void procriate(BOARD* board, BOARD* board_aux, OBJECT* obj, int x, int y) {
    if (obj->type == 'R') {
        // reset the rabbit gen_proc
        obj->GEN_PROC_RABBITS = 0;

        // create a new rabbit
        OBJECT* new_rabbit = malloc(sizeof(OBJECT));
        new_rabbit->type = 'R';

        // move the old one and replace with the newer
        board_aux->map[x][y].object = new_rabbit;
    } else {
        // reset the fox gen_proc
        obj->GEN_PROC_FOXES = 0;

        // create a new fox
        OBJECT* new_fox = malloc(sizeof(OBJECT));
        new_fox->type = 'F';

        // move the old one and replace with the newer
        board_aux->map[x][y].object = new_fox;
    }
}
void move_foxes(BOARD* board, BOARD* board_aux) {
    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            if (board->map[i][j].object->type == 'F') {
                board->map[i][j].object->GEN_FOOD_FOXES++;
                board->map[i][j].object->GEN_PROC_FOXES++;

                POS p = get_cell(board, i, j, check_rabbit);

                // if found a rabbit

                if (p.x != -1) {
                    if (board_aux->map[p.x][p.y].object->type == 'F') {
                        resolve_conflit(board_aux,
                                        board_aux->map[p.x][p.y].object, p.x,
                                        p.y);
                    } else {
                        board->map[i][j].object->GEN_FOOD_FOXES = 0;

                        board_aux->map[p.x][p.y].object =
                            board->map[i][j].object;

                        if (board->map[i][j].object->GEN_PROC_FOXES ==
                            board->GEN_PROC_FOXES + 1) {
                            procriate(board, board_aux,
                                      board_aux->map[p.x][p.y].object, i, j);
                        } else {
                            OBJECT* empty = malloc(sizeof(OBJECT));
                            empty->type = '.';
                            board_aux->map[i][j].object = empty;
                        }
                    }
                } else {
                    // see if starved
                    if (board->map[i][j].object->GEN_FOOD_FOXES ==
                        board->GEN_FOOD_FOXES) {
                        OBJECT* empty = malloc(sizeof(OBJECT));
                        empty->type = '.';
                        board_aux->map[i][j].object = empty;
                    } else {
                        p = get_cell(board, i, j, check_empty);
                        if (board_aux->map[p.x][p.y].object->type == 'F') {
                            resolve_conflit(board_aux,
                                            board_aux->map[p.x][p.y].object,
                                            p.x, p.y);
                        } else {
                            board_aux->map[p.x][p.y].object =
                                board->map[i][j].object;

                            if (board->map[i][j].object->GEN_PROC_FOXES ==
                                board->GEN_PROC_FOXES + 1) {
                                procriate(board, board_aux,
                                          board_aux->map[p.x][p.y].object, i,
                                          j);
                            } else {
                                OBJECT* empty = malloc(sizeof(OBJECT));
                                empty->type = '.';
                                board_aux->map[i][j].object = empty;
                            }
                        }
                    }
                }
            }
        }
    }
}

void move_rabbits(BOARD* board, BOARD* board_aux) {
    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            if (board->map[i][j].object->type == 'R') {
                board->map[i][j].object->GEN_PROC_RABBITS++;

                POS p = get_cell(board, i, j, check_empty);

                if (p.x != -1) {
                    /* conflict */
                    if (board_aux->map[p.x][p.y].object->type == 'R') {
                        resolve_conflit(board_aux,
                                        board_aux->map[p.x][p.y].object, p.x,
                                        p.y);
                    } else {
                        board_aux->map[p.x][p.y].object =
                            board->map[i][j].object;

                        if (board->map[i][j].object->GEN_PROC_RABBITS ==
                            board->GEN_PROC_RABBITS - 1) {
                            procriate(board, board_aux,
                                      board_aux->map[p.x][p.y].object, i, j);
                        } else {
                            OBJECT* empty = malloc(sizeof(OBJECT));
                            empty->type = '.';
                            board_aux->map[i][j].object = empty;
                        }
                    }
                }
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
    board->C = aux->C = C;
    board->R = aux->R = R;

    empty_board(board);
    recieve_input(board, N, GEN_PROC_FOXES, GEN_FOOD_FOXES, GEN_PROC_FOXES);
    for (int i = 0; i < N_GEN; i++) {
        copy_board(aux, board);
        move_rabbits(board, aux);
        copy_board(board, aux);
        move_foxes(board, aux);
        copy_board(board, aux);
        // print_board(board);

        board->generation++;
    }
    printf("--------------------------------------------------\n");
    for (int i = 0; i < board->R; i++) {
        for (int j = 0; j < board->C; j++) {
            if (board->map[i][j].object->type == '*') {
                printf("ROCK %d %d\n", i, j);
            } else if (board->map[i][j].object->type == 'R') {
                printf("RABBIT %d %d\n", i, j);

            } else if (board->map[i][j].object->type == 'F') {
                printf("FOX %d %d\n", i, j);
            }
        }
    }
    return 0;
}
