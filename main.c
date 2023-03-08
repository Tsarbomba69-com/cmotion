#include <stdio.h>
#include "cmotion.h"
Color color = {.r = 1, .b = 0, .g = 1};
// Shape shape = {.c = {.color = color, .r = 0.1, .render = draw_circle}}; // Circle
extern Shape shape; // Rect
extern Object o;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Falling Object");
    shape = new_circle(color, 0.1); // Circle
    // shape = {.s = {.color = color, .x = 0, .y = 15, .width = 0.1, .height = 0.5, .render = draw_rect}}; // Rect
    o = new_object(new_vector2(0, 20), shape);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glutMainLoop();
    return 0;
}