#include<stdio.h>
#include"game.h"
#include<stdlib.h>
#include"conio.h"
#include<unistd.h>
#include<pthread.h>


void init_board(Matrix *m, int rows, int columns)
{
    *m = malloc(rows*sizeof(Array));
    for (int j=0;j<columns;j++) {
        (*m)[j] = malloc(columns*sizeof(int));
    }
}

void print_board(Game *g)
{
    //update_board(g);
    g->board[g->c.i][g->c.j] = 1;
    for (int i=0;i<g->rows;i++)
        for (int j=0;j<g->columns;j++)
            printf("%c%s", (g->board[i][j]==1) ? 'o' : ' ', ((j==g->columns-1) ? "\n" : ""));
}

void load_new_frame(Game *g)
{
    g->board[g->c.i][g->c.j] = 0;
    g->c.i = (g->c.i + g->c.direction.di) % g->rows;
    if (g->c.i == -1)
        g->c.i = g->rows-1;
    g->c.j = (g->c.j + g->c.direction.dj) % g->rows;
    if (g->c.j == -1)
        g->c.j = g->columns-1;
    g->board[g->c.i][g->c.j] = 1;
}


void init_game(Game *g, int rows, int columns)
{
    init_board(&(g->board), rows, columns);
    init_cell(&(g->c), 0, 0, 0, 1);
    g->rows = rows;
    g->columns = columns;
    g->i=0;
    g->j=0; 
}

void init_cell(Cell *c, int i, int j, int di, int dj)
{
    c->i = i;
    c->j = j;
    c->direction.di = di;
    c->direction.dj = dj;
}

int valid(Game *g, int i, int j)
{
    return i>=0 && i<g->rows && j>=0 && j<g->columns;
}

void step_forward(Game *g)
{
    g->c.i += g->c.direction.di;
    g->c.j += g->c.direction.dj;
}

int move_player(Game *g, char d)
{
    switch(d) {
        case 'w':
            g->c.direction.di = -1;
            g->c.direction.dj = 0; 
            return 1;
            break;
        case 's':
            g->c.direction.di = 1;
            g->c.direction.dj = 0;
            return 1; 
            break;
        case 'd':
            g->c.direction.di = 0;
            g->c.direction.dj = 1; 
            return 1;
            break;
        case 'a':
            g->c.direction.di = 0;
            g->c.direction.dj = -1; 
            return 1;
            break;
        default:
            return 0;
            break;
    }
    return 0;
}


void start(Game *g)
{
    char cmd;
    system("clear");
    print_board(g);
    while (1) {
        //update_board(g);
        //cmd = getch();
        //move_player(g, cmd);
        sleep(1);
        load_new_frame(g);
        system("clear");
        print_board(g);
    }
}

static void * get_cmd(void *g)
{
    while (1) {
        char cmd = -1;
        cmd = getch();
        move_player(g, cmd);
    }
}

int main(int argc, char *argv[])
{
    int status;
    Game g;
    init_game(&g, 20, 20);
    pthread_t cmd_thread;
    status = pthread_create(&cmd_thread, NULL, get_cmd, &g);


    start(&g);
    return 0;
}
