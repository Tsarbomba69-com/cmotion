#pragma once
#ifndef _CMOTION_H_
#define _CMOTION_H_

#include "clinear.h"
#include <GL/glut.h>
#include <stddef.h>

extern double g;
extern double radius;
extern int previous_time;

typedef struct Color
{
    float r;
    float g;
    float b;
} Color;

typedef struct Circle
{
    float r; // radius of the circle
    Color color;
    void (*render)();
} Circle;

typedef struct Rect
{
    float x; // x-coordinate of top-left corner
    float y; // y-coordinate of top-left corner
    float w; // width of rectangle
    float h; // height of rectangle
    Color color;
    void (*render)();
} Rect;

typedef union Shape
{
    Circle c;
    Rect s;
} Shape;

typedef struct RigidBody
{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    Shape shape;
} RigidBody;

Shape new_circle(Color color, float radius);
Shape new_rect(Color color, Vector2 top_corner, Vector2 size);
RigidBody new_rigidbody(float m, Vector2 position, Shape shape);
void add_force(RigidBody *rb, Vector2 force);
void update();
void display();
#endif