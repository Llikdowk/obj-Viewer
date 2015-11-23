#pragma once

#include <string.h>
#include <math.h>

#include "GLVector3f.h"
#include "Constants.h"

namespace GLMatrixf {
	class GLMatrixf {
	public:
		float m[4][4] = { 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };

		GLMatrixf();

		void translate(float dx, float dy, float dz);
		void rotate(float degrees, GLVector3f::GLVector3f axis);
		void scale(float sx, float sy, float sz);
		void transpose();
	};

	GLMatrixf multiply(const GLMatrixf& m1, const GLMatrixf& m2);
	GLVector3f::GLVector3f transformPoint(const GLMatrixf& m, const GLVector3f::GLVector3f& p);

	void print(const GLMatrixf& m);
}