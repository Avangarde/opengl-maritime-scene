#include "cylinder.h"

using namespace std;
using namespace qglviewer;

GLfloat cylinderPoints [NO_POINTS][4][3];

Cylinder::Cylinder(float height, float radius) :
height(height / 2),
position(Vec(0.0, 0.0, 0.0)),
radius(radius) {
};

Cylinder::Cylinder(Vec pos, float height, float radius) :
height(height / 2),
position(pos),
radius(radius) {
}

void Cylinder::init(Viewer& viewer) {
    float t, s;
    for (int i = 0; i <= NO_POINTS; ++i) {
        t = 2 * M_PI * (float) (i) / (float) NO_POINTS;
        s = 2 * M_PI * (float) (i + 1) / (float) NO_POINTS;

        // First corner
        cylinderPoints[i][0][0] = cos(t) * radius;
        cylinderPoints[i][0][1] = sin(t) * radius;
        cylinderPoints[i][0][2] = -height;

        // Second corner
        cylinderPoints[i][1][0] = cos(s) * radius;
        cylinderPoints[i][1][1] = sin(s) * radius;
        cylinderPoints[i][1][2] = -height;

        // Third corner
        cylinderPoints[i][2][0] = cos(t) * radius;
        cylinderPoints[i][2][1] = sin(t) * radius;
        cylinderPoints[i][2][2] = height;

        // Fourth corner
        cylinderPoints[i][3][0] = cos(s) * radius;
        cylinderPoints[i][3][1] = sin(s) * radius;
        cylinderPoints[i][3][2] = height;


    }
}

void Cylinder::draw() {
    glPushMatrix();

    // draw immediate (center Cylinder)
    glTranslatef(position.x, position.y, position.z);
    glRotatef(angleRotation, vectorRotation.x, vectorRotation.y, vectorRotation.z);
    drawImmediate();

    glPopMatrix();
}

GLfloat * Cylinder::normalize(GLfloat vec[3]) {
    GLfloat l = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    vec[0] /= l;
    vec[1] /= l;
    vec[2] /= l;
    return vec;
}

// return normalized cross product of input vectors

GLfloat * Cylinder::cross(GLfloat a[3], GLfloat b[3]) {
    GLfloat * cross = (GLfloat *) malloc(3 * sizeof (GLfloat));
    cross[0] = (a[1] * b[2] - a[2] * b[1]);
    cross[1] = (a[2] * b[0] - a[0] * b[2]);
    cross[2] = (a[0] * b[1] - a[1] * b[0]);
    return cross;
}

// return unit normal to surface defined by the three input points

GLfloat * Cylinder::normal(GLfloat av[3], GLfloat bv[3], GLfloat cv[3]) {
    GLfloat vc0[3], vc1[3];
    for (int i = 0; i < 3; i++) {
        vc0[i] = bv[i] - av[i];
        vc1[i] = cv[i] - av[i];
    }
    return normalize(cross(vc0, vc1));
}

// Draw a circle variable in z axis

void Cylinder::drawCircle(float z) {
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, z);
    glVertex3f(0, 0, z);
    for (int n = 0; n <= NO_POINTS; ++n) {
        float const t = 2 * M_PI * (float) n / (float) NO_POINTS;
        glVertex3f(cos(t) * radius, sin(t) * radius, z);
    }
    glEnd();
}

//==================== 1. Immediate method ===================================
// immediate definition of individual vertex properties

void Cylinder::drawImmediate() {
    drawCircle(-height);
    drawCircle(height);

    for (int i = 0; i <= NO_POINTS; ++i) {
        glBegin(GL_QUADS);
        {

            glNormal3fv(normal(cylinderPoints[i][0], cylinderPoints[i][1], cylinderPoints[i][3]));

            cylinderPoints[i][0][2] = -height;
            cylinderPoints[i][1][2] = -height;
            cylinderPoints[i][2][2] = height;
            cylinderPoints[i][3][2] = height;

            glVertex3fv(cylinderPoints[i][0]);
            glVertex3fv(cylinderPoints[i][1]);
            glVertex3fv(cylinderPoints[i][3]);
            glVertex3fv(cylinderPoints[i][2]);

        }
        glEnd();
    }
}
