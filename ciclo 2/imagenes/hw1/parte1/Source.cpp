#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>

using namespace std;

#include "traits.h"

void test_load_save_bitmap()
{
	Bitmap bitmap;
	
	std::cout << "****1bit.bmp" << std::endl;
	bitmap.Load("data\\1bit.bmp");
	bitmap.Save("results\\1bit_o.bmp");
	std::cout << "__________" << std::endl;

	std::cout << "****4bits.bmp" << std::endl;
	bitmap.Load("data\\4bits.bmp");
	bitmap.Save("results\\4bits_o.bmp");
	std::cout << "__________" << std::endl;

	std::cout << "****8bits.bmp" << std::endl;
	bitmap.Load("data\\8bits.bmp");
	bitmap.Save("results\\8bits_o.bmp");
	std::cout << "__________" << std::endl;

	std::cout << "****24bits.bmp" << std::endl;
	bitmap.Load("data\\24bits.bmp");
	bitmap.Save("results\\24bits_o.bmp");
	std::cout << "__________" << std::endl;
}

void test_equalization()
{
	Bitmap bitmap;
	UCImage *img, ok;
	p_vector<UCImage> rgb;
	p_vector<p_double> bins;

	bitmap.Load("data\\lena.bmp");
	bitmap.PrintHeaders();
	img = new UCImage(bitmap);

	rgb = core::Split(*img);

	for (auto i = 0; i < rgb.size(); i++)
	{
		core::Hist(&rgb[i], bins, true, true);
		core::Equ(&rgb[i], bins);
	}
	
	core::Merge(rgb, ok);

	bitmap.data = ok.data;
	bitmap.Save("results\\equalization.bmp");
}

int main()
{
	test_equalization();
	test_load_save_bitmap();

	getchar();
}