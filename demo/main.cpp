#include <cmath>

#include "glass/Scene3D"
#include "glass/utils/geometry.h"

using namespace std;

int main()
{
	Scene3D scene(1000, 800);

	Model ball = glass::sphere();
	ball.setTexture("../resources/images/planet_texture/earth/earth/earth_map_1K.jpg");
	ball.zoom(0.5);

	scene.dir_lights["light"] = DirLight();
	scene.dir_lights["light"].setBrightness(2);
	scene.dir_lights["light"].setDirection(-1,-1,-1);
	// scene.dir_lights["light"].useShadow();
	// scene.dir_lights["light"].close();

	scene.point_lights["point_light"] = PointLight();
	scene.point_lights["point_light"].moveTo(2, 2, -2);
	scene.point_lights["point_light"].setCoverage(50);
	scene.point_lights["point_light"].setShape(ball);
	// scene.point_lights["point_light"].useShadow();

	scene.models["floor"] = glass::floor();
	scene.models["floor"].moveYTo(-0.01);
	scene.models["floor"].setTexture("../resources/images/block.jpg");

	scene.models["man"].load("../resources/models/nanosuit/nanosuit.blend");
	scene.models["man"].zoom(0.3);
	scene.models["man"].moveTo(-2.5, 0, -5);

	scene.models["duck"].load("../resources/models/duck/duck.dae");
	scene.models["duck"].moveTo(0, 0, -5);
	scene.models["duck"].zoom(0.01);

	scene.models["spider"].load("../resources/models/spider/spider.fbx");
	scene.models["spider"].moveTo(2.5, 0, -5);
	scene.models["spider"].zoom(0.01);

	scene.models["COLLADA"].load("../resources/models/COLLADA.dae");
	scene.models["COLLADA"].moveTo(-2.5, 0, -10);
	scene.models["COLLADA"].zoom(0.01);

	scene.models["box"] = glass::cube();
	scene.models["box"].moveTo(-1.5, 0.5, -10);
	scene.models["box"].setTexture("../resources/images/parallax/diffuse.png");
	scene.models["box"].setTexture("../resources/images/parallax/normal.png", Material::NORMAL);
	scene.models["box"].setTexture("../resources/images/parallax/depth.png", Material::BUMP, 0.1, true);

	scene.show();

	return 0;
}