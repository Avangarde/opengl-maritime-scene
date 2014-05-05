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
    float cylinderHeight = beginningTube.z / (float) PRECISION_PIPE;
    Vec initPos = beginningTube;
    for (int i = 0; i < PRECISION_PIPE; i++) {
        initPos.z -= cylinderHeight;
        Cylinder* cylinder = new Cylinder(cylinderHeight, RADIUS_PIPE);
        cylinder->init(viewer);
        this->cylinders.push_back(cylinder);
        Particle * particle1 = new Particle(initPos, Vec(), 0.7, 0.1);
        if (i != 0)
            springs.push_back(new Spring(particles.back(), particle1, PRECISION_PIPE * .55, cylinderHeight / (PRECISION_PIPE), 2));
        this->particles.push_back(particle1);

    }
    Particle * particle1 = new Particle(initPos, Vec(), 1.0, 0.1);
    springs.push_back(new Spring(particles.back(), particle1, 1, 1, 1));
    this->particles.push_back(particle1);
}

void Tube::draw() {
    float cylinderHeight = beginningTube.z / (float) PRECISION_PIPE;
    Vec initPos = Vec(0.0, 0.0, cylinderHeight / 2);
    vector<Cylinder *>::iterator itC;
    vector<Particle *>::iterator itP;
    for (size_t p = 0, c = 0;
            p < getParticles().size() - 1 && c < getCylinders().size();
            p++, c++) {
        Particle *part1 = getParticles()[p];
        Particle *part2 = getParticles()[p + 1];
        Cylinder *cyl = getCylinders()[c];
        Vec pos0 = part1->getPosition();
        Vec pos1 = part2->getPosition();
        Vec posm = pos0 + 0.5f * (pos1 - pos0);
        Vec vDir = pos1 - pos0;
        cyl->setHeight(Utils::lengthVec(vDir) / 2);
        vDir.normalize();

        float num = abs(Utils::dotProduct(vDir, Vec(0.0, 0.0, 1.0)));
        float den = Utils::lengthVec(vDir) * Utils::lengthVec(Vec(0.0, 0.0, 1.0));
        float angle = acos(num / den)*(180 / M_PI);

        cyl->setPosition(posm);
        part1->draw();

        Vec Z = Vec(0, 0, 1);    
        Vec norm = cross(Z, vDir);
        
        if (pos0.z > pos1.z) {
            angle = 180 - angle;
        }
        
        cyl->setAngleRotation(angle);
        cyl->setVectorRotation(norm);
        
        
        glPushMatrix();
        {
            cyl->draw();
        }
        glPopMatrix();
        initPos += this->beginningTube / (float) PRECISION_PIPE;
    }
    getParticles().back()->draw();
    //Springs
    glColor3f(1.0, 0.28, 0.0);
    glLineWidth(5.0);
    vector<Spring *>::iterator itS;
    for (itS = springs.begin(); itS != springs.end(); ++itS) {
        (*itS)->draw();
    }
}



