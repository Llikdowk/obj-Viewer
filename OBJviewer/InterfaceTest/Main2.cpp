#include <string.h>
#include <GL/glui.h>

#include <iostream>
#include <windows.h>
#include <string>

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

// GLUI Live Variables

// Rendering Modes
int   wireframe = 0;
int   textures = 1;
int   lighting = 1;

// Lighting
int   light0_enabled = 1;
float light0_intensity = 1.0;
float light0_X = 0.0;
float light0_Y = 0.0;
float light0_Z = 0.0;

int   light1_enabled = 1;
float light1_intensity = .4;
float light1_X = 0.0;
float light1_Y = 0.0;
float light1_Z = 0.0;




int   obj_type = 1;
int   segments = 8;
int   segments2 = 8;

int   main_window;
float scale = 1.0;
float sphere_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float torus_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
char *string_list[] = { "Hello World!", "Foo", "Testing...", "Bounding box: on" };
int   curr_string = 0;

/** Pointers to the windows and some of the controls we'll create **/
GLUI_Spinner     *light0_spinner, *lightPosX0, *lightPosY0, *lightPosZ0;
GLUI_Scrollbar   *light0_R, *light0_G, *light0_B;
GLUI_Spinner     *light1_spinner, *lightPosX1, *lightPosY1, *lightPosZ1;
GLUI_Scrollbar   *light1_R, *light1_G, *light1_B;

GLUI_FileBrowser *fileBrowser;
GLUI_StaticText  *vertexText, *polygonText;

/********** User IDs for callbacks ********/
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260
#define LIGHT0_X_ID	         270
#define LIGHT0_Y_ID	         271
#define LIGHT0_Z_ID	         272
#define LIGHT1_X_ID	         280
#define LIGHT1_Y_ID	         281
#define LIGHT1_Z_ID	         282
#define OPENFILE_ID          999


/********** Miscellaneous global variables **********/

int vertexCount, polygonCount;

GLfloat light0_ambient[] = { 0.1f, 0.1f, 0.3f, 1.0f };
GLfloat light0_diffuse[] = { .6f, .6f, 1.0f, 1.0f };
GLfloat light0_position[] = { .5f, .5f, 1.0f, 0.0f };

GLfloat light1_ambient[] = { 0.1f, 0.1f, 0.3f, 1.0f };
GLfloat light1_diffuse[] = { .9f, .6f, 0.0f, 1.0f };
GLfloat light1_position[] = { -1.0f, -1.0f, 1.0f, 0.0f };

GLfloat lights_rotation[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

void lights() {
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
}

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb(int control)
{
	if (control == OPENFILE_ID) {
		TCHAR NPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, NPath);
		std::wstring s = NPath;
		char path[MAX_PATH];
		size_t x;
		wcstombs_s(&x, path, MAX_PATH, s.c_str(), MAX_PATH);
		std::cout << path << "\\" << fileBrowser->get_file() << std::endl;
	} else if (control == LIGHT0_ENABLED_ID) {
		if (light0_enabled) {
			glEnable(GL_LIGHT0);
			light0_spinner->enable();
			lightPosX0->enable();
			lightPosY0->enable();
			lightPosZ0->enable();
			light0_R->enable();
			light0_G->enable();
			light0_B->enable();
		}
		else {
			glDisable(GL_LIGHT0);
			light0_spinner->disable();
			lightPosX0->disable();
			lightPosY0->disable();
			lightPosZ0->disable();
			light0_R->disable();
			light0_G->disable();
			light0_B->disable();
		}
	}
	else if (control == LIGHT1_ENABLED_ID) {
		if (light1_enabled) {
			glEnable(GL_LIGHT1);
			light1_spinner->enable();
			lightPosX1->enable();
			lightPosY1->enable();
			lightPosZ1->enable();
			light1_R->enable();
			light1_G->enable();
			light1_B->enable();
		}
		else {
			glDisable(GL_LIGHT1);
			light1_spinner->disable();
			lightPosX1->disable();
			lightPosY1->disable();
			lightPosZ1->disable();
			light1_R->disable();
			light1_G->disable();
			light1_B->disable();
		}
	}
	else if (control == LIGHT0_INTENSITY_ID) {
		float v[] = {
			light0_diffuse[0],  light0_diffuse[1],
			light0_diffuse[2],  light0_diffuse[3] };

		v[0] *= light0_intensity;
		v[1] *= light0_intensity;
		v[2] *= light0_intensity;

		glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
	}
	else if (control == LIGHT1_INTENSITY_ID) {
		float v[] = {
			light1_diffuse[0],  light1_diffuse[1],
			light1_diffuse[2],  light1_diffuse[3] };

		v[0] *= light1_intensity;
		v[1] *= light1_intensity;
		v[2] *= light1_intensity;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
	}
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case 27:
	case 'q':
		exit(0);
		break;
	};

	glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/


/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	/*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
	application  */

	glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y)
{
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y)
{
	glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);

	xy_aspect = (float)tw / (float)th;

	glutPostRedisplay();
}


/************************************************** draw_axes() **********/
/* Disables lighting, then draws RGB axes                                */

void draw_axes(float scale)
{
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glScalef(scale, scale, scale);

	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(.8f, 0.05f, 0.0);  glVertex3f(1.0, 0.25f, 0.0); /* Letter X */
	glVertex3f(0.8f, .25f, 0.0);  glVertex3f(1.0, 0.05f, 0.0);
	glVertex3f(0.0, 0.0, 0.0);  glVertex3f(1.0, 0.0, 0.0); /* X axis      */

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 1.0, 0.0); /* Y axis      */

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 0.0, 1.0); /* Z axis    */
	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHTING);
}


/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(.9f, .9f, .9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glMultMatrixf(lights_rotation);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -2.6f);
	glTranslatef(obj_pos[0], obj_pos[1], -obj_pos[2]);
	glMultMatrixf(view_rotate);

	glScalef(scale, scale, scale);

	/*** Now we render object, using the variables 'obj_type', 'segments', and
	'wireframe'.  These are _live_ variables, which are transparently
	updated by GLUI ***/

	glPushMatrix();
	glTranslatef(-.5, 0.0, 0.0);
	glMultMatrixf(sphere_rotate);
	if (wireframe)
		glutWireSphere(.4, segments, segments);
	else 
		glutSolidSphere(.4, segments, segments);
	draw_axes(.52f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.5, 0.0, 0.0);
	glMultMatrixf(torus_rotate);
	if (wireframe)
		glutWireTorus(.15, .3, 16, segments);
	else 
		glutSolidTorus(.15, .3, 16, segments);
	draw_axes(.52f);
	glPopMatrix();

	glDisable(GL_LIGHTING);  /* Disable lighting while we render text */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3ub(0, 0, 0);
	glRasterPos2i(10, 10);

	/*  printf( "text: %s\n", text );              */

	/*** Render the live character array 'text' ***/
	int i;
	for (i = 0; i<(int)strlen(string_list[curr_string]); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string_list[curr_string][i]);

	glEnable(GL_LIGHTING);


	glutSwapBuffers();
}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1280, 768);
	main_window = glutCreateWindow("Object viewer");

	glutDisplayFunc(myGlutDisplay);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);

	//Change on input
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
	GLUI_Master.set_glutSpecialFunc(NULL);
	GLUI_Master.set_glutMouseFunc(myGlutMouse);
	glutMotionFunc(myGlutMotion);

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	//glClearColor(.1f, .1f, .1f, 1.0);

	lights();
}

void buildGUI() {
	printf("GLUI version: %3.2f\n", GLUI_Master.get_version());

	// Status bar 
	{
		GLUI *statusBar = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_BOTTOM);
		vertexText = statusBar->add_statictext("");
		vertexText->set_text("Vertex: 0");

		statusBar->add_column();
		polygonText = statusBar->add_statictext("");
		polygonText->set_text("Polygons: 0");
	}

	// Side subwindow
	{
		GLUI *rightSubwindow = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);
		rightSubwindow->set_main_gfx_window(main_window);

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

			GLUI_Checkbox *cb = new GLUI_Checkbox(obj_panel, "Wireframe                         ", &wireframe, 1, control_cb);
			new GLUI_Checkbox(obj_panel, "Textures", &textures, 1, control_cb);
		}
		
		// Lights
		{
			GLUI_Panel *roll_lights = new GLUI_Panel(rightSubwindow, "Lights");
			new GLUI_Checkbox(roll_lights, "Lighting", &lighting, 1, control_cb);

			// Light0
			GLUI_Rollout *light0 = new GLUI_Rollout(roll_lights, "Light 1");
			new GLUI_Checkbox(light0, "Enabled", &light0_enabled, LIGHT0_ENABLED_ID, control_cb);

			// Position
			GLUI_Panel *lightPos0 = new GLUI_Panel(light0, "Position");
			lightPosX0 = new GLUI_Spinner(lightPos0, "X:", &light0_X, LIGHT0_X_ID, control_cb);
			lightPosX0->set_float_limits(-1000.0, 1000.0);
			lightPosY0 = new GLUI_Spinner(lightPos0, "Y:", &light0_Y, LIGHT0_Y_ID, control_cb);
			lightPosY0->set_float_limits(-1000.0, 1000.0);
			lightPosZ0 = new GLUI_Spinner(lightPos0, "Z:", &light0_Z, LIGHT0_Z_ID, control_cb);
			lightPosZ0->set_float_limits(-1000.0, 1000.0);

			// Color
			light0_spinner = new GLUI_Spinner(light0, "Intensity:", &light0_intensity, LIGHT0_INTENSITY_ID, control_cb);
			light0_spinner->set_float_limits(0.0, 1.0);
			GLUI_Panel *lightColor0 = new GLUI_Panel(light0, "Color (R, G, B)");
			light0_R = new GLUI_Scrollbar(lightColor0, "Red", GLUI_SCROLL_HORIZONTAL, &light0_diffuse[0], LIGHT0_INTENSITY_ID, control_cb);
			light0_R->set_float_limits(0, 1);
			light0_G = new GLUI_Scrollbar(lightColor0, "Green", GLUI_SCROLL_HORIZONTAL, &light0_diffuse[1], LIGHT0_INTENSITY_ID, control_cb);
			light0_G->set_float_limits(0, 1);
			light0_B = new GLUI_Scrollbar(lightColor0, "Blue", GLUI_SCROLL_HORIZONTAL, &light0_diffuse[2], LIGHT0_INTENSITY_ID, control_cb);
			light0_B->set_float_limits(0, 1);


			// Light 1
			GLUI_Rollout *light1 = new GLUI_Rollout(roll_lights, "Light 2");
			new GLUI_Checkbox(light1, "Enabled", &light1_enabled, LIGHT1_ENABLED_ID, control_cb);

			// Position
			GLUI_Panel *lightPos1 = new GLUI_Panel(light1, "Position");
			lightPosX1 = new GLUI_Spinner(lightPos1, "X:", &light1_X, LIGHT1_X_ID, control_cb);
			lightPosX1->set_float_limits(-1000.0, 1000.0);
			lightPosY1 = new GLUI_Spinner(lightPos1, "Y:", &light1_Y, LIGHT1_Y_ID, control_cb);
			lightPosY1->set_float_limits(-1000.0, 1000.0);
			lightPosZ1 = new GLUI_Spinner(lightPos1, "Z:", &light1_Z, LIGHT1_Z_ID, control_cb);
			lightPosZ1->set_float_limits(-1000.0, 1000.0);

			// Color
			light1_spinner = new GLUI_Spinner(light1, "Intensity:", &light1_intensity, LIGHT1_INTENSITY_ID, control_cb);
			light1_spinner->set_float_limits(0.0, 1.0);
			GLUI_Panel *lightColor1 = new GLUI_Panel(light1, "Color (R, G, B)");
			light1_R = new GLUI_Scrollbar(lightColor1, "Red", GLUI_SCROLL_HORIZONTAL, &light1_diffuse[0], LIGHT1_INTENSITY_ID, control_cb);
			light1_R->set_float_limits(0, 1);
			light1_G = new GLUI_Scrollbar(lightColor1, "Green", GLUI_SCROLL_HORIZONTAL, &light1_diffuse[1], LIGHT1_INTENSITY_ID, control_cb);
			light1_G->set_float_limits(0, 1);
			light1_B = new GLUI_Scrollbar(lightColor1, "Blue", GLUI_SCROLL_HORIZONTAL, &light1_diffuse[2], LIGHT1_INTENSITY_ID, control_cb);
			light1_B->set_float_limits(0, 1);
		}
	}

	// Camera Controls
	{
		GLUI *glui2 = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_BOTTOM);
		glui2->set_main_gfx_window(main_window);

		GLUI_Panel *cameraMovement = new GLUI_Panel(glui2, "Camera Movement");
		GLUI_Translation *trans_xy = new GLUI_Translation(cameraMovement, "Objects XY", GLUI_TRANSLATION_XY, obj_pos);
		trans_xy->set_speed(.005);
		new GLUI_Column(cameraMovement, false);
		GLUI_Translation *trans_x = new GLUI_Translation(cameraMovement, "Objects X", GLUI_TRANSLATION_X, obj_pos);
		trans_x->set_speed(.005);
		new GLUI_Column(cameraMovement, false);
		GLUI_Translation *trans_y = new GLUI_Translation(cameraMovement, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1]);
		trans_y->set_speed(.005);
		new GLUI_Column(cameraMovement, false);
		GLUI_Translation *trans_z = new GLUI_Translation(cameraMovement, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2]);
		trans_z->set_speed(.005);
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	init();
	buildGUI();
	
	glutMainLoop();
	return EXIT_SUCCESS;
}

