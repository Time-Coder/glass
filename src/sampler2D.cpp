#include <cmath>

#include "glass/common.h"
#include "glass/sampler2D"
#include "glass/utils/exceptions.h"
#include "glass/utils/helper.h"

using namespace std;

uint sampler2D::active_sampler2D = 0;
multiset<uint> sampler2D::existing_sampler2Ds;
list<sampler2D::Instance> sampler2D::existing_instances;
map<string, sampler2D::Instance*> sampler2D::path_map;

void sampler2D::destruct_all()
{
	if(sampler2D::active_sampler2D != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		sampler2D::active_sampler2D = 0;
	}
	for(auto it = sampler2D::existing_instances.begin(); it != sampler2D::existing_instances.end(); it++)
	{
	#ifdef _DEBUG
		cout << "destructing sampler2D " << it->id << endl;
	#endif
		glDeleteTextures(1, &(it->id));
	}
	sampler2D::existing_instances.clear();
	sampler2D::existing_sampler2Ds.clear();
	sampler2D::path_map.clear();
}

uint sampler2D::max_units()
{
	int _max_units = 16;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_max_units);
	return _max_units;
}

void sampler2D::init()
{
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
			throw glass::RuntimeError("Failed to create sampler2D!");
		}
		existing_sampler2Ds.insert(instance->id);
	}
	else if(existing_sampler2Ds.count(instance->id) == 0)
	{
		throw glass::RuntimeError("sampler2D " + str::str(instance->id) + " has already been destructed.");
	}
}

void sampler2D::del()
{
	if(instance == NULL)
	{
		has_path = false;
		return;
	}

	uint count = existing_sampler2Ds.count(instance->id);
	if(count > 0)
	{
		multiset_pop(existing_sampler2Ds, instance->id);
		if(count == 1 && !has_path)
		{
		#ifdef _DEBUG
			cout << "destructing sampler2D " << instance->id << endl;
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

sampler2D::sampler2D() {}

sampler2D::sampler2D(uint _width, uint _height, uint _format, uint _dtype)
{
	this->malloc(_width, _height, _format, _dtype);
}

sampler2D::sampler2D(const Image& image)
{
	setImage(image);
}

sampler2D::sampler2D(const string& filename)
{
	setImage(filename);
}

sampler2D::sampler2D(const sampler2D& sampler) :
instance(sampler.instance),
has_path(sampler.has_path)
{
	if(instance != NULL && existing_sampler2Ds.count(instance->id) > 0)
	{
		existing_sampler2Ds.insert(instance->id);
	}
}

sampler2D::sampler2D(sampler2D&& sampler) :
instance(move(sampler.instance)),
has_path(move(sampler.has_path))
{
	sampler.instance = NULL;
	sampler.has_path = false;
}

sampler2D::~sampler2D()
{
	del();
	if(existing_sampler2Ds.size() == 0 && existing_instances.size() > 0)
	{
		destruct_all();
	}
}

sampler2D& sampler2D::operator =(const sampler2D& sampler)
{
	if(this != &sampler && instance != sampler.instance)
	{
		del();

		instance = sampler.instance;
		has_path = sampler.has_path;

		if(instance != NULL && existing_sampler2Ds.count(instance->id) > 0)
		{
			existing_sampler2Ds.insert(instance->id);
		}
	}

	return *this;
}

sampler2D& sampler2D::operator =(sampler2D&& sampler)
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

bool sampler2D::empty()const
{
	return (instance == NULL || id() == 0 || width() == 0 || height() == 0);
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
	if(instance->min_filter_type != 0)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, instance->min_filter_type);
	if(instance->mag_filter_type != 0)
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

bool sampler2D::isBind()const
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
			throw glass::ValueError("Not supported wrap_type " + str::str(wrap_type) + ".\n" +
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
			throw glass::ValueError("Unsupported wrap_type " + str::str(wrap_type) + ".\n" +
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
	return (instance != NULL ? instance->min_filter_type : 0);
}

uint sampler2D::filter_mag()const
{
	return (instance != NULL ? instance->mag_filter_type : 0);
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
			throw glass::ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
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
			throw glass::ValueError("Unsupported filter_type " + str::str(filter_type) + ".\n" +
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

void width_adapt(uint width)
{
	if(width % 4 != 0)
	{
		if(width % 2 == 0) glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		else glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
}

uint sampler2D::generateMipmap()
{
	_bind();
	// glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateTextureMipmap(id());

	int max_levels;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_levels);
    return (uint)(log2((double)max_levels));
}

void sampler2D::setImage(const Image& image)
{
	_bind();

	instance->format = get_format(image.channels());
	instance->dtype = GL_UNSIGNED_BYTE;
	instance->width = image.width();
	instance->height = image.height();

	width_adapt(instance->width);
	glTexImage2D(GL_TEXTURE_2D, 0, instance->format, instance->width, instance->height, 0, instance->format, instance->dtype, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void sampler2D::setImage(const string& filename)
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
		setImage(Image(filename, true));
	    path_map[abspath] = instance;
	}
	has_path = true;
}

GLenum get_external_format(GLenum internal_format)
{
	GLenum external_format = GL_RGB;
	switch(internal_format)
	{
		case GL_RED:
		case GL_R8:
		case GL_R8_SNORM:
		case GL_R16:
		case GL_R16_SNORM:
		case GL_R16F:
		case GL_R32F:
		case GL_R8I:
		case GL_R8UI:
		case GL_R16I:
		case GL_R16UI:
		case GL_R32I:
		case GL_R32UI:
		case GL_COMPRESSED_RED:
		case GL_COMPRESSED_RED_RGTC1:
		case GL_COMPRESSED_SIGNED_RED_RGTC1:
			external_format = GL_RED;
			break;

		case GL_RG:
		case GL_RG8:
		case GL_RG8_SNORM:
		case GL_RG16:
		case GL_RG16_SNORM:
		case GL_RG16F:
		case GL_RG32F:
		case GL_RG8I:
		case GL_RG8UI:
		case GL_RG16I:
		case GL_RG16UI:
		case GL_RG32I:
		case GL_RG32UI:
		case GL_COMPRESSED_RG:
		case GL_COMPRESSED_RG_RGTC2:
		case GL_COMPRESSED_SIGNED_RG_RGTC2:
			external_format = GL_RG;
			break;

		case GL_RGB:
		case GL_R3_G3_B2:
		case GL_RGB4:
		case GL_RGB5:
		case GL_RGB8:
		case GL_RGB8_SNORM:
		case GL_RGB10:
		case GL_RGB12:
		case GL_RGB16_SNORM:
		case GL_SRGB8:
		case GL_RGB16F:
		case GL_RGB32F:
		case GL_R11F_G11F_B10F:
		case GL_RGB9_E5:
		case GL_RGB8I:
		case GL_RGB8UI:
		case GL_RGB16I:
		case GL_RGB16UI:
		case GL_RGB32I:
		case GL_RGB32UI:
		case GL_COMPRESSED_RGB:
		case GL_COMPRESSED_SRGB:
		case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
		case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
			external_format = GL_RGB;
			break;

		case GL_RGBA:
		case GL_RGBA2:
		case GL_RGBA4:
		case GL_RGB5_A1:
		case GL_RGBA8:
		case GL_RGBA8_SNORM:
		case GL_RGB10_A2:
		case GL_RGB10_A2UI:
		case GL_RGBA12:
		case GL_RGBA16:
		case GL_SRGB8_ALPHA8:
		case GL_RGBA16F:
		case GL_RGBA32F:
		case GL_RGBA8I:
		case GL_RGBA8UI:
		case GL_RGBA16I:
		case GL_RGBA16UI:
		case GL_RGBA32I:
		case GL_RGBA32UI:
		case GL_COMPRESSED_RGBA:
		case GL_COMPRESSED_SRGB_ALPHA:
		case GL_COMPRESSED_RGBA_BPTC_UNORM:
		case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
			external_format = GL_RGBA;
			break;

		case GL_STENCIL_INDEX:
			external_format = GL_STENCIL_INDEX;
			break;

		case GL_DEPTH_COMPONENT:
			external_format = GL_DEPTH_COMPONENT;
			break;

		case GL_DEPTH_STENCIL:
			external_format = GL_DEPTH_STENCIL;
			break;

		default:
			external_format = GL_RGB;
	}

	return external_format;
}

void sampler2D::malloc(uint _width, uint _height, uint _format, uint _dtype)
{
	if(width() == _width && height() == _height && format() == _format && dtype() == _dtype)
	{
		return;
	}

	_bind();

	width_adapt(_width);
	glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, get_external_format(_format), _dtype, NULL);
	instance->width = _width;
	instance->height = _height;
	instance->format = _format;
	instance->dtype = _dtype;
}

void sampler2D::realloc(uint _width, uint _height)
{
	if(width() == _width && height() == _height)
	{
		return;
	}

	_bind();

	width_adapt(_width);
	glTexImage2D(GL_TEXTURE_2D, 0, instance->format, _width, _height, 0, get_external_format(instance->format), instance->dtype, NULL);
	instance->width = _width;
	instance->height = _height;
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