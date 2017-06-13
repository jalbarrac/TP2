#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define SPLIT_CANT_MIN 2;

/* Se asume que todas las cadenas terminan en '\0'. */

/* Prototipo de free_strv */
void free_strv(char**);

/*
 *	Funcion auxiliar: copia los caracteres de "str" en una
 *	nueva cadena "substr". Devuelve NULL en caso de no haber
 *	memoria disponible para "substr".
 */
char *obtener_str(const char *str, size_t desde, size_t hasta){

	char *substr = malloc((desde + 1) * sizeof(char));
	if(!substr)
		return NULL;

	size_t pos = hasta - desde;
	for(size_t j = 0; j < desde; j++){
		substr[j] = str[pos];
		pos++;
	}

	substr[desde] = '\0';
	return substr;
}

/*
 * Devuelve en un arreglo dinámico terminado en NULL con todos los subsegmentos
 * de ‘str’ separados por el carácter ‘sep’. Tanto el arreglo devuelto como las
 * cadenas que contiene son allocadas dinámicamente.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica del
 * arreglo devuelto. La función devuelve NULL si falló alguna llamada a
 * malloc(), o si ‘sep’ es '\0'.
 */
char** split(const char* str, char sep){

	if(sep == '\0')
		return NULL;


	/* Defino la cantidad de cadenas en strv[] */
	size_t cant = SPLIT_CANT_MIN;
	size_t len = strlen(str) + 1;

	for(size_t i = 0; i < len; i++)
		if (str[i] == sep)
			cant++;


	/* Obtengo strv[] */
	char **strv = malloc(cant * sizeof(char*));
	if(!strv)
		return NULL;


	/* Guardo cada subcadena en strv[] */
	size_t cont = 0, j = 0;

	for(size_t i = 0; i < len; i++){

		if(str[i] == sep || str[i] == '\0'){
			strv[j] = obtener_str(str, cont, i);

			if (!strv[j]){
				free_strv(strv);
				return NULL;
			}

			j++;
			cont = 0;
		}
		else cont++;
	}

	strv[cant - 1] = NULL;
	return strv;
}


/*
 *	Funcion auxiliar: copia los caracteres de "input" en "str"
 *	y en lugar de '\0' coloca el separador 'sep'.
 */
void copiar_str(char *str, char *input, size_t *pos, char sep){

	size_t i = 0;
	while(input[i] != '\0'){

		str[*pos] = input[i];
		i++;
		(*pos)++;
	}

	str[*pos] = sep;
}

/*
 * Devuelve una cadena, allocada dinámicamente, resultado de unir todas las
 * cadenas del arreglo terminado en NULL ‘strv’.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica de la
 * cadena devuelta. La función devuelve NULL si no se pudo allocar memoria.
 */
char* join(char** strv, char sep){


	/* Obtengo el tamanio de la cadena final. */
	size_t cant = 0;
	size_t i = 0;
	while (strv[i] != NULL){
		cant += (strlen(strv[i]) + 1);
		i++;
	}


	/* Si no hay cadenas en strv[], devolver cadena vacia. */
	if(!cant){
		char *str_null = malloc(sizeof(char));
		str_null[0] = '\0';
		return str_null;
	}


	/* Pido memoria para la cadena. */
	char *str = malloc(cant * sizeof(char));
	if (!str)
		return NULL;


	/* Copio cada subcadena. */
	i = 0;
	size_t pos = 0;
	while (strv[i] != NULL){
		copiar_str(str, strv[i], &pos, sep);
		pos++;
		i++;
	}

	str[pos - 1] = '\0';
	return str;
}

/*
 * Libera un arreglo dinámico de cadenas, y todas las cadenas que contiene.
 */
void free_strv(char* strv[]){
	if (!strv)
		return;

	int j = 0;
	while (strv[j] != NULL){
		free(strv[j]);
		j++;
	}
	free(strv);
}
