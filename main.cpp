//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm/glm.h"
#include "Objeto3D.h"
#define CANT_MESHES 6

//-----------------------------------------------------------------------------


class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   GLuint ProgramObject;
   clock_t time0,time1;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.
   /*Objeto3D mallas[CANT_MESHES];*/
   GLMmodel* objmodel_casa;
   GLMmodel* objmodel_man;
   GLMmodel* objmodel_Terreno;
   GLMmodel* objmodel_mioStation;
   GLMmodel* objmodel_MtFuji;
   GLMmodel* objmodel_CyberpunkDeLorean;

public:
	myWindow(){}

	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      //timer010 = 0.09; //for screenshot!
      glPushMatrix();
      if (shader) shader->begin();
         //glRotatef(timer010*360, 0.5, 1.0f, 0.1f);
         //glutSolidTeapot(1.0);
         glPushMatrix();
         glScalef(3, 3, 3);
         glmDraw(objmodel_casa, GLM_SMOOTH);
         glPopMatrix();

         glPushMatrix();
         glScalef(3, 3, 3);
         glmDraw(objmodel_man, GLM_SMOOTH);
         glPopMatrix();

         glPushMatrix();
         glScalef(10, 0, 10);
         glmDraw(objmodel_Terreno, GLM_SMOOTH);
         glPopMatrix();

         glPushMatrix();
         glScalef(3, 3, 3);
         glmDraw(objmodel_mioStation, GLM_SMOOTH);
         glPopMatrix();

         glPushMatrix();
         glScalef(3, 3, 3);
         glmDraw(objmodel_MtFuji, GLM_SMOOTH);
         glPopMatrix();

         glPushMatrix();
         glScalef(1, 1, 1);
         glmDraw(objmodel_CyberpunkDeLorean, GLM_SMOOTH);
         glPopMatrix();
         /*mallas[0].drawMeshe(0.0, 0.0, 0.0);*/

      if (shader) shader->end();
      glutSwapBuffers();
      glPopMatrix();

      UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

        objmodel_casa = NULL;
        if (!objmodel_casa)
        {
            objmodel_casa = glmReadOBJ("./modelos/casa.obj");
            if (!objmodel_casa)
                exit(0);

            glmUnitize(objmodel_casa);
            glmFacetNormals(objmodel_casa);
            glmVertexNormals(objmodel_casa, 90.0);
        }
        
        objmodel_man = NULL;
        if (!objmodel_man)
        {
            objmodel_man = glmReadOBJ("./modelos/man.obj");
            if (!objmodel_man)
                exit(0);

            glmUnitize(objmodel_man);
            glmFacetNormals(objmodel_man);
            glmVertexNormals(objmodel_man, 90.0);
        }
        objmodel_Terreno = NULL;
        if (!objmodel_Terreno)
        {
            objmodel_Terreno = glmReadOBJ("./modelos/Terreno.obj");
            if (!objmodel_Terreno)
                exit(0);

            glmUnitize(objmodel_Terreno);
            glmFacetNormals(objmodel_Terreno);
            glmVertexNormals(objmodel_Terreno, 90.0);
        }
        objmodel_MtFuji = NULL;
        if (!objmodel_MtFuji)
        {
            objmodel_MtFuji = glmReadOBJ("./modelos/MtFuji.obj");
            if (!objmodel_MtFuji)
                exit(0);

            glmUnitize(objmodel_MtFuji);
            glmFacetNormals(objmodel_MtFuji);
            glmVertexNormals(objmodel_MtFuji, 90.0);
        }
        objmodel_mioStation = NULL;
        if (!objmodel_mioStation)
        {
            objmodel_mioStation = glmReadOBJ("./modelos/mioStation.obj");
            if (!objmodel_mioStation)
                exit(0);

            glmUnitize(objmodel_mioStation);
            glmFacetNormals(objmodel_mioStation);
            glmVertexNormals(objmodel_mioStation, 90.0);
        }
        objmodel_CyberpunkDeLorean = NULL;
        if (!objmodel_CyberpunkDeLorean)
        {
            objmodel_CyberpunkDeLorean = glmReadOBJ("./modelos/CyberpunkDeLorean.obj");
            if (!objmodel_CyberpunkDeLorean)
                exit(0);

            glmUnitize(objmodel_CyberpunkDeLorean);
            glmFacetNormals(objmodel_CyberpunkDeLorean);
            glmVertexNormals(objmodel_CyberpunkDeLorean, 90.0);
        }

        /*for (int i = 0; i < CANT_MESHES; i++) {
            switch (i) {
                case 0:
                    mallas[i].openMeshe("./meshes/Terreno.obj");
                    break;
                case 1:
                    mallas[i].openMeshe("./meshes/MtFuji.obj");
                    break;
                case 2:
                    mallas[i].openMeshe("./meshes/mioStation.obj");
                    break;
                case 3:
                    mallas[i].openMeshe("./meshes/casa.obj");
                    break;
                case 4:
                    mallas[i].openMeshe("./meshes/man.obj");
                    break;
                case 5:
                    mallas[i].openMeshe("./meshes/CyberpunkDeLorean.obj");
                    break;
            }
        }*/

		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		if (shader==0) 
         std::cout << "Error Loading, compiling or linking shader\n";
      else
      {
         ProgramObject = shader->GetProgramObject();
      }

      time0 = clock();
      timer010 = 0.0f;
      bUp = true;

      DemoLight();

	}

	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(100,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
      if (cAscii == 's')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
	}

   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
   }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

