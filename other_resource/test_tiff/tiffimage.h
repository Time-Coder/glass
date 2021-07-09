#ifndef __TIFF_IMAGE__
#define __TIFF_IMAGE__

#include <tiffio.h>
#include <string>
#include <exception>

typedef unsigned int uint;

class TIFFImage
{
	TIFF* tif = NULL;
	std::string file_name;
	uint n_directories = 0;
	uint _width = 0;
	uint _height = 0;
	uint _channels = 0;
	uint _depth = 0;

	class FileNotExistsError: public std::exception
	{
		std::string message;

	public:
		FileNotExistsError(const std::string& file_name)
		{
			message = "File \"" + file_name + "\" is not exists.";
		}

		virtual const char* what()const noexcept
		{
			return message.c_str();
		}
	};

	class RuntimeError: public std::exception
	{
		std::string message;

	public:
		RuntimeError(const std::string& _message): message(_message) {}
		virtual const char* what()const noexcept
		{
			return message.c_str();
		}
	};

public:
	TIFFImage() {};
	TIFFImage(const std::string& file_name);
	void load(const std::string& file_name);
	uint directories()const;
	uint depth()const;
	uint width()const;
	uint height()const;
	uint channels()const;
};

#endif