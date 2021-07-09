#include "tiffimage.h"
#include <iostream>

using namespace std;

int main()
{
	TIFFImage image("python.tiff");
	cout << image.directories() << endl;
	cout << image.width() << endl;
	cout << image.height() << endl;
	cout << image.channels() << endl;
	cout << image.depth() << endl;

	return 0;
}