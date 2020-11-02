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
    cwc::glShader* shader;
    GLuint ProgramObject;
    clock_t time0, time1;
    float timer010;  // timer counting 0->1->0
    bool bUp;        // flag if counting up or down.
    Objeto3D* mallas[CANT_MESHES];   //Arreglo de las mallas
    float posCarX, posCarY, posCarZ; //Variables para mover el carro en el escenario
    bool movIzqX, movDerX, movArribaY, movAbajoY, movAtrasZ, movDelanteZ; 

public:
    myWindow() {}
    
    //Funcion para cambiar los valores de posicion del carro en sus respectivos ejes
    void moverCarro() {
        if (movIzqX) {
            posCarX -= 0.1;
        }
        if (movDerX) {
            posCarX += 0.1;
        }
        if (movAbajoY) {
            posCarY -= 0.1;
        }
        if (movArribaY) {
            posCarY += 0.1;
        }
        if (movAtrasZ) {
            posCarZ -= 0.1;
        }
        if (movDelanteZ) {
            posCarZ += 0.1;
        }
    }

    virtual void OnRender(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //timer010 = 0.09; //for screenshot!
        moverCarro();

        glPushMatrix();
        if (shader) shader->begin();

        //Rotacion de la camara para visualizar de buena forma el escenario
        glRotatef(45, 0.5, 0, 0);

        //Dibujando cada malla y realizandole los cambios a cada una
        //house
        glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        mallas[3]->drawMeshe(-4.5, 3, 0.0);
        glPopMatrix();

        //man
        glPushMatrix();
        glScalef(0.3, 0.3, 0.3);
        mallas[4]->drawMeshe(-7.0, 6.5, 0.0);
        glPopMatrix();

        //mountain
        glPushMatrix();
        glScalef(3, 10, 3);
        mallas[1]->drawMeshe(-1, 0.13, 0.0);
        glPopMatrix();

        //road
        glPushMatrix();
        glScalef(1, 1, 4);
        mallas[0]->drawMeshe(0.0, 0.0, 0.0);
        glPopMatrix();

        //mio
        glPushMatrix();
        glScalef(1, 1, 1);
        mallas[2]->drawMeshe(1, 1, 0.0);
        glPopMatrix();

        //car
        glPushMatrix();
        glScalef(0.3, 0.3, 0.3);
        mallas[5]->drawMeshe(posCarX, posCarY, posCarZ); //Aplicando el movimiento al carro
        glPopMatrix();


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

        //Inicializar las variables para mover el carro
        posCarX = -8;
        posCarY = 6;
        posCarZ = 0;
        movIzqX = false;
        movDerX = false;
        movArribaY = false;
        movAbajoY = false;
        movAtrasZ = false;
        movDelanteZ = false;

        //Abrir las mallas y almacenarlas en un arreglo
        for (int i = 0; i < CANT_MESHES; i++) {
            switch (i) {
            case 0:
                mallas[i] = new Objeto3D("./modelos/Terreno.obj");
                break;
            case 1:
                mallas[i] = new Objeto3D("./modelos/MtFuji.obj");
                break;
            case 2:
                mallas[i] = new Objeto3D("./modelos/mioStation.obj");
                break;
            case 3:
                mallas[i] = new Objeto3D("./modelos/casa.obj");
                break;
            case 4:
                mallas[i] = new Objeto3D("./modelos/man.obj");
                break;
            case 5:
                mallas[i] = new Objeto3D("./modelos/CyberpunkDeLorean.obj");
                break;
            }
        }

        shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); // load (and compile, link) from file
        if (shader == 0)
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
        if (h == 0) h = 1;
        float ratio = 1.0f * (float)w / (float)h;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glViewport(0, 0, w, h);

        gluPerspective(100, ratio, 1, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0f, 0.0f, 4.0f,
            0.0, 0.0, -1.0,
            0.0f, 1.0f, 0.0f);
    }
    virtual void OnClose(void) {}
    virtual void OnMouseDown(int button, int x, int y) {}
    virtual void OnMouseUp(int button, int x, int y) {}
    virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y) {}

    virtual void OnKeyDown(int nKey, char cAscii)
    {
        if (cAscii == 27) // 0x1b = ESC
        {
            this->Close(); // Close Window!
        }
        //Saber si se presiono la tecla para mover el carro. W y S minusculas mueven el carro en el eje Y de OpenGL, mientras que
        //W y S en mayusculas lo mueven en el eje Z de OpenGL, A y D minusculas son para moverlo en el eje X de OpenGL.
        else if (cAscii == 'w') {
            movArribaY = true;
        }
        else if (cAscii == 's') {
            movAbajoY = true;
        }
        else if (cAscii == 'a') {
            movIzqX = true;
        }
        else if (cAscii == 'd') {
            movDerX = true;
        }
        else if (cAscii == 'W') {
            movDelanteZ = true;
        }
        else if (cAscii == 'S') {
            movAtrasZ = true;
        }
    };

    virtual void OnKeyUp(int nKey, char cAscii)
    {
        if (cAscii == 'z')      // s: Shader
            shader->enable();
        else if (cAscii == 'f') // f: Fixed Function
            shader->disable();
        //Saber si se dejo de presionar la tecla para mover el carro. W y S minusculas mueven el carro en el eje Y de OpenGL, mientras que
        //W y S en mayusculas lo mueven en el eje Z de OpenGL, A y D minusculas son para moverlo en el eje X de OpenGL.
        else if (cAscii == 'w') {
            movArribaY = false;
        }
        else if (cAscii == 's') {
            movAbajoY = false;
        }
        else if (cAscii == 'a') {
            movIzqX = false;
        }
        else if (cAscii == 'd') {
            movDerX = false;
        }
        else if (cAscii == 'W') {
            movDelanteZ = false;
        }
        else if (cAscii == 'S') {
            movAtrasZ = false;
        }
    }

    void UpdateTimer()
    {
        time1 = clock();
        float delta = static_cast<float>(static_cast<double>(time1 - time0) / static_cast<double>(CLOCKS_PER_SEC));
        delta = delta / 4;
        if (delta > 0.00005f)
        {
            time0 = clock();
            if (bUp)
            {
                timer010 += delta;
                if (timer010 >= 1.0f) { timer010 = 1.0f; bUp = false; }
            }
            else
            {
                timer010 -= delta;
                if (timer010 <= 0.0f) { timer010 = 0.0f; bUp = true; }
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

        GLfloat lmKa[] = { 0.0, 0.0, 0.0, 0.0 };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

        glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

        // -------------------------------------------
        // Spotlight Attenuation

        GLfloat spot_direction[] = { 1.0, -1.0, -1.0 };
        GLint spot_exponent = 30;
        GLint spot_cutoff = 180;

        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
        glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
        glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

        GLfloat Kc = 1.0;
        GLfloat Kl = 0.0;
        GLfloat Kq = 0.0;

        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


        // ------------------------------------------- 
        // Lighting parameters:

        GLfloat light_pos[] = { 0.0f, 5.0f, 5.0f, 1.0f };
        GLfloat light_Ka[] = { 1.0f, 0.5f, 0.5f, 1.0f };
        GLfloat light_Kd[] = { 1.0f, 0.1f, 0.1f, 1.0f };
        GLfloat light_Ks[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

        // -------------------------------------------
        // Material parameters:

        GLfloat material_Ka[] = { 0.5f, 0.0f, 0.0f, 1.0f };
        GLfloat material_Kd[] = { 0.4f, 0.4f, 0.5f, 1.0f };
        GLfloat material_Ks[] = { 0.8f, 0.8f, 0.0f, 1.0f };
        GLfloat material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
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
    virtual void OnInit() { std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
    myApplication* pApp = new myApplication;
    myWindow* myWin = new myWindow();

    pApp->run();
    delete pApp;
    return 0;
}

//-----------------------------------------------------------------------------

