#include <image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <path.h>
#include <exception.h>
#include <color.h>

#ifdef USE_TIFF
#include <tiffio.h>
#endif

Image::ColorRef::Component::Component(unsigned char& value)
{
	ptr = &value;
	first = false;
}

Image::ColorRef::Component::operator float()const
{
	return (*ptr)/255.0f;
}

unsigned char Image::ColorRef::Component::operator =(unsigned char& value)
{
	if(first)
	{
		ptr = &value;
		first = false;
	}
	else
	{
		*ptr = value;
	}

	return value;
}

float Image::ColorRef::Component::operator =(const float& value)
{
	*ptr = (unsigned char)(value * 255);
	return value;
}

Image::ColorRef::Component& Image::ColorRef::Component::operator =(const Image::ColorRef::Component& component)
{
	*ptr = *(component.ptr);
	return *this;
}

ostream& operator <<(ostream& out, const Image::ColorRef::Component& component)
{
	out << (float)component;
	return out;
}

Image::ColorRef::ColorRef(Color& color)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;
}

Image::ColorRef& Image::ColorRef::operator =(const Color& color)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;

	return *this;
}

Image::ColorRef& Image::ColorRef::operator =(const ColorRef& cr)
{
	if(this == &cr)
	{
		return *this;
	}

	R = cr.R;
	G = cr.G;
	B = cr.B;
	A = cr.A;

	return *this;
}

Image::ColorRef::operator Color()const
{
	Color color;
	color.R = R;
	color.G = G;
	color.B = B;
	color.A = A;

	return color;
}

ostream& operator <<(ostream& out, const Image::ColorRef& ref)
{
	out << (Color)ref;
	// out << "[" << ref.R << ", " << ref.G << ", " << ref.B << ", " << ref.A << "]^T";
	return out;
}

Image::Image(int _rows, int _cols, int _channels, const string& Name) :
n_rows(_rows), n_cols(_cols), n_channels(_channels), _name(Name)
{
	_data = new unsigned char[_rows * _cols * _channels]();
}

Image::Image(const string& filename, bool flip_y) : _name(filename)
{
	read(filename, flip_y);
}

Image::Image(const Image& image)
{
	n_rows = image.n_rows;
	n_cols = image.n_cols;
	n_channels = image.n_channels;
	trash = image.trash;
	_data = new unsigned char[n_rows * n_cols * n_channels];
	_name = image._name;

	memcpy((void*)_data, (void*)image._data, n_rows * n_cols * n_channels * sizeof(unsigned char));
}

Image::Image(Image&& image)
{
	n_rows = image.n_rows;
	n_cols = image.n_cols;
	n_channels = image.n_channels;
	trash = image.trash;
	_data = image._data;
	_name = move(image._name);

	image.n_rows = 0;
	image.n_cols = 0;
	image.n_channels = 0;
	image.trash = 255;
	image._data = nullptr;
}

Image::~Image()
{
#ifdef __IMAGE_PROCESS__
	img::wait(*this);
#endif
	stbi_image_free(_data);
}

Image::operator GLFWimage ()const
{
	GLFWimage icon;
	icon.width = n_cols;
	icon.height = n_rows;
	icon.pixels = new unsigned char[size()];
	memcpy((void*)icon.pixels, (void*)_data, size() * sizeof(unsigned char));

	return icon;
}

Image& Image::operator =(const Image& image)
{
	if(this == &image)
	{
		return *this;
	}
	bool need_to_new = false;
	if(size() != image.size())
	{
		need_to_new = true;
	}

	n_rows = image.n_rows;
	n_cols = image.n_cols;
	n_channels = image.n_channels;
	trash = image.trash;
	if(need_to_new)
	{
		if(_data != nullptr)
		{
			stbi_image_free(_data);
		}
		_data = new unsigned char[image.size()];
	}
	memcpy((void*)_data, (void*)image._data, n_rows * n_cols * n_channels * sizeof(unsigned char));
	_name = image._name;

	return *this;
}

Image& Image::operator =(Image&& image)
{
	if(this == &image)
	{
		return *this;
	}

	n_rows = move(image.n_rows);
	n_cols = move(image.n_cols);
	n_channels = move(image.n_channels);
	trash = move(image.trash);
	_data = move(image._data);
	_name = move(image._name);

	image.n_rows = 0;
	image.n_cols = 0;
	image.n_channels = 0;
	image.trash = 255;
	image._data = nullptr;

	return *this;
}

#ifdef USE_TIFF
void Image::read_tiff(const string& filename)
{
	_name = filename;

	TIFF *tif_file = TIFFOpen(filename.data(), "r");
	if(!tif_file)
	{
		cout << "Failed to read " << filename << "!" << endl;
		exit(-1);
	}

	short channel_short;
	TIFFGetField(tif_file, TIFFTAG_SAMPLESPERPIXEL, &channel_short);
	n_channels = (int)channel_short;
	TIFFGetField(tif_file, TIFFTAG_IMAGEWIDTH, &n_cols);
	TIFFGetField(tif_file, TIFFTAG_IMAGELENGTH, &n_rows);

	int _size = n_cols * n_rows;
	uint32* raster = (uint32*) _TIFFmalloc(_size * sizeof(uint32));
	if(!raster)
	{
		cout << "Failed to read " << filename << "!" << endl;
		exit(-1);
	}
	if(!TIFFReadRGBAImage(tif_file, n_cols, n_rows, raster, 0))
	{
		cout << "Failed to read " << filename << "!" << endl;
		exit(-1);
	}

	_data = new unsigned char[_size * n_channels];
	if(n_channels == 4)
	{
		for(int i = 0; i < n_rows; i++)
		{
			for(int j = 0; j < n_cols; j++)
			{
				int it_data = n_channels*(i*n_cols+j);
				uint32 raster_value = raster[(n_rows-1-i)*n_cols+j];
				_data[it_data] = (unsigned char)TIFFGetR(raster_value);
				_data[it_data+1] = (unsigned char)TIFFGetG(raster_value);
				_data[it_data+2] = (unsigned char)TIFFGetB(raster_value);
				_data[it_data+3] = (unsigned char)TIFFGetA(raster_value);
			}
		}
	}
	else if(n_channels == 3)
	{
		for(int i = 0; i < n_rows; i++)
		{
			for(int j = 0; j < n_cols; j++)
			{
				int it_data = n_channels*(i*n_cols+j);
				uint32 raster_value = raster[(n_rows-1-i)*n_cols+j];
				_data[it_data] = (unsigned char)TIFFGetR(raster_value);
				_data[it_data+1] = (unsigned char)TIFFGetG(raster_value);
				_data[it_data+2] = (unsigned char)TIFFGetB(raster_value);
			}
		}
	}
	else if(n_channels == 1)
	{
		for(int i = 0; i < n_rows; i++)
		{
			for(int j = 0; j < n_cols; j++)
			{
				int it_data = n_channels*(i*n_cols+j);
				uint32 raster_value = raster[(n_rows-1-i)*n_cols+j];
				_data[it_data] = (unsigned char)TIFFGetR(raster_value);
			}
		}
	}

	_TIFFfree(raster);
}
#endif

void Image::read(const string& filename, bool flip_y)
{
	if(!path::isfile(filename))
	{
		throw IOError("File \"" + filename + "\" is not exist.");
	}

	if(_data != nullptr)
	{
		stbi_image_free(_data);
	}

	string type = filename.substr(filename.size()-3, 3);
#ifdef USE_TIFF
	if(type == "tif" || type == "iff")
	{
		read_tiff(filename);
	}
	else
	{
#endif
		if(flip_y)
		{
			stbi_set_flip_vertically_on_load(true);
		}
		_data = stbi_load(filename.data(), &n_cols, &n_rows, &n_channels, 0);
#ifdef USE_TIFF
	}
#endif
}

void Image::load(const string& filename, bool flip_y)
{
	read(filename, flip_y);
}

void Image::write(const string& filename)const
{
	string type = filename.substr(filename.size()-3, 3);

	if(type == "jpg")
	{
		stbi_write_jpg(filename.c_str(), n_cols, n_rows, n_channels, _data, 0);
	}
	else if(type == "png")
	{
		stbi_write_png(filename.c_str(), n_cols, n_rows, n_channels, _data, 0);
	}
	else if(type == "bmp")
	{
		stbi_write_bmp(filename.c_str(), n_cols, n_rows, n_channels, _data);
	}
}

void Image::save(const string& filename)const
{
	write(filename);
}

unsigned char& Image::operator ()(int i, int j, int channel)
{
	switch(channel)
	{
		case 0: return _data[n_channels * (i * n_cols + j)];
		case 1:
		case 2:
		{
			switch(n_channels)
			{
				case 1: return _data[n_channels * (i * n_cols + j)];
				case 3:
				case 4: return _data[n_channels * (i * n_cols + j) + channel];
			}
		}
		case 3:
		{
			switch(n_channels)
			{
				case 1:
				case 3: return (trash = 255);
				case 4: return _data[n_channels * (i * n_cols + j) + 3];
			}
		}
		default: throw IndexError(channel, 0, 3);
	}
}

unsigned char  Image::operator ()(int i, int j, int channel)const
{
	switch(channel)
	{
		case 0: return _data[n_channels * (i * n_cols + j)];
		case 1:
		case 2:
		{
			switch(n_channels)
			{
				case 1: return _data[n_channels * (i * n_cols + j)];
				case 3:
				case 4: return _data[n_channels * (i * n_cols + j) + channel];
			}
		}
		case 3:
		{
			switch(n_channels)
			{
				case 1:
				case 3: return 255;
				case 4: return _data[n_channels * (i * n_cols + j) + 3];
			}
		}
		default:
			throw IndexError(0, 3, channel);
	}
}

Image::ColorRef Image::operator ()(int i, int j)
{
	Image::ColorRef cr;
	switch(n_channels)
	{
		case 1:
		{
			cr.R = _data[n_channels * (i * n_cols + j)];
			cr.G = _data[n_channels * (i * n_cols + j)];
			cr.B = _data[n_channels * (i * n_cols + j)];
			trash = 255;
			cr.A = trash;
			break;
		}

		case 3:
		{
			cr.R = _data[n_channels * (i * n_cols + j)];
			cr.G = _data[n_channels * (i * n_cols + j) + 1];
			cr.B = _data[n_channels * (i * n_cols + j) + 2];
			trash = 255;
			cr.A = trash;
			
			break;
		}

		case 4:
		{
			cr.R = _data[n_channels * (i * n_cols + j)];
			cr.G = _data[n_channels * (i * n_cols + j) + 1];
			cr.B = _data[n_channels * (i * n_cols + j) + 2];
			cr.A = _data[n_channels * (i * n_cols + j) + 3];
			break;
		}

		default: break;
	}
	
	return cr;
}

Color Image::operator ()(int i, int j)const
{
	Color c;
	switch(n_channels)
	{
		case 1:
		{
			c.R = _data[n_channels * (i * n_cols + j)]/255.0f;
			c.G = _data[n_channels * (i * n_cols + j)]/255.0f;
			c.B = _data[n_channels * (i * n_cols + j)]/255.0f;
			c.A = 1.0f;
			break;
		}

		case 3:
		{
			c.R = _data[n_channels * (i * n_cols + j)]/255.0f;
			c.G = _data[n_channels * (i * n_cols + j) + 1]/255.0f;
			c.B = _data[n_channels * (i * n_cols + j) + 2]/255.0f;
			c.A = 1.0f;
			break;
		}

		case 4:
		{
			c.R = _data[n_channels * (i * n_cols + j)]/255.0f;
			c.G = _data[n_channels * (i * n_cols + j) + 1]/255.0f;
			c.B = _data[n_channels * (i * n_cols + j) + 2]/255.0f;
			c.A = _data[n_channels * (i * n_cols + j) + 3]/255.0f;
			break;
		}

		default: break;
	}

	return c;
}

unsigned char* Image::data()const
{
	return _data;
}

int Image::width()const
{
	return n_cols;
}

int Image::height()const
{
	return n_rows;
}

int Image::rows()const
{
	return n_rows;
}

string Image::name()const
{
	return _name;
}

int Image::cols()const
{
	return n_cols;
}

int Image::channels()const
{
	return n_channels;
}

int Image::size()const
{
	return n_rows * n_cols * n_channels;
}

Image Image::subimg(unsigned int row_start, unsigned int _rows, unsigned int col_start, unsigned int _cols)const
{
	Image image(_rows, _cols, n_channels);
	for(int i = 0; i < _rows; i++)
	{
		for(int j = 0; j < _cols; j++)
		{
			for(int channel = 0; channel < n_channels; channel++)
			{
				image(i, j, channel) = (*this)(row_start + i, col_start + j, channel);
			}
		}
	}

	return image;
}