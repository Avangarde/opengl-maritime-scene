#include "viewer.h"
#include "dynamicSystem.h"

DynamicSystem::DynamicSystem(Terrain * terrain, Human * human)
: defaultMediumViscosity(0.5), dt(0.05), fishMass(1.0),
defaultGravity(0.0, 0.0, -10.0), terrain(terrain), human(human) {
    step = 0;
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

void DynamicSystem::setCollisionsDetection(bool onOff) {
    handleCollisions = onOff;
}

void DynamicSystem::setHuman(bool onOff) {
    handleHuman = onOff;
}

void DynamicSystem::setFishes(bool onOff) {
    handleFishes = onOff;
}

void DynamicSystem::setBubbles(bool onOff) {
    handleBubbles = onOff;
}

void DynamicSystem::setSand(bool onOff) {
    handleSand = onOff;
}

void DynamicSystem::init(Viewer& viewer) {
    toggleGravity = true;
    toggleViscosity = true;
    toggleCollisions = true;
    toggleHuman = true;
    toggleFishes = true;
    toggleBubbles = true;
    toggleSand = true;
    clear();
    defaultGravity = Vec(0.0, 0.0, -10.0);
    gravity = defaultGravity;
    defaultMediumViscosity = 1.0;
    mediumViscosity = defaultMediumViscosity;
    handleCollisions = true;
    handleBubbles=true;
    handleFishes=true;
    handleHuman=true;
    handleSand=true;
    human->init(viewer);
    human->getTube()->getParticles()[0]->setPosition(Vec(0.0, 0.0, HEIGHT_SCENE));
    viewer.setManipulatedFrame(new qglviewer::ManipulatedFrame());
    viewer.manipulatedFrame()->setPosition(human->getTube()->getBeginningTube());

    for (int i = 0; i < 30; i++) {
        Vec initPos = Vec(((double) rand() / RAND_MAX)*40 - 20, ((double) rand() / RAND_MAX)*40 - 20, ((double) rand() / RAND_MAX)*20);
        Vec initVel = Vec(((double) rand() / RAND_MAX)*20 - 10, ((double) rand() / RAND_MAX)*20 - 10, ((double) rand() / RAND_MAX)*20 - 10);
        Vec initDir = initVel;
        fishes.push_back(new Fish(initPos, initVel, initDir, fishMass, 2.0, 4.0));
    }
    fishes[0]->setColour(Vec(1.0, 0.0, 0.0));
    fishes[0]->setPosition(Vec(10, 10, 10));
    goal = Vec(40, 40, 10);
    humanGoal = Vec(-80, -80, 10);
    //Submarine
    Vec pos = Vec(150, 50, 30);
    Vec vel = Vec(-6, -2, 0);
    Vec dir = Vec(-6, -2, 0);
    submarine = new Submarine(pos, vel, dir, 50.0, 40.0, 60.0);
    submarineGoal = Vec(-150, -50, 30);
    toggleSubmarine = false;
}

void DynamicSystem::createBubbles(Vec origin, Vec vel, int maxBubbles, double maxRad) {
    int nbBubbles = (int) (maxBubbles * ((double) rand() / RAND_MAX) + 5);
    vector<Particle *> newBubbles;
    for (int i = 0; i < nbBubbles; i++) {
        Vec initPos = Vec(origin.x + 2 * ((double) rand() / RAND_MAX) - 1, origin.y + 2 * ((double) rand() / RAND_MAX) - 1, origin.z + 2 * ((double) rand() / RAND_MAX));
        Vec initVel = Vec(vel.x, vel.y, vel.z + 2 * ((double) rand() / RAND_MAX));
        double radius = maxRad * ((double) rand() / RAND_MAX);
        newBubbles.push_back(new Particle(initPos, initVel, 0.0, radius));
    }
    bubbles.push_back(newBubbles);
}

void DynamicSystem::createSand(Vec origin, Vec vel, int maxSand) {
    int nbSand = (int) (maxSand * ((double) rand() / RAND_MAX) + 10);
    vector<Particle *> newSand;
    for (int i = 0; i < nbSand; i++) {
        int rad = 4 * (((double) rand() / RAND_MAX));
        float posx = 2 * rad * ((double) rand() / RAND_MAX) - rad;
        float posy = sqrt(rad * rad - posx * posx);
        Vec initPos = Vec(origin.x + posx, origin.y + posy, origin.z + 1 * ((double) rand() / RAND_MAX));
        Vec initPos2 = Vec(origin.x + posx, origin.y - posy, origin.z + 1 * ((double) rand() / RAND_MAX));
        Vec initVel = Vec(vel.x / 4, vel.y / 4, vel.z / 8 * ((double) rand() / RAND_MAX));
        newSand.push_back(new Particle(initPos, initVel, 0.0, 0.0));
        newSand.push_back(new Particle(initPos2, initVel, 0.0, 0.0));
    }
    sand.push_back(newSand);
}

void DynamicSystem::draw() {
    if (handleHuman) human->draw();
    if (handleFishes) {
        vector<Fish *>::iterator itP;
        for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
            (*itP)->draw();
        }
    }
    if (handleBubbles) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (unsigned int i = 0; i < bubbles.size(); i++) {
            for (unsigned int j = 0; j < bubbles[i].size(); j++) {
                bubbles[i][j]->draw();
            }
        }
        glDisable(GL_BLEND);
    }
    if (handleSand) {
        glColor3f(1.0f, 0.89f, 0.8f);
        for (unsigned int i = 0; i < sand.size(); i++) {
            for (unsigned int j = 0; j < sand[i].size(); j++) {
                sand[i][j]->draw();
            }
        }
    }
    glDisable(GL_BLEND);
    
    if (toggleSubmarine) {
        submarine->draw();
    }

}

void DynamicSystem::animate() {
    map<const Particle *, Vec> forcesTube;
    if (handleFishes) {
        map<const Fish *, Vec> forces;
        vector<Fish *>::iterator itP;

        // forces
        for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
            Fish *f = *itP;
            forces[f] = (-mediumViscosity * f->getVelocity()); /*+ gravity * f->getMass();*/
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

        //Collisions
        if (handleCollisions) {
            for (itP = fishes.begin(); itP != fishes.end(); ++itP) {
                Fish *f = *itP;
                collisionLimits(f);
                collisionParticleGround(f);
            }
        }

        Vec goalSchool = fishes[0]->getPosition();
        fishes[0]->animate((float) dt, 0, fishes, goal);
        for (unsigned int i = 1; i < fishes.size(); i++) {
            fishes[i]->animate(dt, i, fishes, goalSchool);
        }
    }
    if (handleHuman) human->animate(dt, humanGoal);
    step++;
    if (step % 5 == 0 && handleFishes) {
        createBubbles(fishes[step % fishes.size()]->getPosition(), fishes[step % fishes.size()]->getVelocity(), 10, 0.3);
    }
    if (step == 50 && handleHuman) {
        createBubbles(human->getPosition(), human->getVelocity(), 20, 0.5);
    }
    if (step == 100 && handleHuman) {
        goal[0] = (terrain->size * 4 * (rand() / (float) RAND_MAX)) - (terrain->size * 2);
        goal[1] = (terrain->size * 4 * (rand() / (float) RAND_MAX)) - (terrain->size * 2);
        goal[2] = (terrain->size * 1.5 * (rand() / (float) RAND_MAX));
        goal *= 0.95;
        float factor = ((terrain->size * 2) - goal[2]) / (terrain->size * 2);
        goal[0] *= factor;
        goal[1] *= factor;
        humanGoal[0] = (terrain->size * 4 * (rand() / (float) RAND_MAX)) - (terrain->size * 2);
        humanGoal[1] = (terrain->size * 4 * (rand() / (float) RAND_MAX)) - (terrain->size * 2);
        humanGoal[2] = ((terrain->size - 10) * (rand() / (float) RAND_MAX) + 10);
        humanGoal *= 0.70;
        factor = ((terrain->size * 2) - humanGoal[2]) / (terrain->size * 2);
        humanGoal[0] *= factor;
        humanGoal[1] *= factor;
        step = 0;
    }
    if (handleBubbles) animateBubbles();
    if (handleSand) animateSand();

    if (handleHuman) {
        // Calcul de forces pour le tube
        human->getTube()->getParticles().back()->setPosition(human->getPosition());
        human->getTube()->getParticles().back()->setVelocity(human->getVelocity());
        for (size_t i = 0; i < human->getTube()->getParticles().size(); i++) {
            Particle *p = human->getTube()->getParticles()[i];
            forcesTube[p] = gravity * p->getMass() - mediumViscosity * p->getVelocity();
        }

        // Calcul de forces pour les springs du tube
        for (size_t i = 0; i < human->getTube()->getSprings().size(); i++) {
            Spring *s = human->getTube()->getSprings()[i];
            Vec f12 = s->getCurrentForce();
            forcesTube[s->getParticle1()] += f12;
            forcesTube[s->getParticle2()] -= f12; // opposite force
        }

        // Calcul de vitesse pour le tube
        for (size_t i = 0; i < human->getTube()->getParticles().size(); i++) {
            Particle *p = human->getTube()->getParticles()[i];
            // q = q + dt * v
            if (i != 0) {
                p->incrPosition(dt * p->getVelocity());
                p->incrVelocity(dt * forcesTube[p] * p->getInvMass());
            }
        }

        // Collisions pour le tube
        if (handleCollisions) {
            for (size_t i = 0; i < human->getTube()->getParticles().size(); i++) {
                collisionParticleGround(human->getTube()->getParticles()[i]);
                if (i == human->getTube()->getParticles().size() - 1) {
                    Vec pos = human->getTube()->getParticles().back()->getPosition();
                    human->setPosition(pos);
                    collisionParticleGround(human);
                    collisionLimits(human);
                }
            }
        }
    }
    
    if (toggleSubmarine) {
        submarine->animate(dt, submarineGoal);
        createBubbles(submarine->getPropPosition(), submarine->getVelocity()*-1, 10, 0.5);
    }
    
}

void DynamicSystem::animateBubbles() {
    vector< vector<Particle *> >::iterator it1;
    for (it1 = bubbles.begin(); it1 != bubbles.end(); ++it1) {
        vector<Particle *>::iterator it2;
        for (it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
            if ((*it2)->getPosition().z >= terrain->size * 2) {
                (*it1).erase((it2));
                it2--;
            } else {
                double chX;
                double chY;
                if ((*it2)->getVelocity().x > 0) {
                    chX = -1.0;
                } else {
                    chX = 1.0;
                }
                if ((*it2)->getVelocity().y > 0) {
                    chY = -1.0;
                } else {
                    chY = 1.0;
                }
                (*it2)->incrVelocity(dt * Vec(chX, chY, 1.0));
                (*it2)->incrPosition(dt * (*it2)->getVelocity());
            }
        }
    }
    //Collisions
    if (handleCollisions) {
        for (unsigned int i = 0; i < bubbles.size(); ++i) {
            for (unsigned int j = 1; j < bubbles[i].size(); ++j) {
                Particle *p1 = bubbles[i][j - 1];
                Particle *p2 = bubbles[i][j];
                if (p1->distance(p2) <= 0) {
                    p1->setRadius(p1->getRadius() + p2->getRadius());
                    p2->setRadius(0);
                }
            }
        }
    }
}

void DynamicSystem::animateSand() {
    vector< vector<Particle *> >::iterator it1;
    for (it1 = sand.begin(); it1 != sand.end(); ++it1) {
        vector<Particle *>::iterator it2;
        for (it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
            if ((*it2)->getPosition().z < 0) {
                (*it1).erase((it2));
                it2--;
            } else {
                (*it2)->incrVelocity(dt * Vec(0, 0, -1));
                (*it2)->incrPosition(dt * (*it2)->getVelocity());
            }
        }
    }
}

void DynamicSystem::collisionParticleGround(Particle *p) {
    if (p->getInvMass() == 0)
        return;
    int xPos = (int) (p->getPosition().x / 4 + terrain->size / 2) % (int) terrain->size;
    int yPos = (int) (p->getPosition().y / 4 + terrain->size / 2) % (int) terrain->size;
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
    p->incrVelocity(-2 * vPen * groundNormal);

    if (handleSand)createSand(p->getPosition(), p->getVelocity(), 200);
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

void DynamicSystem::keyPressEvent(QKeyEvent* e, Viewer& viewer) {
    // Get event modifiers key
    const Qt::KeyboardModifiers modifiers = e->modifiers();

    /* Controls added for Lab Session 6 "Physicall Modeling" */
    if ((e->key() == Qt::Key_G) && (modifiers == Qt::NoButton)) {
        toggleGravity = !toggleGravity;
        viewer.displayMessage("Set gravity : "
                + (toggleGravity ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_V) && (modifiers == Qt::NoButton)) {
        toggleViscosity = !toggleViscosity;
        viewer.displayMessage("Set viscosity : "
                + (toggleViscosity ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_C) && (modifiers == Qt::NoButton)) {
        toggleCollisions = !toggleCollisions;
        setCollisionsDetection(toggleCollisions);
        viewer.displayMessage("Detect collisions : "
                + (toggleCollisions ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_P) && (modifiers == Qt::NoButton)) {
        toggleHuman = !toggleHuman;
        setHuman(toggleHuman);
        viewer.displayMessage("Placing human : "
                + (toggleHuman ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_F) && (modifiers == Qt::NoButton)) {
        toggleFishes = !toggleFishes;
        setFishes(toggleFishes);
        viewer.displayMessage("Placing fishes : "
                + (toggleFishes ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_B) && (modifiers == Qt::NoButton)) {
        toggleBubbles = !toggleBubbles;
        setBubbles(toggleBubbles);
        viewer.displayMessage("Placing bubbles : "
                + (toggleBubbles ? QString("true") : QString("false")));

    } else if ((e->key() == Qt::Key_D) && (modifiers == Qt::NoButton)) {
        toggleSand = !toggleSand;
        setSand(toggleSand);
        viewer.displayMessage("Placing sand : "
                + (toggleSand ? QString("true") : QString("false")));
    } else if ((e->key() == Qt::Key_K) && (modifiers == Qt::NoButton)) {
        terrain->toggleCaustics = !terrain->toggleCaustics;
        terrain->setCaustics(terrain->toggleCaustics);
        viewer.displayMessage("Caustics : "
                + (terrain->toggleCaustics ? QString("true") : QString("false")));
    } else if ((e->key() == Qt::Key_U) && (modifiers == Qt::NoButton)) {
        toggleSubmarine = !toggleSubmarine;
        submarine->setPosition(Vec(120, 50, 30));
        viewer.displayMessage("Placing Submarine "
                + (toggleSubmarine ? QString("true") : QString("false")));
    } else if ((e->key() == Qt::Key_Home) && (modifiers == Qt::NoButton)) {
        // stop the animation, and reinit the scene
        viewer.stopAnimation();
        init(viewer);
        //viewer.manipulatedFrame()->setPosition(getFixedParticlePosition());
        toggleGravity = true;
        toggleViscosity = true;
        toggleCollisions = true;
        toggleHuman = true;
        toggleFishes = true;
        toggleSand = true;
        toggleBubbles = true;
        terrain->toggleCaustics = true;
        
    }
}

void DynamicSystem::mouseMoveEvent(QMouseEvent*, Viewer& v) {
    //human->getTube()->getParticles().back()->setPosition(v.manipulatedFrame()->position());
}
