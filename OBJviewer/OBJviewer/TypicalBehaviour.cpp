#pragma once
#include <iostream>
#include "Behaviours.h"

void TypicalBehaviour::onKey(unsigned char key, int x, int y) {
    //Behaviour::onKey(key, x, y);
    std::cout << "TYPICAL" << std::endl;
}