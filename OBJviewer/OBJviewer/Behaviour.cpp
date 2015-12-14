#include <gl/freeglut.h>
#include "Behaviours.h"

void end(); // from main

void Behaviour::onKey(uchar key, int x, int y) {
    if (key == 27) {
        end();
    }
};
void Behaviour::onKeyUp(uchar key, int x, int y) {};
void Behaviour::onSpecialKey(int specialKey, int x, int y) {};
void Behaviour::onSpecialKeyUp(int specialKey, int x, int y) {};
void Behaviour::onMouse(int button, int state, int x, int y) {};
void Behaviour::onMotion(int x, int y) {};
void Behaviour::onPassiveMotion(int x, int y) {};