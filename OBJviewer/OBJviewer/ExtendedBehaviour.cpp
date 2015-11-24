#pragma once
#include <iostream>
#include "Behaviours.h"

void ExtendedBehaviour::onKey(unsigned char key, int x, int y) {
    TypicalBehaviour::onKey(key, x, y);
    std::cout << "GREAT " << key << std::endl;
}