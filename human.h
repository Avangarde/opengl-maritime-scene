#ifndef _Human_
#define _Human_

#include "renderable.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Human : public Renderable {
public:

    Human() {
        scale = 1;
    }

    Human(float theScale) : scale(theScale) {
    }
    void draw();

    float getScale() const {
        return scale;
    }

    void setScale(float scale) {
        this->scale = scale;
    }

private:
    float scale;
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
    static const float SHOULDER_ANGLE = 25;
    static const float SHOULDER_DISTANCE = 2;
    static const float SHOULDER_RADIUS = 0.6;
    static const float THIGH_HEIGHT = 1.8;
    static const float THIGH_DOWN_RADIUS = 0.6;
    static const float THIGH_UP_RADIUS = 0.8;
    static const float TORSO_DOWN_RADIUS = 2;
    static const float TORSO_UP_RADIUS = 1.5;
    static const float TORSO_HEIGHT = 4;

    GLUquadricObj *quadratic;

    void drawArm();
    void drawElbow();
    void drawFoot();
    void drawForearm();
    void drawFullArm(float angleShoulder);
    void drawFullLeg();
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

