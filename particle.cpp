#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "particle.h"
#include <math.h>

Particle::Particle(Vec pos, Vec vel, double m, double r)
: position(pos),
velocity(vel),
mass(m),
radius(r) {
    invMass = (m > 0 ? 1 / m : 0.0);
}

Particle::~Particle() {
}

const Vec & Particle::getPosition() const {
    return position;
}

const Vec & Particle::getVelocity() const {
    return velocity;
}

double Particle::getMass() const {
    return mass;
}

double Particle::getInvMass() const {
    return invMass;
}

double Particle::getRadius() const {
    return radius;
}

void Particle::setRadius(double rad) {
    radius = rad;
}

void Particle::setPosition(const Vec &pos) {
    position = pos;
}

void Particle::setVelocity(const Vec &vel) {
    velocity = vel;
}

void Particle::incrPosition(const Vec &pos) {
    position += pos;
}

void Particle::incrVelocity(const Vec &vel) {
    velocity += vel;
}

void Particle::draw() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    if (radius != 0) {
        glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
        glutSolidSphere(radius, 12, 12);
    } else {
        glColor3f(1.0f, 0.89f, 0.8f);
        glPointSize(1.5);
        glBegin(GL_POINTS);
        glVertex3fv(Vec());
        glEnd();
    }
    glPopMatrix();

}

double Particle::distance(Particle* p) {
    //((a.x – b.x) * (a.x – b.x)) + ((a.y – b.y) * (a.y – b.y)) + ((a.z – b.z) * (a.z – b.z))
    Vec pos = p->getPosition() - getPosition();
    double distance = sqrt((pos.x * pos.x)+(pos.y * pos.y)+(pos.z * pos.z));
    distance = abs(distance) - getRadius() - p->getRadius();
    return distance;
}

std::ostream& operator<<(std::ostream& os, const Particle& p) {
    os << "pos (" << p.getPosition() << "), vel (" << p.getVelocity() << ")";
    return os;
}
