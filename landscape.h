/* 
 * File:   landscape.h
 * Author: rojascle
 *
 * Created on 4 avril 2014, 14:25
 */

#ifndef LANDSCAPE_H
#define	LANDSCAPE_H

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "renderable.h"

#define PI 3.14159265

class Landscape : public Renderable {
public:
    Landscape();
    void init(Viewer&);
    void draw();

private:
    void loadTexture(const char *filename);
        GLuint texture;
};

#endif	/* LANDSCAPE_H */

