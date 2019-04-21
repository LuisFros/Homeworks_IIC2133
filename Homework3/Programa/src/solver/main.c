#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "board.h"
#include "hash.h"
#include "queue.h" 
 // TODO : solo por facilidad, arreglar la funcion update_tabla_hash() sin tablero
// tablero->hash_table->items[81]->value
// run tests/3x3/test0.txt output.txt
// Valores utiles para tamaños de tabla de hash, estos asumen que los moviemientos son casi optimos -> muy buena heuristica + A*
// 31 es el numero maximo de movimientos para resolver 8 puzzle http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.40.9889
// 80 para 15 puzzle http://mathworld.wolfram.com/15Puzzle.html 
// 3x3 se utilizara h=k mod 31
// 4x4 se utilizara h=k mod 83 (numero primo mas cercano mayor)


// TODO2: col-1 en debugger.. numero 48 magico

// fuente: https://gist.github.com/amirmasoudabdol/f1efda29760b97f16e0e , modificado para el problema
void read_csv(int tamano, FILE *file, Board *data){

	// file = fopen(filename, "r");

	int i = 0;
    char line[4098];
  char primeralinea[100];
  fgets(primeralinea, 100, file); // Se ignora la primera linea
	while (fgets(line, 4098, file) && (i < tamano))
    {
    	// double row[ssParams->nreal + 1];
        char* tmp = strdup(line);

	    int j = 0;
	    const char* tok;
	    for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n"))
	    {
	        data->grid[i][j] = atof(tok);
          if(atof(tok)==0){
            data->empty_x=i;
            data->empty_y=j;
          }
	        // printf("%d\t", data[i][j]);
	    }
	    // printf("\n");

        free(tmp);
        i++;
    }
}

int print_tablero(Board *tablero,int n){
  for (int i=0;i<n;i++){
    for (int j=0;j<n;j++){
      printf("%d ",tablero->grid[i][j]);
    }
    printf("\n");
  }
  return 1;
}

void print_hash(Board *tablero){
  HashTable *table=tablero->hash_table;
  int len=table->size;
  for ( int i=0;i<len;i++){
    if(table->items[i]!=NULL){
      printf("indice tabla %d\n",i);
      printf("el %d \n",table->items[i]->value);
    }
  }
}

void test_function(Board *tablero){
  int hash_1=tablero->hash_table->value;
  printf("%d \n ",hash_1);

  tablero->grid[0][0]=0;
  tablero->grid[0][1]=2; // Moviemiento  la derecha para ej test0.txt
  incremental_update1(tablero,0,0,0,1);
  int hash_2=tablero->hash_table->value;
  printf("%d \n ",hash_2);


  tablero->grid[0][0]=2;
  tablero->grid[0][1]=0; // Moviemiento  la derecha para ej test0.txt
  incremental_update1(tablero,0,0,0,1);
  hash_2=tablero->hash_table->value;
  printf("%d \n ",hash_2);
}

void  test_function2(Board *tablero){
  int hash_1=tablero->hash_table->value;
  printf(" test f %d \n ",hash_1);
  tablero->grid[0][0]=0;
  tablero->grid[0][1]=2; // Moviemiento  la derecha para ej test0.txt (0,0)-->(0,1)
  print_tablero(tablero,3);
  incremental_update1(tablero,0,0,0,1);
  int hash_2=tablero->hash_table->value;
  printf("despues de incremenetal %d \n ",hash_2);
  update_hash_table(tablero,hash_2); // remover el 0
  // printf("despues duh %d \n ",*tablero->hash_table->items[hash_2%83]->value);

  tablero->grid[0][0]=2;
  tablero->grid[0][1]=0; // Moviemiento  la derecha para ej test0.txt
  incremental_update1(tablero,0,0,0,1);
  hash_2=tablero->hash_table->value;
  printf("%d \n ",hash_2);
  // return tablero;
}

Board* update_board(Board *tablero,Node *state){
  int posx=state->x;
  int posy=state->y;
  int current_value=tablero->grid[posx][posy];
  // printf("Update board state (%d,%d) -> empty (%d,%d)",posx,posy,
  // tablero->empty_x,tablero->empty_y);
  // (posx,posy)-->(e x,e y)
  tablero->grid[state->empty_x][state->empty_y]=current_value; //se realiza movimiento
  tablero->grid[posx][posy]=0; //se actualiza el valor de la pieza vacia

  /// Actualzizaciones de hash incremental

  // incremental_update1(tablero,posx,posy,tablero->empty_x,tablero->empty_y);
  tablero->hash_table->value=state->value;
  // update_hash_table(tablero,hash);

  tablero->empty_x=posx;
  tablero->empty_y=posy;
  return tablero;
}

void recursive_remake_board(Board *tablero,Node *state){
  if (state->parent==NULL){
    return;
  }
  int posx=state->x;
  int posy=state->y;
  int current_value=tablero->grid[state->empty_x][state->empty_y];
  // printf("Update board state (%d,%d) -> empty (%d,%d)",posx,posy,
  // tablero->empty_x,tablero->empty_y);
  // (posx,posy)-->(e x,e y)
  tablero->grid[posx][posy]=current_value; //se realiza movimiento
  tablero->grid[state->empty_x][state->empty_y]=0;

  recursive_remake_board(tablero,state->parent);
}
int fake_update_board(Board *tablero,int posx,int posy){
  // int posx=state->x;
  // int posy=state->y;
  int current_value=tablero->grid[posx][posy];
  // (posx,posy)-->(e x,e y)
  // int hash=incremental_update1(tablero,posx,posy,tablero->empty_x,tablero->empty_y);

  tablero->grid[tablero->empty_x][tablero->empty_y]=current_value; //se realiza movimiento
  tablero->grid[posx][posy]=0; //se actualiza el valor de la pieza vacia

  /// Actualzizaciones de hash incremental
  
  int hash=get_state(tablero);
  // update_hash_table(tablero,hash);
  
  tablero->grid[tablero->empty_x][tablero->empty_y]=0; //se realiza movimiento
  tablero->grid[posx][posy]=current_value; //se actualiza el va

  // incremental_update1(tablero,posx,posy,tablero->empty_x,tablero->empty_y);

  // tablero->empty_x=posx;
  // tablero->empty_y=posy;
  if (hash<0){
    printf("wird\n");
  }
  return hash;
}  


Node * new_succ(Board *tablero,int new_row,int new_col){
  // int aux_value=tablero->grid[new_row][new_col];
  
  /// se modifica temporalmente el tablero para calcular su estado hash
  // tablero->grid[new_row][new_col]=0;
  // tablero->grid[current_emptyx][current_emptyy]=aux_value;
  int hash_value=fake_update_board(tablero,new_row,new_col);
  Node *new_succ=node_init(hash_value); // se inicializa el nodo con el valor de hash
  // se revierten los cambios
  // tablero->grid[new_row][new_col]=aux_value;
  // tablero->grid[current_emptyx][current_emptyy]=0;
  new_succ->x=new_row;
  new_succ->y=new_col;
  new_succ->empty_x=tablero->empty_x;
  new_succ->empty_y=tablero->empty_y; 

  return new_succ;
}

bool isObjetive(Board *tablero,Node *v){
  // print_tablero(tablero,3);
  int new_col=v->y;
  int new_row=v->x;
  int aux_value=tablero->grid[new_row][new_col];
  int current_emptyx=tablero->empty_x;
  int current_emptyy=tablero->empty_y; 

  /// se modifica temporalmente el tablero para calcular su estado hash
  tablero->grid[new_row][new_col]=0;
  tablero->grid[current_emptyx][current_emptyy]=aux_value;
  int counter=1;
  for(int i=0;i<tablero->size;i++){
    for(int j=0;j<tablero->size;j++){
      // funcion de valor correcto segun pos:
      // val(i,j,size)=i+j+1+size*i (elaboracion propia)
      if((i==tablero->size-1)&(j==tablero->size-1)){
        if (tablero->grid[i][j]!=0){
          tablero->grid[new_row][new_col]=aux_value;
          tablero->grid[current_emptyx][current_emptyy]=0;
          return false;
        }
      }
      else if (tablero->grid[i][j]!=counter){
        tablero->grid[new_row][new_col]=aux_value;
        tablero->grid[current_emptyx][current_emptyy]=0;
        return false;
      }
      counter++;
    }
  }
  return true;
  }

Node * succ(Node *u,Queue *open,Board *tablero){
  bool upper_wall=false;
  bool bottom_wall=false;
  bool left_wall=false;
  bool right_wall=false;

  int current_emptyx=tablero->empty_x;
  int current_emptyy=tablero->empty_y; 
   
  if (current_emptyx==0){ // Si estoy en la fila 0
    upper_wall=true;
  }
  if (current_emptyx==tablero->size-1){ // si estoy en la ultima fila
    bottom_wall=true;
  }

   if (current_emptyy==0){ // Si estoy en la col 0
    left_wall=true;
  }
  if (current_emptyy==tablero->size-1){ // si estoy en la ultima col
    right_wall=true;
  }

  // movimiento izquierda
  int new_row=tablero->empty_x;
  int new_col=tablero->empty_y-1;
  
  if (left_wall){
    // printf("entro \n");
    new_col=tablero->size-1;
  }
  if (new_col==-1){
    new_col=tablero->size-1;
  }
  Node *left_succ=new_succ(tablero,new_row,new_col);

  // movimiento derecha

  new_row=tablero->empty_x;
  new_col=tablero->empty_y+1;

  if (right_wall){
    new_col=0;
  }
  
  Node *right_succ=new_succ(tablero,new_row,new_col);

  // movimiento arriba
  new_row=tablero->empty_x-1;
  new_col=tablero->empty_y;

  if (upper_wall){
    new_row=tablero->size-1;
  }
  Node *top_succ=new_succ(tablero,new_row,new_col);

  // movimiento abajo
  new_row=tablero->empty_x+1;
  new_col=tablero->empty_y;

  if (bottom_wall){
    new_row=0;
  }
  Node *bottom_succ=new_succ(tablero,new_row,new_col);
  // retrn succesors

  // suc=left--right--top--down
  // int value_left=(left_succ->y+1)+(left_succ->x)*tablero->size);
  // if(value_left==tablero->

  top_succ->next=left_succ;
  left_succ->previous=top_succ;

  left_succ->next=right_succ;
  right_succ->previous=left_succ;

  right_succ->next=bottom_succ;
  bottom_succ->previous=right_succ;


  // print_tablero(tablero,3);
  return top_succ;
}

void * recursive_update(Board *tablero,Node *u){
  if (u->parent==NULL){
    return;
  }
  recursive_update(tablero,u->parent);
  update_board(tablero,u);

  }

Node* bfs(Board *tablero,int s0,int GoalState){
  // HashTable *closed=tablero->hash_table;
  int estados=1;// estado inicial
  Queue * open=queue_init();
  Node * head=node_init(s0); // el estado inicial como nodo
  // head->x=tablero->empty_x;
  // head->y=tablero->empty_y;
  open->head=head;
  open->tail=head;
  // append(s0,lista_open) //open=nodo estado inicial
  head->parent=NULL;
  int initial_state=true;
  while (open->head!=NULL){
    // printf("open before (%d,%d) value %d \n",open->head->x,open->head->y,open->head->value);
    Node *u=pop(open);
    // printf("pop u (%d,%d) value %d \n",u->x,u->y,u->value);
    // if (open->head!=NULL){
    // printf("open after (%d,%d) value %d \n",open->head->x,open->head->y,open->head->value);
    // }
   
    // print_tablero(tablero,tablero->size);

    if (initial_state>1){
      // Node *current=u->parent;
      // while(current=NULL){
        // update_board(tablero,)
      // }
      recursive_update(tablero,u);
      // update_board(tablero,u);
    }
    // tablero->empty_x=u->empty_x;
    // tablero->empty_y=u->empty_y;

    // printf("AFTER U\n");
    // print_tablero(tablero,tablero->size);
    // printf("------------\n");
    update_hash_table(tablero,u->value); // insertar en closed

    Node *v=succ(u,open,tablero); // lista ligada de nodos estados sucesores
    // for estado in lista_succesors
    while(v!=NULL){
      // printf("succ v (%d,%d) value %d \n",v->x,v->y,v->value);
      v->parent=u;
      if (isObjetive(tablero,v)){
        printf("ESTADOS: %d",estados);
        return v;
      }

      // if v not in hash table

      // hay que actualizar el tablero para calcular su valor..
      HashItem * item=tablero->hash_table->items[v->value % tablero->hash_table->size];
      Node *next_succ=v->next;
      if (item!=NULL){
        if (item->value!=v->value){ // si el estado no esta en esa llave
        // update_hash_table(tablero,v->value);
        // printf("aca %d %d \n",item->value,potential_hash);
        v->next=NULL;
        append(open,v);
        }
      }
      else{
        // update_hash_table(tablero,v->value);
        v->next=NULL;
        append(open,v);

      }
      // printf("largo open %d \n",open->lenght);
      if (next_succ!=NULL){
      next_succ->previous=NULL;
      }
      v=next_succ;
    }

    // revertir el estado anterior
    if (initial_state>1){
      // tablero->empty_x=last_x;
      // tablero->empty_y=last_y;
      recursive_remake_board(tablero,u);
    }
    estados++;
    initial_state++;
    // open->head=open->head->next;
  }
  return 0;
}

static Node* reverse(Node* head_ref) 
{ 
    Node *prev   = NULL; 
    Node *current = head_ref; 
    Node *next; 
    while (current != NULL) 
    { 
        next  = current->parent;   
        current->parent = prev;    
        prev = current; 
        current = next; 
    } 
    return prev;
} 

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    printf("Modo de uso: ./solver [test.txt] [output.txt]\n");
    printf("Donde:\n");
    printf("\t[test.txt] es el archivo de input\n");
    printf("\t[output.txt] es el nombre del archivo a escribir con el output\n");
    // return 0;
  }
FILE* input_file = fopen(argv[1], "r"); // test.txt file
FILE* output_file = fopen(argv[2], "w"); // output.txt file 

if (!input_file)
	{
		printf("¡Error abriendo los archivos!");
		return 2;
	}

// char fname[256]; strcpy(fname, argv[1]); // Guardamos el nombre del archivo
int n;
fscanf(input_file, "%d", &n);
Board *tablero=board_init(n);
read_csv(n,input_file,tablero);
// tablero->size=3;
// update_hash_table(tablero,1); FUNCION PARA HACER REHASHING! 

int key=get_state(tablero); // Se crea un valor de hash para el estado inicial S0

Node *solution=bfs(tablero,key,1);
int print_counter=-1; // nodo raiz y primera iteracion
// Node *reversed=node_init(1);
solution=reverse(solution);
Node *old=solution;
while(solution!=NULL){
  // if (print_counter>-1){
  // printf(" (%d,%d) \n" ,solution->x,solution->y);
  // printf("solution value %d \n",solution->value);
  // }
  solution=solution->parent;
  print_counter++;
}
fprintf(output_file,"%d\n",print_counter);
int output_counter=0;
while(old!=NULL){
  if (output_counter>0){
  fprintf(output_file,"%d,%d\n",old->y,old->x);
  }
  old=old->parent;
  output_counter++;

}

// for (int s=0;s<tablero->hash_table->size;s++){
//   if (tablero->counter_array[s]>0){
//   printf("%d;%d\n",s,tablero->counter_array[s]);
//   }  
// }






return 1;

}


