/* 
 * File:   utils.cpp
 * Author: juanmanuelmartinezromero
 * 
 * Created on 1 mai 2014, 17:30
 */

#include "utils.h"

Utils::Utils() {
}

Utils::Utils(const Utils& orig) {
}

Utils::~Utils() {
}

float Utils::lengthVec(Vec vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float Utils::dotProduct(Vec vec1, Vec vec2) {
    return vec1.x*vec2.x+vec1.y*vec2.y+vec1.z*vec2.z;
}

void Utils::drawCircle(float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 1.0);
    for (int n = 0; n <= NO_POINTS; ++n) {
        float const t = 2 * M_PI * (float) n / (float) NO_POINTS;
        glVertex3f(cos(t) * radius, sin(t) * radius, 0);
    }
    glEnd();
}


