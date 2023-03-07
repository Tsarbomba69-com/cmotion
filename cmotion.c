#include "cmotion.h"

double posX = 0;
double posY = 0;
double velX = 0;
double velY = 0;
double accX = 0;
double g = -9.8;
int previous_time = 0;
double radius = 0.1;

void draw_circle(Shape *shape)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(posX, posY, 0.0);
    glColor3f(shape->c.color.r, shape->c.color.g, shape->c.color.b);
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++)
    {
        double theta = i * PI / 180.0;
        double x = shape->c.r * cos(theta);
        double y = shape->c.r * sin(theta);
        glVertex2d(x, y);
    }
    glEnd();
    glutSwapBuffers();
}

void update()
{
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (current_time - previous_time) / 1000.0f;
    previous_time = current_time;
    velX += accX * delta_time;
    velY += g * delta_time;
    posX += velX * delta_time;
    posY += 0.5 * velY * delta_time;

    if (posY - radius < -1.0)
    {
        posY = -1.0 + radius;
        velY = -velY * 0.2;
    }
    glutPostRedisplay();
}

void display()
{

    Vector2 pos = new_vector2(posX, posY);
    Vector2 v = new_vector2(velX, velY);
    Color color = {.r = 1, .b = 0, .g = 0};
    Shape shape = {.c = {.color = color, .r = 0.1, .render = draw_circle}};
    Object o = {.position = new_vector2(1, 20), .shape = shape};
    o.shape.c.render(&o.shape);
}