#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  int position = hash(key, map->capacity);

  // Se comprueba que la posicion actual esta libre
  // Añade el par clave-valor
  if (map->buckets[position] == NULL || map->buckets[position]->key == NULL) {
    map->buckets[position] = createPair(key, value);

  } else {
    // Si esta ocupada entra en el bucle while hasta encontrar una posicion libre.
    // Añade el par clave-valor
    while (map->buckets[position] != NULL &&
           map->buckets[position]->key != NULL) {
      position = (position + 1) % map->capacity;
    }
    map->buckets[position] = createPair(key, value);
  }
  map->size++;
  map->current = position;
}

void enlarge(HashMap *map) {
  enlarge_called = 1; // no borrar (testing purposes)

  if (map == NULL) return;

  Pair **oldBuckets = map->buckets;
  //duplicamos la capacity
  map->capacity *= 2;
  // Reserva memoria para los buckets
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->size = 0;

  int indice = 0;
  while (indice < map->capacity / 2) {
    if (oldBuckets[indice] != NULL && oldBuckets[indice]->key != NULL) {
      // Reinserta los elementos en la nueva tabla, después de duplicar capacity.
      insertMap(map, oldBuckets[indice]->key, oldBuckets[indice]->value);
    }
    indice++;
  }

  free(oldBuckets); // Libera la memoria de la tabla anterior.
}

HashMap *createMap(long capacity) {
  // Se crea una nueva tabla
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  // Reservar memoria
  map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
  // Iniciar las variables dentro del mapa
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;

  return map; //Se retorna la tabla creada
}

void eraseMap(HashMap *map, char *key) {
    int position = hash(key, map->capacity);    
    
    if (map == NULL || key == NULL) return;

    while (map->buckets[position] != NULL) {
      // Se compara la clave actual con la que se esta buscando
      if (is_equal(map->buckets[position]->key, key)) {
      map->current = position;
      map->buckets[position]->key = NULL;
      map->size--;
      return;
    }
    position = (position + 1) % map->capacity;
  }
}

Pair *searchMap(HashMap *map, char *key) {
  int position = hash(key, map->capacity);

  if (map == NULL || key == NULL) return NULL;
  // Funciona de manera similar a la funcion eraseMap
  // Cambia el interior del ciclo if ** *
  while (map->buckets[position] != NULL) {
    // Se compara la clave actual con la que se esta buscando
    if (is_equal(map->buckets[position]->key, key)) {
      map->current = position;
      return map->buckets[position];
    }
    position = (position + 1) % map->capacity;
  }
  // Si no se encuentra la clave se retorna NULL
  return NULL;
}

Pair *firstMap(HashMap *map) {
  int indice = 0;

  // Encuentra el primer par clave-valor
  while (indice < map->capacity){
    if (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL){
      map->current = indice;
      return (map->buckets[indice]);
    }
    indice++;
  }  
    return NULL;// Si no se logra encontrar retorna NULL
}

Pair *nextMap(HashMap *map) { 
  int indice = map->current +1;
  
  // Encuentra el siguiente par clave-valor
  while (indice < map->capacity){
    if (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL){
      map->current = indice;
      return (map->buckets[indice]);
    }
    indice++;
  }  
   return NULL; // Si no se logra encontrar retorna NULL
}
