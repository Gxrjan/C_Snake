#include<stdio.h>

typedef int *  Array;
typedef int ** Matrix;

typedef struct vector {
    int di, dj;
} Vector;

typedef struct cell {
    int i, j;
    Vector direction;
} Cell;

typedef struct game {
    Matrix board;
    Cell c;
    int rows, columns;
    int i, j;
} Game;



void init_game(Game *g, int rows, int columns);
void init_cell(Cell *c, int i, int j, int di, int dj);
void init_board(Matrix *m, int rows, int columns);
void update_board(Game *g);

