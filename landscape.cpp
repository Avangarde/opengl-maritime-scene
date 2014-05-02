#include "landscape.h"
#include <QGLViewer/qglviewer.h>

using namespace std;

Landscape::Landscape() {

}

GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gray[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat fogColor[] = {0.0f, 0.0f, 0.6f, 1.0f};
GLfloat light_position[] = {0.0f, 0.0f, 100.0f, 1.0f};

double* normal(double p1[], double p2[], double p3[]) {
    double v1x = p2[0] - p1[0];
    double v1y = p2[1] - p1[1];
    double v1z = p2[2] - p1[2];
    double v2x = p3[0] - p1[0];
    double v2y = p3[1] - p1[1];
    double v2z = p3[2] - p1[2];
    double crossX = ((v1y * v2z) - (v1z * v2y));
    double crossY = ((v1z * v2x) - (v1x * v2z));
    double crossZ = ((v1x * v2y) - (v1y * v2x));
    double mod = sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ);
    crossX = crossX / mod;
    crossY = crossY / mod;
    crossZ = crossZ / mod;
    double normal[3] = {crossX, crossY, crossZ};
    return normal;
}

void Landscape::loadTexture(const char* filename) {
    glGenTextures(1, &texture);

    QImage img = QGLWidget::convertToGLFormat(QImage(filename));

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0,
            GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Landscape::init(Viewer&) {
    //Texture

    loadTexture("images/underwater_2.jpg");

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    //Light

    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT1, GL_AMBIENT, black);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.005);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_FOG);

    glClearColor(0.0f, 0.0f, 0.6f, 1.0f);

}

void Landscape::draw() {

    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    int radius = RADIUS_SCENE;

    double x, y, z;

    glColor3f(1, 1, 1);

    glPushMatrix();

    double p1[3], p2[3], p3[3];

    double X = 0, Y = 0, Z = 0;
    for (double phi = 0.0; phi <= 80.0; phi += 10.0) {
        glBegin(GL_QUAD_STRIP);

        for (double theta = -180.0; theta <= 180.0; theta += 10.0) {
            x = radius * sin(PI / 180 * theta) * cos(PI / 180 * phi);
            y = radius * cos(PI / 180 * theta) * cos(PI / 180 * phi);
            z = radius * sin(PI / 180 * phi);

            p1[0] = x;
            p1[1] = y;
            p1[2] = z;

            X += 1 / 37.;
            Y += 1 / 37.;

            x = radius * sin(PI / 180 * theta) * cos(PI / 180 * (phi + 10.0));
            y = radius * cos(PI / 180 * theta) * cos(PI / 180 * (phi + 10.0));
            z = radius * sin(PI / 180 * (phi + 10.0));

            p2[0] = x;
            p2[1] = y;
            p2[2] = z;

            x = radius * sin(PI / 180 * theta + 10) * cos(PI / 180 * (phi));
            y = radius * cos(PI / 180 * theta + 10) * cos(PI / 180 * (phi));
            z = radius * sin(PI / 180 * (phi + 10.0));

            p3[0] = x;
            p3[1] = y;
            p3[2] = z;

            //double * norm = normal(p1, p2, p3);
            glNormal3d(0.0, 0.0, 1.0);

            glTexCoord3f(X, Z, Y);
            glVertex3d(p1[0], p1[1], p1[2]);
            glTexCoord3f(X + 1 / 36, Z + 1 / 9., Y);
            glVertex3d(p2[0], p2[1], p2[2]);
        }
        Z += 1 / 9.;
        X = 0;
        Y = 0;

        glEnd();

    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

