/* 
 * File:   terrain.h
 * Author: juanmanuelmartinezromero
 *
 * Created on 18 mars 2014, 20:13
 */

#ifndef TERRAIN_H
#define	TERRAIN_H

#include "renderable.h"


class Terrain:public Renderable {
public:
    Terrain();
    Terrain(const Terrain& orig);
    virtual ~Terrain();
private:

protected:
    int sizeHeightMap;

};

#endif	/* TERRAIN_H */

