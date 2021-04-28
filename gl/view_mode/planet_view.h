#ifndef __PLANET_VIEW_H__
#define __PLANET_VIEW_H__

#include "glsl_objects.h"
#include "camera.h"
#include "model.h"
#include "figure.h"
#include "transform.hpp"
#include "geometry.h"
#include "light.h"

class ModelView: public GLFigure
{
	VAO vao;
	VBO vbo;
	EBO ebo;
	Shader shader;
	Model model;
	Camera camera;
	PointLight point_light;
	DirLight dir_light;
	MaterialMap material_map;

	mat4 screen_mat;

	float camera_handle_theta = pi/2;
	float camera_handle_phi = 0;
	float camera_handle_length = 4;

public:
	ModelView(int width = 800, int height = 600, const string& name = "", const Color& color = Color(0.8, 0.8, 0.8)) :
	GLFigure(width, height, name, color) {}

	void prepare()
	{
		shader.compile_vertex_code("glsl/Phong/vertex.glsl");
		shader.compile_fragment_code("glsl/Phong/fragment.glsl");
		shader.link();

		model = sphere(200, 100, sampler2D("images/planet_texture/earth/earth/earth_bump_10K.jpg"), 0.03);

	    update_camera();
	    model.setCamera(camera);
	    model.pitch(deg2rad(-23.5));

	    shader.uniform["camera_pos"] = camera.position;
	    shader.uniform["point_light"] = point_light;
	    shader.uniform["dir_light"] = dir_light;

	    material_map.ambient.addImage("images/planet_texture/earth/earth/earth_night_4K.jpg");
	    material_map.diffuse.addImage("images/planet_texture/earth/earth/earth_map_4k.jpg");
	    material_map.specular.addImage("images/planet_texture/earth/earth/earth_specular_4K.jpg");
	    material_map.mask.addImage("images/planet_texture/earth/earth/earth_cloudmap.png");
	    material_map.use_specular = true;
	    material_map.use_ambient = true;
	    material_map.use_mask = true;
	    shader.uniform["material_map"] = material_map;

		// Material material("turquoise");
		// material.ambient = vec3(12, 40, 63)/255.0f;
		// material.diffuse = vec3(14, 49, 78)/255.0f;
		// material.specular = vec3(1.0f, 1.0f, 1.0f);
		// material.shininess = 0.05;
		// shader.uniform["material"] = material;

	    update();
	}

	void draw()
	{
		// model.yaw_to(-0.1*toc());
		update();
		// vao.draw();
		shader.draw(model.layout);
	}

	void update_camera()
	{
		camera.position.x =  camera_handle_length * cos(camera_handle_phi) * sin(camera_handle_theta);
	    camera.position.y = -camera_handle_length * cos(camera_handle_phi) * cos(camera_handle_theta);
	    camera.position.z =  camera_handle_length * sin(camera_handle_phi);
	    camera.look_at(0, 0, 0);
	}

	void update()
	{
	    shader.uniform["transform_mat"] = screen_mat * camera.mat() * model.model_mat();
	    shader.uniform["model_mat"] = model.model_mat();
		shader.uniform["normal_mat"] = transpose(inverse(mat3(model.model_mat())));
		shader.uniform["dir_light"] = dir_light;
		shader.uniform["camera_pos"] = camera.position;
	}

	void change_size_callback(int width, int height)
	{
		camera.screen_width = width;
		camera.screen_height = height;
		update();
	}

	void double_click_callback()
	{
		screen_mat = mat4();
	    model.reset();
	    camera_handle_theta = 0;
		camera_handle_phi = 0;
	    update_camera();
	    dir_light.reset();
	    update();
	}

	void scroll_callback(int step)
	{
		screen_mat = screen_mat * tran::scale(1-step*0.2, 1-step*0.2, 1.0f);
		update();
	}

	void middle_drag_callback()
	{
		screen_mat = tran::translate(-2.0*dx()/__width, 2.0*dy()/__height, 0) * screen_mat;
		update();
	}

	void drag_callback()
	{
		// model.relative_pitch(0.01*dy());
		camera_handle_phi += 0.01*dy();
		if(camera_handle_phi > pi/2-1E-6)
		{
			camera_handle_phi = pi/2-1E-6;
		}
		else if(camera_handle_phi < -pi/2+1E-6)
		{
			camera_handle_phi = -pi/2+1E-6;
		}
		camera_handle_theta -= 0.01*dx();
		update_camera();
		update();
	}

	void key_repeat_callback(const string& key)
	{
		float d = 0.1;
		if(key == "W")
		{
			camera_handle_phi += d;
			if(camera_handle_phi > pi/2-1E-6)
			{
				camera_handle_phi = pi/2-1E-6;
			}

			update_camera();
		    update();
		}
		else if(key == "S")
		{
			camera_handle_phi -= d;
			if(camera_handle_phi < -pi/2+1E-6)
			{
				camera_handle_phi = -pi/2+1E-6;
			}

			update_camera();
		    update();
		}
		else if(key == "A")
		{
			model.yaw(d);
		    update();
		}
		else if(key == "D")
		{
			model.yaw(-d);
		    update();
		}
		else if(key == "UP")
		{
			dir_light.pitch(-d);
			if(dir_light.pitch() > deg2rad(23.5))
			{
				dir_light.pitch_to(deg2rad(23.5));
			}
			else if(dir_light.pitch() < -deg2rad(23.5))
			{
				dir_light.pitch_to(deg2rad(-23.5));
			}
			update();
		}
		else if(key == "DOWN")
		{
			dir_light.pitch(d);
			if(dir_light.pitch() > deg2rad(23.5))
			{
				dir_light.pitch_to(deg2rad(23.5));
			}
			else if(dir_light.pitch() < -deg2rad(23.5))
			{
				dir_light.pitch_to(deg2rad(-23.5));
			}
			update();
		}
	}
};

#endif