/* 
 * File:   fish.h
 * Author: rojascle
 *
 * Created on 7 avril 2014, 10:59
 */

#ifndef FISH_H
#define	FISH_H

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <QGLViewer/vec.h>
using namespace qglviewer;

#include "particle.h"

class Fish : public Particle {

private:
    double height;
    Vec acceleration;
    
public:
    Fish(Vec pos, Vec vel, double m, double r, double h);
    virtual ~Fish();
    void draw() const;
};

#endif	/* FISH_H */

