#ifndef SKETCH_H
#define SKETCH_H

#include <stddef.h>
#include "hashes.h"

#define FUNCIONES 5
#define ANCHO_MATRIZ 180463

/*
*	TDA Count Min Sketch
*/

typedef struct cm_sketch {
	fhash_t hashes[FUNCIONES];
	size_t matriz[FUNCIONES][ANCHO_MATRIZ];
} cm_sketch_t;


/* Crea e inicializa un count min sketch */
cm_sketch_t *sketch_crear(fhash_t funciones[]);

/* Destruye un count min sketch */
void sketch_destruir(cm_sketch_t *filter);

#endif // SKETCH_H
