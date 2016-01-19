#pragma once
#include <iostream>
#include "Behaviours.h"
#include "render.h"

#define SPEED 50

void TypicalBehaviour::onKey(unsigned char key, int x, int y) {
    Behaviour::onKey(key, x, y);

	if (isMouseButton2Pressed) {
		switch (key) {
		case 'w':
			render.moveCamera(SPEED);
			break;
		case 's':
			render.moveCamera(-SPEED);
			break;
		case 'a':
			render.panCamera(-SPEED);
			break;
		case 'd':
			render.panCamera(SPEED);
			break;
		}
	}
}

void TypicalBehaviour::onMouse(int button, int state, int x, int y) {
	std::cout << button << ", " << state << ": " << x << ", " << y << std::endl;
	if (button == 2) {
		if (state == 0) {
			isMouseButton2Pressed = true;
		}
		else if (state == 1) {
			isMouseButton2Pressed = false;
		}
	}

	mouseX = x;
	mouseY = y;
}

void TypicalBehaviour::onMotion(int x, int y) {
	
	if (isMouseButton2Pressed) {
		render.camera.yaw(mouseX - x);
		render.camera.pitch(mouseY - y);
	}

	mouseX = x;
	mouseY = y;
}

void TypicalBehaviour::onPassiveMotion(int x, int y) {
	mouseX = x;
	mouseY = y;
}
