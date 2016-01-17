#pragma once
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
#include "lights.h"

namespace render {
    namespace {
        Timer timer;
        Camera camera;
        ObjReader* model;
        GLuint axis;
        GLint width = 0, height = 0;
    }

    void orthographic() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
    }

    void perspective(GLint w, GLint h) {
        width = w;
        height = h;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = (float)w / h;
        gluPerspective(60, ratio, 0.1, 1000);
        glMatrixMode(GL_MODELVIEW);
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
        
        //orthographic();
        //glCallList(axis);
        //perspective(width, height);

        GLMatrixf::GLMatrixf m;
        m.rotate(90.0f * timer.getDeltaTime()*.5f, GLVector3f::GLVector3f(0, 1, 0));
        GLVector3f::GLVector3f newPos = GLMatrixf::transformPoint(m, camera.position);
        
        camera.newPosition(newPos);
        camera.lookAt(GLVector3f::GLVector3f(0.0, 0.0, 0.0));

        camera.update();
        float color[3] = { .3f, .3f, .3f };

        glPushMatrix();
        //glRotatef(90, 1, 0, 0);

        shapes::drawGrid(16, 16, 1, color);

        texture::model();
        glMatrixMode(GL_MODELVIEW);
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
        perspective(w, h);
    }

    void init() {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
        glutInitWindowSize(800, 600);
        glutCreateWindow("Viewer");

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glEnable(GL_DEPTH_TEST);
        glClearColor(.1f, .1f, .1f, 1.0);
        
        axis = shapes::axis();
        lights::init();

        //texture::load("resources/mercedes/mercedes.jpg"); 
        model = new ObjReader("resources/mercedes/clkgtr.obj"); // triangles
        //model = new ObjReader("resources/delorean/DeLorean.obj"); // quads -> to be done

        //model = new ObjReader("resources/house/3dmodels/house.obj");
        try {
            model->createModel();
        }
        catch (const std::invalid_argument& e) {
            std::cerr << e.what();
        }
        timer = Timer();
        camera.newPosition(GLVector3f::GLVector3f(5, 3, 2));
        camera.lookAt(GLVector3f::GLVector3f(0, 0, 0));
        timer.startDeltaChrono();
    }

    void end() {
        delete model;
        glutLeaveMainLoop();
    }
}