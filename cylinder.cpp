#include <iostream>
#include "cylinder.h"
#include <math.h>
#include <vector>

using namespace std;

Cylinder::Cylinder(float height, float radius) {
    this->height = height / 2;
    this->radius = radius;
};

void Cylinder::draw() {
    glPushMatrix();

    // draw immediate (center Cylinder)
    drawImmediate();

    // draw arrays (left Cylinder)
    //glTranslatef(-2, 0, 0);
    //drawArrays();

    // draw elements (right Cylinder)
    //glTranslatef(+4, 0, 0);
    //drawElements();

    glPopMatrix();
}

GLfloat * Cylinder::normalize(GLfloat vec[3]) {
    GLfloat l = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    vec[0] /= l;
    vec[1] /= l;
    vec[2] /= l;
    return vec;
}

// return normalized cross product of input vectors

GLfloat * Cylinder::cross(GLfloat a[3], GLfloat b[3]) {
    GLfloat * cross = (GLfloat *) malloc(3 * sizeof (GLfloat));
    cross[0] = (a[1] * b[2] - a[2] * b[1]);
    cross[1] = (a[2] * b[0] - a[0] * b[2]);
    cross[2] = (a[0] * b[1] - a[1] * b[0]);
    return cross;
}

// return unit normal to surface defined by the three input points

GLfloat * Cylinder::normal(GLfloat av[3], GLfloat bv[3], GLfloat cv[3]) {
    GLfloat vc0[3], vc1[3];
    for (int i = 0; i < 3; i++) {
        vc0[i] = bv[i] - av[i];
        vc1[i] = cv[i] - av[i];
    }
    return normalize(cross(vc0, vc1));
}

// Draw a circle variable in z axis

void Cylinder::drawCircle(float z) {
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, z);
    glVertex3f(0, 0, z);
    for (int n = 0; n <= NO_POINTS; ++n) {
        float const t = 2 * M_PI * (float) n / (float) NO_POINTS;
        glVertex3f(cos(t) * radius, sin(t) * radius, z);
    }
    glEnd();

}

//==================== 1. Immediate method ===================================
// immediate definition of individual vertex properties

void Cylinder::drawImmediate() {

    drawCircle(-height);
    drawCircle(height);

    for (int i = 0; i <= NO_POINTS; ++i) {
        float const t = 2 * M_PI * (float) (i) / (float) NO_POINTS;
        float const s = 2 * M_PI * (float) (i + 1) / (float) NO_POINTS;
        GLfloat leftDown[3] = {cos(t) * radius, sin(t) * radius, -height};
        GLfloat rightDown[3] = {cos(s) * radius, sin(s) * radius, -height};
        GLfloat leftUp[3] = {cos(t) * radius, sin(t) * radius, height};
        GLfloat rightUp[3] = {cos(s) * radius, sin(s) * radius, height};

        glBegin(GL_QUADS);
        glNormal3fv(normal(leftDown, rightDown, rightUp));
        glVertex3fv(leftDown);
        glVertex3fv(rightDown);
        glVertex3fv(rightUp);
        glVertex3fv(leftUp);
        glEnd();
    }
}


//==================== 2. Arrays - drawArrays ================================
// - store vertex-related data (coordinates, normals, colors, etc) in arrays
//   => reduces the number of function calls

// Define the coordinates of all vertices, ordered face by face.
// Since a vertex is shared by 3 faces, it is repeated 3 times!

/*static GLfloat allVertices[] = {
    +0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, +0.5, -0.5, +0.5, +0.5, -0.5, // 0 3 2 1
    +0.5, -0.5, -0.5, +0.5, -0.5, +0.5, -0.5, -0.5, +0.5, -0.5, -0.5, -0.5, // 0 4 7 3
    +0.5, +0.5, -0.5, +0.5, +0.5, +0.5, +0.5, -0.5, +0.5, +0.5, -0.5, -0.5, // 1 5 4 0
    -0.5, +0.5, -0.5, -0.5, +0.5, +0.5, +0.5, +0.5, +0.5, +0.5, +0.5, -0.5, // 2 6 5 1
    -0.5, -0.5, -0.5, -0.5, -0.5, +0.5, -0.5, +0.5, +0.5, -0.5, +0.5, -0.5, // 3 7 6 2
    +0.5, -0.5, +0.5, +0.5, +0.5, +0.5, -0.5, +0.5, +0.5, -0.5, -0.5, +0.5, // 4 5 6 7 
};

// Define the normals of vertices, using the same vertex order.
static GLfloat allNormals[] = {
    0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, // 0 3 2 1
    0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, // 0 4 7 3
    1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, // 1 5 4 0
    0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, // 2 6 5 1
    -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, // 3 7 6 2
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, // 4 5 6 7
};*/

void Cylinder::drawArrays() {

    // Fill the vertices of the down-circle

    GLfloat points[NO_POINTS * 6];
    GLfloat normals[NO_POINTS * 6];
    //vector<GLfloat> points,normals;
    int n = 0, a = 0;
    //D'abord on met les points du circle superieur 
    while (n < NO_POINTS) {
        int a = n * 3;
        float const t = 2 * M_PI * (float) n / (float) NO_POINTS;
        normals[a] = 0;
        points[a++] = cos(t) * radius;
        normals[a] = 0;
        points[a++] = sin(t) * radius;
        normals[a] = 1;
        points[a++] = 1;
        n++;
    }
    //Apres on met les points du circle inferieur
    while (n < NO_POINTS * 2) {
        cout << "n = " << n << "\n";
        //		system("clear");
        int a = n * 3;
        float const t = 2 * M_PI * (float) n / (float) NO_POINTS;
        normals[a] = 0;
        points[a++] = cos(t) * radius;
        normals[a] = 0;
        points[a++] = sin(t) * radius;
        normals[a] = -1;
        points[a++] = -1;
        n++;
    }

    for (int i = 0; i <= NO_POINTS; ++i) {
        float const t = 2 * M_PI * (float) (i) / (float) NO_POINTS;
        float const s = 2 * M_PI * (float) (i + 1) / (float) NO_POINTS;
        GLfloat leftDown[3] = {cos(t) * radius, sin(t) * radius, -1};
        GLfloat rightDown[3] = {cos(s) * radius, sin(s) * radius, -1};
        GLfloat rightUp[3] = {cos(s) * radius, sin(s) * radius, 1};

        GLfloat * normal1 = normal(leftDown, rightDown, rightUp);
    }


    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // specify the arrays to use
    glNormalPointer(GL_FLOAT, 0, normals);
    glVertexPointer(3, GL_FLOAT, 0, points);

    // draw quads using 24 data stored in arrays, starting at index 0
    glDrawArrays(GL_TRIANGLE_FAN, 0, NO_POINTS - 1);
    glDrawArrays(GL_TRIANGLE_FAN, NO_POINTS, NO_POINTS - 1);
    // disable the use of arrays (do not forget!)
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // activate the use of GL_VERTEX_ARRAY and GL_NORMAL_ARRAY
    /*glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
	
    // specify the arrays to use
    glNormalPointer(GL_FLOAT, 0, allNormals);
    glVertexPointer(3, GL_FLOAT, 0 , allVertices);
	
    // draw quads using 24 data stored in arrays, starting at index 0
    glDrawArrays(GL_QUADS, 0, 24);
	
    // disable the use of arrays (do not forget!)
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);*/

    // Others arrays could also be used: 
    // GL_COLOR_ARRAY, GL_INDEX_ARRAY, GL_TEXTURE_COORD_ARRAY, ...
};



//==================== 3. Arrays - drawElements ==============================
// - single definition of shared data
// - draw face by face, using face indices

// vertex coordinates (defined once only)
static GLfloat vertices[] = {
    +0.5, -0.5, -0.5, // 0
    +0.5, +0.5, -0.5, // 1
    -0.5, +0.5, -0.5, // 2
    -0.5, -0.5, -0.5, // 3
    +0.5, -0.5, +0.5, // 4
    +0.5, +0.5, +0.5, // 5
    -0.5, +0.5, +0.5, // 6
    -0.5, -0.5, +0.5, // 7
};

// indexes of the 6 faces (in a 2D array)
static GLubyte indices[6][4] = {
    {0, 3, 2, 1},
    {0, 4, 7, 3},
    {1, 5, 4, 0},
    {2, 6, 5, 1},
    {3, 7, 6, 2},
    {4, 5, 6, 7}
};

// normals (defined once only)
static GLfloat normals[6][3] = {
    { 0.0, 0.0, -1.0},
    { 0.0, -1.0, 0.0},
    { 1.0, 0.0, 0.0},
    { 0.0, 1.0, 0.0},
    {-1.0, 0.0, 0.0},
    { 0.0, 0.0, 1.0}
};

void Cylinder::drawElements() {
    // activate the use of GL_VERTEX_ARRAY (not GL_NORMALS_ARRAY)
    /*glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0 , vertices);
	
    // for each face: 
    //   - set the current normal (state machine)
    //   - draw a face using glDrawElements with the indices of the face
	
    glNormal3fv(normals[0]);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[0]);
    glNormal3fv(normals[1]);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[1]);
    glNormal3fv(normals[2]);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[2]);
    glNormal3fv(normals[3]);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[3]);
    glNormal3fv(normals[4]);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[4]);
    glNormal3fv(normals[5]);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[5]);

    glDisableClientState(GL_VERTEX_ARRAY);*/
}



