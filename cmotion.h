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
    float x;      // x-coordinate of top-left corner
    float y;      // y-coordinate of top-left corner
    float width;  // width of rectangle
    float height; // height of rectangle
    Color color;
    void (*render)();
} Rect;

typedef enum
{
    CIRCLE,
    RECT,
} ShapeType;

typedef union Shape
{
    Circle c;
    Rect s;
} Shape;

typedef struct Object
{
    Vector2 position;
    Vector2 velocity;
    Shape shape;
} Object;

Shape new_circle(Color color, float radius);
Shape new_rect(Color color, Vector2 top_corner, Vector2 size);
Object new_object(Vector2 position, Shape shape);
void update();
void display();
#endif