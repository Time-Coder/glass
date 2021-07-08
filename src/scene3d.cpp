#include <cmath>

#include "glass/Scene3D"
#include "glass/utils/path.h"
#include "glass/utils/geometry.h"
#include "glass/utils/shaders.h"
#include "glass/sampler2D"
#include "glass/RBO"

#ifdef USE_QT
#include <QDebug>
#include <QCursor>
#include <QPoint>
#endif

using namespace std;

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

template<int n>
void gauss_kernel(float sigma, float* weight)
{
	double sum = 0;
	double k = -0.5 / sigma / sigma;
	for(int i = 0; i < n; i++)
	{
		weight[i] = exp(k * i*i);
		sum += (i == 0 ? 1 : 2) * weight[i];
	}
	for(int i = 0; i < n; i++)
	{
		weight[i] /= sum;
	}
}

int step(double x)
{
	return x > 0 ? 1 : 0;
}

void Scene3D::init_debug()
{
	if(shader.count("debug"))
	{
		return;
	}

	shader["debug"].compile(Shader::VERTEX, glass::debug_vertex_shader);
	shader["debug"].compile(Shader::FRAGMENT, glass::debug_fragment_shader);
	shader["debug"].link();

	fbo["debug"].bufferResize(width(), height());
	fbo["debug"].screenResize(width(), height());
	#ifdef USE_QT
	fbo["debug"].setPaintDevice(this);
	#endif
	fbo["debug"].attach(FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGBA16F, GL_FLOAT));

	if(model.count("plane") == 0)
	{
		model["plane"] = glass::plane();
	}
}

void Scene3D::init_skybox()
{
	if(shader.count("skybox"))
	{
		return;
	}

	shader["skybox"].compile(Shader::VERTEX, glass::skybox_vertex_shader);
	shader["skybox"].compile(Shader::FRAGMENT, glass::skybox_fragment_shader);
	shader["skybox"].link();
	shader["skybox"].uniform["skybox_map"] = skybox_map;
	shader["skybox"].uniform["camera"].bind(camera);
	shader["skybox"].warningOff();
	model["skybox"] = glass::cube(2);
}

void Scene3D::init_shadering_gbuffer()
{
	if(shader.count("gbuffer"))
	{
		return;
	}

	shader["gbuffer"].compile(Shader::VERTEX, glass::gbuffer_vertex_shader);
	shader["gbuffer"].compile(Shader::FRAGMENT, glass::gbuffer_fragment_shader);
	shader["gbuffer"].link();
	shader["gbuffer"].uniform.define<Material>();
	shader["gbuffer"].uniform["camera"].bind(camera);

	fbo["gbuffer"].bufferResize(width(), height());
	fbo["gbuffer"].screenResize(width(), height());
	fbo["gbuffer"].clearColor(vec4(0));
	#ifdef USE_QT
	fbo["gbuffer"].setPaintDevice(this);
	#endif
	fbo["gbuffer"].attach("diffuse_specular", FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGBA16F, GL_FLOAT));
	fbo["gbuffer"].attach("position_shininess", FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGBA16F, GL_FLOAT));
	fbo["gbuffer"].attach("normal", FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGB16F, GL_FLOAT));
	fbo["gbuffer"].attach("lights", FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGB16F, GL_FLOAT));
	fbo["gbuffer"].attach("skybox", FBO::Attachment<sampler2D>(FBO::COLOR));
	fbo["gbuffer"].attach(FBO::Attachment<RBO>(FBO::DEPTH));
	fbo["gbuffer"].color<sampler2D>("diffuse_specular").wrap(GL_CLAMP_TO_EDGE);
	fbo["gbuffer"].color<sampler2D>("lights").wrap(GL_CLAMP_TO_EDGE);
}

void Scene3D::init_shadering_blur()
{
	if(shader.count("blur"))
	{
		return;
	}

	shader["blur"].compile(Shader::VERTEX, glass::blur_vertex_shader);
	shader["blur"].compile(Shader::FRAGMENT, glass::blur_fragment_shader);
	shader["blur"].link();
	gauss_kernel<5>(30, gauss_weights);
	shader["blur"].uniform["weights"] = gauss_weights;

	fbos["blur"].push_back(FBO(width(), height(), width()/4, height()/4));
	fbos["blur"][0].clearColor(vec4(0));
	#ifdef USE_QT
	fbos["blur"][0].setPaintDevice(this);
	#endif
	fbos["blur"][0].attach(FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGB16F, GL_FLOAT));
	fbos["blur"][0].color<sampler2D>().wrap(GL_CLAMP_TO_EDGE);

	fbos["blur"].push_back(FBO(width(), height(), width()/4, height()/4));
	fbos["blur"][1].clearColor(vec4(0));
	#ifdef USE_QT
	fbos["blur"][1].setPaintDevice(this);
	#endif
	fbos["blur"][1].attach(FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGB16F, GL_FLOAT));
	fbos["blur"][1].color<sampler2D>().wrap(GL_CLAMP_TO_EDGE);

	if(!model.count("plane"))
	{
		model["plane"] = glass::plane();
	}
}

void Scene3D::init_shadering_merge()
{
	if(shader.count("merge"))
	{
		return;
	}

	shader["merge"].compile(Shader::VERTEX, glass::merge_vertex_shader);
	shader["merge"].compile(Shader::FRAGMENT, glass::merge_fragment_shader);
	shader["merge"].link();

	if(!model.count("plane"))
	{
		model["plane"] = glass::plane();
	}
}

void Scene3D::init_shadering_HDR()
{
	if(shader.count("luminance"))
	{
		return;
	}

	shader["luminance"].compile(Shader::VERTEX, glass::luminance_vertex_shader);
	shader["luminance"].compile(Shader::FRAGMENT, glass::luminance_fragment_shader);
	shader["luminance"].link();

	shader["HDR"].compile(Shader::VERTEX, glass::HDR_vertex_shader);
	shader["HDR"].compile(Shader::FRAGMENT, glass::HDR_fragment_shader);
	shader["HDR"].link();

	int luminance_width = 256;
	int i = 0;
	while(luminance_width >= 2)
	{
		fbos["luminance"].push_back(FBO(width(), height(), i == 0 ? 1024 : luminance_width, i == 0 ? 1024 : luminance_width));
		if(i == 0)
		{
			fbos["luminance"][i].attach(FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGB16F, GL_FLOAT));
		}
		else
		{
			fbos["luminance"][i].attach(FBO::Attachment<sampler2D>(FBO::COLOR, GL_R16F, GL_FLOAT));
		}
		#ifdef USE_QT
		fbos["luminance"][i].setPaintDevice(this);
		#endif
		fbos["luminance"][i].color<sampler2D>().filter(GL_LINEAR);
		fbos["luminance"][i].color<sampler2D>().wrap(GL_CLAMP_TO_EDGE);
		i++;
		luminance_width /= 2;
	}

	if(!model.count("plane"))
	{
		model["plane"] = glass::plane();
	}
}

void Scene3D::init_shadering_lighting()
{
	if(shader.count("lighting"))
	{
		return;
	}

	shader["lighting"].compile(Shader::VERTEX, glass::lighting_vertex_shader);
	shader["lighting"].compile(Shader::FRAGMENT, glass::lighting_fragment_shader);
	shader["lighting"].link();
	shader["lighting"].uniform["camera"].bind(camera);

	fbos["lighting"].push_back(FBO(width(), height(), width(), height()));
	fbos["lighting"][0].attach(FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGB16F, GL_FLOAT));
	#ifdef USE_QT
	fbos["lighting"][0].setPaintDevice(this);
	#endif

	fbos["lighting"].push_back(FBO(width(), height(), width(), height()));
	fbos["lighting"][1].attach(FBO::Attachment<sampler2D>(FBO::COLOR, GL_RGB16F, GL_FLOAT));
	#ifdef USE_QT
	fbos["lighting"][1].setPaintDevice(this);
	#endif

	if(!model.count("plane"))
	{
		model["plane"] = glass::plane();
	}
}

void Scene3D::init_shadering_point_light_shadow()
{
	if(shader.count("point_light_shadow"))
	{
		return;
	}

	shader["point_light_shadow"].compile(Shader::VERTEX, glass::shadow_point_light_vertex_shader);
	shader["point_light_shadow"].compile(Shader::GEOMETRY, glass::shadow_point_light_geometry_shader);
	shader["point_light_shadow"].compile(Shader::FRAGMENT, glass::shadow_point_light_fragment_shader);
	shader["point_light_shadow"].link();
	shader["point_light_shadow"].uniform.define<Material>();
	shader["point_light_shadow"].warningOff();

	fbo["point_light_shadow"].screenResize(width(), height());
	#ifdef USE_QT
	fbo["point_light_shadow"].setPaintDevice(this);
	#endif
	fbo["point_light_shadow"].bufferResize(PointLight::frame_width, PointLight::frame_height);
	fbo["point_light_shadow"].attach(FBO::Attachment<samplerCube>(FBO::DEPTH));
}

void Scene3D::init_shadering_dir_light_shadow()
{
	if(shader.count("dir_light_shadow"))
	{
		return;
	}

	shader["dir_light_shadow"].compile(Shader::VERTEX, glass::shadow_dir_light_vertex_shader);
	shader["dir_light_shadow"].compile(Shader::FRAGMENT, glass::shadow_dir_light_fragment_shader);
	shader["dir_light_shadow"].link();
	shader["dir_light_shadow"].warningOff();

	fbos["dir_light_shadow"].push_back(FBO(width(), height(), DirLight::frame_width, DirLight::frame_height));
	fbos["dir_light_shadow"][0].attach(FBO::Attachment<sampler2D>(FBO::DEPTH));
	#ifdef USE_QT
	fbos["dir_light_shadow"][0].setPaintDevice(this);
	#endif

	fbos["dir_light_shadow"].push_back(FBO(width(), height(), DirLight::frame_width, DirLight::frame_height));
	fbos["dir_light_shadow"][1].attach(FBO::Attachment<sampler2D>(FBO::DEPTH));
	#ifdef USE_QT
	fbos["dir_light_shadow"][1].setPaintDevice(this);
	#endif

	fbos["dir_light_shadow"].push_back(FBO(width(), height(), DirLight::frame_width, DirLight::frame_height));
	fbos["dir_light_shadow"][2].attach(FBO::Attachment<sampler2D>(FBO::DEPTH));
	#ifdef USE_QT
	fbos["dir_light_shadow"][2].setPaintDevice(this);
	#endif

	fbos["dir_light_shadow"].push_back(FBO(width(), height(), DirLight::frame_width, DirLight::frame_height));
	fbos["dir_light_shadow"][3].attach(FBO::Attachment<sampler2D>(FBO::DEPTH));
	#ifdef USE_QT
	fbos["dir_light_shadow"][3].setPaintDevice(this);
	#endif
}

void Scene3D::prepare()
{
#ifndef USE_QT
    hideCursor();
#endif
	camera.moveYTo(1.7);
	camera.moveZTo(2);
    test_image.setImage("E:/Learning/OpenGL/demo/resources/images/floor.png");
}

bool Scene3D::lights_shape_not_empty()
{
	for(auto it = point_lights.begin(); it != point_lights.end(); it++)
	{
		if(it->second.isOpen() && !(it->second.shape.empty()))
		{
			return true;
		}
	}
	return false;
}

bool Scene3D::models_not_empty()
{
	for(auto it = models.begin(); it != models.end(); it++)
	{
		if(it->second.isVisible() && !(it->second.empty()))
		{
			return true;
		}
	}
	for(auto it = spot_lights.begin(); it != spot_lights.end(); it++)
	{
		if(it->second.isOpen() && !(it->second.shape.empty()))
		{
			return true;
		}
	}
	if(camera.head_light.isOpen() && !(camera.head_light.shape.empty()))
	{
		return true;
	}

	return false;
}

void Scene3D::draw()
{
	bool has_model = models_not_empty();
	bool has_light_shape = lights_shape_not_empty();
	bool has_skybox = skybox_map.completed();

	if(!has_model && !has_light_shape && !has_skybox)
	{
		return;
	}

	// Draw to G-Buffer
	init_shadering_gbuffer();

	fbo["gbuffer"].bind();
	if(has_model)
	{
		for(auto it = models.begin(); it != models.end(); it++)
		{
			if(it->second.isHidden())
			{
				continue;
			}
			shader["gbuffer"].uniform["model"] = it->second;
			shader["gbuffer"].draw(it->second);
		}
		for(auto it = spot_lights.begin(); it != spot_lights.end(); it++)
		{
			if(it->second.isClose())
			{
				continue;
			}
			shader["gbuffer"].uniform["model"] = it->second.shape;
			shader["gbuffer"].draw(it->second.shape);
		}
		if(camera.head_light.isOpen())
		{
			shader["gbuffer"].uniform["model"] = camera.head_light.shape;
			shader["gbuffer"].draw(camera.head_light.shape);
		}
	}
	if(has_light_shape)
	{
		for(auto it = point_lights.begin(); it != point_lights.end(); it++)
		{
			if(it->second.isClose())
			{
				continue;
			}
			shader["gbuffer"].uniform["model"] = it->second.shape;
			shader["gbuffer"].draw(it->second.shape);
		}
	}
	if(has_skybox)
	{
		glDepthFunc(GL_LEQUAL);
		shader["skybox"].draw(model["skybox"]);
		glDepthFunc(GL_LESS);
	}
	fbo["gbuffer"].unbind();

	// Lighting
	int index_lighting = -1;
	if(has_model)
	{
		init_shadering_lighting();

		shader["lighting"].uniform["gbuffer_diffuse_specular"] = fbo["gbuffer"].color<sampler2D>("diffuse_specular");
		shader["lighting"].uniform["gbuffer_position_shininess"] = fbo["gbuffer"].color<sampler2D>("position_shininess");
		shader["lighting"].uniform["gbuffer_normal"] = fbo["gbuffer"].color<sampler2D>("normal");

		shader["lighting"].uniform["type"] = 0; // shadering ambient
		fbos["lighting"][step(index_lighting)].bind();
		shader["lighting"].draw(model["plane"]);
		fbos["lighting"][step(index_lighting)].unbind();

		shader["lighting"].uniform["type"] = 1; // shadering dir light;
		for(auto it = dir_lights.begin(); it != dir_lights.end(); it++)
		{
			if(it->second.isClose())
			{
				continue;
			}

			if(it->second.usingShadow())
			{
				if(!(it->second.bindedCamera()))
				{
					camera.addDirLight(it->second);
				}
				
				init_shadering_dir_light_shadow();
				for(int i = 0; i < 4; i++)
				{
					fbos["dir_light_shadow"][i].bind();
					shader["dir_light_shadow"].uniform["light_mat"] = it->second.mat(i);
					for(auto sub_it = models.begin(); sub_it != models.end(); sub_it++)
					{
						if(sub_it->second.isHidden())
						{
							continue;
						}
						shader["dir_light_shadow"].uniform["model_mat"] = sub_it->second.mat();
						shader["dir_light_shadow"].draw(sub_it->second);
					}
					for(auto sub_it = spot_lights.begin(); sub_it != spot_lights.end(); sub_it++)
					{
						if(sub_it->second.isClose())
						{
							continue;
						}
						shader["dir_light_shadow"].uniform["model_mat"] = sub_it->second.shape.mat();
						shader["dir_light_shadow"].draw(sub_it->second.shape);
					}
					if(camera.head_light.isOpen())
					{
						shader["dir_light_shadow"].uniform["model_mat"] = camera.head_light.shape.mat();
						shader["dir_light_shadow"].draw(camera.head_light.shape);
					}

					fbos["dir_light_shadow"][i].unbind();
				}
			}

			shader["lighting"].uniform["base_image"] = fbos["lighting"][step(index_lighting)].color<sampler2D>();
			index_lighting *= -1;

			fbos["lighting"][step(index_lighting)].bind();
			shader["lighting"].uniform["dir_light"] = it->second;
			if(it->second.usingShadow())
			{
				shader["lighting"].uniform["dir_light_depth_map_zero"] = fbos["dir_light_shadow"][0].depth<sampler2D>();
				shader["lighting"].uniform["dir_light_depth_map_one"] = fbos["dir_light_shadow"][1].depth<sampler2D>();
				shader["lighting"].uniform["dir_light_depth_map_two"] = fbos["dir_light_shadow"][2].depth<sampler2D>();
				shader["lighting"].uniform["dir_light_depth_map_three"] = fbos["dir_light_shadow"][3].depth<sampler2D>();
			}
			shader["lighting"].draw(model["plane"]);
			fbos["lighting"][step(index_lighting)].unbind();
		}

		shader["lighting"].uniform["type"] = 2; // shadering point light
		for(auto it = point_lights.begin(); it != point_lights.end(); it++)
		{
			if(it->second.isClose())
			{
				continue;
			}

			if(it->second.usingShadow())
			{
				init_shadering_point_light_shadow();

				shader["point_light_shadow"].uniform["type"] = 0; // point light

				fbo["point_light_shadow"].bind();
				shader["point_light_shadow"].uniform["point_light"] = it->second;
				for(auto sub_it = models.begin(); sub_it != models.end(); sub_it++)
				{
					if(sub_it->second.isHidden())
					{
						continue;
					}
					shader["point_light_shadow"].uniform["model_mat"] = sub_it->second.mat();
					shader["point_light_shadow"].draw(sub_it->second);
				}
				for(auto sub_it = spot_lights.begin(); sub_it != spot_lights.end(); sub_it++)
				{
					if(sub_it->second.isClose())
					{
						continue;
					}
					shader["point_light_shadow"].uniform["model_mat"] = sub_it->second.shape.mat();
					shader["point_light_shadow"].draw(sub_it->second.shape);
				}
				if(camera.head_light.isOpen())
				{
					shader["point_light_shadow"].uniform["model_mat"] = camera.head_light.shape.mat();
					shader["point_light_shadow"].draw(camera.head_light.shape);
				}

				fbo["point_light_shadow"].unbind();
				it->second.depth_map = fbo["point_light_shadow"].depth<samplerCube>();
			}
			
			shader["lighting"].uniform["base_image"] = fbos["lighting"][step(index_lighting)].color<sampler2D>();
			index_lighting *= -1;

			fbos["lighting"][step(index_lighting)].bind();
			shader["lighting"].uniform["point_light"] = it->second;
			shader["lighting"].draw(model["plane"]);
			fbos["lighting"][step(index_lighting)].unbind();
		}

		shader["lighting"].uniform["type"] = 3; // shadering spot light
		if(camera.head_light.isOpen())
		{
			if(camera.head_light.usingShadow())
			{
				init_shadering_point_light_shadow();

				shader["point_light_shadow"].uniform["type"] = 1; // spot light

				fbo["point_light_shadow"].bind();

				shader["point_light_shadow"].uniform["spot_light"] = camera.head_light;
				for(auto sub_it = models.begin(); sub_it != models.end(); sub_it++)
				{
					if(sub_it->second.isHidden())
					{
						continue;
					}
					shader["point_light_shadow"].uniform["model_mat"] = sub_it->second.mat();
					shader["point_light_shadow"].draw(sub_it->second);
				}
				for(auto sub_it = spot_lights.begin(); sub_it != spot_lights.end(); sub_it++)
				{
					if(sub_it->second.isClose())
					{
						continue;
					}
					shader["point_light_shadow"].uniform["model_mat"] = sub_it->second.shape.mat();
					shader["point_light_shadow"].draw(sub_it->second.shape);
				}
				if(camera.head_light.isOpen())
				{
					shader["point_light_shadow"].uniform["model_mat"] = camera.head_light.shape.mat();
					shader["point_light_shadow"].draw(camera.head_light.shape);
				}

				fbo["point_light_shadow"].unbind();
				camera.head_light.depth_map = fbo["point_light_shadow"].depth<samplerCube>();
			}
			
			shader["lighting"].uniform["base_image"] = fbos["lighting"][step(index_lighting)].color<sampler2D>();
			index_lighting *= -1;

			fbos["lighting"][step(index_lighting)].bind();
			shader["lighting"].uniform["spot_light"] = camera.head_light;
			shader["lighting"].draw(model["plane"]);
			fbos["lighting"][step(index_lighting)].unbind();
		}
		for(auto it = spot_lights.begin(); it != spot_lights.end(); it++)
		{
			if(it->second.isClose())
			{
				continue;
			}

			if(it->second.usingShadow())
			{
				init_shadering_point_light_shadow();

				shader["point_light_shadow"].uniform["type"] = 1; // spot light

				fbo["point_light_shadow"].bind();
				shader["point_light_shadow"].uniform["spot_light"] = it->second;
				for(auto sub_it = models.begin(); sub_it != models.end(); sub_it++)
				{
					if(sub_it->second.isHidden())
					{
						continue;
					}
					shader["point_light_shadow"].uniform["model_mat"] = sub_it->second.mat();
					shader["point_light_shadow"].draw(sub_it->second);
				}
				for(auto sub_it = spot_lights.begin(); sub_it != spot_lights.end(); sub_it++)
				{
					if(sub_it->second.isClose())
					{
						continue;
					}
					shader["point_light_shadow"].uniform["model_mat"] = sub_it->second.shape.mat();
					shader["point_light_shadow"].draw(sub_it->second.shape);
				}
				if(camera.head_light.isOpen())
				{
					shader["point_light_shadow"].uniform["model_mat"] = camera.head_light.shape.mat();
					shader["point_light_shadow"].draw(camera.head_light.shape);
				}

				fbo["point_light_shadow"].unbind();
				it->second.depth_map = fbo["point_light_shadow"].depth<samplerCube>();
			}
			
			shader["lighting"].uniform["base_image"] = fbos["lighting"][step(index_lighting)].color<sampler2D>();
			index_lighting *= -1;

			fbos["lighting"][step(index_lighting)].bind();
			shader["lighting"].uniform["spot_light"] = it->second;
			shader["lighting"].draw(model["plane"]);
			fbos["lighting"][step(index_lighting)].unbind();
		}
	}

	int index_blur = -1;
	if(has_light_shape)
	{
		init_shadering_blur();

		for(int i = 0; i < 5; i++)
		{
			fbos["blur"][step(index_blur)].bind();
			shader["blur"].uniform["color_map"] = 
			(i == 0 ? fbo["gbuffer"].color<sampler2D>("lights") :
			          fbos["blur"][step(-index_blur)].color<sampler2D>());
			shader["blur"].uniform["horizontal"] = (bool)step(index_blur);
			shader["blur"].draw(model["plane"]);
			fbos["blur"][step(index_blur)].unbind();
			index_blur *= -1;
		}
	}

	init_shadering_merge();
	if(has_model)
	{
		shader["merge"].uniform["color_without_lights"] = fbos["lighting"][step(index_lighting)].color<sampler2D>();
	}
	if(has_light_shape)
	{
		shader["merge"].uniform["color_lights_blur"] = fbos["blur"][step(index_blur)].color<sampler2D>();
		shader["merge"].uniform["color_lights_clear"] = fbo["gbuffer"].color<sampler2D>("lights");
	}
	if(has_skybox)
	{
		shader["merge"].uniform["skybox_map"] = fbo["gbuffer"].color<sampler2D>("skybox");
	}

	if(!using_HDR)
	{
		shader["merge"].draw(model["plane"]);
	}
	else
	{
		init_shadering_HDR();
		fbos["luminance"][0].bind();
		shader["merge"].draw(model["plane"]);
		fbos["luminance"][0].unbind();

        for(uint i = 1; i < fbos["luminance"].size(); i++)
		{
			fbos["luminance"][i].bind();
			shader["luminance"].uniform["type"] = (i == 1 ? 0 : (i == fbos["luminance"].size()-1 ? 2 : 1));
			shader["luminance"].uniform["frame_image"] = fbos["luminance"][i-1].color<sampler2D>();
			shader["luminance"].draw(model["plane"]);
			fbos["luminance"][i].unbind();
		}
		
		shader["HDR"].uniform["luminance_texture"] = fbos["luminance"].back().color<sampler2D>();
		shader["HDR"].uniform["frame_image"] = fbos["luminance"][0].color<sampler2D>();
		shader["HDR"].draw(model["plane"]);
	}
}

void Scene3D::onChangeSize(int width, int height)
{
	camera.screenResize(width, height);

	for(auto it = fbo.begin(); it != fbo.end(); it++)
	{
		it->second.screenResize(width, height);
	}

	for(auto it = fbos.begin(); it != fbos.end(); it++)
	{
        for(uint i = 0; i < it->second.size(); i++)
		{
			it->second[i].screenResize(width, height);
		}
	}
	if(fbos.count("blur"))
	{
		fbos["blur"][0].bufferResize(width/4, height/4);
		fbos["blur"][1].bufferResize(width/4, height/4);
	}

#ifdef USE_QT
    update();
#endif
}

#ifndef USE_QT
void Scene3D::onMouseMove()
{
	if(can_move)
	{
		camera.yaw(-2.0*dx()/width());
		camera.pitch(-2.0*dy()/height());
	}
}
#else
void Scene3D::onDrag()
{
    camera.yaw(2.0*dx()/width());
    camera.pitch(2.0*dy()/height());
    update();
}
#endif

void Scene3D::onKeyRepeat(const string& key)
{
	float d = 0.1;
	if(key == "W")
	{
		if(can_move)
		{
			camera.moveForward(d);
			if(camera.position().y < camera.zNear()+1E-6)
			{
				camera.moveYTo(camera.zNear()+1E-6);
			}
		}
	}
	else if(key == "S")
	{
		if(can_move)
		{
			camera.moveBack(d);
			if(camera.position().y < camera.zNear()+1E-6)
			{
				camera.moveYTo(camera.zNear()+1E-6);
			}
		}
	}
	else if(key == "A")
	{
		if(can_move)
		{
			camera.moveLeft(d);
			if(camera.position().y < camera.zNear()+1E-6)
			{
				camera.moveYTo(camera.zNear()+1E-6);
			}
		}
	}
	else if(key == "D")
	{
		if(can_move)
		{
			camera.moveRight(d);
			if(camera.position().y < camera.zNear()+1E-6)
			{
				camera.moveYTo(camera.zNear()+1E-6);
			}
		}
	}
	else if(key == "E")
	{
		if(can_move)
		{
			camera.moveUp(d);
			if(camera.position().y < camera.zNear()+1E-6)
			{
				camera.moveYTo(camera.zNear()+1E-6);
			}
		}
	}
	else if(key == "C")
	{
		if(can_move)
		{
			camera.moveDown(d);
			if(camera.position().y < camera.zNear()+1E-6)
			{
				camera.moveYTo(camera.zNear()+1E-6);
			}
		}
	}
#ifndef USE_QT
	else if(key == "ESC")
	{
		can_move = false;
		showCursor();
	}
	else if(key == "ENTER")
	{
		camera.screenOffset(0, 0);
		camera.screenZoom(1);
        can_move = true;
		hideCursor();
	}
#endif
	else if(key == "F")
	{
		cout << fps() << endl;
	}

#ifdef USE_QT
    update();
#endif
}

string Scene3D::addModel(const vec3& position, const Model& model, string name)
{
	name = getValidName(name, models);
	
	models[name] = model;
	models[name].moveTo(position);

#ifdef USE_QT
    update();
#endif

	return name;
}

string Scene3D::addModel(const Model& model, string name)
{
	name = getValidName(name, models);
	
	models[name] = model;

#ifdef USE_QT
    update();
#endif

	return name;
}

string Scene3D::addDirLight(const DirLight& dir_light, string name)
{
	name = getValidName(name, dir_lights);

	dir_lights[name] = dir_light;
	camera.addDirLight(dir_lights[name]);

#ifdef USE_QT
    update();
#endif

	return name;
}

string Scene3D::addPointLight(const vec3& position, const PointLight& point_light, string name)
{
	name = getValidName(name, point_lights);

	point_lights[name] = point_light;
	point_lights[name].moveTo(position);

#ifdef USE_QT
    update();
#endif

	return name;
}

string Scene3D::addPointLight(const PointLight& point_light, string name)
{
	name = getValidName(name, point_lights);

	point_lights[name] = point_light;

#ifdef USE_QT
    update();
#endif

	return name;
}

void Scene3D::setSkyBox(const string& filename)
{
	skybox_map.setImage(filename);
	init_skybox();

#ifdef USE_QT
    update();
#endif
}

void Scene3D::setSkyBoxUp(const string& filename)
{
	skybox_map.setUpImage(filename);

	if(skybox_map.completed())
	{
		init_skybox();
	}

#ifdef USE_QT
    update();
#endif
}

void Scene3D::setSkyBoxDown(const string& filename)
{
	skybox_map.setDownImage(filename);

	if(skybox_map.completed())
	{
		init_skybox();
	}

#ifdef USE_QT
    update();
#endif
}

void Scene3D::setSkyBoxLeft(const string& filename)
{
	skybox_map.setLeftImage(filename);

	if(skybox_map.completed())
	{
		init_skybox();
	}

#ifdef USE_QT
    update();
#endif
}

void Scene3D::setSkyBoxRight(const string& filename)
{
	skybox_map.setRightImage(filename);

	if(skybox_map.completed())
	{
		init_skybox();
	}

#ifdef USE_QT
    update();
#endif
}

void Scene3D::setSkyBoxFront(const string& filename)
{
	skybox_map.setFrontImage(filename);

	if(skybox_map.completed())
	{
		init_skybox();
	}

#ifdef USE_QT
    update();
#endif
}

void Scene3D::setSkyBoxBack(const string& filename)
{
	skybox_map.setBackImage(filename);

	if(skybox_map.completed())
	{
		init_skybox();
	}

#ifdef USE_QT
    update();
#endif
}

void Scene3D::useHDR(bool flag)
{
	using_HDR = flag;

#ifdef USE_QT
    update();
#endif
}

bool Scene3D::usingHDR()const
{
	return using_HDR;
}
