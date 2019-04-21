#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
// #include "board.h"

typedef struct hash_item{
    int key;
    int value;
}HashItem;

typedef struct hash_table{
    int size;
    int value; // ultimo valor de hash usado
    int count; //counter de espacios ocupados
    HashItem** items;
}HashTable;

typedef struct board{
    int size;
    int **grid;
    int empty_x;
    int empty_y;
    HashTable *hash_table;
    int *counter_array;
}Board;





HashTable * ht_init();
HashTable * ht_init_sized(const int size);


HashItem * hi_init(int key,int value);


int get_nth_prime(int n);

int get_state(Board *tablero);
int  incremental_update1(Board *tablero,int row1,int col1,int row2,int col2);
void update_hash_table(Board *tablero,int hash_value);


Board * board_init(int size);
#endif 
