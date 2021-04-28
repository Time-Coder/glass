#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <iostream>
#include <GLFW/glfw3.h>

class Color;
class Image
{
public:
	class ColorRef
	{
	public:
		class Component
		{
			unsigned char* ptr = nullptr;
			bool first = true;
		public:
			Component(){}
			Component(unsigned char& value);
			unsigned char operator =(unsigned char& value);
			float operator =(const float& value);
			Component& operator =(const Component& component);
			operator float()const;
			friend std::ostream& operator <<(std::ostream& out, const ColorRef& c);
		};

	public:
		Component R;
		Component G;
		Component B;
		Component A;

		ColorRef(){}
		ColorRef(Color& color);
		ColorRef& operator =(const Color& color);
		ColorRef& operator =(const ColorRef& cr);
		operator Color()const;
		friend std::ostream& operator <<(std::ostream& out, const ColorRef& c);
	};

private:
	unsigned char* _data = nullptr;
	int n_rows = 0;
	int n_cols = 0;
	int n_channels = 0;

	unsigned char trash = 255;
	std::string _name;

	void read_tiff(const std::string& filename);

public:
	int window_index = -1;

public:
	Image() : _name("Memory Image") {}
	Image(int rows, int cols, int channels = 3, const std::string& Name = "Memory Image");
	Image(const std::string& filename, bool flip_y = false);
	Image(const Image& image);
	Image(Image&& image);
	virtual ~Image();

	Image& operator =(const Image& image);
	Image& operator =(Image&& image);

	unsigned char& operator ()(int i, int j, int channel);
	unsigned char  operator ()(int i, int j, int channel)const;
	ColorRef operator ()(int i, int j);
	Color operator ()(int i, int j)const;

	operator GLFWimage ()const;

	void read(const std::string& filename, bool flip_y = false);
	void load(const std::string& filename, bool flip_y = false);
	void write(const std::string& filename)const;
	void save(const std::string& filename)const;
	Image subimg(unsigned int row_start, unsigned int _rows, unsigned int col_start, unsigned int _cols)const;

	unsigned char* data()const;
	int width()const;
	int height()const;
	int rows()const;
	int cols()const;
	int channels()const;
	int size()const;
	std::string name()const;
};

#endif