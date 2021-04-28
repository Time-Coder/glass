#include <cmath>

#include "glsl_objects.h"
#include "image_process.h"
#include "exception.h"

uint sampler2D::active_sampler2D = 0;
set<uint> sampler2D::existing_sampler2Ds;
list<sampler2D::Instance> sampler2D::existing_instances;
map<string, sampler2D::Instance*> sampler2D::path_map;
bool sampler2D::destruct_all_is_registered = false;

void sampler2D::init()
{
	if(!destruct_all_is_registered)
	{
		atexit(sampler2D::destruct_all);
		destruct_all_is_registered = true;
	}

	if(instance == NULL)
	{
		existing_instances.push_back(Instance());
		instance = &(existing_instances.back());

		glGenTextures(1, &(instance->id));
	#ifdef _DEBUG
		cout << "constructing sampler2D " << instance->id << endl;
	#endif
		if(instance->id == 0)
		{
			throw RuntimeError("Failed to create sampler2D!");
		}
		existing_sampler2Ds.insert(instance->id);
	}
	else if(existing_sampler2Ds.count(instance->id) == 0)
	{
		throw RuntimeError("sampler2D " + str::str(instance->id) + " has already been destructed.");
	}
}

sampler2D::sampler2D() {}

sampler2D::sampler2D(uint _width, uint _height, uint _format, uint _dtype)
{
	malloc(_width, _height, _format, _dtype);
}

sampler2D::sampler2D(const Image& image)
{
	addImage(image);
}

sampler2D::sampler2D(const string& filename)
{
	addImage(filename);
}

sampler2D::sampler2D(const sampler2D& sampler) :
instance(sampler.instance)
{}

sampler2D::sampler2D(sampler2D&& sampler) :
instance(move(sampler.instance))
{}

sampler2D::~sampler2D()
{
	instance = NULL;
}

void sampler2D::destruct_all()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	active_sampler2D = 0;
	for(auto it = existing_instances.begin(); it != existing_instances.end(); it++)
	{
	#ifdef _DEBUG
		cout << "destructing sampler2D " << it->id << endl;
	#endif
		glDeleteTextures(1, &(it->id));
	}
	existing_instances.clear();
	existing_sampler2Ds.clear();
	path_map.clear();
}

sampler2D& sampler2D::operator =(const sampler2D& sampler)
{
	if(this != &sampler)
	{
		instance = sampler.instance;
	}

	return *this;
}

sampler2D& sampler2D::operator =(sampler2D&& sampler)
{
	if(this != &sampler)
	{
		instance = move(sampler.instance);
	}

	return *this;
}

uint sampler2D::id()const
{
	return (instance != NULL ? instance->id : 0);
}

uint sampler2D::width()const
{
	return (instance != NULL ? instance->width : 0);
}

uint sampler2D::height()const
{
	return (instance != NULL ? instance->height : 0);
}

uint sampler2D::format()const
{
	return (instance != NULL ? instance->format : GL_RGB);
}

uint sampler2D::dtype()const
{
	return (instance != NULL ? instance->dtype : GL_UNSIGNED_BYTE);
}

void sampler2D::_bind()
{
	init();
	glBindTexture(GL_TEXTURE_2D, instance->id);
	active_sampler2D = instance->id;
}

void sampler2D::bind()
{
	_bind();
	if(instance->is_applied)
	{
		return;
	}
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, instance->border_color.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, instance->s_wrap_type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, instance->t_wrap_type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, instance->min_filter_type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, instance->mag_filter_type);
	instance->is_applied = true;
}

void sampler2D::unbind()const
{
	if(active_sampler2D == id())
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		active_sampler2D = 0;
	}
}

bool sampler2D::is_bind()const
{
	return (existing_sampler2Ds.count(id()) && active_sampler2D == id());
}

uint sampler2D::wrap_s()const
{
	return (instance != NULL ? instance->s_wrap_type : GL_REPEAT);
}

uint sampler2D::wrap_t()const
{
	return (instance != NULL ? instance->t_wrap_type : GL_REPEAT);
}

void sampler2D::wrap_s(uint wrap_type)
{
	switch(wrap_type)
	{
		case GL_REPEAT:
		case GL_MIRRORED_REPEAT:
		case GL_CLAMP_TO_EDGE:
		case GL_CLAMP_TO_BORDER:
		{
			init();
			instance->is_applied = false;
			instance->s_wrap_type = wrap_type;
			break;
		}
		default:
			throw ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
				             "Please choose wrap_type in [GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER].");
	}
}

void sampler2D::wrap_t(uint wrap_type)
{
	switch(wrap_type)
	{
		case GL_REPEAT:
		case GL_MIRRORED_REPEAT:
		case GL_CLAMP_TO_EDGE:
		case GL_CLAMP_TO_BORDER:
		{
			init();
			instance->is_applied = false;
			instance->t_wrap_type = wrap_type;
			break;
		}
		default:
			throw ValueError("Unsupported wrap_type " + str::str(wrap_type) + ".\n" +
				             "Please choose wrap_type in [GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER].");
	}
}

void sampler2D::wrap(uint wrap_type_s, uint wrap_type_t)
{
	if(wrap_type_t == -1)
	{
		wrap_type_t = wrap_type_s;
	}

	wrap_s(wrap_type_s);
	wrap_t(wrap_type_t);
}

void sampler2D::filter(uint filter_type_min, uint filter_type_mag)
{
	if(filter_type_mag == -1)
	{
		filter_type_mag = filter_type_min;
	}

	filter_min(filter_type_min);
	filter_mag(filter_type_mag);
}

uint sampler2D::filter_min()const
{
	return (instance != NULL ? instance->min_filter_type : GL_LINEAR);
}

uint sampler2D::filter_mag()const
{
	return (instance != NULL ? instance->mag_filter_type : GL_LINEAR);
}

void sampler2D::filter_min(uint filter_type)
{
	switch(filter_type)
	{
		case GL_NEAREST:
		case GL_LINEAR:
		{
			init();
			instance->is_applied = false;
			instance->min_filter_type = filter_type;
			break;
		}
		default:
			throw ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
				             "Please choose filter_type in [GL_NEAREST, GL_LINEAR].");
	}
}

void sampler2D::filter_mag(uint filter_type)
{
	switch(filter_type)
	{
		case GL_NEAREST:
		case GL_LINEAR:
		{
			init();
			instance->is_applied = false;
			instance->mag_filter_type = filter_type;
			break;
		}
		default:
			throw ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
				             "Please choose filter_type in [GL_NEAREST, GL_LINEAR].");
	}
}

uint get_format(uint n_channels)
{
	switch(n_channels)
	{
		case 1: return GL_RED;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: return GL_RGB;
	}
}

void sampler2D::addImage(const Image& image)
{
	_bind();

	instance->format = get_format(image.channels());
	instance->dtype = GL_UNSIGNED_BYTE;
	instance->width = image.width();
	instance->height = image.height();

	glTexImage2D(GL_TEXTURE_2D, 0, instance->format, instance->width, instance->height, 0, instance->format, instance->dtype, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void sampler2D::addImage(const string& filename)
{
	string abspath = path::abspath(filename);
	bool need_add = (path_map.count(abspath) == 0);
	if(!need_add)
	{
		instance = path_map[abspath];
		_bind();
	}
	else
	{
		instance = NULL;
		Image image(filename, true);
		addImage(image);
	    path_map[abspath] = instance;
	}
}

void sampler2D::malloc(uint _width, uint _height, uint _format, uint _dtype)
{
	if(width() == _width && height() == _height && format() == _format && dtype() == _dtype)
	{
		return;
	}

	if(width() != 0 || height() != 0)
	{
		instance = NULL;
	}

	_bind();
	glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, _dtype, NULL);
	instance->width = _width;
	instance->height = _height;
	instance->format = _format;
	instance->dtype = _dtype;
}

void sampler2D::border_color(const vec4& color)
{
	_bind();
	instance->border_color = color;
	instance->is_applied = false;
}

void sampler2D::border_color(const vec3& color)
{
	_bind();
	instance->border_color = vec4(color, 1);
	instance->is_applied = false;
}

vec4 sampler2D::border_color()const
{
	return (instance != NULL ? instance->border_color : vec4(0));
}

uint samplerCube::active_samplerCube = 0;
set<uint> samplerCube::existing_samplerCubes;
list<samplerCube::Instance> samplerCube::existing_instances;
map<string, samplerCube::Instance*> samplerCube::path_map;
bool samplerCube::destruct_all_is_registered = false;

void samplerCube::destruct_all()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	active_samplerCube = 0;
	for(auto it = existing_instances.begin(); it != existing_instances.end(); it++)
	{
	#ifdef _DEBUG
		cout << "destructing samplerCube " << it->id << endl;
	#endif
		glDeleteTextures(1, &(it->id));
	}
	existing_instances.clear();
	path_map.clear();
	existing_samplerCubes.clear();
}

void samplerCube::init()
{
	if(!destruct_all_is_registered)
	{
		atexit(destruct_all);
		destruct_all_is_registered = true;
	}

	if(instance == NULL)
	{
		existing_instances.push_back(Instance());
		instance = &(existing_instances.back());

		glGenTextures(1, &(instance->id));
	#ifdef _DEBUG
		cout << "constructing samplerCube " << instance->id << endl;
	#endif
		if(instance->id == 0)
		{
			throw Exception("Failed to create samplerCube!");
		}
		existing_samplerCubes.insert(instance->id);
	}
	else if(existing_samplerCubes.count(instance->id) == 0)
	{
		throw RuntimeError("samplerCube " + str::str(instance->id) + " has already been destructed.");
	}
}

samplerCube::samplerCube() {}

samplerCube::samplerCube(vector<Image>& images)
{
	addImages(images);
}

samplerCube::samplerCube(const vector<string>& filenames)
{
	addImages(filenames);
}

samplerCube::samplerCube(const Image& right, const Image& left,
	                          const Image& up   , const Image& down,
	                          const Image& back , const Image& front)
{
	addImages(right, left, up, down, back, front);
}

samplerCube::samplerCube(const string& right, const string& left,
	                          const string& up   , const string& down,
	                          const string& back , const string& front)
{
	addImages(right, left, up, down, back, front);
}

samplerCube::samplerCube(const Image& image)
{
	addImage(image);
}

samplerCube::samplerCube(const string& filename)
{
	addImage(filename);
}

samplerCube::samplerCube(const samplerCube& sampler) :
instance(sampler.instance)
{}

samplerCube::samplerCube(samplerCube&& sampler) :
instance(move(sampler.instance))
{
	sampler.instance = NULL;
}

samplerCube::~samplerCube()
{
	instance = NULL;
}

samplerCube& samplerCube::operator =(const samplerCube& sampler)
{
	if(this != &sampler)
	{
		instance = sampler.instance;
	}

	return *this;
}

samplerCube& samplerCube::operator =(samplerCube&& sampler)
{
	if(this != &sampler)
	{
		instance = move(sampler.instance);
		sampler.instance = NULL;
	}

	return *this;
}

uint samplerCube::id()const
{
	return (instance != NULL ? instance->id : 0);
}

void samplerCube::_bind()
{
	init();
	glBindTexture(GL_TEXTURE_CUBE_MAP, instance->id);
	active_samplerCube = instance->id;
}

void samplerCube::bind()
{
	_bind();
	if(instance->is_applied)
	{
		return;
	}

	glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, instance->border_color.data());
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, instance->min_filter_type);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, instance->mag_filter_type);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, instance->s_wrap_type);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, instance->t_wrap_type);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, instance->r_wrap_type);
    instance->is_applied = true;
}

void samplerCube::unbind()const
{
	if(id() == active_samplerCube)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		active_samplerCube = 0;
	}
}

bool samplerCube::is_bind()const
{
	return (existing_samplerCubes.count(id()) && active_samplerCube == id());
}

void samplerCube::addRightImage(const string& filename)
{
	addRightImage(Image(filename));
}

void samplerCube::addRightImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
}

void samplerCube::addLeftImage(const string& filename)
{
	addLeftImage(Image(filename));
}

void samplerCube::addLeftImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
}

void samplerCube::addUpImage(const string& filename)
{
	addUpImage(Image(filename));
}

void samplerCube::addUpImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
}

void samplerCube::addDownImage(const string& filename)
{
	addDownImage(Image(filename));
}

void samplerCube::addDownImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
}

void samplerCube::addFrontImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
}

void samplerCube::addBackImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
}

string get_path_key(const vector<string>& filenames)
{
	return "(" + path::abspath(filenames[0]) + ", " +
                 path::abspath(filenames[1]) + ", " +
                 path::abspath(filenames[2]) + ", " +
                 path::abspath(filenames[3]) + ", " +
                 path::abspath(filenames[4]) + ", " +
                 path::abspath(filenames[5]) + ")";
}

void samplerCube::addImages(const vector<Image>& images)
{
	if(images.size() != 6)
	{
		throw RuntimeError("samplerCube must have 6 faces. " + str::str(images.size()) + " were given.");
	}

	_bind();
	for(int i = 0; i < 6; i++)
	{
		uint format = get_format(images[i].channels());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, images[i].width(), images[i].height(), 0, format, GL_UNSIGNED_BYTE, images[i].data());
	}
}

void samplerCube::addImages(const vector<string>& filenames)
{
	if(filenames.size() != 6)
	{
		throw RuntimeError("samplerCube must have 6 faces. " + str::str(filenames.size()) + " were given.");
	}

	string path_key = get_path_key(filenames);
	bool need_add = (path_map.count(path_key) == 0);
	if(!need_add)
	{
		instance = path_map[path_key];
		_bind();
	}
	else
	{
		instance = NULL;
		_bind();
		Image image;
		for(int i = 0; i < 6; i++)
		{
			image.load(filenames[i], true);
			uint format = get_format(image.channels());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
		}
		path_map[path_key] = instance;
	}
}

void samplerCube::addImages(const Image& right, const Image& left,
	           const Image& up   , const Image& down,
	           const Image& back , const Image& front)
{
	vector<Image> images({right, left, up, down, back, front});
	addImages(images);
}

void samplerCube::addImages(const string& right, const string& left,
	           const string& up   , const string& down,
	           const string& back , const string& front)
{
	vector<string> filenames({right, left, up, down, back, front});
	addImages(filenames);
}

void samplerCube::addImage(const Image& image)
{
	uint rows = image.rows()/3;
	uint cols = image.cols()/4;

	addLeftImage(image.subimg(rows, rows, 0, cols));
	addUpImage(image.subimg(0, rows, cols, cols));
	addFrontImage(image.subimg(rows, rows, cols, cols));
	addDownImage(image.subimg(2*rows, rows, cols, cols));
	addRightImage(image.subimg(rows, rows, 2*cols, cols));
	addBackImage(image.subimg(rows, rows, 3*cols, cols));
}

void samplerCube::addImage(const string& filename)
{
	string path_key = path::abspath(filename);
	bool need_add = (path_map.count(path_key) == 0);
	if(!need_add)
	{
		instance = path_map[path_key];
		_bind();
	}
	else
	{
		instance = NULL;
		addImage(Image(filename));
		path_map[path_key] = instance;
	}
}

void samplerCube::border_color(const vec4& color)
{
	init();
	instance->border_color = color;
	instance->is_applied = false;
}

void samplerCube::border_color(const vec3& color)
{
	init();
	instance->border_color = vec4(color, 1);
	instance->is_applied = false;
}

vec4 samplerCube::border_color()const
{
	return (instance != NULL ? instance->border_color : vec4(0));
}

uint samplerCube::wrap_s()const
{
	return (instance != NULL ? instance->s_wrap_type : GL_CLAMP_TO_EDGE);
}

uint samplerCube::wrap_t()const
{
	return (instance != NULL ? instance->t_wrap_type : GL_CLAMP_TO_EDGE);
}

uint samplerCube::wrap_r()const
{
	return (instance != NULL ? instance->r_wrap_type : GL_CLAMP_TO_EDGE);
}

void samplerCube::wrap_s(uint wrap_type)
{
	switch(wrap_type)
	{
		case GL_REPEAT:
		case GL_MIRRORED_REPEAT:
		case GL_CLAMP_TO_EDGE:
		case GL_CLAMP_TO_BORDER:
		{
			init();
			instance->s_wrap_type = wrap_type;
			instance->is_applied = false;
			break;
		}
		default:
			throw ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
				             "Please choose wrap_type in [GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER].");
	}
}

void samplerCube::wrap_t(uint wrap_type)
{
	switch(wrap_type)
	{
		case GL_REPEAT:
		case GL_MIRRORED_REPEAT:
		case GL_CLAMP_TO_EDGE:
		case GL_CLAMP_TO_BORDER:
		{
			init();
			instance->t_wrap_type = wrap_type;
			instance->is_applied = false;
			break;
		}
		default:
			throw ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
				             "Please choose wrap_type in [GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER].");
	}
}

void samplerCube::wrap_r(uint wrap_type)
{
	switch(wrap_type)
	{
		case GL_REPEAT:
		case GL_MIRRORED_REPEAT:
		case GL_CLAMP_TO_EDGE:
		case GL_CLAMP_TO_BORDER:
		{
			init();
			instance->r_wrap_type = wrap_type;
			instance->is_applied = false;
			break;
		}
		default:
			throw ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
				             "Please choose wrap_type in [GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER].");
	}
}

void samplerCube::wrap(uint wrap_type_s, uint wrap_type_t, uint wrap_type_r)
{
	if(wrap_type_t == -1)
	{
		wrap_type_t = wrap_type_s;
	}
	if(wrap_type_r == -1)
	{
		wrap_type_r = wrap_type_t;
	}

	wrap_s(wrap_type_s);
	wrap_t(wrap_type_t);
	wrap_r(wrap_type_r);
}

uint samplerCube::filter_min()const
{
	return (instance != NULL ? instance->min_filter_type : GL_LINEAR);
}

uint samplerCube::filter_mag()const
{
	return (instance != NULL ? instance->mag_filter_type : GL_LINEAR);
}

void samplerCube::filter_min(uint filter_type)
{
	switch(filter_type)
	{
		case GL_NEAREST:
		case GL_LINEAR:
		{
			init();
			instance->min_filter_type = filter_type;
			instance->is_applied = false;
			break;
		}
		default:
			throw ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
				             "Please choose filter_type in [GL_NEAREST, GL_LINEAR].");
	}
}

void samplerCube::filter_mag(uint filter_type)
{
	switch(filter_type)
	{
		case GL_NEAREST:
		case GL_LINEAR:
		{
			init();
			instance->mag_filter_type = filter_type;
			instance->is_applied = false;
			break;
		}
		default:
			throw ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
				             "Please choose filter_type in [GL_NEAREST, GL_LINEAR].");
	}
}

void samplerCube::filter(uint filter_type_min, uint filter_type_mag)
{
	if(filter_type_mag == -1)
	{
		filter_type_mag = filter_type_min;
	}

	filter_min(filter_type_min);
	filter_mag(filter_type_mag);
}

// vec4 texture(const sampler2D& sampler, const vec2& texture_coord)
// {
// 	float x = texture_coord.x;
// 	float y = texture_coord.y;
// 	if(x < 0 || x > 1)
// 	{
// 		switch(sampler.wrap_s())
// 		{
// 			case GL_REPEAT: x = x - floor(x); break;
// 			case GL_MIRRORED_REPEAT: x = fabs(x-trunc(x)); break;
// 			case GL_CLAMP_TO_EDGE: x = min(max(x, 0.0f), 1.0f); break;
// 			default: break;
// 		}
// 	}
// 	if(y < 0 || y > 1)
// 	{
// 		switch(sampler.wrap_t())
// 		{
// 			case GL_REPEAT: y = y - floor(y); break;
// 			case GL_MIRRORED_REPEAT: y = fabs(y-trunc(y)); break;
// 			case GL_CLAMP_TO_EDGE: y = min(max(y, 0.0f), 1.0f); break;
// 			default: break;
// 		}
// 	}

// 	if(x < 0 || x > 1 || y < 0 || y > 1)
// 	{
// 		return vec4(sampler.border_color(), 1);
// 	}

// 	float col = x*(sampler.image().cols() - 1);
// 	float row = (1-y)*(sampler.image().rows() - 1);
// 	switch(sampler.filter_mag())
// 	{
// 		case GL_NEAREST:
// 		{
// 			int i = round(row);
// 			int j = round(col);
// 			return vec4(sampler.image()(i, j, 0)/255.0,
// 				        sampler.image()(i, j, 1)/255.0,
// 				        sampler.image()(i, j, 2)/255.0,
// 				        sampler.image()(i, j, 3)/255.0);
// 		}
// 		default:
// 		{
// 			vec4 result;
// 			int row_min = floor(row);
// 			int row_max = ceil(row);
// 			int col_min = floor(col);
// 			int col_max = floor(col);
// 			float row_offset = row - row_min;
// 			float col_offset = col - col_min;
// 			for(int i = 0; i < 4; i++)
// 			{
// 				result[i] = (1 - col_offset) * ((1 - row_offset) * sampler.image()(row_min, col_min, i)/255.0 +
// 					                                 row_offset  * sampler.image()(row_max, col_min, i)/255.0) +
// 				                 col_offset  * ((1 - row_offset) * sampler.image()(row_min, col_max, i)/255.0 +
// 					                                 row_offset  * sampler.image()(row_max, col_max, i)/255.0);
// 			}
// 			return result;
// 		}
// 	}
// }
