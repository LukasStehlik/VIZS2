/**
* @file Morphology_1.cpp
* @brief Erosion and Dilation sample code
* @author OpenCV team
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>;

using namespace cv;

/// Global variables
void Predspacovanie(Mat *src)
{
	Mat Keny, AFilter;
	GaussianBlur(*src, AFilter, Size(3, 3), 0, 0); //Rozmazáva hrany
	Canny(AFilter, Keny, 50, 200, 3);
	int w = Keny.cols;
	int h = Keny.rows;
	//printf("%d %d", w, h);
	imshow("Kanny", Keny);
}

void Hough_Transform()
{

}

int main(int, char*argv)
{
	Mat src=imread("../../VIZS2/Data/frame1.jpg");
	if (src.empty())
	{
		printf("can not open");
		return -1;
	}
	Predspacovanie(&src);
	imshow("Hough tansformation", src);

	waitKey(0);

	return 0;
}




