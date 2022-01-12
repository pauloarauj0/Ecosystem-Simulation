#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

/* global variables */
int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;
int cur_gen;

object** board;
object** board_aux;
omp_lock_t* lock;
/* ---------------- */

/**
 * @brief Innitialize the board
 *
 */
void start_board() {
    int x, y;
#pragma omp parallel for private(x, y)
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            object empty;
            empty.type = '.';
            empty.GEN_FOOD_FOXES = 0;
            empty.GEN_PROC_FOXES = 0;
            empty.GEN_PROC_RABBITS = 0;
            board[x][y] = empty;
            board_aux[x][y] = empty;
        }
    }
}
/**
 * @brief Debug
 *
 */
void print_board() {
    int x, y;

    printf("\n");
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            printf("%c ", board[x][y].type);
        }
        printf("\n");
    }
    printf("\n");
}
/**
 * @brief Scan all the foxes/rabbits
 *
 */
void recieve_objects() {
    char* type = malloc(sizeof(char));
    int i, x, y;

    for (i = 0; i < N; i++) {
        scanf("%s %d %d", type, &x, &y);
        object obj;
        if (strcmp(type, "RABBIT") == 0) {
            obj.type = 'R';
            obj.GEN_PROC_RABBITS = 0;
        } else if (strcmp(type, "FOX") == 0) {
            obj.type = 'F';
            obj.GEN_PROC_FOXES = 0;
            obj.GEN_FOOD_FOXES = 0;
        } else if (strcmp(type, "ROCK") == 0) {
            obj.type = '*';
        }
        board[x][y] = obj;
    }
}
int inside_board(int x, int y) {
    if (x < 0 || x >= R) return 0;
    if (y < 0 || y >= C) return 0;
    return 1;
}

int empty_cell(int x, int y) {
    if (inside_board(x, y) && board[x][y].type == '.') return 1;

    return 0;
}
/**
 * @brief Checks for conflicts between rabbits
 *
 * @param rabbit new rabbit
 * @param cur already at that pos
 */
void rabbit_conflict(object* rabbit, object cur) {
    // has already a rabbit
    if (rabbit->type == 'R') {
        if (cur.GEN_PROC_RABBITS + 1 > rabbit->GEN_PROC_RABBITS) {
            rabbit->GEN_PROC_RABBITS = cur.GEN_PROC_RABBITS + 1;
        }
    } else {
        rabbit->type = cur.type;
        rabbit->GEN_PROC_RABBITS = cur.GEN_PROC_RABBITS + 1;
    }
}
/**
 * @brief Checks if a rabbit can procriate. If it can, leave a new rabbit.
 *
 * @param cur current rabbit
 * @param x
 * @param y
 */
void procriate_rabbit(object* cur, int x, int y) {
    if (cur->GEN_PROC_RABBITS == GEN_PROC_RABBITS) {
        object* new_rabbit = &board_aux[x][y];
        new_rabbit->type = cur->type;
        new_rabbit->GEN_PROC_RABBITS = 0;

        /*if put at 0, the cur would procriate and
        move and the gen_proc_rabbit would be 1 instead of 0
        */
        cur->GEN_PROC_RABBITS = -1;
    }
}

/**
 * @brief Move each rabbit
 *
 */
void move_rabbit(int x, int y) {
    object cur = board[x][y];
    pos list_pos[4];
    int count = 0;
    // North
    if (empty_cell(x - 1, y) == 1) {
        list_pos[count].x = x - 1;
        list_pos[count].y = y;
        count++;
    }
    // East
    if (empty_cell(x, y + 1) == 1) {
        list_pos[count].x = x;
        list_pos[count].y = y + 1;
        count++;
    }
    // South
    if (empty_cell(x + 1, y) == 1) {
        list_pos[count].x = x + 1;
        list_pos[count].y = y;
        count++;
    }

    // West
    if (empty_cell(x, y - 1) == 1) {
        list_pos[count].x = x;
        list_pos[count].y = y - 1;
        count++;
    }

    // didnt move
    if (count == 0) {
        list_pos[0].x = x;
        list_pos[0].y = y;
        count = 1;

        // if in the same place, dont procriate
        if (cur.GEN_PROC_RABBITS == GEN_PROC_RABBITS) {
            cur.GEN_PROC_RABBITS = GEN_PROC_RABBITS - 1;
        }
    } else {
        // procriate?
        procriate_rabbit(&cur, x, y);
    }
    pos pos = list_pos[(x + y + cur_gen) % count];
    omp_set_lock(&lock[pos.x]);

    // move
    object* rabbit = &board_aux[pos.x][pos.y];

    rabbit_conflict(rabbit, cur);

    omp_unset_lock(&lock[pos.x]);
}
/**
 * @brief Move all the rabbits
 *
 */
void move_rabbits() {
    int x, y;
#pragma omp parallel for private(x, y)
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            if (board[x][y].type == 'R') {
                move_rabbit(x, y);
            }
        }
    }
}
void procriate_foxes(object* cur, int x, int y) {
    if (cur->GEN_PROC_FOXES == GEN_PROC_FOXES) {
        object* new_fox = &board_aux[x][y];
        new_fox->type = cur->type;
        new_fox->GEN_FOOD_FOXES = 0;
        new_fox->GEN_PROC_FOXES = 0;
        cur->GEN_PROC_FOXES = -1;
    }
}
void foxes_conflicts(object* fox, object cur) {
    if (fox->type == 'F') {
        if (cur.GEN_PROC_FOXES + 1 > fox->GEN_PROC_FOXES) {
            fox->GEN_PROC_FOXES = cur.GEN_PROC_FOXES + 1;

        } else if (cur.GEN_PROC_FOXES + 1 == fox->GEN_PROC_FOXES) {
            if (cur.GEN_FOOD_FOXES + 1 < fox->GEN_FOOD_FOXES) {
                fox->GEN_FOOD_FOXES = cur.GEN_FOOD_FOXES + 1;
            }
        }
    } else {
        if (fox->type == 'R') {
            // ate a rabbit
            fox->GEN_FOOD_FOXES = 0;
        } else {
            fox->GEN_FOOD_FOXES = cur.GEN_FOOD_FOXES + 1;
        }
        fox->GEN_PROC_FOXES = cur.GEN_PROC_FOXES + 1;
        fox->type = 'F';
    }
}
void move_fox(int x, int y) {
    object cur = board[x][y];
    pos list_pos[4];
    int count = 0;

    // North
    if (inside_board(x - 1, y) && board[x - 1][y].type == 'R') {
        list_pos[count].x = x - 1;
        list_pos[count].y = y;
        count++;
    }
    // East
    if (inside_board(x, y + 1) && board[x][y + 1].type == 'R') {
        list_pos[count].x = x;
        list_pos[count].y = y + 1;
        count++;
    }
    // South
    if (inside_board(x + 1, y) && board[x + 1][y].type == 'R') {
        list_pos[count].x = x + 1;
        list_pos[count].y = y;
        count++;
    }
    // West
    if (inside_board(x, y - 1) && board[x][y - 1].type == 'R') {
        list_pos[count].x = x;
        list_pos[count].y = y - 1;
        count++;
    }

    if (count == 0) {
        // will starve
        if (cur.GEN_FOOD_FOXES == GEN_FOOD_FOXES - 1) return;

        // North
        if (empty_cell(x - 1, y) == 1) {
            list_pos[count].x = x - 1;
            list_pos[count].y = y;
            count++;
        }
        // East
        if (empty_cell(x, y + 1) == 1) {
            list_pos[count].x = x;
            list_pos[count].y = y + 1;
            count++;
        }
        // South
        if (empty_cell(x + 1, y) == 1) {
            list_pos[count].x = x + 1;
            list_pos[count].y = y;
            count++;
        }

        // West
        if (empty_cell(x, y - 1) == 1) {
            list_pos[count].x = x;
            list_pos[count].y = y - 1;
            count++;
        }
    }
    if (count == 0) {
        list_pos[0].x = x;
        list_pos[0].y = y;
        count = 1;
        if (cur.GEN_PROC_FOXES == GEN_PROC_FOXES) {
            cur.GEN_PROC_FOXES = GEN_PROC_FOXES - 1;
        }
    } else {
        // procriate?
        procriate_foxes(&cur, x, y);
    }

    pos p = list_pos[(x + y + cur_gen) % count];
    omp_set_lock(&lock[p.x]);
    object* fox = &board_aux[p.x][p.y];

    foxes_conflicts(fox, cur);
    omp_unset_lock(&lock[p.x]);
}

void move_foxes() {
    int x, y;
#pragma omp parallel for private(x, y)
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            if (board[x][y].type == 'F') {
                move_fox(x, y);
            }
        }
    }
}

void swap() {
    object** aux;
    aux = board;
    board = board_aux;
    board_aux = aux;
}
void copy_rabbits() {
    int x, y;
#pragma omp parallel for private(x, y)
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            if (board[x][y].type == 'R') {
                board_aux[x][y] = board[x][y];
            }
        }
    }
}
void copy_foxes() {
    int x, y;
#pragma omp parallel for private(x, y)
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            if (board[x][y].type == 'F') {
                board_aux[x][y] = board[x][y];
            }
        }
    }
}
void copy_rocks() {
    int x, y;
#pragma omp parallel for private(x, y)
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            if (board[x][y].type == '*') {
                board_aux[x][y] = board[x][y];
            }
        }
    }
}

void reset_aux() {
    int x, y;
#pragma omp parallel for private(x, y)
    for (x = 0; x < R; x++) {
        for (y = 0; y < C; y++) {
            if (board_aux[x][y].type != '*') {
                board_aux[x][y].type = '.';
                board_aux[x][y].GEN_FOOD_FOXES = 0;
                board_aux[x][y].GEN_PROC_FOXES = 0;
                board_aux[x][y].GEN_PROC_RABBITS = 0;
            }
        }
    }
}
int main() {
    scanf("%d %d %d %d %d %d %d ", &GEN_PROC_RABBITS, &GEN_PROC_FOXES,
          &GEN_FOOD_FOXES, &N_GEN, &R, &C, &N);

    int nthreads = 2;
    omp_set_num_threads(nthreads);

    /*
        make the 2 boards
    */
    board = malloc(sizeof(object*) * R);
    board_aux = malloc(sizeof(object*) * R);

    for (int i = 0; i < R; i++) {
        board[i] = (object*)malloc(sizeof(object) * C);
        board_aux[i] = (object*)malloc(sizeof(object) * C);
    }

    start_board();
    recieve_objects();

    // copy the rocks to the board_aux
    copy_rocks();

    double start = omp_get_wtime();

    lock = (omp_lock_t*)malloc(sizeof(omp_lock_t) * R);
    for (int i = 0; i < R; i++) {
        omp_init_lock(&lock[i]);
    }
    for (cur_gen = 0; cur_gen < N_GEN; cur_gen++) {
        copy_foxes();
        move_rabbits();
        swap();
        reset_aux();
        copy_rabbits();
        move_foxes();
        swap();
        reset_aux();
        // print_board();
    }
    double final = omp_get_wtime();

    // output
    for (int x = 0; x < R; x++) {
        for (int y = 0; y < C; y++) {
            if (board[x][y].type == 'R')
                printf("RABBIT %d %d\n", x, y);
            else if (board[x][y].type == 'F')
                printf("FOX %d %d\n", x, y);
            else if (board[x][y].type == '*')
                printf("ROCK %d %d\n", x, y);
        }
    }
    for (int i = 0; i < R; i++) {
        omp_destroy_lock(&lock[i]);
    }
    printf("Execution time: %lf\n", (final - start));
    return 0;
}
