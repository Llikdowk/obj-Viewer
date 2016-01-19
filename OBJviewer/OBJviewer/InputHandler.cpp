#include <iostream>
#include "InputHandler.h"

InputHandler::InputHandler() : behaviour(new Behaviour()) {
	GLUI_Master.set_glutKeyboardFunc(InputHandler::GLCallbackOnKey);
	GLUI_Master.set_glutSpecialFunc(InputHandler::GLCallbackOnSpecialKey);
	GLUI_Master.set_glutMouseFunc(InputHandler::GLCallbackOnMouse);
	glutMotionFunc(InputHandler::GLCallbackOnMotion);

	//glutKeyboardFunc(InputHandler::GLCallbackOnKey);
	glutKeyboardUpFunc(InputHandler::GLCallbackOnKeyUp);
	//glutSpecialFunc(InputHandler::GLCallbackOnSpecialKey);
	glutSpecialUpFunc(InputHandler::GLCallbackOnSpecialKeyUp);
	//glutMouseFunc(InputHandler::GLCallbackOnMouse);
	//glutMotionFunc(InputHandler::GLCallbackOnMotion);
	glutPassiveMotionFunc(InputHandler::GLCallbackOnPassiveMotion);
}

InputHandler& InputHandler::getInstance() {
	static InputHandler instance;
	return instance;
}

void InputHandler::setBehaviour(Behaviour* b) {
    delete behaviour;
    behaviour = b;
}

/*****************\
*	GLCallbacks   *
\*****************/

void InputHandler::GLCallbackOnKey(unsigned char key, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.behaviour->onKey(key, x, y);
}

void InputHandler::GLCallbackOnKeyUp(unsigned char key, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.behaviour->onKeyUp(key, x, y);
}

void InputHandler::GLCallbackOnSpecialKey(int specialKey, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.behaviour->onSpecialKey(specialKey, x, y);
}

void InputHandler::GLCallbackOnSpecialKeyUp(int specialKey, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.behaviour->onSpecialKeyUp(specialKey, x, y);
}

void InputHandler::GLCallbackOnMouse(int button, int state, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.behaviour->onMouse(button, state, x, y);
}

void InputHandler::GLCallbackOnMotion(int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.behaviour->onMotion(x, y);
}

void InputHandler::GLCallbackOnPassiveMotion(int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.behaviour->onPassiveMotion(x, y);
}