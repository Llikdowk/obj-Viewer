#pragma once
#include <iostream>
#include "Behaviours.h"

void TypicalBehaviour::onKey(unsigned char key, int x, int y) {
    Behaviour::onKey(key, x, y);

    if (key == 'p') {
        wireframe = !wireframe;
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}