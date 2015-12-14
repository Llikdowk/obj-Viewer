#pragma once
#include <gl/freeglut.h>

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
            glVertex3f(i*step, -range_x, 0);
            glVertex3f(i*step, range_x, 0);
        }
        glEnd();

        for (int i = -m; i <= m; ++i) {
            glBegin(GL_LINES);
            glVertex3f(-range_y, i*step, 0);
            glVertex3f(range_y, i*step, 0);
            glEnd();
        }
        glPopAttrib();
        glEndList();

        return gridlist;
    }

    void drawGrid(int n, int m, float step, float color[3]) {
        static GLuint gridlist = -1;
        if (gridlist == -1) {
            gridlist = createGridList(n, m, step, color);
        }
        glCallList(gridlist);
    }
}