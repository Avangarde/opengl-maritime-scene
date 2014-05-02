/* 
 * File:   Terrain.cpp
 * Author: martijua
 * 
 * Created on 26 mars 2014, 16:41
 */

#include <math.h>
#include <vector>
#include <QGLViewer/vec.h>
#include <iostream>
#include <fstream>

#include "terrain.h"
#include "texload.h"

#define NUM_PATTERNS 32

//using namespace std;
using namespace qglviewer;

std::ofstream myfile;

double Terrain::unused = -10032.4775;
qglviewer::Vec noNormal = (Vec(0,0,0));

GLfloat causticScale = 0.2;
static int causticIncrement = 2;
static int currentCaustic = 0;

Terrain::Terrain() {
}

Terrain::Terrain(const Terrain& orig) {
}

Terrain::~Terrain() {
}

Terrain::Terrain(int r) : size(r) {
    std::vector<double> nullRow(size, unused);
    std::vector<std::vector<double> > nullMatrix(size, nullRow);
    rise = nullMatrix;
    std::vector<qglviewer::Vec> nullRowNormal(size, noNormal);
    std::vector<std::vector<qglviewer::Vec> > nullMatrixNormal(size, nullRowNormal);
    normals = nullMatrixNormal;
}

void Terrain::init(Viewer &) {
    rise[0][0] = ((double) rand() / RAND_MAX);
    rise[0][size - 1] = ((double) rand() / RAND_MAX);
    rise[size - 1][0] = ((double) rand() / RAND_MAX);
    rise[size - 1][size - 1] = ((double) rand() / RAND_MAX);
    createTerrain(0, 0, size - 1, size - 1, 0.30);
    
    //Caustics
    int width, height;
    GLubyte *imageData;
    int causticIncrement = 1;
    glEnable(GL_TEXTURE_2D);

    /* Load the caustic ripple textures. */
    printf("loading caustics:");
    for (int i = 0; i < NUM_PATTERNS; i += causticIncrement) {
        char filename[80];

        sprintf(filename, "images/caust%02d.bw", i);
        printf(" %d", i);
        fflush(stdout);
        imageData = read_alpha_texture(filename, &width, &height);
        if (imageData == NULL) {
            fprintf(stderr, "\n%s: could not load image file\n", filename);
            exit(1);
        }

        glBindTexture(GL_TEXTURE_2D, i + 1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, height, width, 0,
                GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);

        free(imageData);

    }
    printf(".\n");

}

void Terrain::createTerrain(int x1, int y1, int x2, int y2, double roughness) {
    int x12 = (x1 + x2) / 2;
    int y12 = (y1 + y2) / 2;

    if (y12 < y2) {
        if (rise[x1][y12] == unused) {
            rise[x1][y12] = (rise[x1][y1] + rise[x1][y2]) / 2 + scale(roughness * (y2 - y1));
        }
        rise[x2][y12] = (rise[x2][y1] + rise[x2][y2]) / 2 + scale(roughness * (y2 - y1));
    }
    if (x12 < x2) {
        if (rise[x12][y1] == unused) {
            rise[x12][y1] = (rise[x1][y1] + rise[x2][y1]) / 2 + scale(roughness * (x2 - x1));
        }
        rise[x12][y2] = (rise[x1][y2] + rise[x2][y2]) / 2 + scale(roughness * (x2 - x1));
    }
    if (x12 < x2 && y12 < y2) {
        rise[x12][y12] = (rise[x1][y1] + rise[x2][y1] + rise[x1][y2] + rise[x2][y2]) / 4
                + scale(roughness * (fabs((double) (x2 - x1)) + fabs((double) (y2 - y1))));
    }
    if (x12 < x2 - 1 || y12 < y2 - 1) {
        createTerrain(x1, y1, x12, y12, roughness);
        createTerrain(x1, y12, x12, y2, roughness);
        createTerrain(x12, y1, x2, y12, roughness);
        createTerrain(x12, y12, x2, y2, roughness);
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (rise[i][j] < 0) rise[i][j] = 0;
        }
    }
    for (int x = 1; x <= size - 2; x++) {
        for (int z = 1; z <= size - 2; z++) {          
            normals[x][z] = getNormal(x, z);
        }
    }
}

qglviewer::Vec Terrain::getNormal(int x, int z) {
    if (x < 1) {
        x = 1;
    }
    if (x > size - 1) {
        x = size - 1;
    }
    if (z < 1) {
        z = 1;
    }
    if (z > size - 1) {
        z = size - 1;
    }

    Vec normal, normal1, normal2, normal3, normal4;
    normal1 = -((Vec(x, rise[x][z + 1], z + 1) - Vec(x, rise[x][z], z))^(Vec(x - 1, rise[x - 1][z], z) - Vec(x, rise[x][z], z)));
    normal2 = -((Vec(x - 1, rise[x - 1][z], z) - Vec(x, rise[x][z], z))^(Vec(x, rise[x][z - 1], z - 1) - Vec(x, rise[x][z], z)));
    normal3 = -((Vec(x, rise[x][z - 1], z - 1) - Vec(x, rise[x][z], z))^(Vec(x + 1, rise[x + 1][z], z) - Vec(x, rise[x][z], z)));
    normal4 = -((Vec(x + 1, rise[x + 1][z], z) - Vec(x, rise[x][z], z))^(Vec(x, rise[x][z + 1], z + 1) - Vec(x, rise[x][z], z)));

    normal1.normalize();
    normal2.normalize();
    normal3.normalize();
    normal4.normalize();

    normal = normal1 + normal2 + normal3 + normal4;
    normal.normalize();

    return normal;
}

void Terrain::draw() {
//    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    glEnable(GL_BLEND);

    GLfloat sPlane[4] = {0.05, 0.03, 0.0, 0.0};
    GLfloat tPlane[4] = {0.0, 0.03, 0.05, 0.0};

    sPlane[0] = 0.05 * causticScale;
    sPlane[1] = 0.03 * causticScale;

    tPlane[1] = 0.03 * causticScale;
    tPlane[2] = 0.05 * causticScale;

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, currentCaustic + 1);


    drawTerrain();
  
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glDisable(GL_BLEND);
      
}

void Terrain::drawTerrain() {
    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glTranslatef(-size*2, 0, -size*2);
//    glScalef(4.0, 0.0, 4.0);
//    glTranslatef(-size/2, 0, -size/2);
    Vec normal;
    glColor3f(0.8f, 0.8f, 0.8f);
    //glClearColor(0.8f, 0.8f, 0.8f, 0.1f);
    glBegin(GL_TRIANGLES);
    for (int x = 1; x < size - 2; x++) {
        for (int z = 1; z < size - 2; z++) {
            
            normal = normals[x][z];
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(4*x, rise[x][z], 4*z);
            
            normal = normals[x+1][z];
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(4*(x + 1), rise[x + 1][z], 4*z);
            
            normal = normals[x][z + 1];
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(4*x, rise[x][z + 1], 4*(z + 1));
            
            normal = normals[x + 1][z];
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(4*(x + 1), rise[x + 1][z], 4*z);
            
            normal = normals[x + 1][z + 1];
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(4*(x + 1), rise[x + 1][z + 1], 4*(z + 1));
            
            normal = normals[x][z + 1];
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(4*x, rise[x][z + 1], 4*(z + 1));
        }
    }
    glEnd();
    glPopMatrix();
}

void Terrain::animate() {
    currentCaustic = (currentCaustic + causticIncrement) % NUM_PATTERNS;
}