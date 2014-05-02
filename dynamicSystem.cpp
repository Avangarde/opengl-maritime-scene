#include "viewer.h"
#include "dynamicSystem.h"

Vec goal;
int step = 0;

DynamicSystem::DynamicSystem(Terrain * terrain)
: defaultMediumViscosity(0.5), dt(0.05), fishMass(1.0), defaultGravity(0.0, 0.0, -10.0),terrain(terrain) {
    //this->terrain = terrain;
}

DynamicSystem::~DynamicSystem() {
    clear();
}

void DynamicSystem::clear() {
    vector<Fish *>::iterator itP;
    for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
        delete(*itP);
    }
    fishes.clear();
}

void DynamicSystem::setGravity(bool onOff) {
    gravity = (onOff ? defaultGravity : Vec());
}

void DynamicSystem::setViscosity(bool onOff) {
    mediumViscosity = (onOff ? defaultMediumViscosity : 0.0);
}

void DynamicSystem::init(Viewer&) {
    toggleGravity = true;
    toggleViscosity = true;
    clear();
    defaultGravity = Vec(0.0, 0.0, -10.0);
    gravity = defaultGravity;
    defaultMediumViscosity = 1.0;
    mediumViscosity = defaultMediumViscosity;
    for (int i = 0; i < 30; i++) {
        Vec initPos = Vec(((double) rand() / RAND_MAX)*40 - 20, ((double) rand() / RAND_MAX)*40 - 20, ((double) rand() / RAND_MAX)*20);
        Vec initVel = Vec(((double) rand() / RAND_MAX)*20 - 10, ((double) rand() / RAND_MAX)*20 - 10, ((double) rand() / RAND_MAX)*20 - 10);
        Vec initDir = initVel;
        fishes.push_back(new Fish(initPos, initVel, initDir, fishMass, 2.0, 4.0));
    }
    fishes[0]->setColour(Vec(1, 0.5, 0.5));
    fishes[0]->setPosition(Vec(10, 10, 10));
    goal = Vec(0, 0, 40);
}

void DynamicSystem::draw() {

    // Draw Goal
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(goal[0], goal[1], goal[2]);
    glutSolidSphere(0.1, 6, 6);
    glPopMatrix();


    vector<Fish *>::iterator itP;
    for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
        (*itP)->draw();
    }
}

void DynamicSystem::animate() {



    map<const Fish *, Vec> forces;
    vector<Fish *>::iterator itP;
    /*
    // forces
    for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
        Fish *f = *itP;
        forces[f] = (-mediumViscosity * f->getVelocity()) + f->getVelocity();
    }

    // update particles velocity
    // update particles positions
    for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
        Fish *f = *itP;
        // v = v + dt * a
        f->incrVelocity(dt * (forces[f] * f->getInvMass()));
        // q = q + dt * v
        f->incrPosition(dt * f->getVelocity());
    }
     */
    //Collisions
    for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
        Fish *f = *itP;
        collisionLimits(f);
        collisionParticleGround(f);
    }
/*
    for (unsigned int i = 0; i < fishes.size(); ++i) {
        for (unsigned int j = 0; j < fishes.size(); ++j) {
            if (i != j) {
                Fish *f1 = fishes[i];
                Fish *f2 = fishes[j];
                collisionFish(f1, f2);
            }
        }
    }
*/



    fishes[0]->animate((float) dt, 0, fishes, goal);
    for (unsigned int i = 1; i < fishes.size(); i++) {
        fishes[i]->animate(dt, i, fishes, fishes[0]->getPosition());
    }

    step++;
    if (step % 100 == 0) {
        goal[0] = (terrain->size * 4 * (rand() / (float) RAND_MAX)) - (terrain->size * 2);
        goal[1] = (terrain->size * 4 * (rand() / (float) RAND_MAX)) - (terrain->size * 2);
        goal[2] = (terrain->size * 2 * (rand() / (float) RAND_MAX));
        goal *= 0.9;
    }


}

void DynamicSystem::collisionParticleGround(Particle *p) {
    if (p->getInvMass() == 0)
        return;
    int xPos = p->getPosition().x / 4 + terrain->size / 2;
    int yPos = p->getPosition().y / 4 + terrain->size / 2;
    Vec groundNormal = Vec(0.0, 0.0, 1.0);
    Vec groundPosition = Vec(0.0, 0.0, terrain->rise[xPos][yPos]);
    // particle-plane distance
    double penetration = (p->getPosition() - groundPosition) * groundNormal;
    penetration -= p->getRadius();

    if (penetration >= 0)
        return;
    // penetration velocity
    double vPen = p->getVelocity() * groundNormal;

    // updates position and velocity of the particle
    p->incrPosition(-penetration * groundNormal);
    p->incrVelocity(-(1 + 1) * vPen * groundNormal);
}

void DynamicSystem::collisionLimits(Particle *p) {
    int xPos = p->getPosition().x;
    int yPos = p->getPosition().y;
    int zPos = p->getPosition().z;
    int size = terrain->size;
    if (xPos <= -size * 2 || xPos >= size * 2
            || yPos <= -size * 2 || yPos >= size * 2
            || zPos <= 0 || zPos >= size * 2) {
        p->setVelocity(p->getVelocity()*-1);
        p->incrPosition(p->getVelocity());
    }
}

void DynamicSystem::collisionFish(Fish* f1, Fish* f2) {
    if (f1->distance(f2) <= 0) {
        f1->setVelocity(f1->getVelocity()*-1);
        f2->setVelocity(f2->getVelocity()*-1);
    }
}

void DynamicSystem::keyPressEvent(QKeyEvent* e, Viewer& viewer) {
    // Get event modifiers key
    const Qt::KeyboardModifiers modifiers = e->modifiers();

    /* Controls added for Lab Session 6 "Physicall Modeling" */
    if ((e->key() == Qt::Key_G) && (modifiers == Qt::NoButton)) {
        toggleGravity = !toggleGravity;
        viewer.displayMessage("Set gravity to "
                + (toggleGravity ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_V) && (modifiers == Qt::NoButton)) {
        toggleViscosity = !toggleViscosity;
        viewer.displayMessage("Set viscosity to "
                + (toggleViscosity ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_Home) && (modifiers == Qt::NoButton)) {
        // stop the animation, and reinit the scene
        viewer.stopAnimation();
        init(viewer);
        //viewer.manipulatedFrame()->setPosition(getFixedParticlePosition());
        toggleGravity = true;
        toggleViscosity = true;
    }
}

void DynamicSystem::mouseMoveEvent(QMouseEvent*, Viewer& v) {
    //setFixedParticlePosition(v.manipulatedFrame()->position());
}
