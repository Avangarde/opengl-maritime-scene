
#include <qapplication.h>
#include "viewer.h"
#include "terrain.h"
#include "human.h"
#include "cube.h"


int main(int argc, char** argv) {
    // Read command lines arguments.
    QApplication application(argc, argv);

    // Instantiate the viewer.
    Viewer viewer;

    // build your scene here

    //viewer.addRenderable(new Terrain(SIZE_TERRAIN));
    //viewer.addRenderable(new Human(0.5));
    glPushMatrix();
    glScalef(50.0f,50.0f,50.0f);
    //viewer.addRenderable(new Cube());
    glPopMatrix();

    viewer.setWindowTitle("Projet Graphique 3D");
    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
