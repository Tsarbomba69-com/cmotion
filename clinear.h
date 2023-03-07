
#pragma once
#ifndef _CLINEAR_H_
#define _CLINEAR_H_

#include "vec.h"
#include <math.h>

// Representation of a 2D vector in space.
typedef struct Vector2
{
    double x;
    double y;
} Vector2;

// Pi (π) constant
#define PI 3.14159265358979323846

// Creates a new 2D vector object.
Vector2 new_vector2(double x, double y);
// Add 2 vectors together. (linked list)
Vec add_vec(Vec *a, Vec *b);
// Subtract 2 vectors. (linked list)
Vec sub_vec(Vec *a, Vec *b);
// Add 2 vectors together. (2D)
Vector2 add_vector2(Vector2 *a, Vector2 *b);
// Subtract 2 vectors. (2D)
Vector2 sub_vector2(Vector2 *a, Vector2 *b);
// Add 2 vectors together. (Array)
double *add(double *a, double *b, size_t len);
// Subtract 2. (Array)
double *sub(double *a, double *b, size_t len);
// Dot product of 2 vectors. (linked list)
double dot_vec(Vec *a, Vec *b);
// Dot product of 2 vectors. (2D)
double dot_vector2(Vector2 *a, Vector2 *b);
// Dot product of 2 vectors. (Array)
double dot(double *a, double *b, size_t len);
// Calculates the distance between 2D vectors a and b. √|A - B| = √(x1^2 + y1^2) - (x2^2 + y2^2)
double distance2(Vector2 *a, Vector2 *b);
// Calculates the magnitude of a 2D vector. |A| = √(x^2 + y^2)
double magnitude2(Vector2 *a);
// Makes this vector have a magnitude of 1.
// When normalized, a vector keeps the same direction but its length is 1.0.
Vector2 normalize2(Vector2 *a);
// Calculates angle between the two vectors. Φ = arccos((A . B) / (|A| * |B|))
double angle2(Vector2 *a, Vector2 *b);
// Prints a 2D vector.
void print_vector2(Vector2 *vector);
#endif