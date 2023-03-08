#include "cmotion.h"
Shape shape; // Rect
Object o;

double g = -9.8;
int previous_time = 0;
double radius = 0.1;

void draw_circle(Shape *shape)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(o.position.x, o.position.y, 0.0);
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
    glFlush();
    glutSwapBuffers();
}

void draw_rect(Shape *shape)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(o.position.x, o.position.y, 0.0);
    glColor3f(shape->s.color.r, shape->s.color.g, shape->s.color.b);
    glBegin(GL_QUADS);
    glVertex2f(shape->s.x, shape->s.y);
    glVertex2f(shape->s.x, shape->s.height);
    glVertex2f(shape->s.width, shape->s.height);
    glVertex2f(shape->s.width, shape->s.y);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

Shape new_circle(Color color, float radius)
{
    assert(radius > 0 && "Radius must be greater than zero");
    Circle c = {.color = color, .r = radius, .render = draw_circle};
    return (Shape)c;
}

Shape new_rect(Color color, Vector2 top_corner, Vector2 size)
{
    assert(size.x > 0 && size.y > 0 && "Rectangle must have positive dimensions");
    Rect c = {
        .color = color,
        .x = top_corner.x,
        .y = top_corner.y,
        .width = size.x,
        .height = size.y,
        .render = draw_rect};
    return (Shape)c;
}

Object new_object(Vector2 position, Shape shape)
{
    Object o = {.position = position, .shape = shape, .velocity = new_vector2(0, 0)};
    return o;
}

void update()
{
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (current_time - previous_time) / 1000.0f;
    previous_time = current_time;
    // o.velocity.x += accX * delta_time;
    // o.velocity.x += o.velocity.x * delta_time;
    o.velocity.y += g * delta_time;
    o.position.y += 0.5 * o.velocity.y * delta_time;

    if (o.position.y - radius < -1.0)
    {
        o.position.y = -1.0 + radius;
        o.velocity.y = -o.velocity.y * 0.2;
        // v' = v - 2 * (v . n) * n
    }
    glutPostRedisplay();
}

void display()
{
    if (o.shape.s.height > 0 && o.shape.s.width > 0)
    {
        o.shape.s.render(&o.shape);
    }
    else if (o.shape.c.r > 0)
    {
        o.shape.c.render(&o.shape);
    }
}