#define _POSIX_C_SOURCE 200809L
#include "tag.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Crea un tag con la cadena pasada por parametro. */
tag_t *tag_crear(const char *str) {
	tag_t *tag = malloc(sizeof(tag_t));
	if (!tag)
		return NULL;

	tag->hashtag = strdup(str);
	if (!tag->hashtag) {
		free(tag);
		return NULL;
	}

	tag->rank = 0;
	return tag;
}

/* Devuelve la cadena asociada a un tag */
char *tag_obtener(tag_t *tag) {
	return tag->hashtag;
}

/* Destruye un tag */
void tag_destruir(tag_t *tag) {
	free(tag->hashtag);
	free(tag);
}
 /* Funcion de comparacion para ser usada en un heap de minimo */
int comparar_tags(const void *tag1, const void *tag2) {
	tag_t *tag11 = (tag_t*)tag1;
	tag_t *tag22 = (tag_t*)tag2;

	if (tag11->rank > tag22->rank)
		return -1;
	else if (tag11->rank == tag22->rank)
		return 0;
	else
		return 1;
}
