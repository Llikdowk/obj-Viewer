#pragma once
#include <utility>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include <GL/glui.h>
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

#define WIREFRAME_MODE 1
#define TEXTURE_MODE   2
#define LIGHTING_MODE  3

namespace render {
	int   main_window;
	int vertexCount, polygonCount;

	// GLUI Live Variables

	// Rendering Modes
	int wireframe = 0;
	int textures = 1;
	int lighting = 0;
	int gizmo = 0;
	int grid = 1;

	

    namespace {
        Timer timer;
        Camera camera;
        ObjReader* model;
        GLuint axis;
        GLint width = 0, height = 0;
        GLuint drawmodel_list;
    }

	void loadModel(const char* directory, const char* name) {

	}

	void changeRenderingMode(int mode) {
		switch (mode) {
		case WIREFRAME_MODE:
			if (wireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			} else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
		case TEXTURE_MODE:
			if (textures) {
				glEnable(GL_TEXTURE_2D);
			} else {
				glDisable(GL_TEXTURE_2D);
			}
			break;
		case LIGHTING_MODE:
			if (lighting) {
				//glEnable(GL_LIGHTING);
				lights::enableLighting();
			} else {
				//glDisable(GL_LIGHTING);
				lights::disableLighting();
			}
			break;
		}
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
        gluPerspective(60, ratio, 0.1, 1000000);
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
        //perspective(width, height);

        GLMatrixf::GLMatrixf m;
        m.rotate(90.0f * timer.getDeltaTime()*.5f, GLVector3f::GLVector3f(0, 1, 0));
        GLVector3f::GLVector3f newPos = GLMatrixf::transformPoint(m, camera.position);
        
        camera.newPosition(newPos);
        camera.lookAt(GLVector3f::GLVector3f(0.0, 0.0, 0.0));

        camera.update();

		// Light positioning
		glLightfv(GL_LIGHT2, GL_POSITION, lights::light2_position);
		glLightfv(GL_LIGHT3, GL_POSITION, lights::light3_position);




        float color[3] = { .3f, .3f, .3f };

        glPushMatrix();
        //glRotatef(90, 1, 0, 0);

		if (grid) {
			shapes::drawGrid(16, 16, 1, color);
		}
		if (gizmo) {
			glCallList(axis);
		}

        //glScalef(.1, .1, .1);
        glMatrixMode(GL_MODELVIEW);
        std::string current_folder = model->path.substr(0, model->path.rfind('/') + 1);
        for (int k = 0; k < model->materials.size(); ++k) {   
            
            ObjReader::node& node = model->materials[k];
            MtlReader::m_def mat;
            
            mat = model->getMaterialInfo(node.material_name);
                
            GLfloat Ke[] = { mat.Ke.x, mat.Ke.y, mat.Ke.z, 1.0f };
            GLfloat Ka[] = { mat.Ka.x, mat.Ka.y, mat.Ka.z, 1.0f };
            GLfloat Kd[] = { mat.Kd.x, mat.Kd.y, mat.Kd.z, 1.0f };
            GLfloat Ks[] = { mat.Ks.x, mat.Ks.y, mat.Ks.z, 1.0f };
            std::string texture_path = mat.map_Kd;

            glMaterialfv(GL_FRONT, GL_EMISSION, Ke);
            glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
            //glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);

            /*if (texture_path.size() > 0) {
                //texture::loadTexture(texture_path);                
                //texture::loadTexture("/Maps/cty2x.jpg");
            }*/
			//glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_TRIANGLES);
            for (unsigned int i = 0; i < node.vertices.size(); ++i) {
                if (node.hasNormals) glNormal3f(node.normals[i].x, node.normals[i].y, node.normals[i].z);
                if (node.hasTexture) glTexCoord2f(node.uvs[i].x, node.uvs[i].y);
                glVertex3f(node.vertices[i].x, node.vertices[i].y, node.vertices[i].z);
            }
            glEnd();
            
        }
        
        glPopMatrix();
        
        glutSwapBuffers();
        glutPostRedisplay();
        showFPS();
    }

    void reshape(GLint w, GLint h) {
		int tx, ty, tw, th;
		GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
		glViewport(tx, ty, tw, th);

        perspective(w, h);
    }

    void init() {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(1280, 768);
        main_window = glutCreateWindow("Object viewer");

        glutDisplayFunc(display);
		GLUI_Master.set_glutReshapeFunc(reshape);

		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);

		lights::init();

        glEnable(GL_DEPTH_TEST);
        glClearColor(.1f, .1f, .1f, 1.0);
        
        axis = shapes::axis();
        try {
            model = new ObjReader("resources/mercedes/clkgtr.obj");
            //model = new ObjReader("resources/delorean/DeLorean.obj");
            //model = new ObjReader("resources/house/3dmodels/house.obj");
            //model = new ObjReader("resources/organodron/organodron.obj");
            //model = new ObjReader("resources/street/street.obj");
            //model = new ObjReader("resources/city/city.obj");
            //model = new ObjReader("resources/cubus/cubus_faun_912_21.obj");
            //model = new ObjReader("resources/dog/Zombie_Dog.obj");
            //model = new ObjReader("resources/us_assault/us_assault.obj");
            // = new ObjReader("resources/mustang/mustang.obj");
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
        for (int k = 0; k < model->materials.size(); ++k) {
            ObjReader::node& node = model->materials[k];
            const MtlReader::m_def mat = model->getMaterialInfo(node.material_name);
            std::string texture_path = mat.map_Kd;
            std::string complete_path = current_folder + texture_path;
            if (texture_path.size() > 0) {
                texture::precharge(current_folder, texture_path);
				texture::loadTexture(texture_path);
            }
        }
        timer = Timer();
        camera.newPosition(GLVector3f::GLVector3f(5, 2, 5));
        camera.lookAt(GLVector3f::GLVector3f(0, 0, 0));
        timer.startDeltaChrono();
    }

    void end() {
        delete model;
        //glutLeaveMainLoop();
    }
}