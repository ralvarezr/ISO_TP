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
 * @file   : list.h
 * @date   : 16 aug. 2022
 * @author : Ricardo Alvarez <ricardo.alvarezr94@gmail.com>
 * @version	v1.0.0
 */

#ifndef __LISTA_H__
#define __LISTA_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


typedef struct nodo {

    uint32_t elemento;
    struct nodo* siguiente;

}nodo_t;

typedef struct lista {

    nodo_t* nodo_inicio;
    nodo_t* nodo_fin;
    size_t cantidad;

}lista_t;



/*
 * Crea la lista reservando la memoria necesaria.
 * Devuelve un puntero a la lista creada o NULL en caso de error.
 */
lista_t* lista_crear(void);


/*
 * Inserta un elemento al final de la lista.
 * Devuelve true si pudo insertar o false si no pudo.
 */
bool lista_insertar(lista_t* lista, uint32_t elemento);

/*
 * Inserta un elemento en la posicion indicada, donde 0 es insertar
 * como primer elemento y 1 es insertar luego del primer elemento.  
 * En caso de no existir la posicion indicada, lo inserta al final.
 * Devuelve true si pudo insertar o false si no pudo.
 */
bool lista_insertar_en_posicion(lista_t* lista, uint32_t elemento, size_t posicion);


/*
 * Quita de la lista el elemento que se encuentra en la ultima posición.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar(lista_t* lista);


/*
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento.  
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.  
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar_de_posicion(lista_t* lista, size_t posicion);


/*
 * Devuelve el elemento en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
 */
bool lista_elemento_en_posicion(lista_t* lista, size_t posicion, uint32_t *buffer);


/*
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía.
 */
bool lista_ultimo(lista_t* lista, uint32_t *buffer);

/* 
 * Devuelve true si la lista está vacía o false en caso contrario.
 * (Si la lista no existe, es una lista vacía)
 */
bool lista_vacia(lista_t* lista);


/*
 * Devuelve la cantidad de elementos almacenados en la lista.
 */
size_t lista_elementos(lista_t* lista);


/* 
 * Encola un elemento.
 * Devuelve true si pudo encolar o false si no pudo.
 */
bool lista_encolar(lista_t* lista, uint32_t elemento);


/*
 * Devuelve el primer elemento de la lista en el buffer.
 * Devuelve true si pudo realizarse la operación, o false en caso contrario.
 */
bool lista_desencolar(lista_t* lista, uint32_t *buffer);


/*
 * Libera la memoria reservada por la lista.
 */
void lista_destruir(lista_t* lista);


#endif /* __LISTA_H__ */
