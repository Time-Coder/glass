#include "glass/FBO"
#include "glass/utils/helper.h"

using namespace std;

multiset<uint> FBO::existing_FBOs;
uint FBO::active_FBO = 0;

int FBO::vector_find(std::vector<_Attachment>& v, const std::string& name)
{
	for(int i = 0; i < v.size(); i++)
	{
		if(v[i].name == name)
		{
			return i;
		}
	}
	return -1;
}

FBO::_Attachment::_Attachment(const string& _name) : name(_name)
{}

FBO::_Attachment::_Attachment(const FBO::_Attachment& attachment) :
parent(attachment.parent),
id(attachment.id),
type(attachment.type),
buffer_type(attachment.buffer_type),
format(attachment.format),
dtype(attachment.dtype),
_buffer(NULL),
name(attachment.name)
{
	if(attachment._buffer == NULL)
	{
		return;
	}

	if(buffer_type == "sampler2D")
		_buffer = (void*)(new sampler2D(*((sampler2D*)attachment._buffer)));
	else if(buffer_type == "samplerCube")
		_buffer = (void*)(new samplerCube(*((samplerCube*)attachment._buffer)));
	else if(buffer_type == "RBO")
		_buffer = (void*)(new RBO(*((RBO*)attachment._buffer)));
	else
		throw glass::TypeError("Unknown buffer type " + buffer_type);

	if(_buffer == NULL)
	{
		throw glass::MemoryError("Failed to allocate memory!");
	}
}

FBO::_Attachment::_Attachment(FBO::_Attachment&& attachment):
parent(move(attachment.parent)),
id(move(attachment.id)),
type(move(attachment.type)),
buffer_type(move(attachment.buffer_type)),
format(move(attachment.format)),
dtype(move(attachment.dtype)),
_buffer(move(attachment._buffer)),
name(move(attachment.name))
{
	attachment.parent = NULL;
	attachment._buffer = NULL;
}

FBO::_Attachment& FBO::_Attachment::operator =(const FBO::_Attachment& attachment)
{
	if(&attachment == this)
	{
		return *this;
	}

	clear();

	parent = attachment.parent;
	id = attachment.id;
	type = attachment.type;
	buffer_type = attachment.buffer_type;
	format = attachment.format;
	dtype = attachment.dtype;
	_buffer = NULL;
	name = attachment.name;

	if(attachment._buffer == NULL)
	{
		return *this;
	}

	if(buffer_type == "sampler2D")
		_buffer = (void*)(new sampler2D(*((sampler2D*)attachment._buffer)));
	else if(buffer_type == "samplerCube")
		_buffer = (void*)(new samplerCube(*((samplerCube*)attachment._buffer)));
	else if(buffer_type == "RBO")
		_buffer = (void*)(new RBO(*((RBO*)attachment._buffer)));
	else
		throw glass::TypeError("Unknown buffer type " + buffer_type);

	if(_buffer == NULL)
	{
		throw glass::MemoryError("Failed to allocate memory!");
	}

	return *this;
}

FBO::_Attachment& FBO::_Attachment::operator =(FBO::_Attachment&& attachment)
{
	if(&attachment == this)
	{
		return *this;
	}

	clear();

	parent = move(attachment.parent);
	id = move(attachment.id);
	type = move(attachment.type);
	buffer_type = move(attachment.buffer_type);
	format = move(attachment.format);
	dtype = move(attachment.dtype);
	_buffer = move(attachment._buffer);
	name = move(attachment.name);

	attachment.parent = NULL;
	attachment._buffer = NULL;

	return *this;
}

bool FBO::_Attachment::empty()const
{
	return (parent == NULL || _buffer == NULL);
}

void FBO::_Attachment::clear()
{
	if(_buffer == NULL)
	{
		return;
	}

	if(buffer_type == "sampler2D")
	{
		delete (sampler2D*)_buffer;
	}
	else if(buffer_type == "samplerCube")
	{
		delete (samplerCube*)_buffer;
	}
	else if(buffer_type == "RBO")
	{
		delete (RBO*)_buffer;
	}
	_buffer = NULL;
}

FBO::_Attachment::~_Attachment()
{
	clear();
}

vec4 getClearColor()
{
	float color[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
	vec4 result;
	result.x = color[0];
	result.y = color[1];
	result.z = color[2];
	result.w = color[3];

	return result;
}

void FBO::init()
{
	if(_id == 0)
	{
		glGenFramebuffers(1, &_id);
		if(_id == 0)
		{
			throw glass::RuntimeError("Failed to create FBO!");
		}
	#ifdef _DEBUG
		cout << "constructing FBO " << _id << endl;
	#endif
		existing_FBOs.insert(_id);
		if(clear_color.x == -1)
		{
			clear_color = getClearColor();
		}
	}
	else if(existing_FBOs.count(_id) == 0)
	{
		throw glass::RuntimeError("FBO " + str::str(_id) + " has already been destructed.");
	}
}

void FBO::del()
{
	uint count = existing_FBOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_FBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing FBO " << _id << endl;
		#endif
			unbind();
			glDeleteFramebuffers(1, &_id);
		}
	}

	if(depth_attachment != NULL)
	{
		delete depth_attachment;
		depth_attachment = NULL;
	}
	if(stencil_attachment != NULL)
	{
		delete stencil_attachment;
		stencil_attachment = NULL;
	}
}

FBO::FBO(uint __screen_width, uint __screen_height,
	     uint __buffer_width, uint __buffer_height) :
_screen_width(__screen_width),
_screen_height(__screen_height),
_buffer_width(__buffer_width),
_buffer_height(__buffer_height)
{
	if(_buffer_width == 0)
	{
		_buffer_width = _screen_width;
	}

	if(_buffer_height == 0)
	{
		_buffer_height = _screen_height;
	}
}

FBO::FBO(const FBO& fbo) :
_id(fbo._id),
_screen_width(fbo._screen_width),
_screen_height(fbo._screen_height),
_buffer_width(fbo._buffer_width),
_buffer_height(fbo._buffer_height),
color_attachments(fbo.color_attachments),
is_applied(fbo.is_applied),
_auto_clear(fbo._auto_clear),
_auto_resize(fbo._auto_resize),
clear_value(fbo.clear_value),
clear_color(fbo.clear_color),
old_clear_color(fbo.old_clear_color),
max_color_attachments(fbo.max_color_attachments)
{
	if(existing_FBOs.count(_id))
	{
		existing_FBOs.insert(_id);
	}

	if(fbo.depth_attachment != NULL)
	{
		depth_attachment = new _Attachment(*fbo.depth_attachment);
		if(depth_attachment == NULL)
		{
			throw glass::MemoryError("Failed to allocate memory!");
		}
	}
	if(fbo.stencil_attachment != NULL)
	{
		stencil_attachment = new _Attachment(*fbo.stencil_attachment);
		if(stencil_attachment == NULL)
		{
			throw glass::MemoryError("Failed to allocate memory!");
		}
	}
}

FBO::FBO(FBO&& fbo) :
_id(move(fbo._id)),
_screen_width(move(fbo._screen_width)),
_screen_height(move(fbo._screen_height)),
_buffer_width(move(fbo._buffer_width)),
_buffer_height(move(fbo._buffer_height)),
color_attachments(move(fbo.color_attachments)),
is_applied(move(fbo.is_applied)),
_auto_clear(move(fbo._auto_clear)),
_auto_resize(move(fbo._auto_resize)),
clear_value(move(fbo.clear_value)),
clear_color(move(fbo.clear_color)),
old_clear_color(move(fbo.old_clear_color)),
max_color_attachments(move(fbo.max_color_attachments)),
depth_attachment(move(fbo.depth_attachment)),
stencil_attachment(move(fbo.stencil_attachment))
{
	fbo._id = 0;
	fbo.depth_attachment = NULL;
	fbo.stencil_attachment = NULL;
}

FBO::~FBO()
{
	del();
}

FBO& FBO::operator =(const FBO& fbo)
{
	if(this != &fbo && _id != fbo._id)
	{
		del();

		_id = fbo._id;
		_screen_width = fbo._screen_width;
		_screen_height = fbo._screen_height;
		_buffer_width = fbo._buffer_width;
		_buffer_height = fbo._buffer_height;
		color_attachments = fbo.color_attachments;
		is_applied = fbo.is_applied;
		_auto_clear = fbo._auto_clear;
		_auto_resize = fbo._auto_resize;
		clear_value = fbo.clear_value;
		clear_color = fbo.clear_color;
		old_clear_color = fbo.old_clear_color;
		max_color_attachments = fbo.max_color_attachments;

		if(fbo.depth_attachment != NULL)
		{
			depth_attachment = new _Attachment(*fbo.depth_attachment);
			if(depth_attachment == NULL)
			{
				throw glass::MemoryError("Failed to allocate memory!");
			}
		}
		if(fbo.stencil_attachment != NULL)
		{
			stencil_attachment = new _Attachment(*fbo.stencil_attachment);
			if(stencil_attachment == NULL)
			{
				throw glass::MemoryError("Failed to allocate memory!");
			}
		}

		if(existing_FBOs.count(_id) > 0)
		{
			existing_FBOs.insert(_id);
		}
	}
	return *this;
}

FBO& FBO::operator =(FBO&& fbo)
{
	if(this != &fbo)
	{
		if(_id != fbo._id)
		{
			del();
		}

		_id = move(fbo._id);
		_screen_width = move(fbo._screen_width);
		_screen_height = move(fbo._screen_height);
		_buffer_width = move(fbo._buffer_width);
		_buffer_height = move(fbo._buffer_height);
		color_attachments = move(fbo.color_attachments);
		is_applied = move(fbo.is_applied);
		_auto_clear = move(fbo._auto_clear);
		_auto_resize = move(fbo._auto_resize);
		clear_value = move(fbo.clear_value);
		clear_color = move(fbo.clear_color);
		old_clear_color = move(fbo.old_clear_color);
		max_color_attachments = move(fbo.max_color_attachments);
		depth_attachment = move(fbo.depth_attachment);
		stencil_attachment = move(fbo.stencil_attachment);

		fbo._id = 0;
		fbo.depth_attachment = NULL;
		fbo.stencil_attachment = NULL;
	}
	return *this;
}

uint FBO::id()const
{
	return _id;
}

uint FBO::screenWidth()const
{
	return _screen_width;
}

uint FBO::screenHeight()const
{
	return _screen_height;
}

uint FBO::bufferWidth()const
{
	return _buffer_width;
}

uint FBO::bufferHeight()const
{
	return _buffer_height;
}

void FBO::resize(uint __width, uint __height)
{
	screenResize(__width, __height);
	bufferResize(__width, __height);
}

void FBO::screenResize(uint __screen_width, uint __screen_height)
{
	_screen_width = __screen_width;
	_screen_height = __screen_height;
}

void FBO::bufferResize(uint __buffer_width, uint __buffer_height)
{
	_buffer_width = __buffer_width;
	_buffer_height = __buffer_height;

	if(depth_attachment != NULL)
	{
		if(depth_attachment->buffer_type == "sampler2D")
		{
			depth_attachment->buffer<sampler2D>().realloc(_buffer_width, _buffer_height);
		}
		else if(depth_attachment->buffer_type == "samplerCube")
		{
			depth_attachment->buffer<samplerCube>().realloc(_buffer_width, _buffer_height);
		}
		else if(depth_attachment->buffer_type == "RBO")
		{
			depth_attachment->buffer<RBO>().realloc(_buffer_width, _buffer_height);
		}
	}

	if(stencil_attachment != NULL)
	{
		if(stencil_attachment->buffer_type == "sampler2D")
		{
			stencil_attachment->buffer<sampler2D>().realloc(_buffer_width, _buffer_height);
		}
		else if(stencil_attachment->buffer_type == "samplerCube")
		{
			stencil_attachment->buffer<samplerCube>().realloc(_buffer_width, _buffer_height);
		}
		else if(stencil_attachment->buffer_type == "RBO")
		{
			stencil_attachment->buffer<RBO>().realloc(_buffer_width, _buffer_height);
		}
	}

	for(auto it = color_attachments.begin(); it != color_attachments.end(); it++)
	{
		if(it->buffer_type == "sampler2D")
		{
			it->buffer<sampler2D>().realloc(_buffer_width, _buffer_height);
		}
		else if(it->buffer_type == "samplerCube")
		{
			it->buffer<samplerCube>().realloc(_buffer_width, _buffer_height);
		}
		else if(it->buffer_type == "RBO")
		{
			it->buffer<RBO>().realloc(_buffer_width, _buffer_height);
		}
	}
}

void FBO::_bind()
{
	init();
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	active_FBO = _id;
}

void FBO::bind()
{
	old_clear_color = getClearColor();
	apply();
	// glCullFace(GL_FRONT);
	if(depth_attachment != NULL)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if(_auto_resize)
	{
		glViewport(0, 0, _buffer_width, _buffer_height);
	}
	if(_auto_clear)
	{
		clear();
	}
}

void FBO::_unbind()const
{
	if(active_FBO == _id)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		active_FBO = 0;
	}
}

void FBO::unbind()const
{
	_unbind();
	// glCullFace(GL_BACK);
	if(_auto_resize)
	{
		glViewport(0, 0, _screen_width, _screen_height);
	}
	if(_auto_clear)
	{
		glClearColor(old_clear_color.x, old_clear_color.y, old_clear_color.z, old_clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void FBO::autoClear(bool flag)
{
	_auto_clear = flag;
}

void FBO::autoResize(bool flag)
{
	_auto_resize = flag;
}

uint FBO::status()
{
	if(_id == 0)
	{
		return 0;
	}

	bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

bool FBO::completed()
{
	if(_id == 0)
	{
		return false;
	}

	bind();
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void FBO::clear()
{
	if(_id == 0 || clear_value == 0)
	{
		return;
	}

	_bind();
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(clear_value);
}

void FBO::apply()
{
	if(is_applied == true)
	{
		return;
	}

	if(color_attachments.empty() &&
	   depth_attachment == NULL && 
	   stencil_attachment == NULL)
	{
		throw glass::RuntimeError("FBO is incompleted!");
	}

	_bind();
	// glEnable(GL_CULL_FACE);
	if(color_attachments.empty())
	{
		glDrawBuffer(GL_NONE);
	    glReadBuffer(GL_NONE);
	}
	else if(color_attachments.size() > 1)
	{
		vector<GLuint> attachments(color_attachments.size());
		for(int i = 0; i < attachments.size(); i++)
		{
			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(attachments.size(), attachments.data());
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		_unbind();
		throw glass::RuntimeError("FBO is incompleted.");
	}

	is_applied = true;
}

void FBO::clearColor(const vec3& c)
{
	clear_color = vec4(c, 1.0);
}

void FBO::clearColor(const vec4& c)
{
	clear_color = c;
}

void FBO::clearColor(float r, float g, float b, float a)
{
	clear_color = vec4(r, g, b, a);
}

vec4 FBO::clearColor()
{
	return clear_color;
}