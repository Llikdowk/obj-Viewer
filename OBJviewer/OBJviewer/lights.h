#pragma once
#include <GL/glui.h>

namespace lights {

	namespace {
		// GLUI Live Variable Lighting
		int   light2_enabled = 0;
		float light2_intensity = 1.0;

		GLfloat light2_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat light2_diffuse[] = { .6f, .6f, 1.0f, 1.0f };
		GLfloat light2_position[] = { .5f, .5f, 1.0f, 0.0f };


		int   light3_enabled = 0;
		float light3_intensity = .4;

		GLfloat light3_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat light3_diffuse[] = { .9f, .6f, 0.0f, 1.0f };
		GLfloat light3_position[] = { -1.0f, -1.0f, 1.0f, 0.0f };
	}

    static enum lights {
        AMBIENT_LIGHT = GL_LIGHT0,
        CENITAL_LIGHT = GL_LIGHT1,

		USER_LIGHT1 = GL_LIGHT2,
		USER_LIGHT2 = GL_LIGHT3
    };

    static void init() {
		GLfloat A[] = { 0.3, 0.3, 0.3, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, A);

		GLfloat Al0[] = { .2f, .2f, .2f, 1.0f };
		GLfloat Dl0[] = { .5f, .5f, .5f, 1.0f };
		GLfloat Sl0[] = { .02f, .02f, .02f, 1.0f };
		GLfloat posl0[] = { 45.0f, 45.0f, 45.0f, 0.f };
		glLightfv(AMBIENT_LIGHT, GL_AMBIENT, Al0);
		glLightfv(AMBIENT_LIGHT, GL_DIFFUSE, Dl0);
		glLightfv(AMBIENT_LIGHT, GL_SPECULAR, Sl0);
		glLightfv(AMBIENT_LIGHT, GL_POSITION, posl0);
		glEnable(AMBIENT_LIGHT);

        /*GLfloat Al1[] = { .4f, .4f, .4f, 1.0f };
        GLfloat Dl1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat Sl1[] = { .8f, .8f, .8f, 1.0f };
        GLfloat dirl1[] = { 0.0f, -1.f, -0.05f };
        GLfloat posl1[] = { 0.f, 5.f, 0.f, 0.f };
        glLightfv(CENITAL_LIGHT, GL_AMBIENT, Al1);
        glLightfv(CENITAL_LIGHT, GL_DIFFUSE, Dl1);
        glLightfv(CENITAL_LIGHT, GL_SPECULAR, Sl1);
        glLightfv(CENITAL_LIGHT, GL_POSITION, posl1);
        glLightfv(CENITAL_LIGHT, GL_SPOT_DIRECTION, dirl1);
        glLightf(CENITAL_LIGHT, GL_SPOT_CUTOFF, 45.0f);
        glLightf(CENITAL_LIGHT, GL_SPOT_EXPONENT, 20.0f);
        //glEnable(CENITAL_LIGHT);*/

		glLightfv(USER_LIGHT1, GL_AMBIENT, light2_ambient);
		glLightfv(USER_LIGHT1, GL_DIFFUSE, light2_diffuse);
		glLightfv(USER_LIGHT1, GL_POSITION, light2_position);

		glLightfv(USER_LIGHT2, GL_AMBIENT, light3_ambient);
		glLightfv(USER_LIGHT2, GL_DIFFUSE, light3_diffuse);
		glLightfv(USER_LIGHT2, GL_POSITION, light3_position);
    }

	static void enableLighting() {
		GLfloat A[] = { 0.05, 0.05, 0.05, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, A);

		GLfloat Al0[] = { .05f, .05f, .05f, 1.0f };
		GLfloat Dl0[] = { .1f, .1f, .1f, 1.0f };
		GLfloat Sl0[] = { .02f, .02f, .02f, 1.0f };
		GLfloat posl0[] = { 45.0f, 45.0f, 45.0f, 0.f };
		glLightfv(AMBIENT_LIGHT, GL_AMBIENT, Al0);
		glLightfv(AMBIENT_LIGHT, GL_DIFFUSE, Dl0);
		glLightfv(AMBIENT_LIGHT, GL_SPECULAR, Sl0);
		glLightfv(AMBIENT_LIGHT, GL_POSITION, posl0);
		glEnable(AMBIENT_LIGHT);

		if (light2_enabled) {
			glEnable(USER_LIGHT1);
		}
		if (light3_enabled) {
			glEnable(USER_LIGHT2);
		}
	}

	static void disableLighting() {
		GLfloat A[] = { 0.3, 0.3, 0.3, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, A);

		GLfloat Al0[] = { .2f, .2f, .2f, 1.0f };
		GLfloat Dl0[] = { .5f, .5f, .5f, 1.0f };
		GLfloat Sl0[] = { .02f, .02f, .02f, 1.0f };
		GLfloat posl0[] = { 45.0f, 45.0f, 45.0f, 0.f };
		glLightfv(AMBIENT_LIGHT, GL_AMBIENT, Al0);
		glLightfv(AMBIENT_LIGHT, GL_DIFFUSE, Dl0);
		glLightfv(AMBIENT_LIGHT, GL_SPECULAR, Sl0);
		glLightfv(AMBIENT_LIGHT, GL_POSITION, posl0);
		glEnable(AMBIENT_LIGHT);

		glDisable(USER_LIGHT1);
		glDisable(USER_LIGHT2);
	}
}