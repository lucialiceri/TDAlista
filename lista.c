#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

#define ERROR_MEMORIA "No se pudo reservar memoria\n"

lista_t* lista_crear(){
	lista_t* lista = malloc(sizeof(lista_t));
	if(!lista) return NULL;

	lista->cantidad = 0;

	return lista;
}

int lista_insertar(lista_t* lista, void* elemento){
	if(!lista) return -1;

	nodo_t* nodo_aux = NULL;
	if(lista->cantidad == 0){

    	nodo_aux = calloc(1, sizeof(nodo_t));
    	if(!nodo_aux){
    		free(nodo_aux);
    		free(lista);
    		return -1;	
    	} 
       	lista->nodo_inicio = nodo_aux;
       	lista->nodo_inicio->elemento = elemento;
       	lista->nodo_inicio->siguiente = NULL;

       	lista->nodo_fin = nodo_aux;
       	lista->cantidad++;
       	return 0;
	}

	nodo_aux = calloc(1, sizeof(nodo_t));
  	if(!nodo_aux){
  		free(nodo_aux);
    	return -1;	
    } 

	lista->nodo_fin->siguiente = nodo_aux;
	lista->nodo_fin = nodo_aux;
	lista->nodo_fin->elemento = elemento;
	lista->cantidad++;

	return 0;
}

void* lista_ultimo(lista_t* lista){
	if(!lista) return NULL;
	if(lista->cantidad == 0) return NULL;
	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
	if(!lista) return false;
	if(lista->cantidad == 0)
		return true;
	return false;
}

size_t lista_elementos(lista_t* lista){
	if(!lista) return 1;
	return lista->cantidad;
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
	if(!lista) return NULL;
	lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
	if(!iterador){
		printf("%s", ERROR_MEMORIA);	
		return NULL;	
	} 

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(iterador->corriente->siguiente != NULL) 
		return true;
	return false; 
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
	if(lista_iterador_tiene_siguiente(iterador) == true){
		nodo_t* actual = iterador->corriente;
		iterador->corriente = actual->siguiente;
		return true;
	}
	return false;
}

int lista_borrar(lista_t* lista){
	if((!lista) || (lista->cantidad == 0)) return -1;
	nodo_t* nodo_aux = lista->nodo_fin;
	lista_iterador_t* it = lista_iterador_crear(lista);
	int contador = 2;
	while((lista_iterador_tiene_siguiente(it)) && (contador < lista->cantidad)){
		lista_iterador_avanzar(it);
		contador++;
	}
	lista->nodo_fin = it->corriente;
	lista->cantidad--;
	free(nodo_aux);
	lista_iterador_destruir(it);
	return 0;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
	free(iterador);
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
	if(iterador->corriente->elemento == NULL) return NULL;
	return iterador->corriente->elemento;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
	if(!lista) return -1;

	if((lista->cantidad == 0) || (lista->cantidad == posicion)){
		int insertar = lista_insertar(lista, elemento);
		if(insertar == 0) return 0;
		return -1;
	}

	lista_iterador_t* it = lista_iterador_crear(lista);
	nodo_t* nodo_nuevo = calloc(1, sizeof(nodo_t));
	if(!nodo_nuevo){
		free(nodo_nuevo);
		return -1;
	}
	if(posicion == 0){
		lista->nodo_inicio = nodo_nuevo;
		lista->nodo_inicio->elemento = elemento;
		lista->nodo_inicio->siguiente = it->corriente;
		lista->cantidad++;
		lista_iterador_destruir(it);
		return 0;
	}

	size_t contador = 0;
	
	while((lista_iterador_tiene_siguiente(it)) && (contador < posicion - 1)){
		lista_iterador_avanzar(it);
		contador++;
	}

	nodo_t* auxiliar_siguiente = it->corriente->siguiente;
	it->corriente->siguiente = nodo_nuevo;
	lista_iterador_avanzar(it);
	it->corriente->elemento = elemento;
	it->corriente->siguiente = auxiliar_siguiente;
	lista->cantidad++;

	lista_iterador_destruir(it);

	return 0;
}

/*
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento.  
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.  
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
	if((!lista) || (lista->cantidad == 0)) return -1;

	if(posicion == (lista->cantidad) -1){
		lista_borrar(lista);
		return 0;
	}
	lista_iterador_t* it = lista_iterador_crear(lista);
	nodo_t* nodo_aux = it->corriente->siguiente;
	if(posicion == 0){
		lista->nodo_inicio = nodo_aux;
		free(it->corriente);
		lista_iterador_destruir(it);
		return 0;
	}
	int contador = 1;
	while((lista_iterador_tiene_siguiente(it)) && (contador < posicion)){
		lista_iterador_avanzar(it);
		contador++;
	}
	
	it->corriente->siguiente = nodo_aux->siguiente;
	free(nodo_aux);
	lista_iterador_destruir(it);
	return 0;
}


void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if((!lista) || (lista->cantidad == 0) || (lista->cantidad <= posicion)) return NULL;

	size_t contador = 0;
	lista_iterador_t* it = lista_iterador_crear(lista);
	while(lista_iterador_tiene_siguiente(it) && (contador < posicion)){
		lista_iterador_avanzar(it);
		contador++;
	}

	void* actual = lista_iterador_elemento_actual(it);
	lista_iterador_destruir(it);
	return actual;
}


void lista_destruir(lista_t* lista){
	if(!lista)
		printf("%s", ERROR_MEMORIA);
	else{
		while(lista->nodo_inicio != lista->nodo_fin){

			nodo_t* puntero_aux = lista->nodo_inicio->siguiente;
			free(lista->nodo_inicio);
			lista->nodo_inicio = puntero_aux;
		}
		free(lista->nodo_fin);
		free(lista);
	}
}

/* 
 * Apila un elemento.
 * Devuelve 0 si pudo o -1 en caso contrario.
 */
int lista_apilar(lista_t* lista, void* elemento){
	if(!lista) return -1;
	nodo_t* nodo_nuevo = calloc(1, sizeof(nodo_t));
	if(!nodo_nuevo){
		free(nodo_nuevo);
		return -1;
	}
	lista->nodo_fin->siguiente = nodo_nuevo;
	nodo_nuevo->elemento = elemento;
	lista->nodo_fin = nodo_nuevo;
	lista->cantidad++;
	return 0;
}

/* 
 * Desapila un elemento.
 * Devuelve 0 si pudo desapilar o -1 si no pudo.
 */
int lista_desapilar(lista_t* lista){
	if(!lista) return -1;
	lista_borrar(lista);
	return 0;
}

/*
 * Devuelve el elemento en el tope de la pila o NULL
 * en caso de estar vacía.
 */
void* lista_tope(lista_t* lista){
	if(!lista) return NULL;
	return lista->nodo_fin;
}

/* 
 * Encola un elemento.
 * Devuelve 0 si pudo encolar o -1 si no pudo.
 */
int lista_encolar(lista_t* lista, void* elemento){
	if(!lista) return -1;
	int devolver = lista_insertar(lista, elemento);
	if(devolver != 0) return -1;
	return 0;
}

/* 
 * Desencola un elemento.
 * Devuelve 0 si pudo desencolar o -1 si no pudo.
 */
int lista_desencolar(lista_t* lista){
	if(!lista) return -1;
	int devolver = lista_borrar_de_posicion(lista, 0);
	if(devolver != 0) return -1;
	return 0;
}

/*
 * Devuelve el primer elemento de la cola o NULL en caso de estar
 * vacía.
 */
void* lista_primero(lista_t* lista){
	if((!lista) || (lista_vacia(lista) == true)) return NULL;
	return lista->nodo_inicio;
}

/*
 * Iterador interno. Recorre la lista e invoca la funcion con cada elemento de
 * la misma. Dicha función puede devolver true si se deben seguir recorriendo
 * elementos o false si se debe dejar de iterar elementos.
 *
 * La función retorna la cantidad de elementos iterados o 0 en caso de error.
 */
size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto);

