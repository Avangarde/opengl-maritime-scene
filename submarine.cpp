#include "submarine.h"
#include "vec.h"

Submarine::Submarine(Vec pos, Vec vel, Vec dir, double m, double r, double h)
: Particle(pos, vel, m, r), direction(dir) {
    direction.normalize();
    height = h;
    propPosition = position - (direction*height);
}

Submarine::~Submarine() {
}

const Vec & Submarine::getPropPosition() const {
    return propPosition;
}

void Submarine::animate(float dt, Vec goal) {
    angleProp += 10;
    if (angleProp >= 360) {
        angleProp -= 360;
    }
    Vec dir = goal - position;
    dir.normalize();
    //dir *= 1/dt;
    Vec oldVel = velocity;
    velocity += dir * dt;
    position += oldVel * dt;
    propPosition = position - (direction*height);
    if (velocity[0] != 0 ||
            velocity[1] != 0 ||
            velocity[2] != 0) {
        direction = oldVel * dt;
        direction.normalize();
    }
}

void Submarine::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    
    // Rotate to point in direction
    float xzLen = sqrt(direction[0] * direction[0] + direction[2] * direction[2]);
    float yRot, xRot;
    if (xzLen == 0) {
        if (direction[0] > 0)
            yRot = 90;
        else
            yRot = -90;
    } else {
        yRot = rad2deg(acos(direction[2] / xzLen));
    }
    xRot = rad2deg(acos(xzLen));
    if (direction[1] > 0) xRot *= -1;
    if (direction[0] < 0) yRot *= -1;

    glRotatef(yRot, 0, 1, 0);
    glRotatef(xRot, 1, 0, 0);
    
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glScalef(1, 1, 4);
    glutSolidSphere(height / 4, 12, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -height);
    glScalef(1, 1, 2);
    glutSolidSphere(height/15, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -height);
    glRotatef(90, 0, 1, 0);
    glRotatef(angleProp, 1, 0, 0);
    glutSolidCone(height/30, height/5, 10, 10);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(height/30, height/5, 10, 10);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(height/30, height/5, 10, 10);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(height/30, height/5, 10, 10);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, height / 2);
    glTranslatef(height / 6, 0, 0);
    glRotatef(90, 0, 1, 0);
    glutSolidSphere(height/7, 10, 10);
    gluCylinder(gluNewQuadric(), height/60, height/60, height/2, 10, 10);
    glPopMatrix();
    
    glPopMatrix();
}