#include "cmotion.h"
Shape shape; // Rect
RigidBody rb;
int WIDTH;
int HEIGHT;
double g = -9.8;
int previous_time = 0;
float MILLISECONDS = 3000.0f;

void draw_circle(Shape *shape)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(rb.position.x, rb.position.y, 0.0);
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
    glTranslated(rb.position.x, rb.position.y, 0.0);
    glColor3f(shape->s.color.r, shape->s.color.g, shape->s.color.b);
    glBegin(GL_QUADS);
    glVertex2f(shape->s.x, shape->s.y);
    glVertex2f(shape->s.x, shape->s.h);
    glVertex2f(shape->s.w, shape->s.h);
    glVertex2f(shape->s.w, shape->s.y);
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
        .w = size.x,
        .h = size.y,
        .render = draw_rect};
    return (Shape)c;
}

RigidBody new_rigidbody(float m, Vector2 position, Shape shape)
{
    RigidBody o = {
        .position = position,
        .shape = shape,
        .velocity = new_vector2(0, 0),
        .acceleration = new_vector2(0, 0),
        .mass = m};
    return o;
}

void add_force(RigidBody *rb, Vector2 force)
{
    // F = m * a => a = F/m
    rb->acceleration = new_vector2(force.x / rb->mass, force.y / rb->mass);
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (current_time - previous_time) / 10000.0f;
    previous_time = current_time;
    rb->velocity.x += rb->acceleration.x * delta_time;
    rb->velocity.y += rb->acceleration.y * delta_time;
}

bool rect_collision(Rect rect1, Rect rect2)
{
    float left1 = rect1.x - rect1.w / 2;
    float right1 = rect1.x + rect1.w / 2;
    float top1 = rect1.y - rect1.h / 2;
    float bottom1 = rect1.y + rect1.h / 2;
    float left2 = rect2.x - rect2.w / 2;
    float right2 = rect2.x + rect2.w / 2;
    float top2 = rect2.y - rect2.h / 2;
    float bottom2 = rect2.y + rect2.h / 2;
    return !(left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2);
}

bool rect_window_collision(Rect rect)
{
    return rect.x - rect.w / 2 < 0 || rect.x + rect.w / 2 > WIDTH || rect.y - rect.h / 2 < 0 || rect.y + rect.h / 2 > HEIGHT;
}

void update() // PROJECTILE SHOOTING
{
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    double delta_time = (current_time - previous_time) / MILLISECONDS;
    previous_time = current_time;
    rb.velocity.y += g * delta_time;
    rb.position.y += 0.5 * rb.velocity.y * delta_time;
    if (rb.position.y - (rb.shape.s.h - rb.shape.s.y)/2 < -1.0) // BOX COLLISION: Edge detection
    {
        rb.position.y = -1.0 + (rb.shape.s.h - rb.shape.s.y)/2;
        long double angle = atan2l(rb.velocity.y, rb.velocity.x) * 180 / PI;
        double v = magnitude2(&rb.velocity);
        rb.velocity.y = v * sin(angle) * 0.9;
        rb.position.y += rb.velocity.y * delta_time + rb.shape.s.h;
    }

    // if (rb.position.y - rb.shape.c.r < -1.0) // CIRCLE COLLISION: Edge detection
    // {
    //     rb.position.y = -1.0 + rb.shape.c.r;
    //     long double angle = atan2l(rb.velocity.y, rb.velocity.x) * 180 / PI;
    //     double v = magnitude2(&rb.velocity);
    //     rb.velocity.y = v * sin(angle) * 0.9;
    //     rb.position.y += rb.velocity.y * delta_time + rb.shape.c.r;
    // }

    rb.position.x += rb.velocity.x * delta_time;
    glutPostRedisplay();
}

void update2() // FREE FALL
{
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (current_time - previous_time) / MILLISECONDS;
    previous_time = current_time;
    rb.velocity.x += rb.velocity.x * delta_time;
    rb.velocity.y += g * delta_time;
    rb.position.y += 0.5 * rb.velocity.y * delta_time;

    // if (o.position.y - 0.02 * o.shape.s.h < -1.0) // BOX COLLISION: Edge detection
    // {
    //     o.position.y = -1.0 + 0.02 * o.shape.s.h;
    //     o.velocity.y = -o.velocity.y * 0.2;
    // }

    if (rb.position.y - rb.shape.c.r < -1.0) // CIRCLE COLLISION: Edge detection
    {
        rb.position.y = -1.0 + rb.shape.c.r;
        rb.velocity.y = -rb.velocity.y * 0.2;
    }
    glutPostRedisplay();
}

void display()
{
    if (rb.shape.s.h > 0 && rb.shape.s.w > 0)
    {
        rb.shape.s.render(&rb.shape);
    }
    else if (rb.shape.c.r > 0)
    {
        rb.shape.c.render(&rb.shape);
    }
}

// TODO: Rect collider
// TODO: