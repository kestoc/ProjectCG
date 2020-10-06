#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLM/glm.h"

class Objeto3D
{
	public:
		GLMmodel* objmodel_ptr;
		Objeto3D();
		virtual void openMeshe();
		virtual void drawMeshe();
};

