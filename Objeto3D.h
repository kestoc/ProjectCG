#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.h"
#include <iostream>

class Objeto3D
{
	public:
		GLMmodel* objmodel_ptr;
		float x, y, z;
		void openMeshe(char*);
		void drawMeshe(float, float, float);
};

