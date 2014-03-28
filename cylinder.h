#ifndef _CYLINDER_
#define _CYLINDER_

#include "renderable.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#ifndef M_PI
#define M_PI 3.14159265
#endif
//Define the NO_POINTS
const int NO_POINTS = 100;

class Cylinder : public Renderable {
public:
    Cylinder(float height, float radius);
    void draw(); 

private:
    float height;
    float radius;
    void drawImmediate();
    void drawElements();
    void drawArrays();
    void drawCircle(float z);
    GLfloat * normalize(GLfloat vec[3]);
    GLfloat * cross(GLfloat a[3], GLfloat b[3]);
    GLfloat * normal(GLfloat av[3], GLfloat bv[3], GLfloat cv[3]);

};

#endif

