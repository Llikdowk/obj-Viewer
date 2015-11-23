#pragma once
#include <math.h>

namespace GLVector3f {

	class GLVector3f {
	public:
		float x, y, z;

		GLVector3f() : x(0.0f), y(0.0f), z(0.0f) {}
		GLVector3f(float x, float y, float z) : x(x), y(y), z(z) {}
		float length() const;
	};

	GLVector3f scale(const GLVector3f& v, float scalar);
	GLVector3f normalize(const GLVector3f& v);

	GLVector3f add(const GLVector3f& v1, const GLVector3f& v2);
	GLVector3f substract(const GLVector3f& v1, const GLVector3f& v2);

	float dotProduct(const GLVector3f& v1, const GLVector3f& v2);
	GLVector3f crossProduct(const GLVector3f& v1, const GLVector3f& v2);

	GLVector3f project(const GLVector3f& v1, const GLVector3f& v2);
}