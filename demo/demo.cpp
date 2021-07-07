#include "glass/Scene3D"
#include "glass/utils/geometry.h"

int main()
{
	Scene3D scene;
	// Create Scene3D object

	scene.models["floor"] = glass::floor();
	// Add floor model

	scene.models["floor"].setTexture("floor.png");
	// Set floor texture as "floor.png"

	scene.models["duck"].load("duck/duck.dae");
	// Add duck model

	scene.models["duck"].zoom(0.01);
	// Zoom duck size to 0.01

	scene.dir_lights["light"].setDirection(-1, -1, -1);
	// Add a point light and move to position (2, 2, 2)

	scene.dir_lights["light"].useShadow();
	// Turn on shadow for this light,
	// otherwise all models won't have shadow.

	scene.camera.head_light.open();

	scene.show();
	// Show the scene
}