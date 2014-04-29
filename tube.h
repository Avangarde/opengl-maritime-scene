/* 
 * File:   tube.h
 * Author: juanmanuelmartinezromero
 *
 * Created on 29 avril 2014, 20:43
 */

#ifndef TUBE_H
#define	TUBE_H

#include "cylinder.h"
#include "spring.h"
#include "renderable.h"
#include <QGLViewer/vec.h>
#include <vector>

using namespace qglviewer;
using namespace std;

class Tube : public Renderable {
public:

    Tube();

    Tube(Vec begin) : beginningTube(begin) {
    }
    Tube(const Tube& orig);
    virtual ~Tube();

    virtual void init(Viewer&);
    virtual void draw();

private:
    static const int PRECISION_PIPE = 20;
    static const float RADIUS_PIPE = 0.2;
    Vec beginningTube;
    vector<Cylinder *> cylinders;
    vector<Spring *> springs;
};

#endif	/* TUBE_H */

