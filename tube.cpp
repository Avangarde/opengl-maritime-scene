/* 
 * File:   tube.cpp
 * Author: juanmanuelmartinezromero
 * 
 * Created on 29 avril 2014, 20:43
 */

#include "tube.h"

Tube::Tube() : beginningTube(0.0, 0.0, 0.0) {
}

Tube::Tube(const Tube& orig) {
}

Tube::~Tube() {
}

void Tube::init(Viewer& viewer) {
    for (int i = 0; i < PRECISION_PIPE; i++) {
        float cylinderHeight = beginningTube.z / (float) PRECISION_PIPE;
        //Dibujar las particulas y poner un Spring entre ellas
        //particle1 = new Particle(initPos, Vec(), 0.0, 0.5);
        Cylinder* cylinder = new Cylinder(cylinderHeight, RADIUS_PIPE);
        cylinder->init(viewer);
        this->cylinders.push_back(cylinder);
    }
}

void Tube::draw() {
    //this->springs.push_back(new Spring(pipes.back(), particle1, 1, 1, 1));
    float cylinderHeight = beginningTube.z / (float) PRECISION_PIPE;
    Vec initPos = Vec(0.0, 0.0, cylinderHeight / 2);
    vector<Cylinder *>::iterator itP;
    for (itP = cylinders.begin(); itP != cylinders.end(); ++itP) {
        glPushMatrix();
        {
            glTranslatef(initPos.x, initPos.y, initPos.z);
            (*itP)->draw();
        }
        glPopMatrix();
        initPos += this->beginningTube / (float) PRECISION_PIPE;
    }
    
    // Springs
    /**glColor3f(1.0, 0.28, 0.0);
    glLineWidth(5.0);
    vector<Spring *>::iterator itS;
    for (itS = springs.begin(); itS != springs.end(); ++itS) {
        cout << "Drawing Spring\n";
        (*itS)->draw();
    }*/
}



