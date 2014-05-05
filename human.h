#ifndef _Human_
#define _Human_

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <vector>
#include <QGLViewer/manipulatedFrame.h>
#include <QGLViewer/vec.h>
#include "cylinder.h"
#include "particle.h"
#include "renderable.h"
#include "spring.h"
#include "tube.h"

using namespace std;

class Human : public Particle {
public:

    Human():Particle(Vec(),Vec(),0.1,5.0),tube(new Tube()) {
        scale = 1;
    }

    Human(float theScale) : Particle(Vec(),Vec(),0.1,5.0),scale(theScale),tube(new Tube()) {
    }

    Human(float scale, Vec position, Vec beginPipe) : Particle(position,Vec(1.0,0.0,0.0),0.1,5.0),scale(scale), tube(new Tube(beginPipe)) {
        direction = Vec(1.0,0.0,0.0);
        incrShoulder = true;
        shoulderAngle = 180;
    }

    void draw();
    void init(Viewer&);
    void animate(float dt, Vec goal);
//    void keyPressEvent(QKeyEvent*, Viewer&);
//    void mouseMoveEvent(QMouseEvent*, Viewer&);
    void setScale(float scale);
    float getScale() const;   
    Tube* getTube() const;
    void setTube(Tube* tube);
    const Vec & getDirection() const;

private:
    static const int PRECISION = 20;
    static const float ARM_UP_RADIUS = 0.3;
    static const float ARM_HEIGHT = 1.5;
    static const float FOREARM_RADIUS = 0.5;
    static const float CODE_RADIUS = 0.5;
    static const float FOREARM_DOWN_RADIUS = 0.6;
    static const float FOREARM_UP_RADIUS = 0.4;
    static const float FOREARM_HEIGHT = 2;
    static const float HEAD_RADIUS = 1;
    static const float HIP_RADIUS = 0.5;
    static const float HIP_HEIGHT = 3;
    static const float KNEE_RADIUS = 0.7;
    static const float SHIN_UP_RADIUS = 0.6;
    static const float SHIN_DOWN_RADIUS = 0.45;
    static const float SHIN_HEIGHT = 2;
    static const float SHOULDER_POSITION = -25;
    static const float SHOULDER_DISTANCE = 2;
    static const float SHOULDER_RADIUS = 0.6;
    static const float THIGH_HEIGHT = 1.8;
    static const float THIGH_DOWN_RADIUS = 0.6;
    static const float THIGH_UP_RADIUS = 0.8;
    static const float TORSO_UP_RADIUS = 2;
    static const float TORSO_DOWN_RADIUS = 1.5;
    static const float TORSO_HEIGHT = 4;

    float scale;
    bool incrShoulder;
    int shoulderAngle;
    Tube* tube;
    Vec direction;

    GLUquadricObj *quadratic;

    void drawArm();
    void drawElbow();
    void drawFoot();
    void drawForearm();
    void drawFullArm(float angleShoulder);
    void drawFullLeg(bool right);
    void drawFullHead();
    void drawFullShoulder(bool right);
    void drawHand();
    void drawHead();
    void drawHip();
    void drawKnee();
    void drawNeck();
    void drawLowerTorso();
    void drawUpperTorso();
    void drawShin();
    void drawShoulder();
    void drawThigh();
    void drawTorso();
};

#endif

