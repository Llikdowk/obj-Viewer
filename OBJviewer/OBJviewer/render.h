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

#include "ObjReader.h"
#include "textures.h"
#include "shapes.h"
#include "lights.h"

#define WIREFRAME_MODE 1
#define TEXTURE_MODE   2
#define LIGHTING_MODE  3

class Render {
private:
	Render* instance;
	Render() {};
public:
	
	static Render& getInstance() {
		static Render instance;
		return instance;
	}

	Render(const Render&) = delete;
	void operator=(const Render&) = delete;

	Camera camera;

	int main_window;
	int vertexCount, polygonCount;

	// GLUI Live Variables

	// Rendering Modes
	int wireframe = 0;
	int textures = 1;
	int lighting = 0;
	int gizmo = 0;
	int grid = 1;

	Timer timer;

	ObjReader* model;
	GLuint axis;
	GLint width = 0, height = 0;
	GLuint drawmodel_list;

	void loadModel(const char* name);

	void changeRenderingMode(int mode);

	void panCamera(float speed);

	void moveCamera(float speed);

	void orthographic();

	void perspective(GLint w, GLint h);

	void showFPS();

	void display();

	void reshape(GLint w, GLint h);

	void init();

	void end();
};