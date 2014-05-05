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
#include "fish.h"
#include "human.h"
#include "renderable.h"
#include "terrain.h"
#include "submarine.h"

class DynamicSystem : public Renderable {
private:
    
    Human* human;
    Terrain* terrain;
    Submarine* submarine;
    
    vector<Fish *> fishes;
    vector< vector<Particle *> > bubbles;
    vector< vector<Particle *> > sand;

    Vec defaultGravity;
    Vec gravity;
    double defaultMediumViscosity;
    double mediumViscosity;
    double dt;    

    double fishMass;
    Vec goal;
    Vec humanGoal;
    Vec submarineGoal;
    int step;

    bool toggleGravity;
    bool toggleViscosity;
    bool toggleCollisions;
    bool toggleHuman;
    bool toggleFishes;
    bool toggleSand;
    bool toggleBubbles;
    bool handleCollisions;
    bool handleHuman;
    bool handleFishes;
    bool handleSand;
    bool handleBubbles;

    void clear();
    void collisionParticleGround(Particle *p);
    void collisionLimits(Particle *p);
    void collisionFish(Fish *f1, Fish *f2);
    void createBubbles(Vec origin, Vec vel, int maxBubbles, double maxRad);
    void createSand(Vec origin, Vec vel, int maxSand);
    void animateBubbles();
    void animateSand();
    void setHuman(bool onOff);
    void setFishes(bool onOff);
    void setBubbles(bool onOff);
    void setSand(bool onOff);
    void setCollisionsDetection(bool onOff);

public:
    DynamicSystem(Terrain *,Human *);
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

