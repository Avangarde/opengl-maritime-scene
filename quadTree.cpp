/* 
 * File:   quadTree.cpp
 * Author: juanmanuelmartinezromero
 * 
 * Created on 18 mars 2014, 20:05
 */

#include "quadTree.h"

QuadTree::QuadTree() {
    this->init();
}

QuadTree::QuadTree(const QuadTree& orig) {
}

QuadTree::~QuadTree() {
}

void QuadTree::init(void) {
    quadMatrix = new unsigned char [sizeHeightMap * sizeHeightMap];
    for (int z = 0; z < sizeHeightMap; z++) {
        for (int x = 0; x < sizeHeightMap; x++) {
            quadMatrix[getMatrixIndex(x, z)] = 1;
        }
    }
}

