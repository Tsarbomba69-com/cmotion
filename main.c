#include <stdio.h>
#include "cmotion.h"

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Falling Particle");
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glutMainLoop();
    return 0;
}