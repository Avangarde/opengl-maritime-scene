#ifndef _CYLINDER_
#define _CYLINDER_



#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "renderable.h"
#include "viewer.h"
#include <QGLViewer/vec.h>
#ifndef M_PI
#define M_PI 3.14159265
#endif
//Define the NO_POINTS
const int NO_POINTS = 16;

using namespace qglviewer;

class Cylinder : public Renderable {
public:
    Cylinder(float height, float radius);
    Cylinder(Vec pos, float height, float radius);
    void init(Viewer&);
    void draw();


    Vec getPosition() const {
        return position;
    }

    void setPosition(Vec position) {
        this->position = position;
    }

    float getHeight() const {
        return height;
    }

    void setHeight(float height) {
        this->height = height;
    }
    
    void setAngleRotation(float angle) {
        this->angleRotation = angle;
    }
    
    void setVectorRotation(Vec rot) {
        this->vectorRotation = rot;
    }

private:
    float height;
    float radius;
    float angleRotation;
    Vec vectorRotation;
    Vec position;

    void drawImmediate();
    void drawElements();
    void drawArrays();
    void drawCircle(float z);
    GLfloat * normalize(GLfloat vec[3]);
    GLfloat * cross(GLfloat a[3], GLfloat b[3]);
    GLfloat * normal(GLfloat av[3], GLfloat bv[3], GLfloat cv[3]);

};

#endif

