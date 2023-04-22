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

void drawGrid()
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

void drawGrid4()
{
    int i;
    float grid_lines = 8;
    glColor3f(0.5, 0.5, 0.5); // Set color to gray
    glBegin(GL_LINES);
    for (i = -grid_lines; i <= grid_lines; i++)
    {
        if (i == 0)
        {
            // set color to red for center line
            glColor3f(1.0, 0.0, 0.0);
        }
        else
        {
            glColor3f(0.5, 0.5, 0.5); // set color to gray for other lines
        }
        // Draw vertical lines
        glVertex3f(i, -grid_lines, 0);
        glVertex3f(i, grid_lines, 0);

        // Draw horizontal lines
        glVertex3f(-grid_lines, i, 0);
        glVertex3f(grid_lines, i, 0);

        glColor3f(0.5, 0.5, 0.5); // Set color back to gray for next line
    }
    glEnd();
}

void drawGrid5()
{
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);

    for (float i = -1.0; i <= 1.0; i += 0.1)
    {
        glVertex2f(i, -1.0);
        glVertex2f(i, 1.0);
        glVertex2f(-1.0, i);
        glVertex2f(1.0, i);
    }
    glEnd();

    // Draw center line
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glEnd();
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
    // Draw the grid on the background
    drawGrid();
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