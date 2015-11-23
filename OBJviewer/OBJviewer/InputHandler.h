#pragma once
#include <GL\freeglut.h>
#include <vector>
#include <set>

class InputHandler {
private:
	int mouseX, mouseY;
	std::set<unsigned char> pressedKeys;
	std::set<int> pressedSpecialKeys;

	InputHandler();
public:
	static InputHandler& getInstance();

	InputHandler(const InputHandler&) = delete;
	void operator=(const InputHandler&) = delete;

	bool isKeyPressed(unsigned char key);
	std::vector<char> getPressedKeys();

	bool isSpecialKeyPressed(int specialKey);
	std::vector<char> getPressedSpecialKeys();

	void getMousePosition();

	virtual void onKey(unsigned char key, int x, int y);
	virtual void onKeyUp(unsigned char key, int x, int y);
	virtual void onSpecialKey(int specialKey, int x, int y);
	virtual void onSpecialKeyUp(int specialKey, int x, int y);
	virtual void onMouse(int button, int state, int x, int y);
	virtual void onMotion(int x, int y);
	virtual void onPassiveMotion(int x, int y);

	static void GLCallbackOnKey(unsigned char key, int x, int y);
	static void GLCallbackOnKeyUp(unsigned char key, int x, int y);
	static void GLCallbackOnSpecialKey(int specialKey, int x, int y);
	static void GLCallbackOnSpecialKeyUp(int specialKey, int x, int y);
	static void GLCallbackOnMouse(int button, int state, int x, int y);
	static void GLCallbackOnMotion(int x, int y);
	static void GLCallbackOnPassiveMotion(int x, int y);
};
