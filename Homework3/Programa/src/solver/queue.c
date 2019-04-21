#include "queue.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

Queue * queue_init(){
    Queue *queue=(Queue *)malloc((sizeof(Queue)));
    queue->tail=NULL;
    queue->head=NULL;
    return queue;
}

Node * node_init(int  value){
    Node * node=(Node*)malloc(sizeof(Node));
    node->value=value;
    node->next=NULL;
    node->previous=NULL;
    return node;
}


Node * pop(Queue * queue){
    Node *first=queue->head;
    if(first->next!=NULL)
    {

        queue->head=first->next;
        queue->head->previous=NULL;
        queue->lenght-=1;

    }
    else
    {
        queue->head=NULL;
        queue->tail=NULL;
        queue->lenght=0;        
    }
    return first;
}

void append(Queue * queue,Node *node){
    if (queue->head==NULL){
        queue->head=node;
        queue->tail=node;
    }
    else{
        Node *last=queue->tail;
        queue->tail=node;
        queue->tail->previous=last;
        last->next=node;
    }  
    queue->lenght++;
}