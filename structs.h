typedef struct {
    int x, y;
} POS;

typedef struct {
    int GEN_PROC_FOXES;
    int GEN_FOOD_FOXES;
    int GEN_PROC_RABBITS;
    POS* pos;
    char* type;

} OBJECT;

typedef struct {
    OBJECT* object;

} CELL;

typedef struct {
    CELL** map;
} BOARD;