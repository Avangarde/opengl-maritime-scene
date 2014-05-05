#include <iostream>
#include <math.h>
#include "human.h"
#include "cylinder.h"
#include "particle.h"
#include "vec.h"

using namespace std;
using namespace qglviewer;

float Human::getScale() const {
    return scale;
}

void Human::setScale(float scale) {
    this->scale = scale;
}

Tube* Human::getTube() const {
    return tube;
}

void Human::setTube(Tube* tube) {
    this->tube = tube;
}

void Human::init(Viewer& viewer) {
    tube = new Tube(Vec(0.0,0.0,HEIGHT_SCENE));
    tube->init(viewer);
}

void Human::draw() {
    glColor3f(119.0f, 136.0f, 153.0f);
    tube->draw();
    glPushMatrix();
    {
        //glRotatef(90, 1, 0, 0);
        glTranslatef(position.x, position.y, position.z);
        // Rotate to point in direction
        float xyLen = sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
        float zRot, xRot;
        if (xyLen == 0) {
            if (direction[0] > 0)
                zRot = 90;
            else
                zRot = -90;
        } else {
            zRot = rad2deg(acos(direction[1] / xyLen));
        }
        xRot = rad2deg(acos(xyLen));
        if (direction[2] < 0) xRot *= -1;
        if (direction[0] > 0) zRot *= -1;

        glRotatef(zRot, 0, 0, 1);
        glRotatef(xRot, 1, 0, 0);


        glScalef(scale, scale, scale);
        drawUpperTorso();
        drawLowerTorso();
    }
    glPopMatrix();
}

void Human::animate(float dt, Vec goal) {
    Vec dir = goal - position;
    normalize(dir);
    //dir *= 1/dt;
    Vec oldVel = velocity;
    velocity += dir * dt;
    position += oldVel * dt;
    if (velocity[0] != 0 ||
            velocity[1] != 0 ||
            velocity[2] != 0) {
        direction = oldVel * dt;
        normalize(direction);
    }
    if (incrShoulder) {
        if (shoulderAngle < 180)shoulderAngle += 2;
        else incrShoulder = false;
    } else {
        if (shoulderAngle > 45)shoulderAngle -= 2;
        else incrShoulder = true;
    }
}

void Human::drawUpperTorso() {
    drawTorso();
    drawFullHead();
    drawFullShoulder(true);
    drawFullShoulder(false);
}

void Human::drawFullShoulder(bool right) {
    int rightArm = right ? 1 : -1;
    glPushMatrix();
    {
        glTranslatef(rightArm*SHOULDER_DISTANCE, -1 * SHOULDER_RADIUS, 0.0);
        glRotatef(rightArm * shoulderAngle, 0, 0, 1);
        drawFullArm(rightArm * SHOULDER_POSITION);
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
            glTranslatef(2.2, -1 * HIP_RADIUS, 0.0);
            drawFullLeg(true);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(0.7, -1 * HIP_RADIUS, 0.0);
            drawFullLeg(false);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void Human::drawHip() {
    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(0, 0, HIP_HEIGHT / 2);
        glutSolidSphere(HIP_HEIGHT / 2, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawTorso() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0f, 0.0f, 0.0f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, TORSO_UP_RADIUS, TORSO_DOWN_RADIUS, TORSO_HEIGHT, PRECISION, PRECISION);
        Utils::drawCircle(TORSO_UP_RADIUS);
    }
    glPopMatrix();
}

void Human::drawHead() {
    glPushMatrix();
    {
        glColor3f(1.0f, 0.64f, 0.52f);
        glutSolidSphere(HEAD_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawNeck() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0f, 0.0f, 0.0f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, 0.40f, 0.40f, 0.30, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawShoulder() {
    glPushMatrix();
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidSphere(SHOULDER_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawForearm() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0f, 0.0f, 0.0f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, FOREARM_DOWN_RADIUS, FOREARM_UP_RADIUS, FOREARM_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawElbow() {
    glPushMatrix();
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidSphere(FOREARM_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawArm() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0f, 0.0f, 0.0f);
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

void Human::drawFullArm(float shoulderPosition) {

    glPushMatrix();
    {
        drawShoulder();
        glRotatef(shoulderPosition, 0, 0, 1);
        glPushMatrix();
        {
            drawForearm();
            glPushMatrix();
            {
                glTranslatef(0.0, -2.0, 0.0);
                drawElbow();
                glPushMatrix();
                {
                    int isRight = shoulderPosition < 0 ? 1 : -1;
                    glRotatef(isRight * (shoulderAngle / 4), 0, 0, 1);
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

void Human::drawFullLeg(bool right) {
    int isRightLeg = right ? 1 : -1;
    glPushMatrix();
    {
        glRotatef(-20, 1, 0, 0);
        glRotatef(isRightLeg * ((shoulderAngle / 4) - 12), 0, 0, 1);
        drawThigh();
        glPushMatrix();
        {
            glTranslatef(0.0, -3.7, 0.0);
            glRotatef(-isRightLeg * shoulderAngle / 4, 0, 0, 1);
            drawKnee();
            glPushMatrix();
            {
                glTranslatef(0.0, -0.4, 0.0);
                drawShin();
                glPushMatrix();
                {
                    glTranslatef(0.0, -2.2, -0.5);
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
        glColor3f(0.0f, 0.0f, 0.0f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, THIGH_UP_RADIUS, THIGH_DOWN_RADIUS, TORSO_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawKnee() {
    glPushMatrix();
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidSphere(KNEE_RADIUS, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawShin() {
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0f, 0.0f, 0.0f);
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, SHIN_UP_RADIUS, SHIN_DOWN_RADIUS, SHIN_HEIGHT, PRECISION, PRECISION);
    }
    glPopMatrix();
}

void Human::drawFoot() {
    glPushMatrix();
    {
        glColor3f(0.8, 0.8, 0.8);
        glRotatef(-45, 1, 0, 0);
        glScalef(0.6, 0.3, 2.2);
        glutSolidCube(1);
    }
    glPopMatrix();
}
