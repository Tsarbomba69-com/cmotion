#include <stdio.h>
#include "cmotion.h"
Color color = {.r = 1, .b = 0, .g = 1};
// Shape shape = {.c = {.color = color, .r = 0.1, .render = draw_circle}}; // Circle
extern Shape shape; // Rect
extern Object o;
extern int WIDTH = 640;
extern int HEIGHT = 480;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Falling Object");
    // shape = new_circle(color, 0.1); // Circle
    shape = new_rect(color, new_vector2(0, 0), new_vector2(0.2, 0.2)); // Rect
    o = new_object(new_vector2(0, 0.9), shape);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glutMainLoop();
    return 0;
}