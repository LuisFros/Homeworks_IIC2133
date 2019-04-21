#pragma once
#include <stdio.h>

typedef struct node
{
  int value;
  struct node *next;
  struct node *previous;
  struct node *parent;
  int x;
  int y;
  int empty_x;
  int empty_y;
  char*board_state;
}Node;

typedef struct queue
{
  struct node *head;
  struct node *tail;
  int lenght;
}Queue;

Queue * queue_init();
Node * node_init(int value);


Node * pop(Queue * queue);

void append(Queue * queue,Node *node);