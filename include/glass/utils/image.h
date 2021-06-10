#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <iostream>
#include "glass/vec"

class Image
{
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
	vec4 operator ()(int i, int j)const;

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

	Image flip_y()const;
	Image flip_x()const;
	Image flip_xy()const;
};

#endif