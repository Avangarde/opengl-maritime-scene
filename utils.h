/* 
 * File:   utils.h
 * Author: juanmanuelmartinezromero
 *
 * Created on 1 mai 2014, 17:29
 */

#ifndef UTILS_H
#define	UTILS_H

#include <QGLViewer/vec.h>

using namespace qglviewer;

class Utils {
public:
    Utils();
    Utils(const Utils& orig);
    virtual ~Utils();
    static float lengthVec(Vec vec);
    static float dotProduct(Vec vec1,Vec vec2);
    static void drawCircle(float);
private:
    static const int NO_POINTS=16;
};

#endif	/* UTILS_H */

