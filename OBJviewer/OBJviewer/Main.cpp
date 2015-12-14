#include <utility>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include <GL/freeglut.h>

#include "GLMatrixf.h"
#include "GLVector3f.h"

#include "Timer.h"
#include "Camera.h"
#include "Constants.h"
#include "InputHandler.h"
#include "ObjReader.h"
#include "textures.h"
#include "shapes.h"

namespace {
    Timer timer;
    Camera camera;
    ObjReader* model;
}


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
	//m.rotate(90.0f * timer.getDeltaTime(), GLVector3f::GLVector3f(0, 0, 1));
	GLVector3f::GLVector3f newPos = GLMatrixf::transformPoint(m, camera.position);
	camera.newPosition(newPos);
	camera.lookAt(GLVector3f::GLVector3f(0.0, 0.0, 0.0));

	camera.update();
    float color[3] = { .4f, .4f, .4f };
    shapes::drawGrid(16, 16, 1, color);

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    
    texture::model();
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < model->size(); ++i) {
        glNormal3f(model->normals[i].x, model->normals[i].y, model->normals[i].z);
        glTexCoord2f(model->uvs[i].x, model->uvs[i].y);
        glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
        
	glutSwapBuffers();
	glutPostRedisplay();

	showFPS();
}

void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (float)w / h;
	gluPerspective(60, ratio, 0.1, 1000);
}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |GLUT_MULTISAMPLE);

	glutInitWindowSize(400, 400);
	glutCreateWindow("Misc Test");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glEnable(GL_DEPTH_TEST);

	glClearColor(.1f, .1f, .1f, 1.0);

    model = new ObjReader("resources/mercedes/clkgtr.obj"); // triangles
    model->loadObj();
}

void end() {
    delete model;
    glutLeaveMainLoop();
}

int main(int argc, char** argv) {

	timer = Timer();
	glutInit(&argc, argv);

	init();
    texture::init();

	InputHandler& input = InputHandler::getInstance();
    input.setBehaviour(new TypicalBehaviour());

	camera.newPosition(GLVector3f::GLVector3f(5, 5, 5));
	camera.lookAt(GLVector3f::GLVector3f(0, 0, 0));

	timer.startDeltaChrono();
	glutMainLoop();
}