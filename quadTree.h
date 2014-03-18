/* 
 * File:   quadTree.h
 * Author: juanmanuelmartinezromero
 *
 * Created on 18 mars 2014, 20:05
 */

#ifndef QUADTREE_H
#define	QUADTREE_H

#include "terrain.h"

class QuadTree : public Terrain {
public:
    QuadTree();
    QuadTree(const QuadTree& orig);
    virtual ~QuadTree();

    void init(void);
private:
    unsigned char* quadMatrix;

    int getMatrixIndex(int x, int z) {
        return ( (z * sizeHeightMap) + x);
    }
};

#endif	/* QUADTREE_H */

