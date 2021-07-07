#include "glass/FBO"
#include "glass/utils/helper.h"

#ifdef USE_QT
#include <QOpenGLWidget>
#endif

using namespace std;

int FBO::vector_find(std::vector<_Attachment>& v, const std::string& name)
{
    for(uint i = 0; i < v.size(); i++)
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
buffer_type(attachment.buffer_type),
type(attachment.type),
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
buffer_type(move(attachment.buffer_type)),
type(move(attachment.type)),
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
	if(userData<FBO_Instance>()->clear_color.x == -1)
	{
		userData<FBO_Instance>()->clear_color = getClearColor();
	}
}

void FBO::del()
{
	BO::delWithUserData<FBO_Instance>();
}

FBO::~FBO()
{
	this->del();
}

FBO::FBO(uint screen_width, uint screen_height,
	     uint buffer_width, uint buffer_height) :
BO(FRAME)
{
	userData<FBO_Instance>()->screen_width = screen_width;
	userData<FBO_Instance>()->screen_height = screen_height;
	userData<FBO_Instance>()->buffer_width = buffer_width;
	userData<FBO_Instance>()->buffer_height = buffer_height;

	if(userData<FBO_Instance>()->buffer_width == 0)
	{
		userData<FBO_Instance>()->buffer_width = screen_width;
	}

	if(userData<FBO_Instance>()->buffer_height == 0)
	{
		userData<FBO_Instance>()->buffer_height = screen_height;
	}
}

FBO::FBO(const FBO& fbo) : BO(fbo) {}

FBO::FBO(FBO&& fbo) : BO(move(fbo)) {}

FBO& FBO::operator =(const FBO& fbo)
{
	if(this != &fbo && self != fbo.self)
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
	if(this != &fbo && self != fbo.self)
	{
		return static_cast<FBO&>(BO::operator=(std::move(fbo)));
	}
	else
	{
		return *this;
	}
}

uint FBO::screenWidth()
{
	return userData<FBO_Instance>()->screen_width;
}

uint FBO::screenHeight()
{
	return userData<FBO_Instance>()->screen_height;
}

uint FBO::bufferWidth()
{
	return userData<FBO_Instance>()->buffer_width;
}

uint FBO::bufferHeight()
{
	return userData<FBO_Instance>()->buffer_height;
}

void FBO::resize(uint __width, uint __height)
{
	screenResize(__width, __height);
	bufferResize(__width, __height);
}

void FBO::screenResize(uint __screen_width, uint __screen_height)
{
	init();
	userData<FBO_Instance>()->screen_width = __screen_width;
	userData<FBO_Instance>()->screen_height = __screen_height;
}

void FBO::bufferResize(uint __buffer_width, uint __buffer_height)
{
	init();
	userData<FBO_Instance>()->buffer_width = __buffer_width;
	userData<FBO_Instance>()->buffer_height = __buffer_height;

	if(userData<FBO_Instance>()->depth_attachment != NULL)
	{
		if(userData<FBO_Instance>()->depth_attachment->buffer_type == "sampler2D")
		{
			userData<FBO_Instance>()->depth_attachment->buffer<sampler2D>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
		else if(userData<FBO_Instance>()->depth_attachment->buffer_type == "samplerCube")
		{
			userData<FBO_Instance>()->depth_attachment->buffer<samplerCube>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
		else if(userData<FBO_Instance>()->depth_attachment->buffer_type == "RBO")
		{
			userData<FBO_Instance>()->depth_attachment->buffer<RBO>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
	}

	if(userData<FBO_Instance>()->stencil_attachment != NULL)
	{
		if(userData<FBO_Instance>()->stencil_attachment->buffer_type == "sampler2D")
		{
			userData<FBO_Instance>()->stencil_attachment->buffer<sampler2D>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
		else if(userData<FBO_Instance>()->stencil_attachment->buffer_type == "samplerCube")
		{
			userData<FBO_Instance>()->stencil_attachment->buffer<samplerCube>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
		else if(userData<FBO_Instance>()->stencil_attachment->buffer_type == "RBO")
		{
			userData<FBO_Instance>()->stencil_attachment->buffer<RBO>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
	}

	for(auto it = userData<FBO_Instance>()->color_attachments.begin(); it != userData<FBO_Instance>()->color_attachments.end(); it++)
	{
		if(it->buffer_type == "sampler2D")
		{
			it->buffer<sampler2D>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
		else if(it->buffer_type == "samplerCube")
		{
			it->buffer<samplerCube>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
		}
		else if(it->buffer_type == "RBO")
		{
			it->buffer<RBO>().realloc(userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
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
	userData<FBO_Instance>()->old_clear_color = getClearColor();
	
	// glCullFace(GL_FRONT);
	if(userData<FBO_Instance>()->depth_attachment != NULL)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if(userData<FBO_Instance>()->auto_resize)
	{
        glViewport(0, 0, userData<FBO_Instance>()->buffer_width, userData<FBO_Instance>()->buffer_height);
	}
	if(userData<FBO_Instance>()->auto_clear)
	{
		clear();
	}
}

void FBO::unbind()
{
	_unbind();
	// glCullFace(GL_BACK);
	if(userData<FBO_Instance>()->auto_resize)
	{
        double device_pixel_ratio = 1;
#ifdef USE_QT
        if(userData<FBO_Instance>()->device != NULL)
        {
            device_pixel_ratio = userData<FBO_Instance>()->device->devicePixelRatio();
        }
#endif
        glViewport(0, 0, device_pixel_ratio*userData<FBO_Instance>()->screen_width, device_pixel_ratio*userData<FBO_Instance>()->screen_height);
	}
	if(userData<FBO_Instance>()->auto_clear)
	{
		glClearColor(userData<FBO_Instance>()->old_clear_color.x, userData<FBO_Instance>()->old_clear_color.y, userData<FBO_Instance>()->old_clear_color.z, userData<FBO_Instance>()->old_clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
#ifdef USE_QT
    if(userData<FBO_Instance>()->device != NULL)
    {
        userData<FBO_Instance>()->device->makeCurrent();
    }
#endif
}

void FBO::autoClear(bool flag)
{
	init();
	userData<FBO_Instance>()->auto_clear = flag;
}

void FBO::autoResize(bool flag)
{
	init();
	userData<FBO_Instance>()->auto_resize = flag;
}

uint FBO::status()
{
	if(id() == 0)
	{
		return 0;
	}

	_bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

bool FBO::completed()
{
	if(id() == 0)
	{
		return false;
	}

	_bind();
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void FBO::clear()
{
	if(id() == 0 || userData<FBO_Instance>()->clear_value == 0)
	{
		return;
	}

	_bind();
	glClearColor(userData<FBO_Instance>()->clear_color.x, userData<FBO_Instance>()->clear_color.y, userData<FBO_Instance>()->clear_color.z, userData<FBO_Instance>()->clear_color.w);
	glClear(userData<FBO_Instance>()->clear_value);
}

void FBO::_apply()
{
	_bind();
	if(userData<FBO_Instance>()->is_applied == true)
	{
		return;
	}

	if(userData<FBO_Instance>()->color_attachments.empty() &&
	   userData<FBO_Instance>()->depth_attachment == NULL && 
	   userData<FBO_Instance>()->stencil_attachment == NULL)
	{
		_unbind();
		throw glass::RuntimeError("FBO is incompleted!");
	}

	// glEnable(GL_CULL_FACE);
	if(userData<FBO_Instance>()->color_attachments.empty())
	{
		glDrawBuffer(GL_NONE);
	    glReadBuffer(GL_NONE);
	}
	else if(userData<FBO_Instance>()->color_attachments.size() > 1)
	{
		vector<GLuint> attachments(userData<FBO_Instance>()->color_attachments.size());
        for(uint i = 0; i < attachments.size(); i++)
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

	userData<FBO_Instance>()->is_applied = true;
}

void FBO::clearColor(const vec3& c)
{
	init();
	userData<FBO_Instance>()->clear_color = vec4(c, 1.0);
}

void FBO::clearColor(const vec4& c)
{
	init();
	userData<FBO_Instance>()->clear_color = c;
}

void FBO::clearColor(float r, float g, float b, float a)
{
	init();
	userData<FBO_Instance>()->clear_color = vec4(r, g, b, a);
}

vec4 FBO::clearColor()
{
	return userData<FBO_Instance>()->clear_color;
}

#ifdef USE_QT
void FBO::setPaintDevice(QOpenGLWidget* device)
{
    init();
    userData<FBO_Instance>()->device = device;
}
#endif
