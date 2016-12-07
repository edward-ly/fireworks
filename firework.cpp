// OpenGL Libraries
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

// C++ Libraries
#include <stdio.h>
#include <cmath>

struct GLintPoint {
  int x, y;
};

// global variables
const int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;
GLdouble windowLeft = 0.0, windowRight = 640.0, windowBottom = 0.0, windowTop = 480.0;

void myInit() {
	glClearColor(0.0, 0.0, 0.0, 0.0); // set black background color
	glColor3f(1.0f, 1.0f, 1.0f); // set the drawing color
	glPointSize(1.0); // a ‘dot’ is 1 by 1 pixels

	// set the window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(windowLeft, windowRight, windowBottom, windowTop);
}

void RoamTimer(int arg) {
	// update firework state

	glutPostRedisplay(); // redraw display
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen

	// draw fireworks

	glutSwapBuffers();

	glutTimerFunc(33, RoamTimer, 0); // wait 33 ms before redrawing
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		static GLintPoint mouse;
		mouse.x = x; // flip y coordinate
		mouse.y = WINDOW_HEIGHT - y;

		// create a new firework at mouse position
	}

	glFlush();
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
