#include "pa2mm.h"
#include "lista.h"
#include <stdio.h>
#include <stdbool.h>

void probar_creacion_lista(){
	lista_t* una_lista = lista_crear();
	pa2m_afirmar(una_lista != NULL, "Puedo crear una lista");
	pa2m_afirmar(una_lista->cantidad == 0, "Se crea una lista vacia");
	pa2m_afirmar(lista_vacia(una_lista) == true, "Puedo comprobar que esta vacia con un booleano");
	free(una_lista);
}

void probar_insertar_elementos(){
	lista_t* una_lista = lista_crear();
	int un_numero = 1;
	char una_letra = 'a';
	lista_insertar(una_lista, &un_numero);
	
	pa2m_afirmar(una_lista->nodo_inicio->elemento == &un_numero, "Puedo insertar un numero en la lista");
	pa2m_afirmar(una_lista->cantidad == 1, "Tengo un elemento en mi lista");
	pa2m_afirmar(una_lista->nodo_inicio->elemento == &un_numero, "El nodo inicio apunta al unico elemento");
	pa2m_afirmar(una_lista->nodo_fin->elemento == &un_numero, "El nodo fin apunta al unico elemento");

	lista_insertar(una_lista, &una_letra);
	pa2m_afirmar(una_lista->nodo_fin->elemento == &una_letra, "Puedo insertar una letra en la lista");
	pa2m_afirmar(una_lista->cantidad == 2, "Tengo dos elementos en mi lista");

	lista_destruir(una_lista);
}

void probar_insertar_varios_elementos(){
	lista_t* una_lista = lista_crear();
	bool salio_todo_ok = true;
	for(unsigned long i = 0; i < 100; i++){
		lista_insertar(una_lista, (void*)i);
		if((unsigned long)una_lista->nodo_fin->elemento != i)
			salio_todo_ok = false;
	}
	pa2m_afirmar(salio_todo_ok == true, "Puedo insertar 100 elementos");
	pa2m_afirmar(lista_elementos(una_lista) == 100, "Tengo 100 elementos");

	lista_destruir(una_lista);
}

void probar_insertar_en_posicion(){
	lista_t* una_lista = lista_crear();
	int un_numero = 5, otro_numero = 10, un_numero_mas = 15;
	char una_letra = 'f', otra_letra = 'k';
	lista_insertar(una_lista, &un_numero);
	lista_insertar(una_lista, &otro_numero);
	lista_insertar(una_lista, &un_numero_mas);

	lista_insertar_en_posicion(una_lista, &otra_letra, 0);
	pa2m_afirmar(lista_elemento_en_posicion(una_lista, 0) == &otra_letra, "Puedo insertar un elemento en la posicion 0");

	lista_insertar_en_posicion(una_lista, &una_letra, 2);
	pa2m_afirmar(lista_elemento_en_posicion(una_lista, 2) == &una_letra, "Puedo insertar un elemento en la posicion 2");




	lista_destruir(una_lista);
}

void probar_devolver_elementos(){
	lista_t* una_lista = lista_crear();
	int un_numero = 5, otro_numero = 10, un_numero_mas = 15;
	lista_insertar(una_lista, &un_numero);
	lista_insertar(una_lista, &otro_numero);
	lista_insertar(una_lista, &un_numero_mas);

	pa2m_afirmar(lista_elemento_en_posicion(una_lista, 0) == &un_numero, "Puedo devolver el elemento en la posicion 0");
	pa2m_afirmar(lista_elemento_en_posicion(una_lista, 1) == &otro_numero, "Puedo devolver el elemento en la posicion 1");
	pa2m_afirmar(lista_ultimo(una_lista) == una_lista->nodo_fin->elemento, "Puedo devolver el ultimo elemento de la lista");

	lista_destruir(una_lista);
}

void probar_iterador(){
	lista_t* una_lista = lista_crear();
	int un_numero = 5;
	char una_letra = 'l', otra_letra = 'j';

	lista_insertar(una_lista, &un_numero);
	lista_iterador_t* iterador = lista_iterador_crear(una_lista);

	pa2m_afirmar(iterador != NULL, "Se crea un iterador");
	pa2m_afirmar(iterador->lista == una_lista, "El iterador apunta a la lista");
	pa2m_afirmar(iterador->corriente == una_lista->nodo_inicio, "Se crea apuntando al primer elemento");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == false, "No tiene siguiente");
	
	lista_insertar(una_lista, &una_letra);
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == true, "Inserto otro elemento, ahora tiene siguiente");
	pa2m_afirmar(lista_iterador_avanzar(iterador) == true, "Puedo avanzar al siguiente elemento");
	pa2m_afirmar(iterador->corriente->elemento == una_lista->nodo_fin->elemento, "El iterador apunta al siguiente elemento");

	lista_insertar(una_lista, &otra_letra);
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == true, "Inserto otro elemento, ahora tiene siguiente");
	pa2m_afirmar(lista_iterador_avanzar(iterador) == true, "Puedo avanzar al siguiente elemento");
	pa2m_afirmar(lista_iterador_elemento_actual(iterador) == &otra_letra, "El iterador apunta al siguiente elemento");

	lista_iterador_destruir(iterador);
	lista_destruir(una_lista);
	
}

int main(){
	pa2m_nuevo_grupo("CREAR LISTA");
	probar_creacion_lista();

	pa2m_nuevo_grupo("INSERTAR ELEMENTOS");
	probar_insertar_elementos();
	probar_insertar_varios_elementos();

	pa2m_nuevo_grupo("DEVOLVER ELEMENTOS");
	probar_devolver_elementos();

	pa2m_nuevo_grupo("INSERTAR ELEMENTOS EN POSICION");
	probar_insertar_en_posicion();

	pa2m_nuevo_grupo("CREAR ITERADOR");
	probar_iterador();
	return 0;
}