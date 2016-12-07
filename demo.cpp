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
const int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480, MAX_FIREWORKS = 15;
GLdouble windowLeft = 0.0, windowRight = 640.0, windowBottom = 0.0, windowTop = 480.0;

// Firework fireworks[MAX_FIREWORKS];
int last = -1;

void myInit() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set black background color
	glColor3f(1.0f, 1.0f, 1.0f); // set the drawing color
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH); // Smooth the points so that they're circular and not square

	// set the window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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

		// initialize firework at mouse position
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
