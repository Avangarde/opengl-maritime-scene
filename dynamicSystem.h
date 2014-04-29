/* 
 * File:   dynamicSystem.h
 * Author: rojascle
 *
 * Created on 7 avril 2014, 11:26
 */

#ifndef DYNAMIC_SYSTEM_H
#define	DYNAMIC_SYSTEM_H

#include <vector>
using namespace std;
#include "renderable.h"
#include "fish.h"
#include "terrain.h"

class DynamicSystem : public Renderable {
private:
    
    Terrain* terrain;
    
    vector<Fish *> fishes;

    Vec defaultGravity;
    Vec gravity;
    double defaultMediumViscosity;
    double mediumViscosity;
    double dt;
    

    double fishMass;

    bool toggleGravity;
    bool toggleViscosity;

    void clear();
    void collisionParticleGround(Particle *p);
    void collisionLimits(Particle *p);
    void collisionFish(Fish *f1, Fish *f2);

public:
    DynamicSystem(Terrain *);
    virtual ~DynamicSystem();

    void setGravity(bool onOff);
    void setViscosity(bool onOff);

    void init(Viewer &);
    void draw();
    void animate();
    // event response
    void keyPressEvent(QKeyEvent*, Viewer&);
    void mouseMoveEvent(QMouseEvent*, Viewer&);

};

#endif	/* DYNAMIC_SYSTEM_H */

