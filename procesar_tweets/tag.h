#ifndef TAG_H
#define TAG_H

#include <stddef.h>

typedef struct tag {
	char *hashtag;
	size_t rank;
} tag_t;

/* Crea un tag */
tag_t *tag_crear(const char *str);

/* Devuelve la cadena asociada a un tag */
char *tag_obtener(tag_t *tag);

/* Destruye un tag */
void tag_destruir(tag_t *tag);

/* Funcion de comparacion */
int comparar_tags(const void *tag1, const void *tag2);

#endif // TAG_H
