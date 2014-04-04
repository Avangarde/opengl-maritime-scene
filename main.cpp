
#include <qapplication.h>
#include "viewer.h"
#include "terrain.h"
#include "human.h"
#include "cube.h"
#include "landscape.h"


int main(int argc, char** argv) {
    // Read command lines arguments.
    QApplication application(argc, argv);

    // Instantiate the viewer.
    Viewer viewer;

    // build your scene here

    //viewer.addRenderable(new Terrain(SIZE_TERRAIN));
    //viewer.addRenderable(new Human(0.5));
    viewer.addRenderable(new Terrain(100));
    viewer.addRenderable(new Landscape());
    viewer.addRenderable(new Cube(1));

    viewer.setWindowTitle("Projet Graphique 3D");
    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
