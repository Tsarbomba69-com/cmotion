#pragma once
#ifndef _VEC_H_
#define _VEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// Supported vector types. You need to pass a type when creating an empty collection.
typedef enum Type
{
    INT,
    FLOAT,
    CHAR,
    STRING,
    BOOL,
    DOUBLE
} Type;

typedef union T
{
    double d;
    float f;
    int i;
    char c;
    bool b;
    char *s;
} T;

// Heap allocated vector node (element).
typedef struct Node
{
    T data;
    struct Node *next;
} Node;

// A contiguous growable array type with heap-allocated contents.
typedef struct Vec
{
    unsigned int len;
    Type type;
    Node *head;
} Vec;

// #define VEC(vec, array) ({                  \
//     assert(array != NULL);                  \
//     size_t size = sizeof array;             \
//     vec.head = new_node(array[0]);          \
//     Node *current = vec.head;               \
//     vec.len++;                              \
//                                             \
//     for (size_t i = 0; i < size; i++)       \
//     {                                       \
//         current->next = new_node(array[i]); \
//         current = current->next;            \
//         vec.len++;                          \
//     }                                       \
// })

#define VEC(vec, array, len) ({      \
    for (size_t i = 0; i < len; i++) \
    {                                \
        enqueue(&vec, (T)array[i]);     \
    }                                \
})
// Creates a new node containig the given data.
Node *new_node(T data);
// Push a element to the beggining of the vector.
void push(Vec *vec, T data);
// Push a element to the end of the vector.
void enqueue(Vec *vec, T data);
// Remove and return the last element of the vector.
T dequeue(Vec *vec);
// Remove and return the first element of the vector.
T pop(Vec *vec);
// Reverse the order of a vector.
void reverse(Vec *vec);
// Deallocate all of the vector data.
void clear(Vec *vec);
/* Print a vector with a given format especifier.
Supported format: INT, FLOAT, CHAR, STRING, BOOL*/
void print_vec(Vec *vec);
// Append 'other' vector to 'dest' vector.
void append(Vec *dest, Vec *other);
// Get an element from the vector with a given index.
T get(Vec *vec, size_t index);
// Clone a vector.
Vec clone(Vec *vec);
// Inserts an element at position index within the vector, shifting all elements after it to the right.
void insert(Vec *vec, T data, size_t index);
// Removes and returns the element at position index within the vector, shifting all elements after it to the left.
T vec_remove(Vec *vec, size_t index);
// Shortens the vector, keeping the first len elements and dropping the rest.
void truncate(Vec *vec, size_t len);
// Creates a new empty vector
Vec new_vec(Type type);

#endif