#include "fish.h"


Fish::Fish(Vec pos, Vec vel, double m, double r, double h)
: Particle(pos, vel, m, r){
    height = h;
}

Fish::~Fish() {
}

void Fish::draw() const
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
        glRotatef(90.0, 1.0, 0.0, 1.0);
	//glutSolidCone(radius, height, 12, 12);
        glutSolidSphere(radius, 12, 12);
	glPopMatrix();
}