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
  if (map->buckets[position] == NULL || map->buckets[position]->key == NULL) {
    map->buckets[position] = createPair(key, value);

  } else {
    // Si esta ocupada entra en el bucle while hasta encontrar una posicion libre.
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
  map->capacity *= 2;
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->size = 0;

  int indice = 0;
  while (indice < map->capacity / 2) {
    if (oldBuckets[indice] != NULL && oldBuckets[indice]->key != NULL) {
      insertMap(map, oldBuckets[indice]->key, oldBuckets[indice]->value);
    }
    indice++;
  }

  free(oldBuckets);
}

HashMap *createMap(long capacity) {
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  // Reservar memoria
  map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
  // Iniciar las variables dentro del mapa
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;

  return map;
}

void eraseMap(HashMap *map, char *key) {
    int position = hash(key, map->capacity);    
    
    if (map == NULL || key == NULL) return;

    while (map->buckets[position] != NULL) {
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

  while (map->buckets[position] != NULL) {
    if (is_equal(map->buckets[position]->key, key)) {
      map->current = position;
      return map->buckets[position];
    }
    position = (position + 1) % map->capacity;
  }

  return NULL;
}

Pair *firstMap(HashMap *map) {
  int indice = 0;

  while (indice > map->capacity){
    if (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL){
      map->current = indice;
      return (map->buckets[indice]);
    }
    indice++;
  }  
    return NULL; 
}

Pair *nextMap(HashMap *map) { 
    if (map == NULL) return NULL;
  
    return NULL; 
}
