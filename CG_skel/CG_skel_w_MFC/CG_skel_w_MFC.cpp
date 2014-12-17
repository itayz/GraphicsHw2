// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum MENU_ITEMS {
	FILE_OPEN = 1, MAIN_DEMO = 1, MAIN_ABOUT = 2, MAIN_FNORMALS,
	MAIN_VNORMALS, MAIN_BOUNDING_BOX, MAIN_CAMERAS, PRIMITIVE_PYRAMID,
	ADD_CAMERA, CHANGE_CAMERA, FRUSTRUM, ORTHO, PERSPECTIVE, WORLD_GRID, REMOVE_CAMERA,
	CONTROL_MODULE,CONTROL_CAMERA,STEP_SCALE,WORLD_FRAME,MODEL_FRAME,
	CHANGE_MODULE,REMOVE_MODULE,CHANGE_UNIFORM_MATERIAL,LIGHT_MENU,LIGHT_SOURCE,ADD_LIGHT,REMOVE_LIGHT,
	CHANGE_LIGHT,CONTROL_LIGHT,LIGHT_COLOR,LIGHT_TYPE,POINT_SOURCE,PARALLEL_SOURCE,
	LIGHT_WHITE, LIGHT_BLUE, LIGHT_YELLOW,FOG,AA



};



// The one and only application object
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include "Renderer.h"
#include <string>
#include "PrimMeshModel.h"
#include "InputDialog.h"
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


Scene *scene;
Renderer *renderer;

int last_x, last_y;
bool lb_down, rb_down, mb_down,control_module=false,control_light=false;
float rotate_step_size, translate_step_size, scale_up_step_size, scale_down_step_size;
float translate_camera_step_size = 0.5;
float step_scale = 1; 
AXES axis = ALL_AXES;
FRAMES frame = MODEL;
CONTROL_TYPES control = CAMERA_CONTROL;
RGB rgbComponent = ALL_RGB;

//----------------------------------------------------------------------------
// Callbacks

void display(void)
{
	//Call the scene and ask it to draw itself
	scene->draw(*renderer);
}

void reshape(int width, int height)
{
	//update the renderer's buffers	
	renderer->UpdateScreenSize(width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	case '+':
		if (control_module)
		{
			if (axis == ALL_AXES) {
				scene->transformActiveModel(MODEL, SCALE, X_AXIS, scale_up_step_size);
				scene->transformActiveModel(MODEL, SCALE, Y_AXIS, scale_up_step_size);
				scene->transformActiveModel(MODEL, SCALE, Z_AXIS, scale_up_step_size);
			}
			else {
				scene->transformActiveModel(frame, SCALE, axis, scale_up_step_size);
			}
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			//if (axis == ALL_AXES) {
			//	scene->transformActiveLight(MODEL, SCALE, X_AXIS, scale_up_step_size);
			//	scene->transformActiveLight(MODEL, SCALE, Y_AXIS, scale_up_step_size);
			//	scene->transformActiveLight(MODEL, SCALE, Z_AXIS, scale_up_step_size);
			//}
			//else {
			//	scene->transformActiveLight(frame, SCALE, axis, scale_up_step_size);
			//}
			scene->changeActiveLightComponent(rgbComponent, LIGHT_DIFFUSE, 1.1);
			scene->changeActiveLightComponent(rgbComponent, LIGHT_SPECULAR, 1.1);
			scene->draw(*renderer);
		}
		break;
	case '-':
		if (control_module)
		{
			if (axis == ALL_AXES) {
				scene->transformActiveModel(MODEL, SCALE, X_AXIS, scale_down_step_size);
				scene->transformActiveModel(MODEL, SCALE, Y_AXIS, scale_down_step_size);
				scene->transformActiveModel(MODEL, SCALE, Z_AXIS, scale_down_step_size);
			}
			else {
				scene->transformActiveModel(frame, SCALE, axis, scale_down_step_size);
			}
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			//if (axis == ALL_AXES) {
			//	scene->transformActiveLight(MODEL, SCALE, X_AXIS, scale_down_step_size);
			//	scene->transformActiveLight(MODEL, SCALE, Y_AXIS, scale_down_step_size);
			//	scene->transformActiveLight(MODEL, SCALE, Z_AXIS, scale_down_step_size);
			//}
			//else {
			//	scene->transformActiveLight(frame, SCALE, axis, scale_down_step_size);
			//}
			scene->changeActiveLightComponent(rgbComponent, LIGHT_DIFFUSE, 0.9);
			scene->changeActiveLightComponent(rgbComponent, LIGHT_SPECULAR, 0.9);
			scene->draw(*renderer);
		}
		break;
	case 'f':
		scene->focusCameraOnActiveModel();
		scene->draw(*renderer);
		break;
	case 'g':
		scene->toggleXZGrid();
		scene->draw(*renderer);
		break;
	case 'm':
		if (!control_module) {
			control_module = true;
			control_light = false;
		}
		scene->changeActiveModel();
		scene->draw(*renderer);
		break;
	case 'c':
		if (control_module || control_light) {
			control_module = false;
			control_light = false;
		}
		else {
			scene->changeActiveCamera();
			renderer->AdjustToCameraAspectRatio(scene->getActiveCameraAspectRatio());
			renderer->SetEye(scene->getActiveCamera()->eye);
			scene->draw(*renderer);
		}
		break;
	case 'l':
		if (!control_light) {
			control_module = false;
			control_light = true;
		}
		else {
			scene->changeActiveLight();
			scene->draw(*renderer);
		}
		break;
	case 'a':
		axis = ALL_AXES;
		break;
	case 'r':
		if (control_light) {
			scene->resetActiveLightToWhite();
			scene->draw(*renderer);
		}
		break;
	case 'x':
		axis = X_AXIS;
		break;
	case 'y':
		axis = Y_AXIS;
		break;
	case 'z':
		axis = Z_AXIS;
		break;
	case '1':
		rgbComponent = R_RGB;
		break;
	case '2':
		rgbComponent = G_RGB;
		break;
	case '3':
		rgbComponent = B_RGB;
		break;
	case '4':
		rgbComponent = ALL_RGB;
		break;
	}
}

void specialFunc(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		if (control_module)
		{

			scene->transformActiveModel(WORLD, TRANSLATE, X_AXIS, -translate_step_size);
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			scene->transformActiveLight(WORLD, TRANSLATE, X_AXIS, -translate_step_size);
			scene->draw(*renderer);
		}
		else
		{
			scene->translateCameraInRightDirection(-translate_camera_step_size);
			renderer->SetEye(scene->getActiveCamera()->eye);
			scene->draw(*renderer);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (control_module)
		{
			scene->transformActiveModel(WORLD, TRANSLATE, X_AXIS, translate_step_size);
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			scene->transformActiveLight(WORLD, TRANSLATE, X_AXIS, translate_step_size);
			scene->draw(*renderer);
		}
		else
		{
			scene->translateCameraInRightDirection(translate_camera_step_size);
			renderer->SetEye(scene->getActiveCamera()->eye);
			scene->draw(*renderer);
		}
		break;
	case GLUT_KEY_DOWN:
		if (control_module)
		{
			scene->transformActiveModel(WORLD, TRANSLATE, Z_AXIS, translate_step_size);
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			scene->transformActiveLight(WORLD, TRANSLATE, Z_AXIS, translate_step_size);
			scene->draw(*renderer);
		}
		else {
			scene->translateCameraInFocusDirection(-translate_camera_step_size);
			renderer->SetEye(scene->getActiveCamera()->eye);
			scene->draw(*renderer);
		}
		break;
	case GLUT_KEY_UP:
		if (control_module)
		{
			scene->transformActiveModel(WORLD, TRANSLATE, Z_AXIS, -translate_step_size);
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			scene->transformActiveLight(WORLD, TRANSLATE, Z_AXIS, -translate_step_size);
			scene->draw(*renderer);
		}
		else
		{
			scene->translateCameraInFocusDirection(translate_camera_step_size);
			renderer->SetEye(scene->getActiveCamera()->eye);
			scene->draw(*renderer);
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (control_module)
		{
			scene->transformActiveModel(WORLD, TRANSLATE, Y_AXIS, -translate_step_size);
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			scene->transformActiveLight(WORLD, TRANSLATE, Y_AXIS, -translate_step_size);
			scene->draw(*renderer);
		}
		break;
	case GLUT_KEY_PAGE_UP:
		if (control_module)
		{
			scene->transformActiveModel(WORLD, TRANSLATE, Y_AXIS, translate_step_size);
			scene->draw(*renderer);
		}
		else if (control_light)
		{
			scene->transformActiveLight(WORLD, TRANSLATE, Y_AXIS, translate_step_size);
			scene->draw(*renderer);
		}
		break;
	}
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		scene->changeCameraZoom(ZOOM_IN);
	}
	else
	{
		scene->changeCameraZoom(ZOOM_OUT);
	}
	scene->draw(*renderer);
	return;
}

void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}

	//set down flags
	switch (button) {
	case GLUT_LEFT_BUTTON:
		lb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_RIGHT_BUTTON:
		rb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_MIDDLE_BUTTON:
		mb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	}

	// add your code
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx = (x - last_x);
	int dy = (y - last_y);
	// update last x,y
	last_x = x;
	last_y = y;

	bool change = false;
	if (lb_down) { //left mouse button
		if (dx != 0) {
			if (control_module) //module control mode.
			{
				if (abs(dx) < 30) // continuous movement condition
				{
					scene->getActiveModel()->_world_transform = Translate(0.02*translate_step_size*dx*vec4(scene->getActiveCamera()->u)) 
						* scene->getActiveModel()->_world_transform;					
					change = true;
				}
			}
			else if (control_light) //module control mode.
			{
				if (abs(dx) < 30) // continuous movement condition
				{
					/*scene->getActiveLight()->_world_transform = Translate(0.02*translate_step_size*dx*vec4(scene->getActiveCamera()->u))
						* scene->getActiveLight()->_world_transform;*/
					change = true;
				}
			}
			else //camera control
			{
				if (abs(dx) < 40) // continuous movement condition
				{
					scene->changeCameraAngles(0, -0.5 * dx*rotate_step_size);
					change = true;

				}
			}
		}
		if (dy != 0) {
			if (control_module)
			{
				if (abs(dy) < 30) // continuous movement condition
				{
					scene->getActiveModel()->_world_transform = Translate(-0.02*translate_step_size*dy*vec4(scene->getActiveCamera()->v))
						*scene->getActiveModel()->_world_transform;
					change = true;
				}
			}
			else if (control_light)
			{
				if (abs(dy) < 30) // continuous movement condition
				{
					/*scene->getActiveLight()->_world_transform = Translate(-0.02*translate_step_size*dy*vec4(scene->getActiveCamera()->v))
						*scene->getActiveLight()->_world_transform;*/
					change = true;
				}
			}
			else //camera
			{
				if (abs(dy) < 40) //continuous movement condition
				{
					scene->changeCameraAngles(0.5 * dy*rotate_step_size, 0);
					change = true;
				}
			}
		}
	}
	if (mb_down) { //middle mouse button
		if (dx != 0) {
			if (control_module)
			{
				if (abs(dx) < 30) // continuous movement condition
				{
					if (axis == ALL_AXES) 
					{
						scene->transformActiveModel(frame, ROTATE, Y_AXIS, dx * rotate_step_size);
					}
					else 
					{
						scene->transformActiveModel(frame, ROTATE, axis, dx * rotate_step_size);
					}
					change = true;
				}
			}
			else if (control_light)
			{
				if (abs(dx) < 30) // continuous movement condition
				{
					if (axis == ALL_AXES)
					{
						scene->transformActiveLight(frame, ROTATE, Y_AXIS, dx * rotate_step_size);
					}
					else
					{
						scene->transformActiveLight(frame, ROTATE, axis, dx * rotate_step_size);
					}
					change = true;
				}
			}

		}
		if (control_module)
		{
			if (abs(dy) < 30 && axis == ALL_AXES) // continuous movement condition
				{
				scene->transformActiveModel(frame, ROTATE, X_AXIS, -dy * rotate_step_size);
				change = true;
				}
		}
		else if (control_light)
		{
			if (abs(dy) < 30 && axis == ALL_AXES) // continuous movement condition
			{
				scene->transformActiveLight(frame, ROTATE, X_AXIS, -dy * rotate_step_size);
				change = true;
			}
		}

	}

	if (change) {
		scene->draw(*renderer);
	}
}

void fileMenu(int id)
{
	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
	switch (id)
	{
	case FILE_OPEN:
		if (dlg.DoModal() == IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
			scene->draw(*renderer);
		}
		break;
	case CONTROL_MODULE:
		control_module = true;
		control_light = false;
		break;
	case WORLD_FRAME:
		frame = WORLD;
		break;
	case MODEL_FRAME:
		frame = MODEL;
		break;
	case CHANGE_MODULE:
		scene->changeActiveModel();
		break;
	case REMOVE_MODULE:
		scene->removeModel();
		break;
	case CHANGE_UNIFORM_MATERIAL:
		CMaterialDialog mDlg;
		const Material& activeModelMaterial = scene->getActiveModelMaterial();
		mDlg.SetMaterial(activeModelMaterial);
		if (mDlg.DoModal() == IDOK) {
			Material newMaterial = mDlg.GetMaterial();
			scene->setActiveModelMaterial(newMaterial);
			scene->draw(*renderer);
		}
		break;
	}
}

void mainMenu(int id)
{
	switch (id)
	{
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics HW1 \n Ben Levy \n Itay Zuker \n Intuitive control:\n Mouse+Arrow keys+ +/-"));
		break;
	}
}

void displayMenu(int id)
{
	switch (id)
	{
	case MAIN_FNORMALS:
		renderer->draw_triangle_normals = !renderer->draw_triangle_normals;
		break;
	case MAIN_VNORMALS:
		renderer->draw_vertex_normals = !renderer->draw_vertex_normals;
		break;
	case MAIN_BOUNDING_BOX:
		renderer->draw_bounding_box = !renderer->draw_bounding_box;
		break;
	case MAIN_CAMERAS:
		scene->showOrHideCameras();
	}
	scene->draw(*renderer);
}

void addMenu(int id)
{
	PrimMeshModel* PRIMITIVE = new PrimMeshModel();
	switch (id)
	{
	case PRIMITIVE_PYRAMID:
		scene->addModel(PRIMITIVE); //add to scene
		scene->draw(*renderer);
		break;
	case WORLD_GRID:
		scene->toggleXZGrid();
		scene->draw(*renderer);
		break;
	}




}

inline vector<GLfloat> split_str(string& input,string &delimiter)
{
	vector<GLfloat> output;
	for (string::size_type p0 = 0, p1 = input.find(',');
		p1 != string::npos || p0 != string::npos; //null indice
		(p0 = (p1 == string::npos) ? p1 : ++p1), p1 = input.find(delimiter, p0))
	{
		output.push_back(strtod(input.c_str() + p0, NULL));
	}
			
	return output;
}

void cameraMenu(int id)
{
	Camera* c;
	CCmdDialog  dlg;
	
	switch (id)
	{
	case ADD_CAMERA:
		c = new Camera();
		scene->addCamera(c); //add to scene
		renderer->SetEye(scene->getActiveCamera()->eye);
		scene->draw(*renderer);
		break;
	case REMOVE_CAMERA:
		scene->removeCamera();
		renderer->SetEye(scene->getActiveCamera()->eye);
		scene->draw(*renderer);
		break;
	case CONTROL_CAMERA:
		control_module = false;
		control_light = false;
		break;
	case CHANGE_CAMERA:
		scene->changeActiveCamera();
		renderer->AdjustToCameraAspectRatio(scene->getActiveCameraAspectRatio());
		renderer->SetEye(scene->getActiveCamera()->eye);
		scene->draw(*renderer);
		break;
	case ORTHO:
		dlg.ctext = "Ortho view Input= left,right,bottom,top,zNear,zFar";
		if (dlg.DoModal() == IDOK) {
			string command = dlg.GetCmd();
			string delimiter = ",";
			vector<GLfloat> o=split_str(command, delimiter);
			scene->changeCameraZoom(ZOOM_RESET);
			scene->getActiveCamera()->Ortho(o[0], o[1], o[2], o[3], o[4], o[5]); //update camera with command.
			renderer->AdjustToCameraAspectRatio(scene->getActiveCameraAspectRatio());
		}
		scene->draw(*renderer);
		break;
	case FRUSTRUM:
		dlg.ctext = "Frustum view Input= left,right,bottom,top,zNear,zFar";
		if (dlg.DoModal() == IDOK) {
			string command = dlg.GetCmd();
			string delimiter = ",";
			vector<GLfloat> o = split_str(command, delimiter);
			scene->changeCameraZoom(ZOOM_RESET);
			scene->getActiveCamera()->Frustum(o[0], o[1], o[2], o[3], o[4], o[5]); //update camera with command.
			renderer->AdjustToCameraAspectRatio(scene->getActiveCameraAspectRatio());
		}
		scene->draw(*renderer);
		break;
	case PERSPECTIVE:
		dlg.ctext = "Perspective view Input= fovy[dd],aspect[w/h],zNear,zFar";
		if (dlg.DoModal() == IDOK) {
			string command = dlg.GetCmd();
			string delimiter = ",";
			vector<GLfloat> o = split_str(command, delimiter);
			scene->changeCameraZoom(ZOOM_RESET);
			scene->getActiveCamera()->Perspective(o[0], o[1], o[2], o[3]); //update camera with command.
			renderer->AdjustToCameraAspectRatio(scene->getActiveCameraAspectRatio());
		}
		scene->draw(*renderer);
		break;
	}
}

void parametersMenu(int id)
{
	CCmdDialog  dlg;
	switch (id)
	{
	case STEP_SCALE:
		dlg.ctext = "Set transformation step size. default 1";
		if (dlg.DoModal() == IDOK) {
			string command = dlg.GetCmd();
			string delimiter = ",";
			vector<GLfloat> o = split_str(command, delimiter);
			step_scale = o[0];
			rotate_step_size = 1 * step_scale;
			translate_step_size = 1 * step_scale;
			scale_up_step_size = 1.1*step_scale;
			scale_down_step_size = 1 / scale_up_step_size;
		}
		break;

	}

}

void lightMenu(int id)
{
	Light* l;
	switch (id)
	{
	case (ADD_LIGHT) :
		l = new Light();
		scene->addLightSource(l); //add to scene
		scene->draw(*renderer);
		break;
	case (REMOVE_LIGHT) :
		scene->removeLight();
		scene->draw(*renderer);
		break;
	case (CHANGE_LIGHT) :
		scene->changeActiveLight();
		scene->draw(*renderer);
		break;
	case (CONTROL_LIGHT) :
		control_module = false;
		control_light = true;
		break;
	}

}

void lightColor(int id)
{
	/*switch (id)
	{
	case (LIGHT_WHITE) :
		scene->getActiveLight()->setColor(WHITE);
		scene->draw(*renderer);
		break;
	case (LIGHT_BLUE) :
		scene->getActiveLight()->setColor(BLUE);
		scene->draw(*renderer);
		break;
	case (LIGHT_YELLOW) :
		scene->getActiveLight()->setColor(YELLOW);
		scene->draw(*renderer);
		break;
	}*/
}

void lightType(int id)
{
	switch (id)
	{
	case (PARALLEL_SOURCE) :
		scene->getActiveLight()->ChangeToParallelSource();
		scene->updateRendererLightSources();
		scene->draw(*renderer);
		break;
	case (POINT_SOURCE) :
		scene->getActiveLight()->ChangeToPointSource();
		scene->updateRendererLightSources();
		scene->draw(*renderer);
		break;
	}

}

void effects(int id)
{
	switch (id)
	{
	case (FOG) :
		renderer->draw_fog = !renderer->draw_fog;
		break;
	case (AA) :
		renderer->ToggleAntiAliasing();
		break;
	}
	scene->draw(*renderer);
}

void shadingType(int id)
{
	renderer->SetShadingType((SHADING_TYPES)id);
	scene->draw(*renderer);
}

void initMenu()
{
	int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open module", FILE_OPEN);
	glutAddMenuEntry("Control active module (m)", CONTROL_MODULE);
	glutAddMenuEntry("Change active module (m)", CHANGE_MODULE);
	glutAddMenuEntry("Remove active module", REMOVE_MODULE);
	glutAddMenuEntry("Transform in model frame", MODEL_FRAME);
	glutAddMenuEntry("Transform in world frame", WORLD_FRAME);
	glutAddMenuEntry("Change uniform material", CHANGE_UNIFORM_MATERIAL);
	int mMenu = glutCreateMenu(mainMenu);
	glutAddSubMenu("Module", menuFile);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	int dispMenu = glutCreateMenu(displayMenu);
	glutSetMenu(mMenu);
	glutAddSubMenu("Display", dispMenu);
	glutSetMenu(dispMenu);
	glutAddMenuEntry("Face Normals", MAIN_FNORMALS);
	glutAddMenuEntry("Vertex Normals", MAIN_VNORMALS);
	glutAddMenuEntry("Bounding Box", MAIN_BOUNDING_BOX);
	glutAddMenuEntry("Show or Hide Cameras", MAIN_CAMERAS);
	int aMenu = glutCreateMenu(addMenu);
	glutSetMenu(mMenu);
	glutAddSubMenu("Primitives", aMenu);
	glutSetMenu(aMenu);
	glutAddMenuEntry("Pyramid", PRIMITIVE_PYRAMID);
	glutAddMenuEntry("World Grid (g)",WORLD_GRID );
	int cMenu = glutCreateMenu(cameraMenu);
	glutSetMenu(mMenu);
	glutAddSubMenu("Cameras", cMenu);
	glutSetMenu(cMenu);
	glutAddMenuEntry("Add", ADD_CAMERA);
	glutAddMenuEntry("Remove", REMOVE_CAMERA);
	glutAddMenuEntry("Control Camera (c)", CONTROL_CAMERA);
	glutAddMenuEntry("Change Camera (c)", CHANGE_CAMERA);
	glutAddMenuEntry("Ortho", ORTHO);
	glutAddMenuEntry("Frustum", FRUSTRUM);
	glutAddMenuEntry("Perspective", PERSPECTIVE);
	int pMenu = glutCreateMenu(parametersMenu);
	glutSetMenu(mMenu);
	glutAddSubMenu("Parameters", pMenu);
	glutAddMenuEntry("About", MAIN_ABOUT);
	glutSetMenu(pMenu);
	glutAddMenuEntry("Transformations Step scale", STEP_SCALE);
	int lMenu = glutCreateMenu(lightMenu);
	glutSetMenu(mMenu);
	glutAddSubMenu("Light", lMenu);
	glutSetMenu(lMenu);
	glutAddMenuEntry("Add light source",ADD_LIGHT);
	glutAddMenuEntry("Remove light source", REMOVE_LIGHT);
	glutAddMenuEntry("Control light source (l)", CONTROL_LIGHT);
	glutAddMenuEntry("Change light source (l)", CHANGE_LIGHT);
	int ltMenu = glutCreateMenu(lightType);
	glutSetMenu(lMenu);
	glutAddSubMenu("Light source type", ltMenu);
	glutSetMenu(ltMenu);
	glutAddMenuEntry("Point source light", POINT_SOURCE);
	glutAddMenuEntry("Parallel source light", PARALLEL_SOURCE);
	int lcMenu = glutCreateMenu(lightColor);
	glutSetMenu(lMenu);
	glutAddSubMenu("Light source color", lcMenu);
	glutSetMenu(lcMenu);
	glutAddMenuEntry("WHITE", LIGHT_WHITE);
	glutAddMenuEntry("BLUE", LIGHT_BLUE);
	glutAddMenuEntry("YELLOW", LIGHT_YELLOW);
	int eMenu = glutCreateMenu(effects);
	glutSetMenu(mMenu);
	glutAddSubMenu("Special effects", eMenu);
	glutSetMenu(eMenu);
	glutAddMenuEntry("FOG ON/OFF", FOG);
	glutAddMenuEntry("Supersample antialiasing", AA);
	int sMenu = glutCreateMenu(shadingType);
	glutSetMenu(mMenu);
	glutAddSubMenu("Shading Type", sMenu);
	glutSetMenu(sMenu);
	glutAddMenuEntry("Flat Shading", FLAT_SHADING);
	glutAddMenuEntry("Gouraud Shading", GOURAUD_SHADING);
	glutAddMenuEntry("Phong Shading", PHONG_SHADING);
}
//----------------------------------------------------------------------------



int my_main(int argc, char **argv)
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Hw1- CG");
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


	rotate_step_size = 1;
	translate_step_size = 1;
	scale_up_step_size = 1.1;
	scale_down_step_size = 1 / 1.1;
	renderer = new Renderer(512, 512);
	scene = new Scene(renderer);
	scene->toggleXZGrid();
	renderer->AdjustToCameraAspectRatio(scene->getActiveCameraAspectRatio());
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(specialFunc);
	initMenu();

	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int main(int argc, char **argv)
{
	int nRetCode = 0;
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		my_main(argc, argv);
	}

	return nRetCode;
}
