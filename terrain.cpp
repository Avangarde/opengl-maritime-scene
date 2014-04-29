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

//using namespace std;
using namespace qglviewer;

std::ofstream myfile;

double Terrain::unused = -10032.4775;

Terrain::Terrain() {
}

Terrain::Terrain(const Terrain& orig) {
}

Terrain::~Terrain() {
}

Terrain::Terrain(int r) : size(r) {
    std::vector<double> nullRow(size, unused);
    std::vector<std::vector<double> > nullMatrix(size, nullRow);
    d = nullMatrix;
}

void Terrain::init(Viewer &) {
    d[0][0] = ((double) rand() / RAND_MAX);
    d[0][size - 1] = ((double) rand() / RAND_MAX);
    d[size - 1][0] = ((double) rand() / RAND_MAX);
    d[size - 1][size - 1] = ((double) rand() / RAND_MAX);
    createTerrain(0, 0, size - 1, size - 1, 0.15);
}

void Terrain::createTerrain(int x1, int y1, int x2, int y2, double roughness) {
    int x12 = (x1 + x2) / 2;
    int y12 = (y1 + y2) / 2;

    if (y12 < y2) {
        if (d[x1][y12] == unused) {
            d[x1][y12] = (d[x1][y1] + d[x1][y2]) / 2 + scale(roughness * (y2 - y1));
        }
        d[x2][y12] = (d[x2][y1] + d[x2][y2]) / 2 + scale(roughness * (y2 - y1));
    }
    if (x12 < x2) {
        if (d[x12][y1] == unused) {
            d[x12][y1] = (d[x1][y1] + d[x2][y1]) / 2 + scale(roughness * (x2 - x1));
        }
        d[x12][y2] = (d[x1][y2] + d[x2][y2]) / 2 + scale(roughness * (x2 - x1));
    }
    if (x12 < x2 && y12 < y2) {
        d[x12][y12] = (d[x1][y1] + d[x2][y1] + d[x1][y2] + d[x2][y2]) / 4
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
            if (d[i][j] < 0) d[i][j] = 0;
        }
    }
    solidId = glGenLists(1);
    //createList();
}

void Terrain::createList() {

    glNewList(solidId, GL_COMPILE);
//    Vec normal;
//    glBegin(GL_TRIANGLES);
//    glColor3f(0.8f, 0.8f, 0.8f);
//    for (int x = 1; x < size - 2; x++) {
//        for (int z = 1; z < size - 2; z++) {
//            
//            normal = getNormal(x, z);
//            glNormal3f(normal[0], normal[1], normal[2]);
//            glVertex3f(x, d[x][z], z);
//            
//            normal = getNormal(x, z);
//            glNormal3f(normal[0], normal[1], normal[2]);
//            glVertex3f(x + 1, d[x + 1][z], z);
//            
//            normal = getNormal(x, z + 1);
//            glNormal3f(normal[0], normal[1], normal[2]);
//            glVertex3f(x, d[x][z + 1], z + 1);
//            
//            normal = getNormal(x + 1, z);
//            glNormal3f(normal[0], normal[1], normal[2]);
//            glVertex3f(x + 1, d[x + 1][z], z);
//            
//            normal = getNormal(x + 1, z + 1);
//            glNormal3f(normal[0], normal[1], normal[2]);
//            glVertex3f(x + 1, d[x + 1][z + 1], z + 1);
//            
//            normal = getNormal(x, z + 1);
//            glNormal3f(normal[0], normal[1], normal[2]);
//            glVertex3f(x, d[x][z + 1], z + 1);
//        }
//    }
//    glEnd();
    glEndList();
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
    normal1 = -((Vec(x, d[x][z + 1], z + 1) - Vec(x, d[x][z], z))^(Vec(x - 1, d[x - 1][z], z) - Vec(x, d[x][z], z)));
    normal2 = -((Vec(x - 1, d[x - 1][z], z) - Vec(x, d[x][z], z))^(Vec(x, d[x][z - 1], z - 1) - Vec(x, d[x][z], z)));
    normal3 = -((Vec(x, d[x][z - 1], z - 1) - Vec(x, d[x][z], z))^(Vec(x + 1, d[x + 1][z], z) - Vec(x, d[x][z], z)));
    normal4 = -((Vec(x + 1, d[x + 1][z], z) - Vec(x, d[x][z], z))^(Vec(x, d[x][z + 1], z + 1) - Vec(x, d[x][z], z)));

    normal1.normalize();
    normal2.normalize();
    normal3.normalize();
    normal4.normalize();

    normal = normal1 + normal2 + normal3 + normal4;
    normal.normalize();

    return normal;
}

void Terrain::draw() {
    
//    glCallList(solidId);
    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glTranslatef(-size, 0, -size);
    Vec normal;
    glBegin(GL_TRIANGLES);
    glColor3f(0.8f, 0.8f, 0.8f);
    for (int x = 1; x < size - 2; x++) {
        for (int z = 1; z < size - 2; z++) {
            
            normal = getNormal(x, z);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(2*x, d[x][z], 2*z);
            
            normal = getNormal(x, z);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(2*(x + 1), d[x + 1][z], 2*z);
            
            normal = getNormal(x, z + 1);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(2*x, d[x][z + 1], 2*(z + 1));
            
            normal = getNormal(x + 1, z);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(2*(x + 1), d[x + 1][z], 2*z);
            
            normal = getNormal(x + 1, z + 1);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(2*(x + 1), d[x + 1][z + 1], 2*(z + 1));
            
            normal = getNormal(x, z + 1);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(2*x, d[x][z + 1], 2*(z + 1));
        }
    }
    glEnd();
    glPopMatrix();
}