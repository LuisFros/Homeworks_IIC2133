#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int int_getter(char *character);
// Definir struct aca
typedef struct trie_node{
    // Maybe unnecesary
    // bool *head;

    bool terminating_char;
    char value;
    char *maximun_word;
    int frequency;
    // Se define que tiene a lo mas 27 hijos por los 26 del aflabeto + 1 de " ".
    // 0 es " " y del 1-26 son los caracteres que tienen representacion ASCII 97-122 (97-96=1 o 122-96=26)
    struct trie_node *children[29];

}Trie;

static void* malloc_or_die(size_t n);

Trie* trie_init(char char_value,int freq,char *whole_word);



void insert_word(char *word,int freq,int lenght,struct trie_node *current);

char *search(char word[],int lenght,Trie *head);

void free_trie(Trie *current);


// Headers de funciones aca
