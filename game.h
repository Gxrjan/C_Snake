#include<stdio.h>

typedef int *  Array;
typedef int ** Matrix;

typedef struct vector {
    int di, dj;
} Vector;

typedef struct cell {
    int i, j;
    int di, dj;
} Cell;
typedef Cell * Snake;

typedef struct game {
    Snake s;
    int rows, columns;
    int len;
} Game;

typedef struct dirchange {
    int di, dj, index
}

void init_game(Game *g, int rows, int columns);
void init_cell(Cell *c, int i, int j, int di, int dj);
