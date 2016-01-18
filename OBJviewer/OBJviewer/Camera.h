#pragma once
#include <GL/glui.h>
#include <cmath>

#include "GLVector3f.h"
#include "GLMatrixf.h"
#include "Constants.h"

class Camera {
private:
	const float maxPitch = 85.0f;
	const float cosMaxPitch = cosf((90 - maxPitch) * Constants::deg_to_rad);
public:
	GLVector3f::GLVector3f position = vec3(0.0f, 0.0f, 0.0f);
	GLVector3f::GLVector3f look = GLVector3f::GLVector3f(1.0f, 0.0f, 0.0f);
	GLVector3f::GLVector3f up = GLVector3f::GLVector3f(0.0f, 1.0f, 0.0f);

	Camera() {}
	Camera(GLVector3f::GLVector3f position, GLVector3f::GLVector3f look);
	void update();

	void moveForward(float distance);
	void pan(float distance);
	void pitch(float delta);
	void yaw(float delta);

	void newPosition(GLVector3f::GLVector3f newPosition);
	void lookAt(GLVector3f::GLVector3f point);
};