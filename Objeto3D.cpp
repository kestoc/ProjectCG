#include "Objeto3D.h"

Objeto3D::Objeto3D(char* meshe) {
    objmodel_ptr = NULL;
    if (!objmodel_ptr)
    {
        objmodel_ptr = glmReadOBJ(meshe);
        if (!objmodel_ptr)
            exit(0);

        glmUnitize(objmodel_ptr);
        glmFacetNormals(objmodel_ptr);
        glmVertexNormals(objmodel_ptr, 90.0);
    }
}

void Objeto3D::drawMeshe(float _x, float _y, float _z) {
    x = _x, y = _y, z = _z;

    glPushMatrix();
    glTranslatef(x, y, z);
    glmDraw(objmodel_ptr, GLM_SMOOTH);
    glPopMatrix();
}