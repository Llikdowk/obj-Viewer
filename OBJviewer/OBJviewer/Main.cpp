#include <GL/freeglut.h>
#include <utility>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include "GLMatrixf.h"
#include "GLVector3f.h"

#include "Timer.h"
#include "Camera.h"
#include "Constants.h"
#include "InputHandler.h"

float rotation;

Timer timer;
Camera camera;

void showFPS() {
	int fps = 1 / timer.getDeltaTime();
	std::stringstream ss;
	ss << "FPS: " << fps;
	glutSetWindowTitle(ss.str().c_str());
}

void display() {
	timer.stopDeltaChrono();
	timer.startDeltaChrono();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLMatrixf::GLMatrixf m;
	m.rotate(90.0f * timer.getDeltaTime(), GLVector3f::GLVector3f(0, 0, 1));
	GLVector3f::GLVector3f newPos = GLMatrixf::transformPoint(m, camera.position);
	camera.newPosition(newPos);
	camera.lookAt(GLVector3f::GLVector3f(0.0, 0.0, 0.0));

	camera.update();

	rotation += 90.0f * timer.getDeltaTime();
	rotation = std::fmodf(rotation, 360);

	glPushMatrix();
	glRotatef(rotation, 1.0, 0.0, 0.0);

	glColor3f(0.0f, 0.0f, 1.0f);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.0, 0.0, -2.0);
	glScalef(50.0, 50.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);

	glutSwapBuffers();
	glutPostRedisplay();

	showFPS();
}

void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (float)w / h;
	gluPerspective(90, ratio, 0.1, 1000);
}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |GLUT_MULTISAMPLE);

	glutInitWindowSize(400, 400);
	glutCreateWindow("Misc Test");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv) {
	timer = Timer();
	glutInit(&argc, argv);

	init();

	InputHandler& a = InputHandler::getInstance();
    a.setBehaviour(new ShowKeysBehaviour());
	rotation = 0.0f;

	camera.newPosition(GLVector3f::GLVector3f(5, 5, 5));
	camera.lookAt(GLVector3f::GLVector3f(0, 0, 0));

	timer.startDeltaChrono();
	glutMainLoop();
}