#include <Windows.h>
#include "render.h"


GLUI_Spinner     *light2_Intensity, *light2_X, *light2_Y, *light2_Z;
GLUI_Scrollbar   *light2_R, *light2_G, *light2_B;
GLUI_Spinner     *light3_Intensity, *light3_X, *light3_Y, *light3_Z;
GLUI_Scrollbar   *light3_R, *light3_G, *light3_B;

GLUI_FileBrowser *fileBrowser;
GLUI_StaticText  *vertexText, *polygonText;


// CALLBACK IDS
#define LIGHT2_ENABLED_ID    200
#define LIGHT2_INTENSITY_ID  250
#define LIGHT2_POS_ID	     270

#define LIGHT3_INTENSITY_ID  260
#define LIGHT3_ENABLED_ID    201
#define LIGHT3_POS_ID	     280

#define OPENFILE_ID          999



void control_cb(int control) {
	if (control == OPENFILE_ID) {
		TCHAR NPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, NPath);
		std::wstring s = NPath;
		char path[MAX_PATH];
		size_t x;
		wcstombs_s(&x, path, MAX_PATH, s.c_str(), MAX_PATH);
		//::cout << path << "\\" << fileBrowser->get_file() << std::endl;
		render::loadModel(path, fileBrowser->get_file());
	}
	else if (control == LIGHT2_ENABLED_ID) {
		if (lights::light2_enabled) {
			glEnable(GL_LIGHT2);
			light2_Intensity->enable();
			light2_X->enable();
			light2_Y->enable();
			light2_Z->enable();
			light2_R->enable();
			light2_G->enable();
			light2_B->enable();
		}
		else {
			glDisable(GL_LIGHT2);
			light2_Intensity->disable();
			light2_X->disable();
			light2_Y->disable();
			light2_Z->disable();
			light2_R->disable();
			light2_G->disable();
			light2_B->disable();
		}
	}
	else if (control == LIGHT3_ENABLED_ID) {
		if (lights::light3_enabled) {
			glEnable(GL_LIGHT3);
			light3_Intensity->enable();
			light3_X->enable();
			light3_Y->enable();
			light3_Z->enable();
			light3_R->enable();
			light3_G->enable();
			light3_B->enable();
		}
		else {
			glDisable(GL_LIGHT3);
			light3_Intensity->disable();
			light3_X->disable();
			light3_Y->disable();
			light3_Z->disable();
			light3_R->disable();
			light3_G->disable();
			light3_B->disable();
		}
	}
	else if (control == LIGHT2_INTENSITY_ID) {
		float v[] = {
			lights::light2_diffuse[0],  lights::light2_diffuse[1],
			lights::light2_diffuse[2],  lights::light2_diffuse[3] };

		v[0] *= lights::light2_intensity;
		v[1] *= lights::light2_intensity;
		v[2] *= lights::light2_intensity;

		glLightfv(GL_LIGHT2, GL_DIFFUSE, v);
	}
	else if (control == LIGHT3_INTENSITY_ID) {
		float v[] = {
			lights::light3_diffuse[0],  lights::light3_diffuse[1],
			lights::light3_diffuse[2],  lights::light3_diffuse[3] };

		v[0] *= lights::light3_intensity;
		v[1] *= lights::light3_intensity;
		v[2] *= lights::light3_intensity;

		glLightfv(GL_LIGHT3, GL_DIFFUSE, v);
	}
	else if (control == LIGHT2_POS_ID) {
		glLightfv(GL_LIGHT2, GL_POSITION, lights::light2_position);
	}
	else if (control == LIGHT3_POS_ID) {
		glLightfv(GL_LIGHT3, GL_POSITION, lights::light3_position);
	}
}

void buildGUI() {
	printf("GLUI version: %3.2f\n", GLUI_Master.get_version());

	// Status bar 
	{
		GLUI *statusBar = GLUI_Master.create_glui_subwindow(render::main_window, GLUI_SUBWINDOW_BOTTOM);
		vertexText = statusBar->add_statictext("");
		vertexText->set_text("Vertex: 0");

		statusBar->add_column();
		polygonText = statusBar->add_statictext("");
		polygonText->set_text("Polygons: 0");
	}

	// Side subwindow
	{
		GLUI *rightSubwindow = GLUI_Master.create_glui_subwindow(render::main_window, GLUI_SUBWINDOW_RIGHT);
		rightSubwindow->set_main_gfx_window(render::main_window);

		// File Browser
		{
			//fileBrowser = new GLUI_FileBrowser(rightSubwindow, "File Browser", GLUI_PANEL_EMBOSSED, OPENFILE_ID, control_cb);
			//fileBrowser->set_allow_change_dir(true);
		}

		// Rendering Options
		{
			GLUI_Panel *obj_panel = new GLUI_Panel(rightSubwindow, "Rendering");

			new GLUI_Checkbox(obj_panel, "Wireframe                         ", &render::wireframe, WIREFRAME_MODE, render::changeRenderingMode);
			new GLUI_Checkbox(obj_panel, "Textures", &render::textures, TEXTURE_MODE, render::changeRenderingMode);
			new GLUI_Checkbox(obj_panel, "Gizmo", &render::gizmo, -1, control_cb);
			new GLUI_Checkbox(obj_panel, "Grid", &render::grid, -1, control_cb);
		}

		// Lights
		{
			GLUI_Panel *roll_lights = new GLUI_Panel(rightSubwindow, "Lights");
			new GLUI_Checkbox(roll_lights, "Lighting", &render::lighting, LIGHTING_MODE, render::changeRenderingMode);

			// Light0
			GLUI_Rollout *light0 = new GLUI_Rollout(roll_lights, "Light 1");
			new GLUI_Checkbox(light0, "Enabled", &lights::light2_enabled, LIGHT2_ENABLED_ID, control_cb);

			// Position
			GLUI_Panel *lightPos0 = new GLUI_Panel(light0, "Position");
			light2_X = new GLUI_Spinner(lightPos0, "X:", &lights::light2_position[0], LIGHT2_POS_ID, control_cb);
			light2_X->set_float_limits(-1000.0, 1000.0);
			light2_Y = new GLUI_Spinner(lightPos0, "Y:", &lights::light2_position[1], LIGHT2_POS_ID, control_cb);
			light2_Y->set_float_limits(-1000.0, 1000.0);
			light2_Z = new GLUI_Spinner(lightPos0, "Z:", &lights::light2_position[2], LIGHT2_POS_ID, control_cb);
			light2_Z->set_float_limits(-1000.0, 1000.0);

			// Color
			light2_Intensity = new GLUI_Spinner(light0, "Intensity:", &lights::light2_intensity, LIGHT2_INTENSITY_ID, control_cb);
			light2_Intensity->set_float_limits(0.0, 1.0);
			GLUI_Panel *lightColor0 = new GLUI_Panel(light0, "Color (R, G, B)");
			light2_R = new GLUI_Scrollbar(lightColor0, "Red", GLUI_SCROLL_HORIZONTAL, &lights::light2_diffuse[0], LIGHT2_INTENSITY_ID, control_cb);
			light2_R->set_float_limits(0, 1);
			light2_G = new GLUI_Scrollbar(lightColor0, "Green", GLUI_SCROLL_HORIZONTAL, &lights::light2_diffuse[1], LIGHT2_INTENSITY_ID, control_cb);
			light2_G->set_float_limits(0, 1);
			light2_B = new GLUI_Scrollbar(lightColor0, "Blue", GLUI_SCROLL_HORIZONTAL, &lights::light2_diffuse[2], LIGHT2_INTENSITY_ID, control_cb);
			light2_B->set_float_limits(0, 1);


			// Light 1
			GLUI_Rollout *light1 = new GLUI_Rollout(roll_lights, "Light 2");
			new GLUI_Checkbox(light1, "Enabled", &lights::light3_enabled, LIGHT3_ENABLED_ID, control_cb);

			// Position
			GLUI_Panel *lightPos1 = new GLUI_Panel(light1, "Position");
			light3_X = new GLUI_Spinner(lightPos1, "X:", &lights::light3_position[0], LIGHT3_POS_ID, control_cb);
			light3_X->set_float_limits(-1000.0, 1000.0);
			light3_Y = new GLUI_Spinner(lightPos1, "Y:", &lights::light3_position[1], LIGHT3_POS_ID, control_cb);
			light3_Y->set_float_limits(-1000.0, 1000.0);
			light3_Z = new GLUI_Spinner(lightPos1, "Z:", &lights::light3_position[2], LIGHT3_POS_ID, control_cb);
			light3_Z->set_float_limits(-1000.0, 1000.0);

			// Color
			light3_Intensity = new GLUI_Spinner(light1, "Intensity:", &lights::light3_intensity, LIGHT3_INTENSITY_ID, control_cb);
			light3_Intensity->set_float_limits(0.0, 1.0);
			GLUI_Panel *lightColor1 = new GLUI_Panel(light1, "Color (R, G, B)");
			light3_R = new GLUI_Scrollbar(lightColor1, "Red", GLUI_SCROLL_HORIZONTAL, &lights::light3_diffuse[0], LIGHT3_INTENSITY_ID, control_cb);
			light3_R->set_float_limits(0, 1);
			light3_G = new GLUI_Scrollbar(lightColor1, "Green", GLUI_SCROLL_HORIZONTAL, &lights::light3_diffuse[1], LIGHT3_INTENSITY_ID, control_cb);
			light3_G->set_float_limits(0, 1);
			light3_B = new GLUI_Scrollbar(lightColor1, "Blue", GLUI_SCROLL_HORIZONTAL, &lights::light3_diffuse[2], LIGHT3_INTENSITY_ID, control_cb);
			light3_B->set_float_limits(0, 1);
		}
	}

	// Camera Controls
	{
		GLUI *glui2 = GLUI_Master.create_glui_subwindow(render::main_window, GLUI_SUBWINDOW_BOTTOM);
		glui2->set_main_gfx_window(render::main_window);

		GLUI_Panel *cameraMovement = new GLUI_Panel(glui2, "Camera Movement");
		/*GLUI_Translation *trans_xy = new GLUI_Translation(cameraMovement, "Objects XY", GLUI_TRANSLATION_XY, obj_pos);
		trans_xy->set_speed(.005);
		new GLUI_Column(cameraMovement, false);
		GLUI_Translation *trans_x = new GLUI_Translation(cameraMovement, "Objects X", GLUI_TRANSLATION_X, obj_pos);
		trans_x->set_speed(.005);
		new GLUI_Column(cameraMovement, false);
		GLUI_Translation *trans_y = new GLUI_Translation(cameraMovement, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1]);
		trans_y->set_speed(.005);
		new GLUI_Column(cameraMovement, false);
		GLUI_Translation *trans_z = new GLUI_Translation(cameraMovement, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2]);
		trans_z->set_speed(.005);*/
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	render::init();

	InputHandler& input = InputHandler::getInstance();
    input.setBehaviour(new TypicalBehaviour());

	buildGUI();

	glutMainLoop();
}