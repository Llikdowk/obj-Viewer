#include "render.h"

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	render::init();

	InputHandler& input = InputHandler::getInstance();
    input.setBehaviour(new TypicalBehaviour());

	glutMainLoop();
}