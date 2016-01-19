#pragma once
#include <GL/glui.h>

namespace shapes {
    GLuint createGridList(int n, int m, float step, float color[3]) {
        GLuint gridlist;
        gridlist = glGenLists(1);
        glNewList(gridlist, GL_COMPILE);
        n /= 2.f;
        m /= 2.f;
        float range_x = step * n;
        float range_y = step * m;
        glPushAttrib(GL_CURRENT_BIT);
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_LINES);
        for (int i = -n; i <= n; ++i) {
            glVertex3f(i*step, 0, -range_x);
            glVertex3f(i*step, 0, range_x);
        }
        for (int i = -m; i <= m; ++i) {
            glVertex3f(-range_y, 0, i*step);
            glVertex3f(range_y, 0, i*step);
        }
        glEnd();
        glPopAttrib();
        glEndList();

        return gridlist;
    }

    void drawGrid(int n, int m, float step, float color[3]) {
        static GLuint gridlist = -1;

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
        if (gridlist == -1) {
            gridlist = createGridList(n, m, step, color);
        }
        glCallList(gridlist);
		glPopAttrib();
    }

    GLuint axis() {
        GLuint draw_list = glGenLists(1);
        glNewList(draw_list, GL_COMPILE);
        glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0); // axis X: red
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glColor3f(0, 1, 0); // axis Y: green
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glColor3f(0, 0, 1); // axis Z: blue
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();
        glColor3f(0.5, 0.5, 0.5); // origin
        glutWireCube(0.1);
        glPopAttrib();
        glEndList();
        return draw_list;
    }
}