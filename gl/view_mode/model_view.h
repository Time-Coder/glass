#ifndef __MODEL_VIEW_H__
#define __MODEL_VIEW_H__

#include "glsl_objects.h"
#include "camera.h"
#include "model.h"
#include "figure.h"
#include "transform.hpp"
#include "geometry.h"
#include "light.h"

#include "type.h"
using namespace glass;

class ModelView: public GLFigure
{
	Shader skybox_shader;
	Shader shader;
	Model floor, earth, model;
	Model skybox;
	Camera camera;
	PointLight point_light;
	DirLight dir_light;
	samplerCube skybox_map;

	mat4 screen_mat;

	float camera_handle_theta = 0;
	float camera_handle_phi = 0;
	float camera_handle_length = 20;
	float sensitivity = 0.01;

public:
	ModelView(int width = 800, int height = 600, const string& name = "", const Color& color = Color(0.8, 0.8, 0.8)) :
	GLFigure(width, height, name, color) {}

	void prepare()
	{
		skybox_shader.compile_vertex_code("glsl/Skybox/vertex.glsl");
		skybox_shader.compile_fragment_code("glsl/Skybox/fragment.glsl");
		skybox_shader.link();

		shader.compile_vertex_code("glsl/Phong/vertex.glsl");
		shader.compile_fragment_code("glsl/Phong/fragment.glsl");
		shader.link();

		skybox = cube(2);
		model.load("resources/models/nanosuit_reflection/nanosuit.blend");

	    update_camera();
	    model.setCamera(camera);
	    model.center.y = 8;

	    // dir_light.direction(vec3(1, -1, 0));
	    shader.uniform["camera_pos"] = camera.position;
	    shader.uniform["point_light"] = point_light;
	    shader.uniform["dir_light"] = dir_light;

	    skybox_map = samplerCube(
	    	vector<string>(
	    		{
	    			"resources/skybox/right.jpg",
	    	        "resources/skybox/left.jpg",
	    	        "resources/skybox/top.jpg",
	    	        "resources/skybox/bottom.jpg",
	    	        "resources/skybox/front.jpg",
	    	        "resources/skybox/back.jpg"
	    	    }
	    	)
	    );
	    shader.uniform["skybox_map"] = skybox_map;
	    skybox_shader.uniform["skybox_map"] = skybox_map;
	    skybox_shader.uniform["projection_mat"] = camera.projection_mat();
	    skybox_shader.uniform["rotation_mat"] = camera.rotation_mat();

	    update();
	}

	void draw()
	{
		update();

		glDepthFunc(GL_LEQUAL);
		skybox_shader.draw(skybox);
		glDepthFunc(GL_LESS);
		
		// model.position.x = -10;
		// shader.uniform["type"] = 0;
		// shader.draw(model);
		// update();

		model.position.x = 0;
		shader.uniform["type"] = 0;
		shader.draw(model);
		update();

		// model.position.x = 10;
		// shader.uniform["type"] = 2;
		// shader.draw(model);
		// update();
	}

	void update_camera()
	{
		if(camera_handle_phi > pi/2-1E-6)
		{
			camera_handle_phi = pi/2-1E-6;
		}
		else if(camera_handle_phi < -pi/2+1E-6)
		{
			camera_handle_phi = -pi/2+1E-6;
		}
		camera.position.x = camera_handle_length * cos(camera_handle_phi) * sin(camera_handle_theta);
	    camera.position.z = camera_handle_length * cos(camera_handle_phi) * cos(camera_handle_theta);
	    camera.position.y = camera_handle_length * sin(camera_handle_phi);
	    camera.look_at(0, 0, 0);
	}

	void update()
	{
	    shader.uniform["transform_mat"] = screen_mat * camera.mat() * model.model_mat();
	    shader.uniform["model_mat"] = model.model_mat();
		shader.uniform["normal_mat"] = transpose(inverse(mat3(model.model_mat())));
		shader.uniform["dir_light"] = dir_light;
		shader.uniform["camera_pos"] = camera.position;
		skybox_shader.uniform["projection_mat"] = camera.projection_mat();
		skybox_shader.uniform["rotation_mat"] = camera.rotation_mat();
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
	    update();
	}

	void scroll_callback(int step)
	{
		sensitivity *= (1-step*0.2);
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
		model.relative_pitch(sensitivity*dy());
		model.yaw(sensitivity*dx());
		update();
	}

	void key_repeat_callback(const string& key)
	{
		double d = 0.1;
		if(key == "W")
		{
			camera_handle_phi += d;
			update_camera();
			update();
		}
		else if(key == "S")
		{
			camera_handle_phi -= d;
			update_camera();
			update();
		}
		else if(key == "A")
		{
			camera_handle_theta -= d;
			update_camera();
			update();
		}
		else if(key == "D")
		{
			camera_handle_theta += d;
			update_camera();
			update();
		}
	}
};

#endif