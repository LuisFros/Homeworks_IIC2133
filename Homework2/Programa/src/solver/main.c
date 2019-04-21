#include "../watcher/watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "board.h"

// Falta disminuir las restricciones !
void destroy_board(Board *board){
	for (int row = 0; row < board->height; row++)
	{
			free(board->types[row]);
			free(board->signs[row]);
		}
	free(board->types);
	free(board->signs);
	free(board);

}

int get_state_representation(Board *board){
	int sum=0;
	int prime=37;
	for (int row = 0; row < board->height; row++)
	{
		for (int col = 0; col < board->width; col++)
		{
			sum+=board->signs[row][col]*col*row*prime;
		}
	}
	return sum;
}

bool is_vertical(char cell_type){
	if (cell_type=='T'){
		return true;
	}
	else if (cell_type=='B'){
		return true;
	}
	return false;
}

void print_board(Board *board){
	for (int row = 0; row < board->height; row++)
	{
		for (int col = 0; col < board->width; col++)
		{
			printf(" %d ",board->signs[row][col]);
		}
		printf("\n");
	}
	printf("\n");

	// for (int row = 0; row < board->height; row++)
	// {
	// 	for (int col = 0; col < board->width; col++)
	// 	{
	// 		printf(" %c ",board->types[row][col]);
	// 	}
	// 	printf("\n");

	// }
	// for (int row = 0; row < board->height; row++){
	// 	printf("%d             %d \n",board->left_restriction[row],board->right_restriction[row]);
	// 	}
	// printf("\n");
	// for (int col = 0; col < board->width; col++)
	// 	{
	// 	printf("%d ",board->top_restriction[col]);
		
	// 	}
	// 	printf(" DWON \n");
	// for (int col = 0; col < board->width; col++)
	// {
	// printf("%d ",board->down_restriction[col]);
	
	// }
}

void insert_magnet(Board *board,int row, int col,bool vertical,bool positive_magnet){
	if (vertical){
		if(board->types[row][col]=='T'){
			watcher_set_magnet(row, col,vertical, positive_magnet);
		}
		else{
			watcher_set_magnet(row-1, col,vertical, !positive_magnet);
		}
	}
	else{
		if(board->types[row][col]=='L'){
			watcher_set_magnet(row, col,vertical, positive_magnet);
		}
		else{
			watcher_set_magnet(row, col-1,vertical, !positive_magnet);
		}				
	}
}

void clear_magnet(Board *board,int row, int col,bool vertical){
	if (vertical){
		if(board->types[row][col]=='T'){
			watcher_clear_magnet(row, col,vertical);
		}
		else{
			watcher_clear_magnet(row-1, col,vertical);
		}
	}
	else{
		if(board->types[row][col]=='L'){
			watcher_clear_magnet(row, col,vertical);
		}
		else{
			watcher_clear_magnet(row, col-1,vertical);
		}				
	}
}

int deassignations;
bool solver(Board * board){
	if (board->done ==0){
		// print_board(board);
		return true;
	}
	for (int row = 0; row < board->height; row++)
	{
		for (int col = 0; col < board->width; col++)
		{
			bool positive=true;
			// for (int mag =0; mag < 2; mag ++){
			// 	if (mag==0){
			// 		positive=true;
			// 		}
			// 	else{
			// 		positive=false;
			// 	}
				// print_board(board);
				// printf("Current (%d %d) \n",row,col);
				if (board_is_safe(board, row, col,positive))
				{
					// printf("IS SAFE (%d %d) \n",row,col);
					int assigned=0;
					bool vertical;
					vertical=is_vertical(board->types[row][col]);
					assigned=board_assign_magnet(board, row, col,positive);
					// 							bool vertical, bool positive
					insert_magnet(board,row,col,vertical,positive);

					// sleep(1);
					board->done -= assigned;

					// printf("%d  Assigned board \n",board->done);
					if (solver(board)){

						return true;
				}	
					// else{
					int deassigned;
					// deassignations++;
					vertical=is_vertical(board->types[row][col]);
					deassigned=board_deassign_magnet(board, row, col,positive); // backtrack
					// watcher_clear_magnet(row, col,vertical);
					// printf("%d  Deassigned \n",assigned);

					clear_magnet(board,row,col,vertical);
					board->done+=assigned;
					// printf("%d  Deassigned board \n",board->done);
					// sleep(1);
					// sleep(1);
					positive=true;

				
			}

		}
	}
	// no existe asignacion posible
	return false;
}



int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Modo de uso: %s test.txt\n", argv[0]);
		return 0;
	}

	// Abrimos la interfaz grafica y cargamos el test (No eliminar esta linea)
	watcher_load_layout(argv[1]);


  	FILE* input_file = fopen(argv[1], "r");

	// Leo el alto y el ancho
	int width, height;

	fscanf(input_file, "%d %d\n", &width, &height);

	Board *board=board_init(height,width);

	// char command[256];
	int *top_restriction = malloc(sizeof(int * ) * width);
	int *down_restriction = malloc(sizeof(int * ) * width);
	/* Leemos los numeros de las orillas de arriba y abajo y los ponemos en la interfaz*/
	for (int col = 0; col < width; col++)
	{
		int val;
		fscanf(input_file, "%d ", &val);
		if (val==0){
			top_restriction[col]=-1;
		}
		else{
			top_restriction[col]=val;
		}
		board->done+=val;
		// watcher_set_number(side, col, val);
	}
	for (int col = 0; col < width; col++)
	{
		int val;
		fscanf(input_file, "%d ", &val);
		if (val==0){
			down_restriction[col]=-1;
		}
		else{
			down_restriction[col]=val;
		}
		board->done+=val;

		// watcher_set_number(side, col, val);
	}

	board->top_restriction=top_restriction;
	board->down_restriction=down_restriction;

	
	/* Leemos los numeros de las orillas de la izquierda y la derecha y los ponemos en la interfaz*/

	int *left_restriction = malloc(sizeof(int * ) * height);
	int *right_restriction = malloc(sizeof(int * ) * height);
	for (int row = 0; row < height; row++)
	{
		int val;
		fscanf(input_file, "%d ", &val);
		if (val==0){
			left_restriction[row]=-1;
		}
		else{

			left_restriction[row]=val;
		}
		board->done+=val;

		// watcher_set_number(side, row, val);
	}
	for (int row = 0; row < height; row++)
	{
		int val;
		fscanf(input_file, "%d ", &val);

		if (val==0){
			right_restriction[row]=-1;
		}
		else{
			right_restriction[row]=val;
		}
		board->done+=val;

		// watcher_set_number(side, row, val);
	}
	
	board->left_restriction=left_restriction;
	board->right_restriction=right_restriction;
	/** Leemos las casillas del tablero para dibujar la forma inicial */
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			char type;
			fscanf(input_file, "%c ", &type);
			board->types[row][col]=type;
			// if (type == 'T')
			// 	// watcher_clear_magnet(row, col, true);
			// else if (type == 'L')
				// watcher_clear_magnet(row, col, false);
		}
	}
	// sleep(5);
	bool initial_rows[board->height];
	bool initial_columns[board->width];
	for (int row = 0; row < height; row++){
		initial_rows[row]=false;
	}
	for (int col = 0; col < width; col++){
		initial_columns[col]=false;
	}
	solver(board);

	//////////// Ejemplos de como dibujar en la interfaz //////////////

	// Estos ejemplos son para el tablero del test_01.txt y son para mostrar el
	// funcionamiento de la interfaz. Puedes experimentar con los ejemplos y luego
	// borrarlos de tu programa

	// La funcion watcher_set_magnet(row, col, vertical, positive) esta encargada
	// de dibujar los imanes. Recibe como input la posicion  izquierda del iman
	// si este es horizontal y la posicion superior del iman si este es vertical.
	// Ademas recibe un bool que indica si el iman a dibujar es vertical, y un
	// bool que indica si la posicion indicada es positiva.

	// Por ejemplo: dibujamos un iman de forma horizontal tal que en en la posicion
	// row = 2, col = 1 tiene el lado negativo, y en la posicion row = 2, col = 2
	// tiene el lado positivo.
	// watcher_set_magnet(2, 1, false, false);

	// Ahora dibujo un iman vertical en las posiciones (2, 0) -> negativo y
	// (3, 0) -> positivo
	// watcher_set_magnet(2, 0, true, false);

	// Ponemos un sleep para poder ver los cambios en la interfaz
	// sleep(5);

	// La funcion watcher_clear_magnet elimina un iman apuntando su celda superior
	// izquierda e indicando si es vertical u orizontal

	// Por ejemplo, para borrar el primer iman que dibujamos indicamos la posicione
	// y su orientacion
	// watcher_clear_magnet(2, 1, false);

	// Borramos el segundo iman de la misma manera
	// watcher_clear_magnet(2, 0, true);

	// Ojo que la interfaz no va a revisar si lo que estas haciendo es valido o no.
	// Esto implica que debes revisar que tus llamadas a la interfaz sean validas
	// para no dibujar incorrectamente y para que no se caiga.

	// Un ejemplo de dibujar mal en la pantalla seria el siguiente (debes
	// descomentarlo para verlo):
	// watcher_set_magnet(3, 0, true, true);
	// En el ejemplo anterior el domino se dibuja fuera de la pantalla

	// En el siguiente ejemplo el programa se cae porque el domino queda fuera
	// de las dimensiones del tablero y fuera de la interfaz.
	// watcher_set_magnet(-4, 6, true, true);

	// Ponemos un sleep para poder ver la interfaz
	// sleep(10);

	// OJO: Al momento de entregar tu codigo debes comentar todos los sleep de tu codigo
	// para que tu programa resuelva los test en menos de 10 segundos

	/////////////////////// Fin ejemplos /////////////////////////////

	// Cerramos la interfaz (no eliminar esta linea)
	watcher_close();
	destroy_board(board);
	fclose(input_file);
	// retornamos OK
	return 0;
}
