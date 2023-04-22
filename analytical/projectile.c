#include <GL/glut.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 640
#define INV_SQRT 0.70710678119
#define THETA 60
#define SQRT_2 1.41421356237
#define DURATION 20000
#define v0 4
#define NUM_PARTICLES 16
const float g = -9.8;
// Define the particle position data
float particlePositions[DURATION][2] = {};

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

/* Draws a slider at the specified position with the specified value */
void draw_slider(float x, float y, float width, float height, float value)
{
    /* Set the slider color */
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Draw the slider track */
    glBegin(GL_QUADS);
    glVertex2f(x, y + height / 2.0f);
    glVertex2f(x + width, y + height / 2.0f);
    glVertex2f(x + width, y - height / 2.0f);
    glVertex2f(x, y - height / 2.0f);
    glEnd();

    /* Draw the slider handle */
    float handle_width = width * 0.05f;
    float handle_height = height * 0.75f;
    float handle_x = x + width * value - handle_width / 2.0f;
    float handle_y = y;
    glBegin(GL_QUADS);
    glVertex2f(handle_x, handle_y + handle_height / 2.0f);
    glVertex2f(handle_x + handle_width, handle_y + handle_height / 2.0f);
    glVertex2f(handle_x + handle_width, handle_y - handle_height / 2.0f);
    glVertex2f(handle_x, handle_y - handle_height / 2.0f);
    glEnd();
}

void display()
{
    // Draw the particle at its current position
    int index = (int)(currentTime / step);
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    for (size_t i = 0; i < NUM_PARTICLES; i++)
    {
        // Set the particle color
        glColor3f(64, 64, 64);
        glPushMatrix();
        glTranslatef(particles[i].x[index], particles[i].y[index], 0);
        glutSolidSphere(0.1, 16, 16);
        glPopMatrix();
        // draw_slider(-2.2, -1.9, 0.5, 0.3, 0.3);
    }
    // Swap the buffers
    glutSwapBuffers();
}

float radians(float degrees)
{
    return degrees * M_PI / 180.0;
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
        particles[i].m = 1;
        float thetas[NUM_PARTICLES] = {0, 30, 45, 60, 90, 120, 135, 150, 180, 210, 225, 240, 270, 300, 215, 330};
        // Create timeline
        for (size_t t = 0; t <= DURATION - 1; t++)
        {
            float time = t * step;
            particles[i].x[t] = time * cosf(radians(thetas[i])) * v0;
            particles[i].y[t] = time * (v0 * sinf(radians(thetas[i])) + time * 0.5 * g);
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