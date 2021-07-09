#include "tiffimage.h"
#include "path.h"

#include <iostream>

using namespace std;

TIFFImage::TIFFImage(const std::string& file_name)
{
	load(file_name);
}

void TIFFImage::load(const std::string& file_name)
{
	if(!path::isfile(file_name))
	{
		throw FileNotExistsError(file_name);
	}
	this->file_name = path::abspath(file_name);

	tif = TIFFOpen(file_name.c_str(), "r");
	if(tif == NULL)
	{
		throw RuntimeError("Failed to load tif file \"" + file_name + "\"");
	}

	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &_width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &_height);
	TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &_channels);
	TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &_depth);

	// directories
	do
	{
	    n_directories++;
	}
	while(TIFFReadDirectory(tif));
	TIFFClose(tif);
}

uint TIFFImage::directories()const
{
	return n_directories;
}

uint TIFFImage::depth()const
{
	return _depth;
}

uint TIFFImage::width()const
{
	return _width;
}

uint TIFFImage::height()const
{
	return _height;
}

uint TIFFImage::channels()const
{
	return _channels;
}