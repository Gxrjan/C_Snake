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

typedef struct apple_node {
    int i, j;
    struct apple_node *next;
} AppleNode;

typedef struct queue {
    Node *head;
    Node *tail;
} Queue;
 
typedef Vector Apple;

typedef struct game {
    AppleNode *apples;
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
int length(Game *g);
int occupied_by_snake(Game *g, int i, int j);
int occupied_by_apple(Game *g, int i, int j);
void add_random_apple(Game *g);
void add_random_apple_with_possibility(Game *g, int bound, int attempts);
void step_back(Game *g);
