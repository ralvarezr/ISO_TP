/*
 * Copyright (c) 2022 Ricardo Alvarez <ricardo.alvarezr94@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : list.c
 * @date   : 16 aug. 2022
 * @author : Ricardo Alvarez <ricardo.alvarezr94@gmail.com>
 * @version	v1.0.0
 */

#include <list.h>
#include <stdlib.h>

static nodo_t* __nodo_crear(int elemento, nodo_t *siguiente) {

    nodo_t* n = malloc(sizeof(nodo_t));
    if(n == NULL)
        return NULL;

    n->elemento = elemento;
    n->siguiente = siguiente;

    return n;
}

lista_t* lista_crear(){

    lista_t* lista = calloc(1, sizeof(lista_t));

    return lista;
}

bool lista_insertar(lista_t* lista, uint32_t elemento){

    //Verifico que no me hayan pasado una lista nula.
    if(!lista)
        return false;

    //Primero veo el caso particular que esté vacía la lista.
    if(lista->cantidad == 0){

        nodo_t* aux = __nodo_crear(elemento, lista->nodo_fin);
        if(!aux)
            return false;

        lista->nodo_inicio = aux;
        lista->nodo_fin = aux;
        lista->cantidad++;
        return true;
    }

    if(lista->cantidad == 1){

        nodo_t* aux = __nodo_crear(elemento, NULL);
        if(!aux){
           // lista_destruir(lista);
            return false;
        }
        lista->nodo_fin = aux;
        lista->nodo_inicio->siguiente = lista->nodo_fin;
        lista->cantidad++;
        return true;
    }

    nodo_t* aux = __nodo_crear(elemento, NULL);
    if(!aux){
        return false;
    }

    lista->nodo_fin->siguiente = aux;
    lista->nodo_fin = aux;
    lista->cantidad++;

    return true;
}

bool lista_insertar_en_posicion(lista_t* lista, uint32_t elemento, size_t posicion){

    if(!lista || lista->cantidad == 0)
        return false;

    if(posicion >= lista->cantidad){
       return lista_insertar(lista, elemento);      
    }   

    if(posicion == 0){
        nodo_t* nuevo = __nodo_crear(elemento, lista->nodo_inicio);
        if(!nuevo)
            return false;
        
        lista->nodo_inicio = nuevo;
        lista->cantidad++;
        return true;
    }

    nodo_t* anterior = lista->nodo_inicio;
    size_t contador = 0;
    while(contador < posicion - 1){

        anterior = anterior->siguiente;
        contador++;
    }
    nodo_t* nuevo = __nodo_crear(elemento, anterior->siguiente);
    if(!nuevo)
        return false;

    anterior->siguiente = nuevo;
    lista->cantidad++;

    return true;
}


int lista_borrar(lista_t* lista){

    if(!lista || lista->cantidad == 0)
        return -1;

    if(lista->cantidad == 1){

        free(lista->nodo_inicio);
        lista->cantidad = 0;
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
        return 0;

    }
    
    nodo_t* borrar = lista->nodo_fin;
    nodo_t* anterior = lista->nodo_inicio;

    while(anterior->siguiente != borrar){
        anterior = anterior->siguiente;
    }

    anterior->siguiente = NULL;
    free(borrar);
    lista->nodo_fin = anterior;
    lista->cantidad--;

    return 0;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){

    if(!lista || lista->cantidad == 0)
        return -1;
    
    if(posicion >= (lista->cantidad) - 1)
        return lista_borrar(lista);

    if(posicion == 0){

        nodo_t* borrar = lista->nodo_inicio;
        lista->nodo_inicio = borrar->siguiente;
        free(borrar);
        lista->cantidad--;
        return 0;
    }
    
    nodo_t* anterior = lista->nodo_inicio;

    int conteo = 0;

    while(conteo < posicion - 1){

        anterior = anterior->siguiente;

        conteo++;
    }

    nodo_t* borrar = anterior->siguiente;

    anterior->siguiente = borrar->siguiente;
    free(borrar);
    lista->cantidad--;

    return 0;
}

size_t lista_elementos(lista_t* lista){
    if(!lista)
        return 0;

    return lista->cantidad;
}


bool lista_ultimo(lista_t* lista, uint32_t *buffer){

	if(lista_vacia(lista))
		return false;

	*buffer = lista->nodo_fin->elemento;

	return true;
}


bool lista_elemento_en_posicion(lista_t* lista, size_t posicion, uint32_t *buffer){

    if(!lista || lista_vacia(lista) || posicion >= lista->cantidad)
        return false;

    
    nodo_t* actual = lista->nodo_inicio;

    int conteo = 0;
    while(conteo < posicion){
        actual = actual->siguiente;
        conteo++;
    }

    *buffer = actual->elemento;

    return true;
}


bool lista_vacia(lista_t* lista){
    if(!lista || lista->cantidad == 0)
        return true;

    return false;
}

void lista_destruir(lista_t* lista){
    
    if(!lista)
        return;

    nodo_t* actual = lista->nodo_inicio;

    while(actual){

        nodo_t* siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    
    free(lista);
}

//--------- COLA ---------------
bool lista_encolar(lista_t* lista, uint32_t elemento){
    return lista_insertar(lista, elemento);
}


bool lista_desencolar(lista_t* lista, uint32_t *buffer){

    bool retval = lista_elemento_en_posicion(lista, 0, buffer);

    if(retval)
    	lista_borrar_de_posicion(lista, 0);

    return retval;

}

