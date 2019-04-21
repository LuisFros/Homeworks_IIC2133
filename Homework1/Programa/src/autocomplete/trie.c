#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Definir funciones aca

static void* malloc_or_die(size_t n)
{
    void* ptr = malloc(n);
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate %zu bytes. Out of memory!\n", n);
        exit(1);
    }

    return ptr;
}



Trie* trie_init(char char_value,int freq,char *whole_word){
    Trie *node=malloc_or_die(sizeof(Trie));
    node->value=char_value;
    node->frequency=freq;
    char *new_word= malloc(strlen(whole_word)+1*sizeof(char));
    strcpy(new_word,whole_word);
    node->maximun_word=new_word;
    return node;
}

int int_getter(char *character){
     if (*character==' '){
            // printf("es 0 \n");  
            return 0;
        }
    else if(*character=='\n'){
            return 28;
    }
    else{
            return (int)*character-96;
        }
}

void insert_word(char *word,int freq,int lenght,Trie *head)

{   

    // Puntero al char array
    char *word_pointer;
    word_pointer=word;


    // Valor numerico de el char para posicionarlo en el array
    int letter_int;

    int freq1;
    // Varaible current para iterar sobre el trie d
     int head_frequency=head->frequency;
    // printf("ESto queda en 0 ? %d \n",head_frequency);
    if (freq > head_frequency){
        head->frequency=freq;
        char *new_word_head;
        new_word_head = malloc(strlen(word)+1*sizeof(char));
        strcpy(new_word_head,word);
        head->maximun_word=new_word_head;
        }

    while(*word_pointer!='\0'){
        // Condicion para chequear si existe el nodo
        letter_int=int_getter(word_pointer);
        if (head->children[letter_int]==NULL){
            Trie *new_node=trie_init(*word_pointer,freq,word);

            for (int i=0;i<=28;i++){
                new_node->children[i]=NULL;
                }
            head->children[letter_int]=new_node;       
            }

        // Verificar si se actualiza maximun_word
        else{

                freq1=head->children[letter_int]->frequency;
                if (freq1<freq){
                    head->children[letter_int]->frequency=freq;
                    char *new_word_head;
                    new_word_head = malloc(strlen(word)+1*sizeof(char));
                    strcpy(new_word_head,word);
                    head->children[letter_int]->maximun_word=new_word_head;
                    }
            }
        word_pointer+=1;
        if(*word_pointer=='\n'){
            head->terminating_char=true;
            break;
        }
      
        head=head->children[letter_int];
    }

}

char *search(char *word,int lenght,Trie *proximo){

    char *word_pointer;
    word_pointer=word;

    bool first=true;
    int counter;
    int letter_int;    
    if (strlen(word_pointer)==0) {
        return proximo->maximun_word;
    }
    counter=0;
    while(*word_pointer!='\0'){

        letter_int=int_getter(word_pointer);
    
        if(proximo->children[letter_int]){
            counter++;
            proximo=proximo->children[letter_int];
        }
        // Si el hijo no existe, es porque la palabra no esta en el diccionario
        else if (counter==lenght+1)
        {
            return proximo->maximun_word;

        }

        else{
            return word;            
        }
        first=false;
        word_pointer+=1;

       
    }
    return proximo->maximun_word;

}
void free_trie(Trie *current){

    if (current==NULL){
        return;
    }
	for (int i=0;i<=28;i++){
		free_trie(current->children[i]);
    }
    free(current->maximun_word);
    free(current);
}