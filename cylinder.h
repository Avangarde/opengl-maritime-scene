#ifndef _CYLINDER_
#define _CYLINDER_

#include "renderable.h"
#include "viewer.h"
#include <QGLViewer/vec.h>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
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
    void init();
    void draw();
    float getAngleXY() {
        return angleXY;
    }

    void setAngleXY(float angleXY) {
        Cylinder::angleXY = angleXY;
    }

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

private:
    float height;
    float radius;
    float angleXY;
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

