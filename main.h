/**
 * Includes
 */
#include <GL/glfw.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "camera.h"

using namespace std;

/**
 * Prototypes
 */
void keyboardCallback( int, int );
void graphics( void );
void mouseCallback( int, int );
void mouseToggleShow( void );

void drawInit( void );

void drawRect( void );
void drawGrid( void );
int drawGLScene( void );
void drawAxis( void );
/**
 * Libs
 */

// Heap
int running = GL_TRUE;
float rotqube = 1;

int win_x = 500, win_y = 500;

void graphics( void ) {

	// Initialize GLFW
	if( !glfwInit() ) {
		exit( EXIT_FAILURE );
	}

	// Open an OpenGL window
	if( !glfwOpenWindow( win_x, win_y, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	// Callbacks
	glfwSetKeyCallback( keyboardCallback );
	glfwSetMousePosCallback( mouseCallback );

	// Setup
	drawInit();

	Camera* camera = Camera::getCamera();
	
	// Main loop
	while( running ) {
		camera->updateCamera();

		// OpenGL rendering goes here...
		glClear( GL_COLOR_BUFFER_BIT );

		drawRect();
		drawGrid();
		drawGLScene();

		// Swap front and back rendering buffers
		glfwSwapBuffers();
	}

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit( EXIT_SUCCESS );

}

void drawRect( ) {
	glPushMatrix();
	
	glTranslated(0, 0, 10);
	glColor3f (1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f (1, 3, 0);
	glVertex3f (1, -3, 0);
	glVertex3f (-1, -3, 0);
	glVertex3f (-1, 3, 0);
	glEnd();
	glFlush();

	glPopMatrix();
	glPushMatrix();
	
	glRotatef(90, 0, 1, 0);
	glTranslated(0, 0, 10);
	glColor3f (1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f (1, 3, 0);
	glVertex3f (1, -3, 0);
	glVertex3f (-1, -3, 0);
	glVertex3f (-1, 3, 0);
	glEnd();
	glFlush();

	glPopMatrix();



}

void drawGrid() {
	float space = 1;
	int num = 800;

	glPushMatrix();
	glTranslated(0, 0, 3);
	
	for(float i=-num/2; i < num/2; i += space) {
		glBegin(GL_LINES);
		glVertex3f(i, 0, -num/2);
		glVertex3f(i, 0, num/2);
		glEnd();
	}
	
	for(float j=-num/2; j < num/2; j += space) {
		glBegin(GL_LINES);
		glVertex3f(-num/2, 0, j);
		glVertex3f(num/2, 0, j);
		glEnd();
	}

	glPopMatrix();
}
	
	

void keyboardCallback(int key, int state) {
	cout << "Got " << state << " on " << key << endl;
	
	if(key == 81) { // Exit on q
		running = false;
		return;
	}

	Camera* camera = Camera::getCamera();

	if( state == 1 ) {
		switch( key ) {
			case 283:
				cout << "Up!" << endl;
				camera->persistentMoveBy(0, 0, 1);
				break;
			case 284:
				cout << "Down!" << endl;
				camera->persistentMoveBy(0, 0, -1);
				break;
			case 285:
				cout << "Left!" << endl;
				camera->persistentMoveBy(-1, 0, 0);
				break;
			case 286:
				cout << "Right!" << endl;
				camera->persistentMoveBy(1, 0, 0);
				break;
			case 87:
				cout << "Up!" << endl;
				camera->persistentMoveForward(0, 0, 1);
				break;
			case 83:
				cout << "Down!" << endl;
				camera->persistentMoveForward(0, 0, -1);
				break;
			case 65:
				cout << "Left!" << endl;
				camera->persistentMoveForward(-1, 0, 0);
				break;
			case 68:
				cout << "Right!" << endl;
				camera->persistentMoveForward(1, 0, 0);
				break;
			case 32:
				cout << "Up!" << endl;
				camera->persistentMoveForward(0, 1, 0);
				break;
			case 289:
				cout << "Down!" << endl;
				camera->persistentMoveForward(0, -1, 0);
				break;
			// No movement
			case 66:
				cout << "Switching mouse mode" << endl;
				mouseToggleShow();
				break;
		}

	} else {
		switch( key ) {
			case 283:
				cout << "Stop Up!" << endl;
				camera->persistentMoveBy(0, 0, -1);
				break;
			case 284:
				cout << "Stop Down!" << endl;
				camera->persistentMoveBy(0, 0, 1);
				break;
			case 285:
				cout << "Stop Left!" << endl;
				camera->persistentMoveBy(1, 0, 0);
				break;
			case 286:
				cout << "Stop Right!" << endl;
				camera->persistentMoveBy(-1, 0, 0);
				break;
			case 87:
				cout << "Stop Up!" << endl;
				camera->persistentMoveForward(0, 0, -1);
				break;
			case 83:
				cout << "Stop Down!" << endl;
				camera->persistentMoveForward(0, 0, 1);
				break;
			case 65:
				cout << "Stop Left!" << endl;
				camera->persistentMoveForward(1, 0, 0);
				break;
			case 68:
				cout << "Stop Right!" << endl;
				camera->persistentMoveForward(-1, 0, 0);
				break;
			case 32:
				cout << "Stop Up!" << endl;
				camera->persistentMoveForward(0, -1, 0);
				break;
			case 289:
				cout << "Stop Down!" << endl;
				camera->persistentMoveForward(0, 1, 0);
				break;
	}

	}
}

int mouseLastX, mouseLastY;

void mouseCallback(int x, int y) {
	Camera *camera = Camera::getCamera();

	float cent_x = win_x/2; // 250
	float cent_y = win_y/2; // 250

	float s_x = (x-cent_x)/cent_x;
	float s_y = (y-cent_y)/cent_y;
	
	camera->lookAt( s_x, s_y, 0);
	
	// Debug
	// cout << "x-cent_x: " << s_x << ", y-cent_y: " << s_y << endl;
}

bool mouseToggleShowState = false;
void mouseToggleShow() {
	if(!mouseToggleShowState) {
		glfwDisable( GLFW_MOUSE_CURSOR );
		mouseToggleShowState = true;
	} else {
		glfwEnable( GLFW_MOUSE_CURSOR );
		mouseToggleShowState = false;
	}
}

void drawInit( ) {
	/*  select clearing (background) color  */
	glClearColor (0.0, 0.0, 0.0, 0.0);

	/*  initialize viewing values  */
	glViewport(0, 0, win_x, win_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum (-3.0, 3.0, -3.0, 3.0, 3, 50.0);
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT);
}

void drawAxis() {
	glPushMatrix();
	glLoadIdentity();	
	glColor3f(0.0f,1.0f,0.0f);	// Color Blue
	glBegin(GL_LINES);
	glVertex3d(0, 90, 10);
	glVertex3d(0, -90, 10);
	glEnd();

	Camera camera = *Camera::getCamera();
	Matrix4 m = camera.look.getMatrix( );

	glMultMatrixf( m.matrix );

	glPopMatrix();
}

int drawGLScene( ) {		// Here's Where We Do All The Drawing
	// Reset The Current Modelview Matrix
	glPushMatrix();

	glTranslatef(0.0f, 0.0f,0.0f);	// Translate Into The Screen 7.0 Units
	glRotatef(rotqube,0.0f,1.0f,0.0f);	// Rotate The cube around the Y axis
	glRotatef(rotqube,1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);		// Draw The Cube Using quads
	glColor3f(0.0f,1.0f,0.0f);	// Color Blue
	glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
	glVertex3f( 1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
	glColor3f(1.0f,0.5f,0.0f);	// Color Orange
	glVertex3f( 1.0f,-1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)
	glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Bottom)
	glColor3f(1.0f,0.0f,0.0f);	// Color Red	
	glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
	glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
	glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
	glColor3f(1.0f,1.0f,0.0f);	// Color Yellow
	glVertex3f( 1.0f,-1.0f,-1.0f);	// Top Right Of The Quad (Back)
	glVertex3f(-1.0f,-1.0f,-1.0f);	// Top Left Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f,-1.0f);	// Bottom Left Of The Quad (Back)
	glVertex3f( 1.0f, 1.0f,-1.0f);	// Bottom Right Of The Quad (Back)
	glColor3f(0.0f,0.0f,1.0f);	// Color Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
	glColor3f(1.0f,0.0f,1.0f);	// Color Violet
	glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Right)
	glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
	glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
	glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Right)
	glEnd();			// End Drawing The Cube

	rotqube +=0.9f;			// Increase Angle
	glPopMatrix();
	
	return true;
}

void physics() {

}
