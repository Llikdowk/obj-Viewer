#include "GLVector3f.h"

namespace GLVector3f {
	float GLVector3f::length() const {
		return sqrtf(x*x + y*y + z*z);
	}

	GLVector3f scale(const GLVector3f& v, float scalar) {
		GLVector3f res = v;
		res.x *= scalar;
		res.y *= scalar;
		res.z *= scalar;

		return res;
	}

	GLVector3f normalize(const GLVector3f& v) {
		float l = v.length();
		if (l <= 0.0f) {
			return v; // Precision problem
		}
		GLVector3f res = v;

		res.x /= l;
		res.y /= l;
		res.z /= l;

		return res;
	}

	GLVector3f add(const GLVector3f& v1, const GLVector3f& v2) {
		GLVector3f result = v1;
		result.x += v2.x;
		result.y += v2.y;
		result.z += v2.z;

		return result;
	}

	GLVector3f substract(const GLVector3f& v1, const GLVector3f& v2) {
		GLVector3f result = v1;
		result.x -= v2.x;
		result.y -= v2.y;
		result.z -= v2.z;

		return result;
	}

	float dotProduct(const GLVector3f& v1, const GLVector3f& v2) {
		float result = v1.x * v2.x;
		result += v1.y * v2.y;
		result += v1.z * v2.z;

		return result;
	}

	GLVector3f crossProduct(const GLVector3f& v1, const GLVector3f& v2) {
		GLVector3f result(0.0f, 0.0f, 0.0f);
		result.x = v1.y * v2.z - v2.y * v1.z;
		result.y = v1.z * v2.x - v2.z * v1.x;
		result.z = v1.x * v2.y - v2.x * v1.y;

		return result;
	}

	GLVector3f project(const GLVector3f& v1, const GLVector3f& v2) {
		GLVector3f normalV2 = normalize(v2);
		float projectionLength = dotProduct(v1, normalV2);
		return scale(normalV2, projectionLength);;
	}
}