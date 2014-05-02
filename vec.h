/* 
 * File:   vec.h
 * Author: edward
 *
 * Created on 30 avril 2014, 05:07
 */

#ifndef VEC_H
#define	VEC_H

#include <QGLViewer/vec.h>
using namespace qglviewer;
#include <cmath>
#include <iostream>

#define M_PI 3.14159265

// Radians/Degree conversion
float rad2deg( float rad ) { return rad * 180.0 / M_PI; }
float deg2rad( float deg ) { return deg * M_PI / 180.0; }

float magnitud2(const Vec &a)
{ return a[0]*a[0] + a[1]*a[1] + a[2]*a[2]; }

float magnitud(const Vec &a)
{ return std::sqrt(magnitud2(a)); }

void normalize(Vec &a) { 
	if ( magnitud(a) == 0 ) return;
	a*=1./magnitud(a);
}

Vec norm(Vec &a) { 
	Vec normVec = a;
	float magVec = magnitud(normVec);
	if ( magVec != 0 ) normVec *= 1./magVec;
	return normVec;
}

float dot(const Vec &a, const Vec &b)
{ return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; }


#endif	/* VEC_H */

