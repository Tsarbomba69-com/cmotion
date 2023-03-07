#pragma once
#ifndef _CMOTION_H_
#define _CMOTION_H_

#include "clinear.h"
#include <GL/glut.h>

extern double posX;
extern double posY;
extern double velX;
extern double velY;
extern double accX;
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
    float r;
    Color color;
    void (*render)();
} Circle;

typedef union Shape
{
    Circle c;
} Shape;

typedef struct Object
{
    Vector2 position;
    Vector2 velocity;
    Shape shape;
} Object;

void update();
void display();
#endif