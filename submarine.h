/* 
 * File:   submarine.h
 * Author: edward
 *
 * Created on 4 mai 2014, 22:43
 */

#ifndef SUBMARINE_H
#define	SUBMARINE_H

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <QGLViewer/vec.h>
using namespace qglviewer;
#include <vector>
using namespace std;

#include "particle.h"

class Submarine : public Particle {
private:
    double height;
    Vec colour;
    Vec direction;
    Vec propPosition;
    float angleProp;
    
public:
    Submarine(Vec pos, Vec vel, Vec dir, double m, double r, double h);
    virtual ~Submarine();
    void draw();
    void animate(float dt, Vec goal);
    const Vec & getPropPosition() const;
    
};

#endif	/* SUBMARINE_H */

