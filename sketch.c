#define _POSIX_C_SOURCE 200809L
#include "sketch.h"
#include <stddef.h>
#include <stdlib.h>


/* Definicion de FUNCIONES y ANCHO_MATRIZ en sketch.h */

/*
 *	TDA Count Min Sketch
*/

/* Crea e inicializa un count min sketch */
cm_sketch_t *sketch_crear(fhash_t funciones[]) {

	cm_sketch_t *sketch = malloc(sizeof(cm_sketch_t));
	if (!sketch)
		return NULL;

	for (size_t i = 0; i < FUNCIONES; i++)
		sketch->hashes[i] = funciones[i];

	for (size_t i = 0; i < FUNCIONES; i++) {
		for (size_t j = 0; j < ANCHO_MATRIZ; j++) {
			sketch->matriz[i][j] = 0;
		}
	}

	return sketch;
}

/* Destruye un count min sketch */
void sketch_destruir(cm_sketch_t *sketch) {
	free(sketch);
}