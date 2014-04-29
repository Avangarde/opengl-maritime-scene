/* 
 * File:   Terrain.h
 * Author: martijua
 *
 * Created on 26 mars 2014, 16:41
 */

#ifndef TERRAIN_H
#define	TERRAIN_H

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <vector>

#include "renderable.h"

//using namespace std;

class Terrain : public Renderable {
public:
    Terrain(int size);
    Terrain();
    Terrain(const Terrain& orig);
    virtual ~Terrain();

    int size;
    int solidId;
    static double unused;
    std::vector<std::vector<double> > d; // La grille d'élévation

    virtual void init(Viewer&);
    void createTerrain(int, int, int, int, double);
    void createList();
    qglviewer::Vec getNormal(int, int);

    void draw();

    double scale(double x) {
        return x * (((double) rand() / RAND_MAX) - 0.5);
    }
private:

};

#endif	/* TERRAIN_H */

