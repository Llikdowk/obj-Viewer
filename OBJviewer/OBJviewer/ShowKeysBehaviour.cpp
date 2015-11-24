#pragma once
#include <iostream>
#include "Behaviours.h"

void ShowKeysBehaviour::onKey(uchar key, int x, int y) {
    Behaviour::onKey(key, x, y);
    std::cout << "pressed: " << key << std::endl;
}
void ShowKeysBehaviour::onKeyUp(uchar key, int x, int y) {
    std::cout << "released: " << key << std::endl;
}
void ShowKeysBehaviour::onSpecialKey(int specialKey, int x, int y) {
    std::cout << "pressed special: " << specialKey << std::endl;
};
void ShowKeysBehaviour::onSpecialKeyUp(int specialKey, int x, int y) {
    std::cout << "released special: " << specialKey << std::endl;
};
void ShowKeysBehaviour::onMouse(int button, int state, int x, int y) {
    std::cout << "mouse_button, state: " << button << "," << state << " at position " << x << ", " << y << std::endl;
};
