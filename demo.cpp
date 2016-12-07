// OpenGL Libraries
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

// C++ Libraries
#include <stdio.h>
#include <cmath>

// Custom libraries
#include "firework.h"

struct GLintPoint {
  int x, y;
};

// global variables
const int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480, MAX_FIREWORKS = 15;
GLdouble windowLeft = 0.0, windowRight = 640.0, windowBottom = 0.0, windowTop = 480.0;

Firework fireworks[MAX_FIREWORKS];
int last = 0;

void myInit() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set black background color
	glColor3f(1.0f, 1.0f, 1.0f); // set the drawing color
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearAccum(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_POINT_SMOOTH); // Smooth the points so that they're circular and not square

	// set the window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(windowLeft, windowRight, windowBottom, windowTop);
}

void timer(int arg) {
	// update firework state
    for (int i = 0; i < MAX_FIREWORKS; i++) {
        fireworks[i].explode();
    }

	glutPostRedisplay(); // redraw display
}

void myDisplay() {
    glAccum(GL_RETURN, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Displacement trick for exact pixelisation
	glTranslatef(0.375, 0.375, 0);

	// draw the fireworks
    for (int loop = 0; loop < MAX_FIREWORKS; loop++)
	{
        // Set the point size of the firework particles (this needs to be called BEFORE opening the glBegin(GL_POINTS) section!)
        glPointSize(fireworks[loop].particleSize);
        glColor4f(fireworks[loop].red, fireworks[loop].green, fireworks[loop].blue, fireworks[loop].alpha);

        glBegin(GL_POINTS);
        for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
		{
			glVertex2f(fireworks[loop].x[particleLoop], fireworks[loop].y[particleLoop]);
		}
        glEnd();
	}

    glAccum(GL_ACCUM, 0.99f);
	glutSwapBuffers();

	glutTimerFunc(1, timer, 0); // wait at least 1 ms before redrawing
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		static GLintPoint mouse;
		mouse.x = x; // flip y coordinate
		mouse.y = WINDOW_HEIGHT - y;

		// initialize firework at mouse position
        if (!fireworks[last].hasExploded) {
            fireworks[last].initialize(mouse.x, mouse.y);
            last = (last + 1) % MAX_FIREWORKS;
        }
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); // initialize the Open-GL toolkit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // set window size
	glutInitWindowPosition(150, 100); // set window position
	glutCreateWindow("Fireworks"); // open the screen window

	// register the callback functions
	glutDisplayFunc(myDisplay);
	// glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	// glutPassiveMotionFunc(myMovedMouse);
	// glutKeyboardFunc(myKeyboard);

	myInit(); // additional initializations as necessary
	glutMainLoop(); // go into a perpetual loop
	return 0;
}
