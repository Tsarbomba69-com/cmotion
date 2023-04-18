#include <GL/glut.h>
#include <stdio.h>
#define INV_SQRT 0.70710678119
#define SQRT_2 1.41421356237
#define DURATION 20000
const float g = -9.8;
// Define the particle position data
float particlePositions[DURATION][2] = {};

// Define the current time (in seconds)
float currentTime = 0;

// Define the time step (in seconds)
float step = 0.01;

void update(int value)
{
    // Update the current time
    currentTime += step;

    // Redraw the scene
    glutPostRedisplay();

    // Set up the next update
    glutTimerFunc(16, update, 0);
}

void display()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the particle color
    glColor3f(64, 64, 64);

    // Draw the particle at its current position
    int index = (int)(currentTime / step);
    glPushMatrix();
    glTranslatef(particlePositions[index][0], particlePositions[index][1], 0);
    glutSolidSphere(0.1, 16, 16);
    glPopMatrix();

    // Swap the buffers
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    // Initialize OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Particle Animation");
    // Create timeline
    for (int t = 0; t <= DURATION - 1; t++)
    {
        // |f| = √2mg, √2/2 = 1/√2 = 0.70710678118, m = 1,
#define v0 5
        float time = t * step;
        particlePositions[t][0] = time * time * 0.5 * INV_SQRT * v0;
        particlePositions[t][1] = time * (v0 * INV_SQRT + time * 0.5 * g);
    }
    // Set up the viewport
    glViewport(0, 0, 640, 480);

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

    return 0;
}
