#define _GNU_SOURCE
#include <stdio.h>
#include "heap.h"
#include "hashes.h"
#include "pila.h"
#include "hash.h"
#include "tag.h"
#include "sketch.h"
#include "strutil.h"
#include <stdlib.h>
#include <string.h>

#define FUNCIONES 5
#define ANCHO_MATRIZ 180463

/*
 *	Recibe un tag, el cual es procesado solo si no forma parte del top k 
 *	de topics. En tal caso se calculan 5 funciones de hash diferentes con 
 *  el tag recibido y se actualizan las correspondientes posiciones dentro
 *  del Count Min Sketch.
 *
 *	Si el tag supera en ocurrencias al menor tag almacenado en el heap, se 
 *	elimina este ultimo y se almacena el tag procesado.
 */
void procesar_tag(tag_t *tag, cm_sketch_t *sketch, heap_t *heap, hash_t *hash, size_t topics) {

	if (hash_pertenece(hash, tag_obtener(tag))) {
		tag_destruir(tag);
		return;
	}
		
	//Se actualiza el sketch
	for (size_t i = 0; i < FUNCIONES; i++) {
		size_t j = sketch->hashes[i](tag_obtener(tag)) % ANCHO_MATRIZ;

		sketch->matriz[i][j]++;
		if (tag->rank < sketch->matriz[i][j])
			tag->rank = sketch->matriz[i][j];
	}

	//Si el tag pertenece al top k, insertarlo y borrar el ultimo
	if (heap_cantidad(heap) >= topics) {
		tag_t *ultimo = heap_desencolar(heap);
		if (tag->rank > ultimo->rank) {
			hash_borrar(hash, tag_obtener(ultimo));
			tag_destruir(ultimo);
		}
		else {
			heap_encolar(heap, ultimo);
			tag_destruir(tag);
			return;
		}
	}

	heap_encolar(heap, tag);
	hash_guardar(hash, tag_obtener(tag), tag_obtener(tag));
}

/*
 *	Imprime los tags ordenados de mayor a menor por ocurrencias y los destruye.
 */
void mostrar_trending(heap_t *heap, hash_t *hash) {
	pila_t *pila = pila_crear();

	while (!heap_esta_vacio(heap))
		pila_apilar(pila, heap_desencolar(heap));

	while (!pila_esta_vacia(pila)) {
		tag_t *tag = pila_desapilar(pila);
		hash_borrar(hash, tag_obtener(tag));
		printf("%s ocurrencias: %ld\n", tag_obtener(tag), tag->rank);
		tag_destruir(tag);
	}

	printf("\n");
	pila_destruir(pila);
}

//Programa
int main(int argc, char *argv[]){

	//Manejo de posibles excepciones
	if(argc != 3){
		fprintf(stderr, "procesar_tweets: el programa debe recibir exactamente 2 argumentos.\n");
		return -1;
	}

	char *resto1, *resto2;
	long lineas = strtol(argv[1], &resto1, 10);
	long topics = strtol(argv[2], &resto2, 10);

	if(*resto1 != '\0' || *resto2 != '\0'){
		fprintf(stderr, "procesar_tweets: los argumentos deben ser enteros positivos.\n");
		return -2;
	}

	if(lineas < 1 || topics < 1){
		fprintf(stderr, "procesar_tweets: los argumentos deben ser enteros positivos.\n");
		return -3;
	}
	
	//arreglo de punteros a funciones de hash
	fhash_t funciones[5] = { hash1, hash2, hash3, hash4, hash5 };

	
	//declaraciones de TDAS
	cm_sketch_t *sketch = sketch_crear(funciones);
	if (!sketch) {
		fprintf(stderr, "procesar_tweets: memoria insuficiente. \n");
		return -4;
	}
	
	//Se usa un heap de minimo para poder identificar y reemplazar 
	//el tag con menos ocurrencias.
	heap_t *heap = heap_crear(comparar_tags);
	if (!heap) {
		fprintf(stderr, "procesar_tweets: memoria insuficiente. \n");
		return -5;
	}
	
	//Se usa un hash para poder decidir en O(1) si un tag ya fue almacenado o no.
	hash_t *hash = hash_crear(NULL);
	if (!hash) {
		fprintf(stderr, "procesar_tweets: memoria insuficiente. \n");
		return -6;
	}

	char *buf = NULL;
	size_t n = 0, n_lineas = lineas;
	size_t leidos = getline(&buf, &n, stdin);

	//Ciclo principal
	while(leidos != -1) {

		buf[leidos - 1] = '\0';
		char **vec_hashtags = split(buf, ',');

		for (size_t i = 1; vec_hashtags[i] != NULL; i++) {
			tag_t *tag = tag_crear(vec_hashtags[i]);
			procesar_tag(tag, sketch, heap, hash, topics);
		}

		n_lineas--;
		if (n_lineas == 0 || leidos == -1) {
			mostrar_trending(heap, hash);
			n_lineas = lineas;
		}

		free_strv(vec_hashtags);
		leidos = getline(&buf, &n, stdin);
	}

	mostrar_trending(heap, hash);
	
	free(buf);
	hash_destruir(hash);
	heap_destruir(heap, NULL);
	sketch_destruir(sketch);
	
	return 0;
}
