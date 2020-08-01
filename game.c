#include<stdio.h>
#include"game.h"
#include<stdlib.h>
#include"conio.h"
#include<unistd.h>
#include<pthread.h>


void init_snake(Snake *s, int len, int di, int dj)
{
    *s = malloc(len*sizeof(Cell));
    for (int n=0;n<len;n++) {
        (*s)[n].i = 0;
        (*s)[n].j = len-n;
        (*s)[n].di = di;
        (*s)[n].dj = dj;
        
    }
        
}


int belongs_to_snake(Snake *s, int len,  int i, int j)
{
    for (int n=0;n<len;n++)
        if ((*s)[n].i==i && (*s)[n].j==j)
            return 1;
    return 0;
}


void print_board(Game *g)
{
    for (int i=0;i<g->rows;i++)
        for (int j=0;j<g->columns;j++) {
            if (belongs_to_snake(&(g->s), g->len, i, j))
                printf("%c", 'o');
            else
                printf(" ");
            if (j==g->columns-1)
                printf("\n");
        }
                        
}


void init_game(Game *g, int rows, int columns)
{
    init_snake(&(g->s), 2, 0, 1);
    g->rows = rows;
    g->columns = columns;
    g->len = 2;
}

void init_cell(Cell *c, int i, int j, int di, int dj)
{
    c->i = i;
    c->j = j;
    c->di = di;
    c->dj = dj;
}

int valid(Game *g, int i, int j)
{
    return i>=0 && i<g->rows && j>=0 && j<g->columns;
}

void step_forward(Game *g)
{
    for (int n=0;n<g->len;n++) {
        g->s[n].i = (g->s[n].i + g->s[n].di) % g->rows;
        if (g->s[n].i == -1)
            g->s[n].i = g->rows - 1;
        g->s[n].j = (g->s[n].j + g->s[n].dj) % g->columns;
        if (g->s[n].j == -1)
            g->s[n].j = g->columns - 1;
    }
}

//int move_player(Game *g, char d)
//{
//    switch(d) {
//        case 'w':
//            g->c.direction.di = -1;
//            g->c.direction.dj = 0; 
//            return 1;
//            break;
//        case 's':
//            g->c.direction.di = 1;
//            g->c.direction.dj = 0;
//            return 1; 
//            break;
//        case 'd':
//            g->c.direction.di = 0;
//            g->c.direction.dj = 1; 
//            return 1;
//            break;
//        case 'a':
//            g->c.direction.di = 0;
//            g->c.direction.dj = -1; 
//            return 1;
//            break;
//        default:
//            return 0;
//            break;
//    }
//    return 0;
//}


void start(Game *g)
{
    char cmd;
    system("clear");
    print_board(g);
    while (1) {
        //update_board(g);
        //cmd = getch();
        //move_player(g, cmd);
        step_forward(g);
        sleep(1);
        system("clear");
        print_board(g);
    }
}

static void * get_cmd(void *g)
{
    while (1) {
        char cmd = -1;
        cmd = getch();
        //move_player(g, cmd);
    }
}

int main(int argc, char *argv[])
{
    int status;
    Game g;
    init_game(&g, 20, 20);
    //pthread_t cmd_thread;
    //status = pthread_create(&cmd_thread, NULL, get_cmd, &g);


    start(&g);
    return 0;
}
