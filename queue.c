#include<stdio.h>
#include<stdlib.h>
#include"game.h"

void init_queue(Queue *q)
{
    q->head = NULL;
    q->tail = NULL;
}

Node *create_node(Dirchange change)
{
    Node *node = malloc(sizeof(Node));
    node->change = change;
    node->next = NULL;
    return node;
}

void enqueue(Queue *q, Dirchange change) {
    if (q->head==NULL && q->tail==NULL) {
        q->head = create_node(change);
        q->tail = q->head;
        return;
    }
    (q->tail)->next = create_node(change);
    q->tail = (q->tail)->next;
}

Dirchange peak(Queue *q)
{
    return (q->head)->change;
}

Dirchange dequeue(Queue *q)
{
    Dirchange result = q->head->change;
    Node *temp = q->head;
    if (q->head==q->tail) {
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head = (q->head)->next;
    }
    free(temp);
    return result;
}

int is_empty(Queue *q)
{
    return q->head == NULL && q->tail == NULL;
}

int length(Game *g)
{
    pthread_mutex_lock(&(g->lock));
    int result = 0;
    for (Node *n=g->q.head;n!=NULL;n=n->next)
        result++;
    pthread_mutex_unlock(&(g->lock));
    return result;
}



//int main(void)
//{
//    Queue q;
//    init_queue(&q);
//    for (int i=0;i<5;i++) {
//        Dirchange ch;
//        ch.di = 1;
//        ch.dj = 1;
//        ch.index = i;
//        enqueue(&q, ch);
//    }
//    for (Node *n = q.head;n!=NULL;n=n->next)
//        printf("%d %d %d\n", n->change.di, n->change.dj, n->change.index++);
//    //for (Node *n = q.head;n!=NULL;n=n->next)
//    //    printf("%d %d %d\n", n->change.di, n->change.dj, n->change.index++);
//
//}
