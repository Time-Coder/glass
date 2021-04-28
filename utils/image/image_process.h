#ifndef __IMAGE_PROCESS__
#define __IMAGE_PROCESS__

class Image;
class Color;
template<class DataType>
class Matrix;

namespace img
{
	Image flip_y(const Image& image);
	Image flip_x(const Image& image);
	Image flip_xy(const Image& image);
	void splitRGBA(const Image& image, Matrix<float>& R, Matrix<float>& G, Matrix<float>& B, Matrix<float>& A);
	void splitRGBA(const Matrix<Color>& C, Matrix<float>& R, Matrix<float>& G, Matrix<float>& B, Matrix<float>& A);
}

#endif