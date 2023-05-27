#include <GL/glut.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#define PI 3.1415926535897932384626433832795
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 640
#define INV_SQRT 0.70710678119
#define THETA 60
#define SQRT_2 1.41421356237
#define DURATION 20000
#define v0 4
#define NUM_PARTICLES 16
const float g = -9.8;
float times[DURATION] = {};

// Define the current time (in seconds)
float currentTime = 0;

// Define the time step (in seconds)
float step = 0.01;

// Particle
typedef struct Particle
{
    float x[DURATION];
    float y[DURATION];
    float m;
    float angle;
} Particle;

Particle particles[NUM_PARTICLES] = {};

void update(int value)
{
    // Update the current time
    currentTime = currentTime > 2 ? 0 : currentTime + step;

    // Redraw the scene
    glutPostRedisplay();

    // Set up the next update
    glutTimerFunc(16, update, 0);
}

void drawGrid2()
{
    // Set the color of the grid lines
    glColor3f(0.5f, 0.5f, 0.5f);

    // Draw horizontal lines
    for (int y = -5; y <= 5; y++)
    {
        glBegin(GL_LINES);
        glVertex2f(-5, y);
        glVertex2f(5, y);
        glEnd();
    }

    // Draw vertical lines
    for (int x = -5; x <= 5; x++)
    {
        glBegin(GL_LINES);
        glVertex2f(x, -5);
        glVertex2f(x, 5);
        glEnd();
    }
}

void draw_grid()
{
    glBegin(GL_LINES);
    float grid_lines = 8;

    for (float i = -grid_lines; i <= grid_lines; i += 0.4f)
    {
        if (i > 0.000001 && i < 0.400001)
        {
            // set color to red for center line
            glColor3f(1.0, 0.0, 0.0);
        }
        else
        {
            glColor3f(0.5, 0.5, 0.5); // set color to gray for other lines
        }
        glVertex3f(i, -grid_lines, 0.0f);
        glVertex3f(i, grid_lines, 0.0f);
        glVertex3f(-grid_lines, i, 0.0f);
        glVertex3f(grid_lines, i, 0.0f);
    }
    puts("");
    glEnd();
}

void display_text(char *text, float x, float y)
{
    // Set the color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Set the position for the text
    glRasterPos2f(x, y);

    // Draw the text
    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }
}

void render_card(GLfloat vertices[], float width, float height, float size)
{
    // Enable blending to allow transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enable vertex arrays and set the vertex data
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, size, vertices);
    glColorPointer(4, GL_FLOAT, size, vertices + 2);
    // Draw the quad using vertex arrays
    glDrawArrays(GL_QUADS, 0, 4);
    // Disable vertex arrays and blending
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_BLEND);
}

void display()
{
    // Draw the particle at its current position
    int index = (int)(currentTime / step);
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw the grid on the background
    draw_grid();

    // Set particle color
    glColor3f(64, 64, 64);

    // Draw the card
    glPushMatrix();
    glTranslatef(-2.7, 0, 0);
    // Define vertex data for the quad
    float width = 1.05;
    float height = 3;
    float size = 6 * sizeof(GLfloat);
    // Define gradient colors
    GLfloat gradient_colors[] = {0, 0, 0, 1.0, 0.2, 0.2, 0.2, 0.7};
    GLfloat vertices[] = {
        0.0f, 0.0f, gradient_colors[0], gradient_colors[1], gradient_colors[2], gradient_colors[3],
        width, 0.0f, gradient_colors[0], gradient_colors[1], gradient_colors[2], gradient_colors[3],
        width, height, gradient_colors[4], gradient_colors[5], gradient_colors[6], gradient_colors[7],
        0.0f, height, gradient_colors[4], gradient_colors[5], gradient_colors[6], gradient_colors[7]};
    render_card(vertices, width, height, size);
    glPopMatrix();

    // Draw particles
    char particle_data[300];
    float angle, y_pos;
    for (size_t i = 0; i < NUM_PARTICLES; i++)
    {
        angle = particles[i].angle;
        y_pos = (float)i / 8 + 0.1;
        glPushMatrix();
        glTranslatef(-2.7, 0, 0);
        sprintf(particle_data, "p: %.2fº, x(%.2f): %.2f, y(%.2f): %.2f\n",
                angle, times[index], particles[i].x[index], times[index], particles[i].y[index]);
        display_text(particle_data, 0.05, y_pos);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(particles[i].x[index], particles[i].y[index], 0);
        glutSolidSphere(0.1, 16, 16);
        glPopMatrix();
    }

    // Swap the buffers
    glutSwapBuffers();
}

float radians(float degrees)
{
    return degrees * PI / 180.0;
}

double *compute_range(double start, double end, double step)
{
    int num_steps = (int)((end - start) / step) + 1;
    double *result = (double *)malloc(num_steps * sizeof(double));

    for (int i = 0; i < num_steps; i++)
    {
        result[i] = start + i * step;
    }

    return result;
}

int main(int argc, char **argv)
{
    // Initialize OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Particle Animation");
    // FILE *fd;
    // fd = fopen("position data.txt", "w");

    // if (fd == NULL)
    // {
    //     printf("ERROR: Could not open the file.\n");
    //     return EXIT_FAILURE;
    // }

    for (size_t i = 0; i < NUM_PARTICLES; i++)
    {
        float thetas[NUM_PARTICLES] = {0, 30, 45, 60, 90, 120, 135, 150, 180, 210, 225, 240, 270, 300, 215, 330};
        particles[i].m = 1;
        particles[i].angle = thetas[i];

        // Create timeline
        for (size_t t = 0; t <= DURATION - 1; t++)
        {
            float time = t * step;
            particles[i].x[t] = time * cosf(radians(thetas[i])) * v0;
            particles[i].y[t] = time * (v0 * sinf(radians(thetas[i])) + time * 0.5 * g);
            times[t] = time;
            // fprintf(fd, "Φ: %f, x(%f): %f y(%f): %f\n", thetas[i], time, particles[i].x[t], time, particles[i].y[t]);
        }
    }
    // fclose(fd);
    // Set up the viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.333, 1, 100);

    // Set up the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // Set the background color
    glClearColor(0.2, 0.2, 0.2, 1.0);

    // Set up the update loop
    glutTimerFunc(0, update, 0);

    // Set up the display function
    glutDisplayFunc(display);

    // Start the main loop
    glutMainLoop();
    return EXIT_SUCCESS;
}