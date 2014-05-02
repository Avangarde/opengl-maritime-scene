/* 
 * File:   tube.h
 * Author: juanmanuelmartinezromero
 *
 * Created on 29 avril 2014, 20:43
 */

#ifndef TUBE_H
#define	TUBE_H

#include <cmath>
#include "cylinder.h"
#include "spring.h"
#include "renderable.h"
#include "utils.h"
#include <QGLViewer/vec.h>
#include <vector>

using namespace qglviewer;
//using namespace std;

class Tube : public Renderable {
public:

    Tube();

    Tube(Vec begin) : beginningTube(begin) {
    }
    Tube(const Tube& orig);
    virtual ~Tube();

    virtual void init(Viewer&);
    virtual void draw();

    Vec getBeginningTube() const {
        return beginningTube;
    }

    void setBeginningTube(Vec beginningTube) {
        this->beginningTube = beginningTube;
    }
    
    vector<Particle*> getParticles() const {
        return particles;
    }

    void setParticles(vector<Particle*> particles) {
        this->particles = particles;
    }
    vector<Spring*> getSprings() const {
        return springs;
    }

    void setSprings(vector<Spring*> springs) {
        this->springs = springs;
    }

    vector<Cylinder*> getCylinders() const {
        return cylinders;
    }

    void setCylinders(vector<Cylinder*> cylinders) {
        this->cylinders = cylinders;
    }


private:
    static const int PRECISION_PIPE = 10;
    static const float RADIUS_PIPE = 0.2;
    Vec beginningTube;
    vector<Cylinder *> cylinders;
    vector<Particle *> particles;
    vector<Spring *> springs;
};

#endif	/* TUBE_H */

