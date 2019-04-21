#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include <time.h>

// #include "board.h"

Board *board_init(int size)
{
    /* Abrimos la interfaz grafica */

    Board* board = malloc(sizeof(Board));
    board->size = size;
    board->grid = malloc(sizeof(int * ) * size);
    for (int row = 0; row < size; row++)
    {
        board->grid[row] = malloc(sizeof(int) * size);
        for (int col = 0; col < size; col++)
        {
            board->grid[row][col] = 0;
        }
    }
    board->hash_table=ht_init();
    int counter_array[board->hash_table->size];
    memset(counter_array,0,sizeof(int)*board->hash_table->size);
    board->counter_array=counter_array;        
     // 250 porque 1*4 + 82*3=250 (83 es por el peor caso de movimientos) +1 para ser primo
    return board;

}

int get_nth_prime(int n){
    int primos[]={233,941, 89, 47, 19, 613, 257, 419, 211, 293, 37, 41, 433, 47, 53, 317, 61, 67,743, 73, 79, 83,1009, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199};
    return primos[n];
}
// fuente IsPrime: https://code.sololearn.com/cTqqdnKMVBe7/#cpp
bool isPrime (int a)
{
    int counter=0; //counter for numbers which devided by the number give reminder 0 (are divisors of the number)
    for (int i=2; i<=a/2; i++) //we start from 2 because 1 is divisor to all numbers. Also we go to the number devided by 2 because there isn't number higher than this that can be devider and give a integer as a result. For example 6: 6/1=6; 6/6=1 --> this is for all numbers;    6/3=2 but 6/4 is not integer. We do not need to count 6 as a devider or check if the range (6/2..6)
    if (a%i==0) //checking if the number is devisor
    counter++; //if there is a devisor we count it
    
    if (counter==0) //if the counter is equal to 0 that means the number doesn't have any other integer devisors other than 1 and itself therefore it is a prime number and we retrun true
    return true;
    else //the counter is not 0, which means the number has other devisors other than 1 and itself therefore it is not a prime number
    return false;
}
HashTable * ht_init(){
    HashTable *new_ht=malloc(sizeof(HashTable));
    int i=10000;
    int number;
    while(true){
        if (isPrime(i)){
            number=i;
            break;
            }
        
        i++;
    }
    new_ht->size=number;
    new_ht->value=0;
    new_ht->count=0;
    new_ht->items = calloc((size_t)new_ht->size, sizeof(HashItem*));
    return new_ht;
}

HashTable* ht_init_sized(const int old_size) {
    HashTable* new_ht = malloc(sizeof(HashTable));
    for (int i=2*old_size;!isPrime(i);i++);
    int i=2*old_size;
    int number;
    while(true){
        if (isPrime(i)){
            number=i;
            break;
            }
        
        i++;
    }
    new_ht->size =number;
    // printf("%d \n", number);
    // ht->count = 0;
    new_ht->items = calloc((size_t)new_ht->size, sizeof(HashItem*));
    return new_ht;
}
HashItem * hi_init(int key,int value){
    HashItem *new_hi=malloc(sizeof(HashItem));
    new_hi->value=value;
    new_hi->key=key;
    return new_hi;
}

int get_state(Board *tablero){
    int hash_key=0;
    int value;
    for (int row=0;row<tablero->size;row++){
        for (int col=0;col<tablero->size;col++){
        int prime_pos=(tablero->size)*row + col;
      // int first_arg=tablero->grid[row][col];
      // int second_arg=primos[prime_pos];
        value=(tablero->grid[row][col])*(get_nth_prime(prime_pos));
        // if ((row==2) & (col==0)){
            // printf("VALUE %d \n",value);
        // }
        // if ((row==2) & (col==2)){
            // printf("VALUE %d \n",value);
        // }
        hash_key+=value;
        }
    }

    tablero->hash_table->value=hash_key;
    return hash_key;
}


int  incremental_update1(Board *tablero,int row1,int col1,int row2,int col2){


    // VALOR 1 antes de cambio
    int prime_pos1;
    prime_pos1=(tablero->size)*row1 + col1;

    int grid_value1=tablero->grid[row1][col1];

    int old_hash_value1;
    old_hash_value1=(grid_value1)*(get_nth_prime(prime_pos1));
    /// VALOR 2 antes de cambio
    int prime_pos2;
    prime_pos2=(tablero->size)*row2 + col2;

    int grid_value2;
    grid_value2=tablero->grid[row2][col2];

    int old_hash_value2;
    old_hash_value2=(grid_value2)*(get_nth_prime(prime_pos2));

    // printf("old value1 %d \n",old_hash_value1);
    // printf("old value2 %d \n",old_hash_value2);

    
    // Ahora se intercambian las posiciones

    // VALOR1 despues de movimiento:
    prime_pos1=(tablero->size)*row2 + col2;
    int new_hash_value1;
    new_hash_value1=(grid_value1)*(get_nth_prime(prime_pos1));

    // VALOR2 despues de movimiento:

    prime_pos2=(tablero->size)*row1 + col1;
    int new_hash_value2;
    new_hash_value2=(grid_value2)*(get_nth_prime(prime_pos2));

    
    // H = H - (OLD1 + OLD2)+(NEW1 + NEW2) actualizar el hash value
    int hash_result=tablero->hash_table->value-(old_hash_value1+old_hash_value2)+(new_hash_value1+new_hash_value2);
    return hash_result;
    // printf("new value hash %d \n",tablero->hash_table->value);
}

void update_hash_table(Board *tablero,int hash_value){
    int key=hash_value % tablero->hash_table->size; // key=funcion_hash(state) MOD size(hash_table)
    // printf("key generated %d \n",key);
    // int *key_pointer;
    // key_pointer=&key;
    // int *value_pointer;
    // value_pointer=&hash_value;
    // HashItem * item=tablero->hash_table->items[key];
    if (tablero->hash_table->items[key]==NULL){
        HashItem *new_item=hi_init(key,hash_value);
        tablero->hash_table->items[key]=new_item;
        // printf("after nukl %d \n", *tablero->hash_table->items[key]->value);

        // item->value=&hash_value;

        tablero->hash_table->count++;
    }
    else{
        // se arregla las colisiones mediante colision lineal por ahora
        // f
        int revisados=0;
        int current_pos=key;
        // tablero->counter_array[key]+=1;
        bool insert_success=false;
        while (revisados<tablero->hash_table->size){ // si se revisaron todos los espacios se detiene
            if (tablero->hash_table->items[current_pos]==NULL){
                key=current_pos; // valor actualizado de la llave
                tablero->hash_table->items[current_pos]=hi_init(key,hash_value);
                // tablero->hash_table->items[current_pos]->value=&hash_value; // new_item(key,function_hash(state))
                // tablero->hash_table->items[current_pos]->key=&key; // new_item(key,function_hash(state))
                insert_success=true;
                break;  // se sale del while porque se ingreso una llave exitosamente
            }
            if (current_pos==tablero->hash_table->size-1){ // si es la ultima posicion de la table de hash, volver al principio
                current_pos=0;
            }
            else{
                current_pos++;
            }
            revisados++;
        }
        float factor=tablero->hash_table->count/tablero->hash_table->size;
        if ((!insert_success)||(factor>=0.7)){ // significa que no existe posicion libre en la tabla y debemos rehashear la tabla
            int current_size=tablero->hash_table->size;
            HashTable *current_table=tablero->hash_table;
            // int * old_table = malloc(current_size * sizeof);
            // memcpy(old_table, tablero->hash_table, current_size * sizeof(HashTable)); // copiamos los valores de la tabla anterior
            HashTable *new_table=ht_init_sized(current_size); // se crea una nueva table con el doble del tamaño
            tablero->hash_table=new_table;
            // printf("colusion \n");
            printf("%d\n",current_size);
            int counter_array[new_table->size];
            memset(counter_array,0,sizeof(int)*new_table->size);
            tablero->counter_array=counter_array;
            clock_t begin = clock();
            for (int pos=0;pos<current_size;pos++){ 
                update_hash_table(tablero,current_table->items[pos]->value); // se envia solo valor de hash que se mantiene, pero el tamaño ahora cambio
            }
            clock_t end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("% f \n",time_spent);
            // se actualiza la tabla de hash que contiene las valores de la tabla anterior

            // for (int pos=current_size;pos<2*current_size;pos++){
                // new_table->items[pos]=tablero->hash_table->items[pos];
            // }

        }
    }
    return;
}



static void ht_del_item(HashItem* i) {
    // free(i->key);
    // free(i->value);
    free(i);
}


void ht_del_hash_table(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        HashItem* item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}
