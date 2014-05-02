
#include <qapplication.h>
#include "viewer.h"
#include "terrain.h"
#include "human.h"
#include "cube.h"
#include "landscape.h"
#include "dynamicSystem.h"


int main(int argc, char** argv) {
    // Read command lines arguments.
    QApplication application(argc, argv);

    // Instantiate the viewer.
    Viewer viewer;

    // build your scene here

    //viewer.addRenderable(new Terrain(SIZE_TERRAIN));
    //viewer.addRenderable(new Human(0.5));
    Terrain * terrain = new Terrain(SIZE_TERRAIN);
    viewer.addRenderable(terrain);
    viewer.addRenderable(new Landscape());
    Human * human = new Human(1.0,Vec(0.0,5.0,0.0),Vec(0.0,0.0,HEIGHT_SCENE));
    viewer.addRenderable(new DynamicSystem(terrain,human));

    viewer.setWindowTitle("Projet Graphique 3D");
    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
