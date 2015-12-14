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
#include "ObjReader.h"
#include "textures.h"
#include "shapes.h"

namespace {
    float rotation;
    Timer timer;
    Camera camera;

    std::vector< GLVector3f::GLVector3f > vertices;
    std::vector< GLVector3f::GLVector3f > uvs;
    std::vector< GLVector3f::GLVector3f > normals;
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
	m.rotate(90.0f * timer.getDeltaTime(), GLVector3f::GLVector3f(0, 0, 1));
	GLVector3f::GLVector3f newPos = GLMatrixf::transformPoint(m, camera.position);
	camera.newPosition(newPos);
	camera.lookAt(GLVector3f::GLVector3f(0.0, 0.0, 0.0));

	camera.update();
    float color[3] = { .4f, .4f, .4f };
    shapes::drawGrid(16, 16, 1, color);

    /*
    glPushMatrix();
	glTranslatef(0.0, 0.0, -2.0);
	glScalef(50.0, 50.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
    glPopMatrix();
    */

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    // se deberia poder encapsular en un glcalllist
    // falta: contexto opengl de texturas
    // falta: contexto opengl de luces
    // (en uvs y vertices esta toda la informacion necesaria para el modelo)
    
    texture::model();
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < vertices.size(); ++i) {
        glNormal3f(normals[i].x, normals[i].y, normals[i].z);
        glTexCoord2f(uvs[i].x, uvs[i].y);
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
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

    ObjReader obj("resources/mercedes/clkgtr.obj"); // triangles
    obj.loadObj(vertices, uvs, normals);
    bool wireframe = false;
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

int main(int argc, char** argv) {
	timer = Timer();
	glutInit(&argc, argv);

	init();
    texture::init();

	InputHandler& a = InputHandler::getInstance();
    //a.setBehaviour(new ShowKeysBehaviour());
	//a.setBehaviour(new ExtendedBehaviour());
	rotation = 0.0f;

	camera.newPosition(GLVector3f::GLVector3f(5, 5, 5));
	camera.lookAt(GLVector3f::GLVector3f(0, 0, 0));

	timer.startDeltaChrono();
	glutMainLoop();
}