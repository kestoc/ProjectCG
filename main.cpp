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
#include <FreeImage.h>
#define CANT_MESHES 6

//-----------------------------------------------------------------------------


class myWindow : public cwc::glutWindow
{
protected:
    cwc::glShaderManager SM;
    cwc::glShader* shader, * shader1;
    GLuint ProgramObject;
    clock_t time0, time1;
    float timer010;  // timer counting 0->1->0
    bool bUp;        // flag if counting up or down.
    Objeto3D* mallas[CANT_MESHES];   //Arreglo de las mallas

    float posCarX, posCarY, posCarZ; //Variables para mover el carro en el escenario
    bool movIzqX, movDerX, movArribaY, movAbajoY, movAtrasZ, movDelanteZ;

    float posCamX, posCamY, posCamZ; // Variables para mover la camara en el escenario
    bool movIzqCX, movDerCX, movArribaCY, movAbajoCY, movAtrasCZ, movDelanteCZ;

    GLMmodel* objmodel_ptr1; //*** Para Textura: variable para objeto texturizado
    GLuint texid;

public:
    myWindow() {}

    void initialize_textures(void)

    {

        int w, h;

        GLubyte* data = 0;

        //data = glmReadPPM("soccer_ball_diffuse.ppm", &w, &h);

        //std::cout << "Read soccer_ball_diffuse.ppm, width = " << w << ", height = " << h << std::endl;



        //dib1 = loadImage("soccer_ball_diffuse.jpg"); //FreeImage



        glGenTextures(1, &texid);

        glBindTexture(GL_TEXTURE_2D, texid);

        glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



        // Loading JPG file

        FIBITMAP* bitmap = FreeImage_Load(

            FreeImage_GetFileType("./modelos/DeLorean.jpg", 0),

            "./modelos/DeLorean.jpg");  //*** Para Textura: esta es la ruta en donde se encuentra la textura



        FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);

        int nWidth = FreeImage_GetWidth(pImage);

        int nHeight = FreeImage_GetHeight(pImage);



        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,

            0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));



        FreeImage_Unload(pImage);

        //

        glEnable(GL_TEXTURE_2D);

    }

    //Funcion para cambiar los valores de posicion de la camara
    void moverCamara() {
        if (movIzqCX) {
            posCamX += 0.1;
        }
        if (movDerCX) {
            posCamX -= 0.1;
        }
        if (movAbajoCY) {
            posCamY += 0.1;
        }
        if (movArribaCY) {
            posCamY -= 0.1;
        }
        if (movAtrasCZ) {
            posCamZ -= 0.1;
        }
        if (movDelanteCZ) {
            posCamZ += 0.1;
        }

        glTranslatef(posCamX, posCamY, posCamZ);
    }

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

        glPushMatrix();

        moverCarro();
        moverCamara();

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



        if (shader) shader->end();

        //--------------------------------------SHADER 1-------------------------------------------
        if (shader1) shader1->begin();

        //Rotacion de la camara para visualizar de buena forma el escenario
       // glRotatef(45, 0.5, 0, 0);

        //Dibujando cada malla y realizandole los cambios a cada una
        //house


        //car
        glPushMatrix();
        glScalef(0.3, 0.3, 0.3);
        glBindTexture(GL_TEXTURE_2D, texid);
        mallas[5]->drawMeshe(posCarX, posCarY, posCarZ); //Aplicando el movimiento al carro
        glPopMatrix();


        if (shader1) shader1->end();

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

        //Inicializar las variables para mover la camara
        posCamX = 0;
        posCamY = 0;
        posCamZ = 0;
        movIzqCX = false;
        movDerCX = false;
        movArribaCY = false;
        movAbajoCY = false;
        movAtrasCZ = false;
        movDelanteCZ = false;

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
        shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt"); // load (and compile, link) from file
        if (shader1 == 0)
            std::cout << "Error Loading,nlnl compiling or linking shader\n";
        else
        {
            ProgramObject = shader1->GetProgramObject();
        }

        time0 = clock();
        timer010 = 0.0f;
        bUp = true;

        objmodel_ptr1 = NULL;

        if (!objmodel_ptr1)

        {

            objmodel_ptr1 = glmReadOBJ("./modelos/CyberpunkDeLorean.obj");

            if (!objmodel_ptr1)

                exit(0);



            glmUnitize(objmodel_ptr1);

            glmFacetNormals(objmodel_ptr1);

            glmVertexNormals(objmodel_ptr1, 90.0);

        }
        initialize_textures();
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

        //Camara. I y K minusculas mueven el carro en el eje Y de OpenGL, mientras que
        //I y K en mayusculas lo mueven en el eje Z de OpenGL, J y L minusculas son para moverlo en el eje X de OpenGL.
        else if (cAscii == 'i') {
            movArribaCY = true;
        }
        else if (cAscii == 'k') {
            movAbajoCY = true;
        }
        else if (cAscii == 'j') {
            movIzqCX = true;
        }
        else if (cAscii == 'l') {
            movDerCX = true;
        }
        else if (cAscii == 'I') {
            movDelanteCZ = true;
        }
        else if (cAscii == 'K') {
            movAtrasCZ = true;
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

        //Camara. I y K minusculas mueven el carro en el eje Y de OpenGL, mientras que
        //I y K en mayusculas lo mueven en el eje Z de OpenGL, J y L minusculas son para moverlo en el eje X de OpenGL.
        else if (cAscii == 'i') {
            movArribaCY = false;
        }
        else if (cAscii == 'k') {
            movAbajoCY = false;
        }
        else if (cAscii == 'j') {
            movIzqCX = false;
        }
        else if (cAscii == 'l') {
            movDerCX = false;
        }
        else if (cAscii == 'I') {
            movDelanteCZ = false;
        }
        else if (cAscii == 'K') {
            movAtrasCZ = false;
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

        /*glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);*/

        // -------------------------------------------
        // Material parameters:

        GLfloat material_Ka[] = { 0.5f, 0.0f, 0.0f, 1.0f };
        GLfloat material_Kd[] = { 0.4f, 0.4f, 0.5f, 1.0f };
        GLfloat material_Ks[] = { 0.8f, 0.8f, 0.0f, 1.0f };
        GLfloat material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
        GLfloat material_Se = 20.0f;

        /*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);*/
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

