#include <iostream>
#include <math.h>
#include "human.h"
#include "cylinder.h"
#include "particle.h"

using namespace std;
using namespace qglviewer;

void Human::draw() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glScalef(scale, scale, scale);
        drawUpperTorso();
        drawLowerTorso();
    }
    glPopMatrix();
}

void Human::drawUpperTorso() {
    drawTorso();
    drawFullHead();
    drawFullShoulder(true);
    drawFullShoulder(false);
}

void Human::drawFullShoulder(bool right) {
    float rightArm = right ? 1 : -1;
    glPushMatrix();
    {
        glTranslatef(rightArm*SHOULDER_DISTANCE, -1*SHOULDER_RADIUS, 0.0);
        glRotatef(rightArm * 120, 0, 0, 1);
        drawFullArm(rightArm * SHOULDER_ANGLE);
    }
    glPopMatrix();
}

void Human::drawLowerTorso() {
    glPushMatrix();
    {
        glTranslatef(-1.5, -4.5, 0.0);
        drawHip();
        glPushMatrix();
        {
            glTranslatef(2.2, -1*HIP_RADIUS, 0.0);
            drawFullLeg();
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(0.7, -1*HIP_RADIUS, 0.0);
            drawFullLeg();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void Human::drawHip() {
    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        glColor3f(0.0f, 0.0f, 0.6f);
        quadratic = gluNewQuadric();
        //gluCylinder(quadratic, HIP_RADIUS, HIP_RADIUS, HIP_HEIGHT, PRECISION, PRECISION);
        glTranslatef(0, 0, HIP_HEIGHT / 2);
        Cylinder* cylinder = new Cylinder(HIP_HEIGHT, HIP_RADIUS);
        cylinder->draw();
    }
    glPopMatrix();
}

void Human::drawTorso() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(1.0f, 0.0f, 0.0f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, TORSO_DOWN_RADIUS, TORSO_UP_RADIUS, TORSO_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawHead() {
    glPushMatrix();
    {
        glColor3f(1.0f, 0.64f, 0.52f);
        glutSolidSphere(HEAD_RADIUS, PRECISION, PRECISION);
        Vec initPos = Vec(0.0,HEAD_RADIUS,0.0);
        Particle * particle1 = new Particle(initPos,Vec(),0.0,0.1);
        this->pipes.push_back(particle1);
    }
    glPopMatrix();
}

void Human::drawNeck() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(1.0f, 0.64f, 0.52f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, 0.40f, 0.40f, 0.30, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawShoulder() {
    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glutSolidSphere(SHOULDER_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawForearm() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(1.0f, 0.64f, 0.52f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, FOREARM_DOWN_RADIUS, FOREARM_UP_RADIUS, FOREARM_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawElbow() {
    glPushMatrix();
    {
        glColor3f(1.0f, 0.64f, 0.52f);
        glutSolidSphere(FOREARM_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawArm() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(1.0f, 0.64f, 0.52f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, FOREARM_RADIUS, ARM_UP_RADIUS, ARM_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawHand() {
    glPushMatrix();
    {
        glColor3f(1.0f, 0.64f, 0.52f);
        glutSolidSphere(SHOULDER_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawFullArm(float angleShoulder) {
    glPushMatrix();
    {
        drawShoulder();
        glRotatef(angleShoulder, 0, 0, 1);
        glPushMatrix();
        {
            drawForearm();
            glPushMatrix();
            {
                glTranslatef(0.0, -2.0, 0.0);
                drawElbow();
                glPushMatrix();
                {
                    drawArm();
                    glPushMatrix();
                    {
                        glTranslatef(0.0, -2.1, 0.0);
                        drawHand();
                    }
                    glPopMatrix();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void Human::drawFullLeg() {
    glPushMatrix();
    {
        glRotatef(45, 1, 0, 0);
        drawThigh();
        glPushMatrix();
        {
            glTranslatef(0.0, -2.2, 0.0);
            drawKnee();
            glPushMatrix();
            {
                glTranslatef(0.0, -0.4, 0.0);
                drawShin();
                glPushMatrix();
                {
                    glTranslatef(0.0, -2.2, 0.5);
                    drawFoot();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();

    }
    glPopMatrix();
}

void Human::drawFullHead() {
    glPushMatrix();
    {
        glTranslatef(0.0, 0.25, 0.0);
        drawNeck();
        glPushMatrix();
        {
            glTranslatef(0.0, 0.9, 0.0);
            drawHead();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void Human::drawThigh() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0f, 0.0f, 0.6f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, THIGH_UP_RADIUS, THIGH_DOWN_RADIUS, TORSO_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawKnee() {
    glPushMatrix();
    {
        glColor3f(0.0f, 0.0f, 0.6f);
        glutSolidSphere(KNEE_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawShin() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0f, 0.0f, 0.6f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, SHIN_UP_RADIUS, SHIN_DOWN_RADIUS, SHIN_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawFoot() {
    glPushMatrix();
    {
        glColor3f(0.8, 0.8, 0.8);
        glScalef(0.4, 0.5, 1.5);
        glutSolidCube(1);
    }
    glPopMatrix();
}
