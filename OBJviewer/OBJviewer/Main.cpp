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
#define LIGHT2_X_ID	         270
#define LIGHT2_Y_ID	         271
#define LIGHT2_Z_ID	         272

#define LIGHT3_INTENSITY_ID  260
#define LIGHT3_ENABLED_ID    201
#define LIGHT3_X_ID	         280
#define LIGHT3_Y_ID	         281
#define LIGHT3_Z_ID	         282

#define OPENFILE_ID          999



void control_cb(int control) {
	if (control == OPENFILE_ID) {
		TCHAR NPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, NPath);
		std::wstring s = NPath;
		char path[MAX_PATH];
		size_t x;
		wcstombs_s(&x, path, MAX_PATH, s.c_str(), MAX_PATH);
		std::cout << path << "\\" << fileBrowser->get_file() << std::endl;
	}
	else if (control == LIGHT2_ENABLED_ID) {
		if (render::light2_enabled) {
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
		if (render::light3_enabled) {
			glEnable(GL_LIGHT3);
			light3_Intensity->enable();
			light3_X->enable();
			light3_Y->enable();
			light3_Z->enable();
			light2_R->enable();
			light2_G->enable();
			light2_B->enable();
		}
		else {
			glDisable(GL_LIGHT3);
			light3_Intensity->disable();
			light3_X->disable();
			light3_Y->disable();
			light3_Z->disable();
			light2_R->disable();
			light2_G->disable();
			light2_B->disable();
		}
	}
	else if (control == LIGHT2_INTENSITY_ID) {
		float v[] = {
			render::light2_diffuse[0],  render::light2_diffuse[1],
			render::light2_diffuse[2],  render::light2_diffuse[3] };

		v[0] *= render::light2_intensity;
		v[1] *= render::light2_intensity;
		v[2] *= render::light2_intensity;

		glLightfv(GL_LIGHT2, GL_DIFFUSE, v);
	}
	else if (control == LIGHT3_INTENSITY_ID) {
		float v[] = {
			render::light3_diffuse[0],  render::light3_diffuse[1],
			render::light3_diffuse[2],  render::light3_diffuse[3] };

		v[0] *= render::light3_intensity;
		v[1] *= render::light3_intensity;
		v[2] *= render::light3_intensity;

		glLightfv(GL_LIGHT3, GL_DIFFUSE, v);
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
			GLUI_Panel *fileBrowserPanel;
			fileBrowserPanel = rightSubwindow->add_panel("File Browser", GLUI_PANEL_EMBOSSED);
			fileBrowser = new GLUI_FileBrowser(fileBrowserPanel, "", GLUI_PANEL_NONE, OPENFILE_ID, control_cb);
			fileBrowser->set_allow_change_dir(true);
			rightSubwindow->add_button_to_panel(fileBrowserPanel, "Open File", OPENFILE_ID, control_cb);
		}

		// Rendering Options
		{
			GLUI_Panel *obj_panel = new GLUI_Panel(rightSubwindow, "Rendering");

			GLUI_Checkbox *cb = new GLUI_Checkbox(obj_panel, "Wireframe                         ", &render::wireframe, 1, control_cb);
			new GLUI_Checkbox(obj_panel, "Textures", &render::textures, 1, control_cb);
		}

		// Lights
		{
			GLUI_Panel *roll_lights = new GLUI_Panel(rightSubwindow, "Lights");
			new GLUI_Checkbox(roll_lights, "Lighting", &render::lighting, 1, control_cb);

			// Light0
			GLUI_Rollout *light0 = new GLUI_Rollout(roll_lights, "Light 1");
			new GLUI_Checkbox(light0, "Enabled", &render::light2_enabled, LIGHT2_ENABLED_ID, control_cb);

			// Position
			GLUI_Panel *lightPos0 = new GLUI_Panel(light0, "Position");
			light2_X = new GLUI_Spinner(lightPos0, "X:", &render::light2_X, LIGHT2_X_ID, control_cb);
			light2_X->set_float_limits(-1000.0, 1000.0);
			light2_Y = new GLUI_Spinner(lightPos0, "Y:", &render::light2_Y, LIGHT2_Y_ID, control_cb);
			light2_Y->set_float_limits(-1000.0, 1000.0);
			light2_Z = new GLUI_Spinner(lightPos0, "Z:", &render::light2_Z, LIGHT2_Z_ID, control_cb);
			light2_Z->set_float_limits(-1000.0, 1000.0);

			// Color
			light2_Intensity = new GLUI_Spinner(light0, "Intensity:", &render::light2_intensity, LIGHT2_INTENSITY_ID, control_cb);
			light2_Intensity->set_float_limits(0.0, 1.0);
			GLUI_Panel *lightColor0 = new GLUI_Panel(light0, "Color (R, G, B)");
			light2_R = new GLUI_Scrollbar(lightColor0, "Red", GLUI_SCROLL_HORIZONTAL, &render::light2_diffuse[0], LIGHT2_INTENSITY_ID, control_cb);
			light2_R->set_float_limits(0, 1);
			light2_G = new GLUI_Scrollbar(lightColor0, "Green", GLUI_SCROLL_HORIZONTAL, &render::light2_diffuse[1], LIGHT2_INTENSITY_ID, control_cb);
			light2_G->set_float_limits(0, 1);
			light2_B = new GLUI_Scrollbar(lightColor0, "Blue", GLUI_SCROLL_HORIZONTAL, &render::light2_diffuse[2], LIGHT2_INTENSITY_ID, control_cb);
			light2_B->set_float_limits(0, 1);


			// Light 1
			GLUI_Rollout *light1 = new GLUI_Rollout(roll_lights, "Light 2");
			new GLUI_Checkbox(light1, "Enabled", &render::light3_enabled, LIGHT3_ENABLED_ID, control_cb);

			// Position
			GLUI_Panel *lightPos1 = new GLUI_Panel(light1, "Position");
			light3_X = new GLUI_Spinner(lightPos1, "X:", &render::light3_X, LIGHT3_X_ID, control_cb);
			light3_X->set_float_limits(-1000.0, 1000.0);
			light3_Y = new GLUI_Spinner(lightPos1, "Y:", &render::light3_Y, LIGHT3_Y_ID, control_cb);
			light3_Y->set_float_limits(-1000.0, 1000.0);
			light3_Z = new GLUI_Spinner(lightPos1, "Z:", &render::light3_Z, LIGHT3_Z_ID, control_cb);
			light3_Z->set_float_limits(-1000.0, 1000.0);

			// Color
			light3_Intensity = new GLUI_Spinner(light1, "Intensity:", &render::light3_intensity, LIGHT3_INTENSITY_ID, control_cb);
			light3_Intensity->set_float_limits(0.0, 1.0);
			GLUI_Panel *lightColor1 = new GLUI_Panel(light1, "Color (R, G, B)");
			light2_R = new GLUI_Scrollbar(lightColor1, "Red", GLUI_SCROLL_HORIZONTAL, &render::light3_diffuse[0], LIGHT3_INTENSITY_ID, control_cb);
			light2_R->set_float_limits(0, 1);
			light2_G = new GLUI_Scrollbar(lightColor1, "Green", GLUI_SCROLL_HORIZONTAL, &render::light3_diffuse[1], LIGHT3_INTENSITY_ID, control_cb);
			light2_G->set_float_limits(0, 1);
			light2_B = new GLUI_Scrollbar(lightColor1, "Blue", GLUI_SCROLL_HORIZONTAL, &render::light3_diffuse[2], LIGHT3_INTENSITY_ID, control_cb);
			light2_B->set_float_limits(0, 1);
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