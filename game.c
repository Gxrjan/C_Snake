#include<stdio.h>
#include"game.h"
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<termios.h>
#include<time.h>
#define DELAY 300000


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
                if (g->s[0].i==i && g->s[0].j==j)
                    printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, 'o');
                else
                    printf("%c", 'o');
            else if (occupied_by_apple(g, i, j))
                printf("%c", 'a');
            else
                printf(" ");
            if (j==g->columns-1)
                printf("\n");
        }
                        
}


void init_game(Game *g, int rows, int columns, int start_len)
{
    g->apples = NULL;
    init_snake(&(g->s), start_len, 0, 1);
    init_queue(&(g->q));
    pthread_mutex_init(&(g->lock), NULL);
    g->rows = rows;
    g->columns = columns;
    g->len = start_len;
    g->changes_len = 0;
    g->collision_detected = 0;
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
    int apple_eaten = 0;
    int collision_detected = 0;
    for (int n=1;n<g->len;n++) {
        if (g->s[0].i == g->s[n].i && g->s[0].j==g->s[n].j) {
            g->collision_detected = 1;
            return;
        }
    }

    AppleNode *prev = NULL;
    AppleNode *n = g->apples;
    while (n!=NULL) {
        if (g->s[0].i==n->i && g->s[0].j==n->j) {
            apple_eaten = 1;
            if (prev==NULL) {
                g->apples = (g->apples)->next;
                free(n);
            } else {
                prev->next = n->next;
                free(n);
            }
            break;
        }
        prev = n;
        n = n->next;
    }
    if (apple_eaten) {
        Snake newSnake = malloc((g->len+1)*sizeof(Cell));
        newSnake[0].i = g->s[0].i;
        newSnake[0].j = g->s[0].j;
        newSnake[0].di = g->s[0].di;
        newSnake[0].dj = g->s[0].dj;
        step_back(g);
        for (int i=0;i<g->len;i++)
            newSnake[i+1] = g->s[i];
        free(g->s);
        g->s = newSnake;
        g->len++;
        //step_back(g);
    }
}

void step_back(Game *g)
{
    for (int n=0;n<g->len;n++) {
        g->s[n].i = (g->s[n].i - g->s[n].di) % g->rows;
        if (g->s[n].i == g->rows)
            g->s[n].i = 0;
        g->s[n].j = (g->s[n].j - g->s[n].dj) % g->columns;
        if (g->s[n].j == g->columns)
            g->s[n].j = 0;
    }

}



int move_player(Game *g, char d)
{
   // pthread_mutex_lock(&(g->lock));
    int k = 0;
    int status;
    while ((status = pthread_mutex_trylock(&(g->lock))) != 0) {
        if (k == 0) {
            //printf("Trying to lock: %d\n", status);
            k++;
        }
    }
    //printf("Move got lock\n");
    Dirchange change;
    change.index = 0;
    switch(d) {
        case 'w':
            if (g->s[0].di != 1) {
                change.di = -1;
                change.dj = 0;
                enqueue(&(g->q), change);
            }
            pthread_mutex_unlock(&(g->lock));
            return 1;
            break;
        case 's':
            if (g->s[0].di != -1) {
                change.di = 1;
                change.dj = 0;
                enqueue(&(g->q), change);
            }
            pthread_mutex_unlock(&(g->lock));
            return 1; 
            break;
        case 'd':
            if (g->s[0].dj != -1) {
                change.di = 0;
                change.dj = 1;
                enqueue(&(g->q), change);
            }
            pthread_mutex_unlock(&(g->lock));
            return 1;
            break;
        case 'a':
            if (g->s[0].dj != 1) {
                change.di = 0;
                change.dj = -1;
                enqueue(&(g->q), change);
            }
            pthread_mutex_unlock(&(g->lock));
            return 1;
            break;
        default:
            pthread_mutex_unlock(&(g->lock));
            return 0;
            break;
    }
    return 0;
    //printf("About to release lock\n");
    pthread_mutex_unlock(&(g->lock));
    //printf("Move released lock\n");
}

void apply_changes(Game *g)
{
    int k = 0;
    int status;
    while ((status = pthread_mutex_trylock(&(g->lock))) != 0) {
        if (k == 0) {
            //printf("Apply Trying to lock: %d\n", status);
            k++;
        }
    }
    //printf("Apply got lock\n");
    Node *n;
    for (n=g->q.head;n!=NULL;n=n->next) {
        //printf("Apply\n");
        Dirchange ch = n->change;
        g->s[ch.index].di = ch.di;
        g->s[ch.index].dj = ch.dj;
        n->change.index++;
    }
    //printf("Apply About to release lock\n");

    pthread_mutex_unlock(&(g->lock));
    //printf("Apply released lock\n");
}


void filter_changes(Game *g)
{
    int k = 0;
    int status;
    while ((status = pthread_mutex_trylock(&(g->lock))) != 0) {
        if (k == 0) {
            //printf("Filter Trying to lock: %d\n", status);
            k++;
        }
    }
    //pthread_mutex_lock(&(g->lock));
    //printf("Filter got lock\n");
    while (!is_empty(&(g->q))) {
        //printf("Filter\n");
        if (peak(&(g->q)).index > g->len-1)
            dequeue(&(g->q));
        else
            break;
    }
    //printf("Filter About to release lock\n");
    pthread_mutex_unlock(&(g->lock));
    //printf("Filter released lock\n");
}

void start(Game *g)
{
    char cmd;
    system("clear");
    print_board(g);
    while (1) {
        apply_changes(g);
        step_forward(g);
        if (g->collision_detected) {
            system("clear");
            print_board(g);
            printf("Game over\n");
            return;
        }
        filter_changes(g);
        add_random_apple_with_possibility(g, 5, 1);
        usleep(DELAY);
        system("clear");
        print_board(g);
        //printf("%d\n", length(g));
    }
}

static void * get_cmd(void *g)
{
    while (1) {
        char cmd = -1;
        cmd = getchar();
        if (cmd != -1)
            move_player(g, cmd);
    }
}

int occupied_by_snake(Game *g, int i, int j)
{
    for (int k=0;k<g->len;k++)
       if (g->s[k].i==i && g->s[k].j==j)
          return 1;
    return 0; 
}

int occupied_by_apple(Game *g, int i, int j)
{
    for (AppleNode *n=g->apples;n!=NULL;n=n->next)
       if (n->i==i && n->j==j)
           return 1;
    return 0;
}

void add_random_apple(Game *g)
{
    AppleNode *n = malloc(sizeof(AppleNode));
    n->next = g->apples;
    int i, j;
    do {
        i = rand() % g->rows;
        j = rand() % g->columns;
    } while(occupied_by_snake(g, i, j) || occupied_by_apple(g, i, j));
    n->i = i;
    n->j = j;
    g->apples = n;
}

void add_random_apple_with_possibility(Game *g, int bound, int attempts)
{
    int n = rand() % bound;
    int i;
    for (int k=0;k<attempts;k++)
        if ((i=(rand() % bound))!=n)
           return;
    add_random_apple(g); 
}

int main(int argc, char *argv[])
{

    int status;
    Game g;

    init_game(&g, 20, 20, 2);
    pthread_t cmd_thread;
    status = pthread_create(&cmd_thread, NULL, get_cmd, &g);
    if (status != 0) {
        printf("Error creating thread\n");
        exit(-1);
    }

    // prepare randomness
    time_t secs = time(NULL);
    srand(secs);

    // prepare console for input
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    start(&g);

    // Reset console
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return 0;
}
