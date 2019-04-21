#pragma once
#include <stdbool.h>



typedef struct cell Cell;


typedef struct cell
{
  int sign;
  char type;
};


typedef struct board Board;


struct board
{
    // int n_queens;
    int width;
    int height;
    int **signs;
    char **types;
    // Done es un int, que es la suma de todas la restricciones, cuando es ==0, es porque termino !
    int done;
    int *top_restriction;
    int *down_restriction;
    int *left_restriction;
    int *right_restriction;

};


Board *board_init(int height, int width);
void board_destroy(Board* board);
int update_restrictions(Board *board, int row, int col,int sign,bool assign);
int board_assign_magnet(Board * board, int row, int col,bool positive_magnet);
bool board_deassign_magnet(Board * board, int row, int col,bool positive_magnet);
bool position_safe(Board *board,int row, int col,bool positive_magnet);
bool neighbours(Board *board,int row,int col,bool positive_magnet);
bool valid_combination(Board *board,int row,int col,bool positive_magnet);
bool board_is_safe(Board * board,int row, int col,bool positive_magnet);

