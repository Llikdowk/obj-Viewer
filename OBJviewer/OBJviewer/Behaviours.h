#pragma once
#include <GL/glui.h>
#include "render.h"

class Behaviour {
protected:
    typedef unsigned char uchar;

public:
    virtual void onKey(uchar key, int x, int y);
    virtual void onKeyUp(uchar key, int x, int y);
    virtual void onSpecialKey(int specialKey, int x, int y);
    virtual void onSpecialKeyUp(int specialKey, int x, int y);
    virtual void onMouse(int button, int state, int x, int y);
    virtual void onMotion(int x, int y);
    virtual void onPassiveMotion(int x, int y);
};

class ShowKeysBehaviour : public Behaviour {
public:
    virtual void onKey(uchar key, int x, int y);
    virtual void onKeyUp(uchar key, int x, int y);
    virtual void onSpecialKey(int specialKey, int x, int y);
    virtual void onSpecialKeyUp(int specialKey, int x, int y);
    virtual void onMouse(int button, int state, int x, int y);
	virtual void onMotion(int x, int y);
	virtual void onPassiveMotion(int x, int y);
};

class TypicalBehaviour : public Behaviour {
public:
    virtual void onKey(unsigned char key, int x, int y);
	virtual void onKeyUp(uchar key, int x, int y);
	virtual void onMouse(int button, int state, int x, int y);
	virtual void onMotion(int x, int y);
	virtual void onPassiveMotion(int x, int y);
private:
	Render& render = Render::getInstance();
	bool isMouseButton2Pressed = false;
	int mouseX, mouseY;
};

class ExtendedBehaviour : public TypicalBehaviour {
public:
    virtual void onKey(unsigned char key, int x, int y);
};