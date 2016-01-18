#pragma once
#include <GL/freeglut.h>

namespace lights {

    static enum lights {
        AMBIENT_LIGHT = GL_LIGHT0,
        CENITAL_LIGHT = GL_LIGHT1,
    };

    static void init() {
        GLfloat A[] = { 0.8, 0.8, 0.8, 1.0 };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, A);

        GLfloat Al0[] = { .5f, .5f, .5f, 1.0f };
        GLfloat Dl0[] = { .05f, .05f, .05f, 1.0f };
        GLfloat Sl0[] = { .02f, .02f, .02f, 1.0f };
        GLfloat posl0[] = { 0.f, 0.f, 10.f, 0.f };
        glLightfv(AMBIENT_LIGHT, GL_AMBIENT, Al0);
        glLightfv(AMBIENT_LIGHT, GL_DIFFUSE, Dl0);
        glLightfv(AMBIENT_LIGHT, GL_SPECULAR, Sl0);
        glLightfv(AMBIENT_LIGHT, GL_POSITION, posl0);
        glEnable(AMBIENT_LIGHT);

        GLfloat Al1[] = { .4f, .4f, .4f, 1.0f };
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
        //glEnable(CENITAL_LIGHT);
    }
}