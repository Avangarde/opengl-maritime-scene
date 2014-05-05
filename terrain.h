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

    bool toggleCaustics;
    bool handleCaustics;
    
    int size;
    int solidId;
    static double unused;
    std::vector<std::vector<double> > rise; // La grille d'élévation
    std::vector<std::vector<qglviewer::Vec> > normals;

    virtual void init(Viewer&);
    void createTerrain(int, int, int, int, double);
    qglviewer::Vec getNormal(int, int);

    void draw();
    void drawTerrain();
    void animate();
    void setCaustics(bool onOff);

    double scale(double x) {
        return x * (((double) rand() / RAND_MAX) - 0.5);
    }
private:

};

#endif	/* TERRAIN_H */

