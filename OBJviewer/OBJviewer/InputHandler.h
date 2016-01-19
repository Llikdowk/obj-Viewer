#pragma once
#include <GL/glui.h>
#include <vector>
#include <set>
#include "Behaviours.h"

class InputHandler {
private:
    Behaviour* behaviour;
	InputHandler();

public:
	static InputHandler& getInstance();

	InputHandler(const InputHandler&) = delete;
	void operator=(const InputHandler&) = delete;

    void setBehaviour(Behaviour*);

	static void GLCallbackOnKey(unsigned char key, int x, int y);
	static void GLCallbackOnKeyUp(unsigned char key, int x, int y);
	static void GLCallbackOnSpecialKey(int specialKey, int x, int y);
	static void GLCallbackOnSpecialKeyUp(int specialKey, int x, int y);
	static void GLCallbackOnMouse(int button, int state, int x, int y);
	static void GLCallbackOnMotion(int x, int y);
	static void GLCallbackOnPassiveMotion(int x, int y);
};
