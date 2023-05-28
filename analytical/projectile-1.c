#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
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
bool TEXT = false;
bool PRINT = false;

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

void draw_grid()
{
    float grid_size = 8.0f;
    float step_size = 0.4f;
    int num_lines = (int)(2.0f * grid_size / step_size) + 1;

    glBegin(GL_LINES);

    for (int i = 0; i < num_lines; i++)
    {
        float line_pos = -grid_size + i * step_size;

        if (line_pos == 0.0f)
            glColor3f(1.0f, 0.0f, 0.0f); // Set color to red for center line
        else
            glColor3f(0.5f, 0.5f, 0.5f); // Set color to gray for other lines

        glVertex3f(line_pos, -grid_size, 0.0f);
        glVertex3f(line_pos, grid_size, 0.0f);
        glVertex3f(-grid_size, line_pos, 0.0f);
        glVertex3f(grid_size, line_pos, 0.0f);
    }

    glEnd();
}

void display_text(char *text, float x, float y)
{
    // Set the color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Set the position for the text
    glRasterPos2f(x, y);
    // Cache the font
    static void *font = GLUT_BITMAP_HELVETICA_18;
    // Draw the text
    while (*text)
    {
        glutBitmapCharacter(font, *text++);
    }
}

void render_card(GLfloat vertices[], float width, float height, float size)
{
    // Create and bind a VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Transfer the vertex data to the VBO
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Specify the vertex attribute pointers
    glVertexPointer(2, GL_FLOAT, size, 0);
    glColorPointer(4, GL_FLOAT, size, (GLvoid *)(2 * sizeof(GLfloat)));

    // Draw the quad using the VBO
    glDrawArrays(GL_QUADS, 0, 4);

    // Disable vertex attributes and unbind the VBO
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Delete the VBO
    glDeleteBuffers(1, &vbo);
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

    if (TEXT)
    {
        clock_t start = clock();
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

        clock_t end = clock();
        double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

        printf("Elapsed time: %f seconds\n", elapsed_time);
    }
    // Draw particles
    char particle_data[44];
    float angle, y_pos;
    for (size_t i = 0; i < NUM_PARTICLES; i++)
    {

        if (TEXT)
        {
            angle = particles[i].angle;
            y_pos = (float)i / 8 + 0.1;
            snprintf(particle_data, sizeof(particle_data), "p: %.2fº, x(%.2f): %.2f, y(%.2f): %.2f\n",
                     angle, times[index], particles[i].x[index], times[index], particles[i].y[index]);
            glPushMatrix();
            glTranslatef(-2.7, 0, 0);
            display_text(particle_data, 0.05, y_pos);
            glPopMatrix();
        }
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
    // Iterate through the command-line arguments
    for (size_t i = 1; i < argc; i++)
    {
        // Compare each argument with the target string
        if (strcmp(argv[i], "text") == 0)
        {
            TEXT = true;
            break; // Stop iterating if a match is found
        }

        // Compare each argument with the target string
        if (strcmp(argv[i], "print") == 0)
        {
            PRINT = true;
            break; // Stop iterating if a match is found
        }
    }
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

    // Initialize GLEW
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
    {
        fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(glewInitResult));
        return EXIT_FAILURE;
    }

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