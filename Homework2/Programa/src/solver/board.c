#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "../watcher/watcher.h"


Board *board_init(int height, int width){
    Board* board = malloc(sizeof(Board));
    board->done = 0;
    board->width = width;
    board->height = height;
    board->signs = malloc(sizeof(int * ) * height);
    board->types = malloc(sizeof(char * ) * height);
    for (int row = 0; row < height; row++)
    {
        board->signs[row] = malloc(sizeof(int) * width);
        board->types[row] = malloc(sizeof(char) * width);

        for (int col = 0; col < width; col++)
        {
            // Cell* new_cell=malloc(sizeof(Cell));
            board->signs[row][col]=0;
            board->types[row][col]='O';
        }
    }
    return board;
}

bool neighbours(Board *board,int row,int col,bool positive_magnet){
    int neighbours=0;
    // check top
    int sign;
    // printf("row %d col %d pos %d \n",row,col,positive_magnet);
    if (positive_magnet){
        sign=1;
    }
    else{
        sign=-1;
    }
    // printf("SIGN %d \n",sign);
    if (row>0){
        if (board->signs[row-1][col]!=sign)
        {
            // printf("row>0 %d \n",board->signs[row-1][col]);
            // printf("%d \n",board->signs[row-1][col]!=sign);
            neighbours+=1;
        }
        else{
            neighbours+=0;
        }
    }
    else if (row==0){

        neighbours+=1;
    }
    // check down
    if (row<board->height-1){
        if (board->signs[row+1][col]!=sign)
        {
        neighbours+=1;
        }
    }
    else if (row==board->height-1){
        neighbours+=1;
    }
    // check left

    if (col>0){
        if (board->signs[row][col-1]!=sign){
    
        neighbours+=1;
        }
    }
    else if (col==0){
        neighbours+=1;
    }

    // check right
    if (col<board->width-1){
        if (board->signs[row][col+1]!=sign){

        neighbours+=1;
        }
    }
    else if (col==board->width-1){
        neighbours+=1;
    }

    return (neighbours==4);
}

bool position_safe(Board *board,int row, int col,bool positive_magnet){
    // printf("Position safe (%d,%d) magnet %d \n",row,col,positive_magnet);
    // printf("TOP %d , DOWN %d, LEFT %d, RIGHT %d",board->top_restriction[col],board->down_restriction[col],board->left_restriction[row],board->right_restriction[row]);
    if (positive_magnet){
        if ((board->top_restriction[col]!=0) &(board->left_restriction[row]!=0)){
            return true;
        }
    }
    else{
        if ((board->down_restriction[col]!=0) &  (board->right_restriction[row]!=0)){
            return true;
        }
    }
    return false;
}

bool valid_combination(Board *board,int row,int col,bool positive_magnet){
    // int positive=1;
    // int negative=-1;
    bool position=position_safe(board,row,col,positive_magnet);
    bool neighbour=neighbours(board,row,col,positive_magnet);
    // printf("Vald %d %d \n",position,neighbour);
    // printf("type %c ",board->types[row][col]);
    if ((board->types[row][col]=='T') & (position) & (neighbour)){
        bool A=position_safe(board,row+1,col,!positive_magnet);
        bool B=neighbours(board,row+1,col,!positive_magnet);
        // printf("%d %d \n",A,B);
        return A & B;
    }
    else if ((board->types[row][col]=='B') & (position) & (neighbour)){
        // printf("Entra al valid comb \n");
        bool A=position_safe(board,row-1,col,!positive_magnet);
        bool B=neighbours(board,row-1,col,!positive_magnet);
        // printf("%d %d \n",A,B);
        return A & B;
    }
    else if ((board->types[row][col]=='L') & (position) & (neighbour)){
        int next=col+1;
        // printf("Entra a revisar \n");
        bool A=position_safe(board,row,next,!positive_magnet);
        // printf("A %d \n",A);
        bool B=neighbours(board,row,next,!positive_magnet);
        // printf("B %d \n",B);
        return A & B;
    }
    else if ((board->types[row][col]=='R') & (position) & (neighbour)){
        bool A=position_safe(board,row,col-1,!positive_magnet);
        bool B=neighbours(board,row,col-1,!positive_magnet);
        return A & B;
    }    
    else{
        // printf("ELSE \n");
        return false;
    }
}

bool board_is_safe(Board * board,int row, int col,bool positive_magnet){
    // printf("Is_safe() (row,col):(%d,%d) \n",row,col);
    // printf("Positive %d \n",positive_magnet);
    if (board->signs[row][col]!=0){
        // printf("condicion 1 \n");
        // printf("first if (%d,%d) \n",row,col);
        return false;
    }
    if (valid_combination(board,row,col,positive_magnet)){
        // printf("condicion 2 \n");
        return true;
    }
    else{
        // printf("condicion 3 \n");
        // printf("else (%d,%d) \n",row,col);
        return false;
    }
}

int update_restrictions(Board *board, int row, int col,int sign,bool assign){
    // printf("ROW %d COL %d assign:%d sign %d \n",row,col,assign,sign);
    //  for (int r = 0; r < board->height; r++){
    //     printf("%d ",board->right_restriction[r]);
    // }
    // printf("\n LEFT: \n");
    //  for (int r = 0; r < board->height; r++){
    //     printf("%d ",board->left_restriction[r]);
    // }
    // printf("\n");
    
    // if (row==3 & col==3){
    //     printf("Left %d \n",board->left_restriction[row]);
    //     printf("Right %d \n",board->right_restriction[row]);
    //     printf("Top %d \n",board->top_restriction[col]);
    //     printf("Down %d \n",board->down_restriction[col]);
    // }
    // printf("BEFORE UPDATE TOP %d , DOWN %d, LEFT %d, RIGHT %d \n",board->top_restriction[col],board->down_restriction[col],board->left_restriction[row],board->right_restriction[row]);

    int affected=0;
    if (assign){
        if (sign==-1){
            if ( board->right_restriction[row]>0){
                affected+=1;
                board->right_restriction[row]-=1;
            }
            if (board->down_restriction[col]>0){
                board->down_restriction[col]-=1;
                affected+=1;

            }

        }
        else if (sign==1){
            if (board->left_restriction[row]>0){
                board->left_restriction[row]-=1;
                affected+=1;

            }
            if (board->top_restriction[col]>0){
                board->top_restriction[col]-=1;
                affected+=1;
            }

        }
    }
    else{
     if (sign==-1){
            if ( board->right_restriction[row]>=0){
                board->right_restriction[row]+=1;
                affected+=1;

            }
            if (board->down_restriction[col]>=0){
                board->down_restriction[col]+=1;
                affected+=1;

            }
           
        }
        else if (sign==1){
            if (board->left_restriction[row]>=0){
                board->left_restriction[row]+=1;
                affected+=1;

            }
            if (board->top_restriction[col]>=0){
                board->top_restriction[col]+=1;
                affected+=1;
            }
        }
    }
    // printf("AFTER \n");
    // for (int r = 0; r < board->height; r++){
    //     printf("%d ",board->right_restriction[r]);
    // }
    // printf("\n LEFT: \n");
    //  for (int r = 0; r < board->height; r++){
    //     printf("%d ",board->left_restriction[r]);
    // }
    // printf("\n");
    // printf("afffe %d \n",affected);
    // printf("AFTER UPDATE TOP %d , DOWN %d, LEFT %d, RIGHT %d \n",board->top_restriction[col],board->down_restriction[col],board->left_restriction[row],board->right_restriction[row]);

    return affected;
}

bool board_deassign_magnet(Board *board, int row, int col,bool positive_magnet){
    // bool vertical=false;
    int assigned=0;
    int a=0;
    int b=0;
    int first_value=0;
    int second_value=0;
    // printf("(%d,%d) \n",row,col);
    // printf(" is positive %d \n",positive_magnet);
    if (positive_magnet){
        first_value=1;
        second_value=-1;
    }
    else{
        first_value=-1;
        second_value=1;
    }
    
    if (board->types[row][col]=='T'){    
            board->signs[row][col]=0;
            board->signs[row+1][col]=0;
            a=update_restrictions(board,row,col,first_value,false);
            b=update_restrictions(board,row+1,col,second_value,false);
            // vertical=true;
        }
    else if (board->types[row][col]=='B'){
        board->signs[row][col]=0;
        board->signs[row-1][col]=0;
        a=update_restrictions(board,row,col,first_value,false);
        b=update_restrictions(board,row-1,col,second_value,false);
        // vertical=true;

    }
    else if (board->types[row][col]=='L'){
        board->signs[row][col]=0;
        board->signs[row][col+1]=0;
        a=update_restrictions(board,row,col,first_value,false);
        b=update_restrictions(board,row,col+1,second_value,false);
        // vertical=false;
    }
    else{ // R
        board->signs[row][col]=0;
        board->signs[row][col-1]=0;
        a=update_restrictions(board,row,col,first_value,false);
        b=update_restrictions(board,row,col-1,second_value,false);
        // vertical=false;

    }
assigned=a+b;
// printf("De_assigned %d \n",assigned);
return assigned;
}

int board_assign_magnet(Board * board, int row, int col,bool positive_magnet){
    // bool vertical=false;
    // printf("Assign ROW: %d COL: %d SIGN: %d \n",row,col,positive);
    // for (int row = 0; row < board->height; row++){
    //     printf("R %d ",board->right_restriction[row]);
    //     printf("L %d ",board->left_restriction[row]);
    // }
    // printf("\n");

    // for (int row = 0; col < board->width; col++){
    //     printf("T %d ",board->top_restriction[col]);
    //     printf("D %d ",board->down_restriction[col]);
    // }
    // printf("\n");
    // printf("(%d,%d) \n",row,col);
    // printf(" is positive %d \n",positive_magnet);
    int first_value=0;
    int second_value=0;
    int assigned=0;
    if (positive_magnet){
        first_value=1;
        second_value=-1;
    }
    else{
        first_value=-1;
        second_value=1;
    }
    if (board->types[row][col]=='T'){
        board->signs[row][col]=first_value;
        board->signs[row+1][col]=second_value;
        assigned+=update_restrictions(board,row,col,first_value,true);
        assigned+=update_restrictions(board,row+1,col,second_value,true);
        // vertical=true;
    }
    else if (board->types[row][col]=='B'){
        board->signs[row][col]=first_value;
        board->signs[row-1][col]=second_value;
        assigned+=update_restrictions(board,row,col,first_value,true);
        assigned+=update_restrictions(board,row-1,col,second_value,true);
        // vertical=true;
    }
    else if (board->types[row][col]=='L'){
        board->signs[row][col]=first_value;
        board->signs[row][col+1]=second_value;
        assigned+=update_restrictions(board,row,col,first_value,true);
        assigned+=update_restrictions(board,row,col+1,second_value,true);
        // vertical=false;
    }
    else{ // R
        board->signs[row][col]=first_value;
        board->signs[row][col-1]=second_value;
        assigned+=update_restrictions(board,row,col,first_value,true);
        assigned+=update_restrictions(board,row,col-1,second_value,true);
        // vertical=false;
    }

    // printf("Assigned %d \n",assigned);
    return assigned;
}

// void board_deassign_magnet(Board * board, int row, int col){
//     bool vertical=board_assign_magnet(board, row, col,0,0);
//     return;
// }