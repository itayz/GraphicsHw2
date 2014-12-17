#pragma once

#include "resource.h"

enum COLORS {
	WHITE, BLACK, RED, BLUE, GREEN, YELLOW, MAGNETA, CYAN
};
enum AXES {
	X_AXIS, Y_AXIS, Z_AXIS, ALL_AXES
};
enum ACTIONS {
	ROTATE, TRANSLATE, SCALE
};
enum FRAMES {
	MODEL, WORLD, VIEW
};
enum SHADING_TYPES {
	FLAT_SHADING, GOURAUD_SHADING, PHONG_SHADING
};
enum RGB {
	R_RGB, G_RGB, B_RGB, ALL_RGB
};
enum LIGHT_COMPONENT {
	LIGHT_AMBIENT, LIGHT_DIFFUSE, LIGHT_SPECULAR, LIGHT_ALL_COMPONENTS
};
enum CONTROL_TYPES {
	CAMERA_CONTROL, MODEL_CONTROL, LIGHT_CONTROL
};

void specialFunc(int key, int x, int y);
void mouseWheel(int button, int dir, int x, int y);

void display( void );
void reshape( int width, int height );
void keyboard( unsigned char key, int x, int y );
void mouseWheel(int button, int dir, int x, int y);
void mouse(int button, int state, int x, int y);
void fileMenu(int id);
void mainMenu(int id);
void initMenu();