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
enum LIGHT_SOURCE_TYPE {
	POINT_LIGHT, PARALLEL_LIGHT
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