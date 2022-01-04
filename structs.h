
typedef struct {
    int x;
    int y;
} POS;

typedef struct {
    int GEN_PROC_FOXES;
    int GEN_FOOD_FOXES;
    int GEN_PROC_RABBITS;
    char type;

} OBJECT;

typedef struct {
    OBJECT* object;

} CELL;

typedef struct {
    CELL** map;
    int GEN_PROC_FOXES;
    int GEN_FOOD_FOXES;
    int GEN_PROC_RABBITS;
    int generation;
    int R;
    int C;
} BOARD;
