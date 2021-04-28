#ifndef __SCENES_VIEW_H__
#define __SCENES_VIEW_H__

#include "glsl_objects.h"
#include "camera.h"
#include "model.h"
#include "figure.h"
#include "transform.hpp"
#include "geometry.h"
#include "light.h"

#include "type.h"
using namespace glass;

class ScenesView : public GLFigure
{
	Shader shader;
	Shader shadow_shader;
	Shader plane_shader;
	sampler2D sampler_test;

	Camera camera;
	Model ball, floor_model, man, plane_model;
	// FBO fbo = FBO(width(), height(), 8192, 8192);

	DirLight dir_light;
	vector<string> materials = Material::list();
	vector<string> planets_map =
	{
		"resources/images/planet_texture/sun/sunmap.jpg",
		"resources/images/planet_texture/mercury/mercurymap.jpg",
		"resources/images/planet_texture/venus/venusmap.jpg",
		"resources/images/planet_texture/earth/earth/earth_map_1K.jpg",
		"resources/images/planet_texture/mars/mars/marsmap1k.jpg",
		"resources/images/planet_texture/jupiter/jupitermap.jpg",
		"resources/images/planet_texture/saturn/saturnmap.jpg",
		"resources/images/planet_texture/uranus/uranusmap.jpg",
		"resources/images/planet_texture/neptune/neptunemap.jpg",
		"resources/images/planet_texture/pluto/plutomap1k.jpg",
	};

	bool can_move = true;
	int times = 0;

protected:
	void prepare()
	{
		glfwSetInputMode(window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		plane_shader.compile_vertex_code("glsl/Plane/vertex.glsl");
		plane_shader.compile_fragment_code("glsl/Plane/fragment.glsl");
		plane_shader.link();

		shadow_shader.compile_vertex_code("glsl/Shadow/vertex.glsl");
		shadow_shader.compile_fragment_code("glsl/Shadow/fragment.glsl");
		shadow_shader.link();
		shadow_shader.warning_off();

		shader.compile_vertex_code("glsl/Phong/vertex.glsl");
		shader.compile_fragment_code("glsl/Phong/fragment.glsl");
		shader.link();
		shader.uniform.define<Material>("Material");

		floor_model = floor();
		floor_model.material.set_diffuse_map("resources/images/parallax/diffuse2.png");
		// floor_model.material.diffuse_map.filter(GL_NEAREST);

		ball = sphere();
		// ball = cube(2);
		man.load("resources/models/nanosuit/nanosuit.blend");
		man.zoom(0.3);
		plane_model = plane();

		camera.screen_size(width(), height());
		camera.move_y_to(1);
		camera.move_z_to(10);

		dir_light.yaw(-pi/4);
		dir_light.pitch(-pi/4);
		// camera.addDirLight(dir_light);

		shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
		shader.uniform["dir_light"] = dir_light;
		shader.uniform["camera"] = camera;
	    shader.uniform["type"] = 0;

	    // fbo.enable_depth_sampler();
	}

	void draw()
	{
  //       fbo.bind();

		// shadow_shader.uniform["model_mat"] = floor_model.mat;
		// shadow_shader.draw(floor_model.layout);

		float distance = 6;

		// for(int x = -distance; x <= distance; x += distance)
		// {
		// 	for(int z = -distance; z <= distance; z += distance)
		// 	{
		// 		ball.move_to(x, 1, z);

		// 		shadow_shader.uniform["model_mat"] = ball.mat;
		// 		shadow_shader.draw(ball);
		// 	}
		// }

		// for(int x = -1.5*distance; x <= 1.5*distance; x += distance)
		// {
		// 	for(int z = -1.5*distance; z <= 1.5*distance; z += distance)
		// 	{
		// 		man.move_to(x, 0, z);

		// 		shadow_shader.uniform["model_mat"] = man.mat;
		// 		shadow_shader.draw(man);
		// 	}
		// }
		// fbo.unbind();

		// plane_shader.uniform["depth_map"] = fbo.depth_sampler();
		// plane_shader.draw(plane_model);

		// shader.uniform["dir_light"] = dir_light;
		// shader.uniform["dir_light"]["depth_map"] = fbo.depth_sampler();

		shader.uniform["type"] = 1;
		ball.material.set_diffuse_map(planets_map[0]);

		ball.move_to(0, distance, 0);
		shader.uniform["model"] = ball;
		shader.draw(ball);


		shader.uniform["type"] = 0;

		shader.uniform["model"] = floor_model;
		shader.draw(floor_model);

		int i = 1;
		for(int x = -distance; x <= distance; x += distance)
		{
			for(int z = -distance; z <= distance; z += distance)
			{
				ball.material.set_diffuse_map(planets_map[i]);
				ball.move_to(x, 1, z);

				shader.uniform["model"] = ball;
				shader.draw(ball);
				i++;
			}
		}

		for(int x = -1.5*distance; x <= 1.5*distance; x += distance)
		{
			for(int z = -1.5*distance; z <= 1.5*distance; z += distance)
			{
				man.move_to(x, 0, z);

				shader.uniform["model"] = man;
				shader.draw(man);
			}
		}
	}

	void change_size_callback(int width, int height)
	{
		camera.screen_size(width, height);
		// fbo.screen_size(width, height);
		shader.uniform["camera"] = camera;
		shader.uniform["dir_light"] = dir_light;
		shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
	}

	void scroll_callback(int step)
	{
		if(!can_move)
		{
			camera.screen_zoom(1-step*0.2);
			shader.uniform["camera"] = camera;
			shader.uniform["dir_light"] = dir_light;
			shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
		}
	}

	void double_click_callback()
	{
		if(!can_move)
		{
			camera.reset();
			shader.uniform["camera"] = camera;
			shader.uniform["dir_light"] = dir_light;
			shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
		}
	}

	void middle_drag_callback()
	{
		if(!can_move)
		{
			camera.screen_offset(dx(), dy());
			shader.uniform["camera"] = camera;
			shader.uniform["dir_light"] = dir_light;
			shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
		}
	}

	void mouse_move_callback()
	{
		if(can_move)
		{
			camera.yaw(-2.0*dx()/width());
			camera.pitch(-2.0*dy()/height());
			// if(camera.pitch()>pi/6)
			// {
			// 	camera.pitch_to(pi/6);
			// }
			// if(camera.pitch()<-pi/6)
			// {
			// 	camera.pitch_to(-pi/6);
			// }
			shader.uniform["camera"] = camera;
			shader.uniform["dir_light"] = dir_light;
			shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
		}
	}

	void key_repeat_callback(const string& key)
	{
		float d = 10*dt();
		if(key == "W")
		{
			if(can_move)
			{
				camera.move_forward(d);
				if(camera.position().y < camera.z_near()+1E-6)
				{
					camera.move_y_to(camera.z_near()+1E-6);
				}
				shader.uniform["camera"] = camera;
				shader.uniform["dir_light"] = dir_light;
				shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
			}
		}
		else if(key == "S")
		{
			if(can_move)
			{
				camera.move_back(d);
				if(camera.position().y < camera.z_near()+1E-6)
				{
					camera.move_y_to(camera.z_near()+1E-6);
				}
				shader.uniform["camera"] = camera;
				shader.uniform["dir_light"] = dir_light;
				shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
			}
		}
		else if(key == "A" || key == "LEFT")
		{
			if(can_move)
			{
				camera.move_left(d);
				if(camera.position().y < camera.z_near()+1E-6)
				{
					camera.move_y_to(camera.z_near()+1E-6);
				}
				shader.uniform["camera"] = camera;
				shader.uniform["dir_light"] = dir_light;
				shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
			}
		}
		else if(key == "D" || key == "RIGHT")
		{
			if(can_move)
			{
				camera.move_right(d);
				if(camera.position().y < camera.z_near()+1E-6)
				{
					camera.move_y_to(camera.z_near()+1E-6);
				}
				shader.uniform["camera"] = camera;
				shader.uniform["dir_light"] = dir_light;
				shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
			}
		}
		else if(key == "UP" || key == "E")
		{
			if(can_move)
			{
				camera.move_up(d);
				if(camera.position().y < camera.z_near()+1E-6)
				{
					camera.move_y_to(camera.z_near()+1E-6);
				}
				shader.uniform["camera"] = camera;
				shader.uniform["dir_light"] = dir_light;
				shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
			}
		}
		else if(key == "DOWN" || key == "C")
		{
			if(can_move)
			{
				camera.move_down(d);
				if(camera.position().y < camera.z_near()+1E-6)
				{
					camera.move_y_to(camera.z_near()+1E-6);
				}
				shader.uniform["camera"] = camera;
				shader.uniform["dir_light"] = dir_light;
				shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
			}
		}
		else if(key == "ESC")
		{
			can_move = false;
			glfwSetInputMode(window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if(key == "ENTER")
		{
			camera.screen_offset(0, 0);
			camera.screen_zoom(1);
			shader.uniform["camera"] = camera;
			shader.uniform["dir_light"] = dir_light;
			shadow_shader.uniform["dir_light_mat"] = dir_light.mat();
			can_move = true;
			glfwSetInputMode(window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

public:
	ScenesView(int width = 800, int height = 600, const string& name = "", const Color& color = Color(0.8, 0.8, 0.8)) :
	GLFigure(width, height, name, color) {}
};

#endif