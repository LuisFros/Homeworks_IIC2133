// #include "board.h"
// // #include "hash.h"

// #include <stdlib.h>
// #include <stdio.h>
// #include <stdbool.h>
// #include <string.h>

// Board *board_init(int size)
// {
//     /* Abrimos la interfaz grafica */

//     Board* board = malloc(sizeof(Board));
//     board->size = size;
//     board->grid = malloc(sizeof(int * ) * size);
//     for (int row = 0; row < size; row++)
//     {
//         board->grid[row] = malloc(sizeof(int) * size);
//         for (int col = 0; col < size; col++)
//         {
//             board->grid[row][col] = 0;
//         }
//     }
//     board->hash_table=ht_init();

//     board->hash_table->size=83;
//     return board;

//     }
// }
