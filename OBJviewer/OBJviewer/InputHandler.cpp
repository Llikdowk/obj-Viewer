#include "InputHandler.h"

#include <iostream>


InputHandler::InputHandler() {
	glutKeyboardFunc(InputHandler::GLCallbackOnKey);
	glutKeyboardUpFunc(InputHandler::GLCallbackOnKeyUp);
	glutSpecialFunc(InputHandler::GLCallbackOnSpecialKey);
	glutSpecialUpFunc(InputHandler::GLCallbackOnSpecialKeyUp);
	glutMouseFunc(InputHandler::GLCallbackOnMouse);
	glutMotionFunc(InputHandler::GLCallbackOnMotion);
	glutPassiveMotionFunc(InputHandler::GLCallbackOnPassiveMotion);
}

InputHandler& InputHandler::getInstance() {
	static InputHandler instance;
	return instance;
}

void InputHandler::onKey(unsigned char key, int x, int y) {
	std::cout << key << std::endl;
}

void InputHandler::onKeyUp(unsigned char key, int x, int y) {

}

void InputHandler::onSpecialKey(int specialKey, int x, int y) {
	std::cout << specialKey << std::endl;
}

void InputHandler::onSpecialKeyUp(int specialKey, int x, int y) {

}

void InputHandler::onMouse(int button, int state, int x, int y) {

}

void InputHandler::onMotion(int x, int y) {

}

void InputHandler::onPassiveMotion(int x, int y) {

}

/*****************\
*	GLCallbacks   *
\*****************/

void InputHandler::GLCallbackOnKey(unsigned char key, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.onKey(key, x, y);

	instance.pressedKeys.insert(key);
}

void InputHandler::GLCallbackOnKeyUp(unsigned char key, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.onKeyUp(key, x, y);

	instance.pressedKeys.erase(key);
}

void InputHandler::GLCallbackOnSpecialKey(int specialKey, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.onSpecialKey(specialKey, x, y);

	instance.pressedSpecialKeys.insert(specialKey);
}

void InputHandler::GLCallbackOnSpecialKeyUp(int specialKey, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.onSpecialKeyUp(specialKey, x, y);

	instance.pressedSpecialKeys.erase(specialKey);
}

void InputHandler::GLCallbackOnMouse(int button, int state, int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.onMouse(button, state, x, y);
}

void InputHandler::GLCallbackOnMotion(int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.onMotion(x, y);
}

void InputHandler::GLCallbackOnPassiveMotion(int x, int y) {
	InputHandler& instance = InputHandler::getInstance();
	instance.onPassiveMotion(x, y);
}