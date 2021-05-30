#include "glass/common.h"
#include "glass/samplerCube"
#include "glass/utils/exceptions.h"
#include "glass/utils/helper.h"

using namespace std;

uint samplerCube::active_samplerCube = 0;
multiset<uint> samplerCube::existing_samplerCubes;
list<samplerCube::Instance> samplerCube::existing_instances;
map<string, samplerCube::Instance*> samplerCube::path_map;

void width_adapt(uint width);

void samplerCube::destruct_all()
{
	if(samplerCube::active_samplerCube != 0)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		samplerCube::active_samplerCube = 0;
	}
	for(auto it = samplerCube::existing_instances.begin(); it != samplerCube::existing_instances.end(); it++)
	{
	#ifdef _DEBUG
		cout << "destructing samplerCube " << it->id << endl;
	#endif
		glDeleteTextures(1, &(it->id));
	}
	samplerCube::existing_instances.clear();
	samplerCube::path_map.clear();
	samplerCube::existing_samplerCubes.clear();
}

void samplerCube::init()
{
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
			throw glass::Exception("Failed to create samplerCube!");
		}
		existing_samplerCubes.insert(instance->id);
	}
	else if(existing_samplerCubes.count(instance->id) == 0)
	{
		throw glass::RuntimeError("samplerCube " + str::str(instance->id) + " has already been destructed.");
	}
}

void samplerCube::del()
{
	if(instance == NULL)
	{
		has_path = false;
		return;
	}

	uint count = existing_samplerCubes.count(instance->id);
	if(count > 0)
	{
		multiset_pop(existing_samplerCubes, instance->id);
		if(count == 1 && !has_path)
		{
		#ifdef _DEBUG
			cout << "destructing samplerCube " << instance->id << endl;
		#endif
			unbind();
			glDeleteTextures(1, &(instance->id));
			for(auto it = existing_instances.begin(); it != existing_instances.end(); it++)
			{
				if(&(*it) == instance)
				{
					existing_instances.erase(it);
					break;
				}
			}
		}
	}
	instance = NULL;
	has_path = false;
}

samplerCube::samplerCube() {}

samplerCube::samplerCube(uint __width, uint __height, uint __format, uint __dtype)
{
	this->malloc(__width, __height, __format, __dtype);
}

samplerCube::samplerCube(vector<Image>& images)
{
	setImages(images);
}

samplerCube::samplerCube(const vector<string>& filenames)
{
	setImages(filenames);
}

samplerCube::samplerCube(const Image& right, const Image& left,
	                     const Image& up   , const Image& down,
	                     const Image& back , const Image& front)
{
	setImages(right, left, up, down, back, front);
}

samplerCube::samplerCube(const string& right, const string& left,
	                     const string& up   , const string& down,
	                     const string& back , const string& front)
{
	setImages(right, left, up, down, back, front);
}

samplerCube::samplerCube(const Image& image)
{
	setImage(image);
}

samplerCube::samplerCube(const string& filename)
{
	setImage(filename);
}

samplerCube::samplerCube(const samplerCube& sampler) :
instance(sampler.instance),
has_path(sampler.has_path)
{
	if(instance != NULL && existing_samplerCubes.count(instance->id) > 0)
	{
		existing_samplerCubes.insert(instance->id);
	}
}

samplerCube::samplerCube(samplerCube&& sampler) :
instance(move(sampler.instance)),
has_path(move(sampler.has_path))
{
	sampler.instance = NULL;
	sampler.has_path = false;
}

samplerCube::~samplerCube()
{
	del();
	if(existing_samplerCubes.size() == 0 && existing_instances.size() > 0)
	{
		destruct_all();
	}
}

samplerCube& samplerCube::operator =(const samplerCube& sampler)
{
	if(this != &sampler && instance != sampler.instance)
	{
		del();

		instance = sampler.instance;
		has_path = sampler.has_path;

		if(instance != NULL && existing_samplerCubes.count(instance->id) > 0)
		{
			existing_samplerCubes.insert(instance->id);
		}
	}

	return *this;
}

samplerCube& samplerCube::operator =(samplerCube&& sampler)
{
	if(this != &sampler)
	{
		if(instance != sampler.instance)
		{
			del();
		}
		instance = move(sampler.instance);
		has_path = move(sampler.has_path);
		sampler.instance = NULL;
		sampler.has_path = false;
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

bool samplerCube::isBind()const
{
	return (existing_samplerCubes.count(id()) && active_samplerCube == id());
}

uint samplerCube::width(uint i)const
{
	if(instance == NULL || i >= 6)
	{
		return 0;
	}

	return instance->width[i];
}

uint samplerCube::height(uint i)const
{
	if(instance == NULL || i >= 6)
	{
		return 0;
	}

	return instance->height[i];
}

uint samplerCube::dtype(uint i)const
{
	if(instance == NULL || i >= 6)
	{
		return 0;
	}

	return instance->dtype[i];
}

uint samplerCube::format(uint i)const
{
	if(instance == NULL || i >= 6)
	{
		return 0;
	}

	return instance->format[i];
}

bool samplerCube::completed()const
{
	return (instance != NULL && id() != 0 &&
		    width(0) != 0 && height(0) != 0 &&
		    width(1) != 0 && height(1) != 0 &&
		    width(2) != 0 && height(2) != 0 &&
		    width(3) != 0 && height(3) != 0 &&
		    width(4) != 0 && height(4) != 0 &&
		    width(5) != 0 && height(5) != 0);
}

void samplerCube::setRightImage(const string& filename)
{
	setRightImage(Image(filename));
}

uint get_format(uint n_channels);
void samplerCube::setRightImage(const Image& image)
{
	_bind();

	uint format = get_format(image.channels());

	width_adapt(image.width());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
	
	instance->width[0] = image.width();
	instance->height[0] = image.height();
	instance->format[0] = format;
	instance->dtype[0] = GL_UNSIGNED_BYTE;
}

void samplerCube::setLeftImage(const string& filename)
{
	setLeftImage(Image(filename));
}

void samplerCube::setLeftImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());

	width_adapt(image.width());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());

	instance->width[1] = image.width();
	instance->height[1] = image.height();
	instance->format[1] = format;
	instance->dtype[1] = GL_UNSIGNED_BYTE;
}

void samplerCube::setUpImage(const string& filename)
{
	setUpImage(Image(filename));
}

void samplerCube::setUpImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());

	width_adapt(image.width());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());

	instance->width[2] = image.width();
	instance->height[2] = image.height();
	instance->format[2] = format;
	instance->dtype[2] = GL_UNSIGNED_BYTE;
}

void samplerCube::setDownImage(const string& filename)
{
	setDownImage(Image(filename));
}

void samplerCube::setDownImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());

	width_adapt(image.width());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());

	instance->width[3] = image.width();
	instance->height[3] = image.height();
	instance->format[3] = format;
	instance->dtype[3] = GL_UNSIGNED_BYTE;
}

void samplerCube::setFrontImage(const string& filename)
{
	setFrontImage(Image(filename));
}

void samplerCube::setFrontImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());

	width_adapt(image.width());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());

	instance->width[5] = image.width();
	instance->height[5] = image.height();
	instance->format[5] = format;
	instance->dtype[5] = GL_UNSIGNED_BYTE;
}

void samplerCube::setBackImage(const string& filename)
{
	setBackImage(Image(filename));
}

void samplerCube::setBackImage(const Image& image)
{
	_bind();
	uint format = get_format(image.channels());

	width_adapt(image.width());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());

	instance->width[4] = image.width();
	instance->height[4] = image.height();
	instance->format[4] = format;
	instance->dtype[4] = GL_UNSIGNED_BYTE;
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

void samplerCube::setImages(const vector<Image>& images)
{
	if(images.size() != 6)
	{
		throw glass::RuntimeError("samplerCube must have 6 faces. " + str::str(images.size()) + " were given.");
	}

	_bind();
	for(int i = 0; i < 6; i++)
	{
		uint format = get_format(images[i].channels());

		width_adapt(images[i].width());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, images[i].width(), images[i].height(), 0, format, GL_UNSIGNED_BYTE, images[i].data());

		instance->width[i] = images[i].width();
		instance->height[i] = images[i].height();
		instance->format[i] = format;
		instance->dtype[i] = GL_UNSIGNED_BYTE;
	}
}

void samplerCube::setImages(const vector<string>& filenames)
{
	if(filenames.size() != 6)
	{
		throw glass::RuntimeError("samplerCube must have 6 faces. " + str::str(filenames.size()) + " were given.");
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

			width_adapt(image.width());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());

			instance->width[i] = image.width();
			instance->height[i] = image.height();
			instance->format[i] = format;
			instance->dtype[i] = GL_UNSIGNED_BYTE;
		}
		path_map[path_key] = instance;
	}
	has_path = true;
}

void samplerCube::setImages(const Image& right, const Image& left,
	           const Image& up   , const Image& down,
	           const Image& back , const Image& front)
{
	vector<Image> images({right, left, up, down, back, front});
	setImages(images);
}

void samplerCube::setImages(const string& right, const string& left,
	           const string& up   , const string& down,
	           const string& back , const string& front)
{
	vector<string> filenames({right, left, up, down, back, front});
	setImages(filenames);
}

void samplerCube::setImage(const Image& image)
{
	uint rows = image.rows()/3;
	uint cols = image.cols()/4;

	setLeftImage(image.subimg(rows, rows, 0, cols));
	setUpImage(image.subimg(0, rows, cols, cols));
	setFrontImage(image.subimg(rows, rows, cols, cols));
	setDownImage(image.subimg(2*rows, rows, cols, cols));
	setRightImage(image.subimg(rows, rows, 2*cols, cols));
	setBackImage(image.subimg(rows, rows, 3*cols, cols));
}

void samplerCube::setImage(const string& filename)
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
		setImage(Image(filename));
		path_map[path_key] = instance;
	}
	has_path = true;
}

GLenum get_external_format(GLenum internal_format);
void samplerCube::malloc(uint _width, uint _height, uint _format, uint _dtype)
{
	_bind();
	for(int i = 0; i < 6; i++)
	{
		width_adapt(_width);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _format, 
	                 _width, _height, 0, get_external_format(_format), _dtype, NULL);

		instance->width[i] = _width;
		instance->height[i] = _height;
		instance->format[i] = _format;
		instance->dtype[i] = _dtype;
	}
}

void samplerCube::realloc(uint _width, uint _height)
{
	_bind();
	for(int i = 0; i < 6; i++)
	{
		width_adapt(_width);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, instance->format[i], 
	                 _width, _height, 0, get_external_format(instance->format[i]), instance->dtype[i], NULL);

		instance->width[i] = _width;
		instance->height[i] = _height;
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
			throw glass::ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
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
			throw glass::ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
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
			throw glass::ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
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
			throw glass::ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
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
			throw glass::ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
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