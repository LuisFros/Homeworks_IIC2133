#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "trie.h"
#include <time.h>
#include <string.h>
// Esta linea permite usar MAX_LENGTH como una constante en el programa
// Uso 101 para considerar el caracter que indica el fin de un string
#define MAX_LENGTH 101

int main(int argc, char *argv[])
{	

	if (argc != 4)
	{
		printf("Modo de uso: ./solver [database.txt] [queries.txt] [output.txt]\n");
		return 0;
	}
	FILE *database = fopen(argv[1], "r");
	FILE *queries  = fopen(argv[2], "r");
	FILE *output   = fopen(argv[3], "w");

	if (!database || !queries || !output)
	{
		printf("¡Error abriendo los archivos!");
		return 2;
	}

	//// Ejemplo de lectura de strings:

	// Leo el numero de entradas en la base de datos
	int n;
	fscanf(database, "%d", &n);

	//Definimos el head
	char starting_node_value='$';

	//Le entregamso una palabra maxima cualquiera, se va a reemplazar en el primer insert
	char starting_max_word[]="KAMIKAZE";


	Trie *trie=trie_init(starting_node_value,0,starting_max_word);

	// Para cada entrada:
	for (int i = 0; i < n; i++)
	{
		// Obtengo la frecuencia y el largo
		int freq, length;
		// Ojo que incluyo un espacio en el formato para que no lo considere como parte del string
		fscanf(database, "%d %d ", &freq, &length);
		// printf("freq = %d, length = %d\n", freq, length);

		// Leo el string aprovechando que se el largo maximo
		char chars[MAX_LENGTH];
		fgets(chars, MAX_LENGTH, database);
		insert_word(chars,freq,length,trie);


	}
	int k;
	fscanf(queries, "%d", &k);
	clock_t begin = clock();
	// Para cada entrada:
	for (int i = 0; i < k; i++)
	{
		int length;
		fscanf(queries, "%d ",&length);

		char chars_search[MAX_LENGTH];

		if (length==0){
			*chars_search="";
		}
		// Leo el string aprovechando que se el largo maximo
		else{
			fgets(chars_search, MAX_LENGTH, queries);
		}
		chars_search[length]='\0';	
		char *result=search(chars_search,length,trie);
		if(result[strlen(result)-1]=='\n'){
			fprintf(output,"%s",result);
		}
		else{
			fprintf(output,"%s\n",result);
		}

	}
	clock_t end = clock();
	// printf("TIEMPO SEARCH : %f",(double)(end - begin) / CLOCKS_PER_SEC);
	free_trie(trie);
	fclose(database);
	fclose(queries);
	fclose(output);
	return 0;
}
