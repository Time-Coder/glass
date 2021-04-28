#include <image_process.h>
#include <image.h>
#include <matrix.hpp>
#include <color.h>

Image img::flip_y(const Image& image)
{
	int n_rows = image.rows();
	int n_cols = image.cols();
	int n_channels = image.channels();

	Image result(n_rows, n_cols, n_channels);
	for(int i = 0; i < n_rows; i++)
	{
		for(int j = 0; j < n_cols; j++)
		{
			result(i, j) = image(n_rows-1-i, j);
		}
	}

	return result;
}

Image img::flip_x(const Image& image)
{
	int n_rows = image.rows();
	int n_cols = image.cols();
	int n_channels = image.channels();

	Image result(n_rows, n_cols, n_channels);
	for(int i = 0; i < n_rows; i++)
	{
		for(int j = 0; j < n_cols; j++)
		{
			result(i, j) = image(i, n_cols-1-j);
		}
	}

	return result;
}

Image img::flip_xy(const Image& image)
{
	int n_rows = image.rows();
	int n_cols = image.cols();
	int n_channels = image.channels();

	Image result(n_rows, n_cols, n_channels);
	for(int i = 0; i < n_rows; i++)
	{
		for(int j = 0; j < n_cols; j++)
		{
			result(i, j) = image(n_rows-1-i, n_cols-1-j);
		}
	}

	return result;
}

void img::splitRGBA(const Image& image, Matrix<float>& R, Matrix<float>& G, Matrix<float>& B, Matrix<float>& A)
{
	int n_rows = image.rows();
	int n_cols = image.cols();

	for(int i = 0; i < n_rows; i++)
	{
		for(int j = 0; j < n_cols; j++)
		{
			Color c = image(i, j);
			R(i, j) = c.R;
			G(i, j) = c.G;
			B(i, j) = c.B;
			A(i, j) = c.A;
		}
	}
}

void img::splitRGBA(const Matrix<Color>& C, Matrix<float>& R, Matrix<float>& G, Matrix<float>& B, Matrix<float>& A)
{
    int n = C.size();
    for(int i = 0; i < n; i++)
    {
        R(i) = C(i).R;
        G(i) = C(i).G;
        B(i) = C(i).B;
        A(i) = C(i).A;
    }
}