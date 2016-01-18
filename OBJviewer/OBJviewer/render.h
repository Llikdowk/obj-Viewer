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
        GLuint drawmodel_list;
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

        glMatrixMode(GL_MODELVIEW);
        std::string current_folder = model->path.substr(0, model->path.rfind('/') + 1);
        
        for (int k = 0; k < model->mats.size(); ++k) {   
            
            ObjReader::node& node = model->mats[k];
            
            const MtlReader::m_def mat = model->getMaterialInfo(node.material_name);
            GLfloat Ke[] = { mat.Ke.x, mat.Ke.y, mat.Ke.z, 1.0f };
            GLfloat Ka[] = { mat.Ka.x, mat.Ka.y, mat.Ka.z, 1.0f };
            GLfloat Kd[] = { mat.Kd.x, mat.Kd.y, mat.Kd.z, 1.0f };
            GLfloat Ks[] = { mat.Ks.x, mat.Ks.y, mat.Ks.z, 1.0f };
            std::string texture_path = mat.map_Kd;

            //glMaterialfv(GL_FRONT, GL_EMISSION, Ke);
            glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
            glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
            glEnable(GL_COLOR_MATERIAL);
            
            if (texture_path.size() > 0) {
                texture::loadTexture(texture_path);
            }
            
            glBegin(GL_TRIANGLES);
            for (unsigned int i = 0; i < node.vertices.size(); ++i) {
                if (node.hasNormals) glNormal3f(node.normals[i].x, node.normals[i].y, node.normals[i].z);
                if (node.hasTexture) glTexCoord2f(node.uvs[i].x, node.uvs[i].y);
                glVertex3f(node.vertices[i].x, node.vertices[i].y, node.vertices[i].z);
            }
            glEnd();
            
        }
        
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
        glutCreateWindow("Object viewer");

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glEnable(GL_DEPTH_TEST);
        glClearColor(.1f, .1f, .1f, 1.0);
        
        axis = shapes::axis();
        glEnable(GL_LIGHTING);
        lights::init();
        try {
            //texture::load("resources/mercedes/mercedes.jpg"); 
            //model = new ObjReader("resources/mercedes/clkgtr.obj");

            //texture::load("resources/delorean/Textures/grill.png");
            //model = new ObjReader("resources/delorean/DeLorean.obj");
        
            //texture::load("resources/house/Texture/HouseBody.bmp");
            //model = new ObjReader("resources/house/3dmodels/house.obj");

            //texture::load("resources/organodron/Maps/cta4.jpg");
            //model = new ObjReader("resources/organodron/organodron.obj");

            //texture::load("resources/street/Building_V01_C.png");
        
            model = new ObjReader("resources/street/street.obj");
        }
        catch (const std::invalid_argument& e) {
            std::cerr << e.what();
        }

        model->createModel();

        if (!model->hasNormals) {
            std::cout << "LIGHTS DISABLED" << std::endl;
            glDisable(GL_LIGHTING);
        }

        std::string current_folder = model->path.substr(0, model->path.rfind('/') + 1);
        for (int k = 0; k < model->mats.size(); ++k) {
            ObjReader::node& node = model->mats[k];
            const MtlReader::m_def mat = model->getMaterialInfo(node.material_name);
            std::string texture_path = mat.map_Kd;
            std::replace(texture_path.begin(), texture_path.end(), '\\', '/');
            std::string complete_path = current_folder + texture_path;
            if (texture_path.size() > 0) {
                texture::precharge(current_folder, texture_path);
            }
        }
        timer = Timer();
        camera.newPosition(GLVector3f::GLVector3f(5, 3, 4));
        camera.lookAt(GLVector3f::GLVector3f(0, 0, 0));
        timer.startDeltaChrono();
    }

    void end() {
        delete model;
        glutLeaveMainLoop();
    }
}