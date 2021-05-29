#include "glass/FBO"
#include "glass/utils/helper.h"

using namespace std;

unordered_map<uint, FBO::FBO_Instance> FBO::existing_FBOs;

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

FBO::FBO_Instance::~FBO_Instance()
{
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
	BO::init();
	if(existing_FBOs[_id].clear_color.x == -1)
	{
		existing_FBOs[_id].clear_color = getClearColor();
	}
}

void FBO::del()
{
	if(existing_BOs[FRAME].count(_id) && existing_BOs[FRAME][_id].n_sources == 1)
	{
		existing_FBOs.erase(_id);
	}
	BO::del();
}

FBO::~FBO()
{
	del();
}

FBO::FBO(uint screen_width, uint screen_height,
	     uint buffer_width, uint buffer_height) :
BO(FRAME)
{
	init();
	existing_FBOs[_id].screen_width = screen_width;
	existing_FBOs[_id].screen_height = screen_height;
	existing_FBOs[_id].buffer_width = buffer_width;
	existing_FBOs[_id].buffer_height = buffer_height;

	if(existing_FBOs[_id].buffer_width == 0)
	{
		existing_FBOs[_id].buffer_width = screen_width;
	}

	if(existing_FBOs[_id].buffer_height == 0)
	{
		existing_FBOs[_id].buffer_height = screen_height;
	}
}

FBO::FBO(const FBO& fbo) : BO(fbo) {}

FBO::FBO(FBO&& fbo) : BO(move(fbo)) {}

FBO& FBO::operator =(const FBO& fbo)
{
	if(this != &fbo && _id != fbo._id)
	{
		return static_cast<FBO&>(BO::operator=(fbo));
	}
	else
	{
		return *this;
	}
}

FBO& FBO::operator =(FBO&& fbo)
{
	if(this != &fbo && _id != fbo._id)
	{
		return static_cast<FBO&>(BO::operator=(std::move(fbo)));
	}
	else
	{
		return *this;
	}
}

uint FBO::screenWidth()const
{
	return existing_FBOs[_id].screen_width;
}

uint FBO::screenHeight()const
{
	return existing_FBOs[_id].screen_height;
}

uint FBO::bufferWidth()const
{
	return existing_FBOs[_id].buffer_width;
}

uint FBO::bufferHeight()const
{
	return existing_FBOs[_id].buffer_height;
}

void FBO::resize(uint __width, uint __height)
{
	screenResize(__width, __height);
	bufferResize(__width, __height);
}

void FBO::screenResize(uint __screen_width, uint __screen_height)
{
	init();
	existing_FBOs[_id].screen_width = __screen_width;
	existing_FBOs[_id].screen_height = __screen_height;
}

void FBO::bufferResize(uint __buffer_width, uint __buffer_height)
{
	init();
	existing_FBOs[_id].buffer_width = __buffer_width;
	existing_FBOs[_id].buffer_height = __buffer_height;

	if(existing_FBOs[_id].depth_attachment != NULL)
	{
		if(existing_FBOs[_id].depth_attachment->buffer_type == "sampler2D")
		{
			existing_FBOs[_id].depth_attachment->buffer<sampler2D>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
		else if(existing_FBOs[_id].depth_attachment->buffer_type == "samplerCube")
		{
			existing_FBOs[_id].depth_attachment->buffer<samplerCube>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
		else if(existing_FBOs[_id].depth_attachment->buffer_type == "RBO")
		{
			existing_FBOs[_id].depth_attachment->buffer<RBO>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
	}

	if(existing_FBOs[_id].stencil_attachment != NULL)
	{
		if(existing_FBOs[_id].stencil_attachment->buffer_type == "sampler2D")
		{
			existing_FBOs[_id].stencil_attachment->buffer<sampler2D>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
		else if(existing_FBOs[_id].stencil_attachment->buffer_type == "samplerCube")
		{
			existing_FBOs[_id].stencil_attachment->buffer<samplerCube>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
		else if(existing_FBOs[_id].stencil_attachment->buffer_type == "RBO")
		{
			existing_FBOs[_id].stencil_attachment->buffer<RBO>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
	}

	for(auto it = existing_FBOs[_id].color_attachments.begin(); it != existing_FBOs[_id].color_attachments.end(); it++)
	{
		if(it->buffer_type == "sampler2D")
		{
			it->buffer<sampler2D>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
		else if(it->buffer_type == "samplerCube")
		{
			it->buffer<samplerCube>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
		else if(it->buffer_type == "RBO")
		{
			it->buffer<RBO>().realloc(existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
		}
	}
}

void FBO::_bind()
{
	BO::bind();
}

void FBO::_unbind()const
{
	BO::unbind();
}

void FBO::bind()
{
	_apply();
	existing_FBOs[_id].old_clear_color = getClearColor();
	
	// glCullFace(GL_FRONT);
	if(existing_FBOs[_id].depth_attachment != NULL)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if(existing_FBOs[_id].auto_resize)
	{
		glViewport(0, 0, existing_FBOs[_id].buffer_width, existing_FBOs[_id].buffer_height);
	}
	if(existing_FBOs[_id].auto_clear)
	{
		clear();
	}
}

void FBO::unbind()const
{
	_unbind();
	// glCullFace(GL_BACK);
	if(existing_FBOs[_id].auto_resize)
	{
		glViewport(0, 0, existing_FBOs[_id].screen_width, existing_FBOs[_id].screen_height);
	}
	if(existing_FBOs[_id].auto_clear)
	{
		glClearColor(existing_FBOs[_id].old_clear_color.x, existing_FBOs[_id].old_clear_color.y, existing_FBOs[_id].old_clear_color.z, existing_FBOs[_id].old_clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void FBO::autoClear(bool flag)
{
	init();
	existing_FBOs[_id].auto_clear = flag;
}

void FBO::autoResize(bool flag)
{
	init();
	existing_FBOs[_id].auto_resize = flag;
}

uint FBO::status()
{
	if(_id == 0)
	{
		return 0;
	}

	_bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

bool FBO::completed()
{
	if(_id == 0)
	{
		return false;
	}

	_bind();
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void FBO::clear()
{
	if(_id == 0 || (existing_FBOs.count(_id) && existing_FBOs[_id].clear_value == 0))
	{
		return;
	}

	_bind();
	glClearColor(existing_FBOs[_id].clear_color.x, existing_FBOs[_id].clear_color.y, existing_FBOs[_id].clear_color.z, existing_FBOs[_id].clear_color.w);
	glClear(existing_FBOs[_id].clear_value);
}

void FBO::_apply()
{
	_bind();
	if(existing_FBOs[_id].is_applied == true)
	{
		return;
	}

	if(existing_FBOs[_id].color_attachments.empty() &&
	   existing_FBOs[_id].depth_attachment == NULL && 
	   existing_FBOs[_id].stencil_attachment == NULL)
	{
		_unbind();
		throw glass::RuntimeError("FBO is incompleted!");
	}

	// glEnable(GL_CULL_FACE);
	if(existing_FBOs[_id].color_attachments.empty())
	{
		glDrawBuffer(GL_NONE);
	    glReadBuffer(GL_NONE);
	}
	else if(existing_FBOs[_id].color_attachments.size() > 1)
	{
		vector<GLuint> attachments(existing_FBOs[_id].color_attachments.size());
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

	existing_FBOs[_id].is_applied = true;
}

void FBO::clearColor(const vec3& c)
{
	init();
	existing_FBOs[_id].clear_color = vec4(c, 1.0);
}

void FBO::clearColor(const vec4& c)
{
	init();
	existing_FBOs[_id].clear_color = c;
}

void FBO::clearColor(float r, float g, float b, float a)
{
	init();
	existing_FBOs[_id].clear_color = vec4(r, g, b, a);
}

vec4 FBO::clearColor()
{
	return existing_FBOs[_id].clear_color;
}