// OpenGL Libraries
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <GL/glui.h>

// C++ Libraries
#include <stdio.h>
#include <cmath>

// Custom libraries
#include "firework.h"

// Constants
#define TINY 1
#define SMALL 2
#define MEDIUM 3
#define LARGE 4
#define RANDOM 5

GLfloat size = -1;
GLfloat red = -1;
GLfloat green = -1;
GLfloat blue = -1;

struct GLintPoint {
  int x, y;
};

// global variables
const int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480, MAX_FIREWORKS = 20, MAX_FRAMES = 5;
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

void draw_fireworks() {
    for (int loop = 0; loop < MAX_FIREWORKS; loop++)
	{
        glPointSize(fireworks[loop].particleSize);
        glColor4f(fireworks[loop].red, fireworks[loop].green, fireworks[loop].blue, fireworks[loop].alpha);
        //glColor4f(red, green, blue, fireworks[loop].alpha);

        glBegin(GL_POINTS);
        for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
		{
			glVertex2f(fireworks[loop].x[particleLoop], fireworks[loop].y[particleLoop]);
		}
        glEnd();
	}
}

void timer(int arg) {
	// update firework state
    for (int i = 0; i < MAX_FIREWORKS; i++) {
        fireworks[i].explode();
    }

	glutPostRedisplay(); // redraw display
}

void myDisplay() {
	glClear(GL_ACCUM_BUFFER_BIT); // clear the screen

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Displacement trick for exact pixelisation
	glTranslatef(0.375, 0.375, 0);

	// draw the fireworks
    for (int i = 0; i < MAX_FRAMES; i++) {
        draw_fireworks();
        glAccum(GL_ACCUM, 1.0 / MAX_FRAMES);
    }

    glAccum(GL_ACCUM, 0.99f);
    glAccum(GL_RETURN, 1.0f);
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
            fireworks[last].initialize(mouse.x, mouse.y, size, red, green, blue);
            last = (last + 1) % MAX_FIREWORKS;
        }
	}
}

void processSizeMenuEvents(int option) {
  switch(option) {
    case TINY: size = 1; break;
    case SMALL: size = 3; break;
    case MEDIUM: size = 4; break;
    case LARGE: size = 5; break;
    case RANDOM: size = -1; break;
  }
}

void processColorMenuEvents(int option) {
  switch(option) {
    case 0: // Red
      red = 0.1;
      green = 0.9;
      blue = 0.8;
      break;
    case 1: // Green
      red = 0.1;
      green = 1.0;
      blue = 0.0;
      break;
    case 2: // Blue
      red = 0.0;
      green = 0.0;
      blue = 1.0;
      break;
    case 3: // Random
      red = -1;
      green = -1;
      blue = -1;
      break;
  }
}

void processMenuEvents(int option){
  switch(option) {
    case 0: exit(0);
  }
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); // initialize the Open-GL toolkit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ACCUM); // set the display mode
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // set window size
	glutInitWindowPosition(150, 100); // set window position
	glutCreateWindow("Fireworks"); // open the screen window

	// register the callback functions
	glutDisplayFunc(myDisplay);
	// glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	// glutPassiveMotionFunc(myMovedMouse);
	// glutKeyboardFunc(myKeyboard);
  //Menu Processing

  int sizeMenuId = glutCreateMenu(processSizeMenuEvents);
  glutAddMenuEntry("Tiny",TINY);
  glutAddMenuEntry("Small",SMALL);
  glutAddMenuEntry("Medium",MEDIUM);
  glutAddMenuEntry("Large",LARGE);
  glutAddMenuEntry("Random",RANDOM);

  int colorMenuId = glutCreateMenu(processColorMenuEvents);
  glutAddMenuEntry("Red",0);
  glutAddMenuEntry("Green",1);
  glutAddMenuEntry("Blue",2);
  glutAddMenuEntry("Random",3);


  glutCreateMenu(processMenuEvents);
  glutAddSubMenu("Size",sizeMenuId);
  glutAddSubMenu("Color",colorMenuId);
  glutAddMenuEntry("Quit",0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);


	myInit(); // additional initializations as necessary
	glutMainLoop(); // go into a perpetual loop
	return 0;
}
