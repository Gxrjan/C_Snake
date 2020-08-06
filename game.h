#include<stdio.h>
#include<pthread.h>

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


typedef struct dirchange {
    int di, dj, index;
} Dirchange;

typedef struct node {
    Dirchange change;
    struct node *next;
} Node;

typedef struct queue {
    Node *head;
    Node *tail;
} Queue;

typedef struct game {
    Snake s;
    Queue q;
    pthread_mutex_t lock;
    int changes_len;
    int rows, columns;
    int len;
} Game;



void init_game(Game *g, int rows, int columns, int start_len);
void init_cell(Cell *c, int i, int j, int di, int dj);
void init_queue(Queue *q);
Node *create_node(Dirchange change);
void enqueue(Queue *q, Dirchange change);
Dirchange peak(Queue *q);
Dirchange dequeue(Queue *q);
int is_empty(Queue *q);
