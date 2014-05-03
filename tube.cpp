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
        Particle * particle1 = new Particle(initPos, Vec(), 0.5, 0.1);
        if (i != 0)
            springs.push_back(new Spring(particles.back(), particle1, PRECISION_PIPE*.55, cylinderHeight/(PRECISION_PIPE*4), 1));
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
        
        float num = abs(Utils::crossProduct(vDir, Vec(0.0, 1.0, 0.0)));
        float den = Utils::lengthVec(vDir) * Utils::lengthVec(Vec(0.0, 1.0, 0.0));
        float angleZX = asin(num / den)*(180 / M_PI);
        //        cout << "angleZX = " << angleZX << "\n";

        num = abs(Utils::crossProduct(vDir, Vec(1.0, 0.0, 0.0)));
        den = Utils::lengthVec(vDir) * Utils::lengthVec(Vec(1.0, 0.0, 0.0));
        float angleYZ = asin(num / den)*(180 / M_PI);
        //        cout << "angleYZ = " << angleYZ << "\n";

        cyl->setPosition(posm);
        if ((part1->getPosition().y < part2->getPosition().y && part1->getPosition().z < part2->getPosition().z)
                || (part2->getPosition().y < part1->getPosition().y && part2->getPosition().z < part1->getPosition().z))
            angleZX *= -1;

        cyl->setAngleXY(angleZX);
        if ((part2->getPosition().z > part1->getPosition().z && part2->getPosition().x < part1->getPosition().x)
                || (part2->getPosition().z < part1->getPosition().z && part2->getPosition().x > part1->getPosition().x))
            angleYZ *= -1;

        cyl->setAngleYZ(angleYZ);
        part1->draw();
        cyl->setHeight(Utils::lengthVec(vDir) / 2);
        glPushMatrix();
        {
            //cyl->draw();
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



