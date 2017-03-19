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
#include <iostream>

using namespace cv;
using namespace std;

/// Global variables
void Predspacovanie(Mat src, Mat *dst);
void Hough_Transform(Mat source, vector<Vec2f> *lines, double rhoRes, double thetaRes, uchar treshold);

int main(int, char*argv)
{
	Mat src = imread("../../OpenCVTest/DataLow/frame120.png");
	if (src.empty())
	{
		printf("can not open");
		return -1;
	}
	Mat preproc;
	Predspacovanie(src, &preproc);
	imshow("Predspracovanie", preproc);

	vector<Vec2f> lines;
	Hough_Transform(preproc, &lines, 1, 1, 80);

	printf("Pocet ciar=%lu", (unsigned long)lines.size());
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(src, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}

	imshow("Lines", src);

	waitKey(0);

	return 0;
}

void Predspacovanie(Mat src, Mat *dst)
{
	Mat AFilter;
	GaussianBlur(src, AFilter, Size(3, 3), 0, 0); //Rozmazáva hrany
	Canny(AFilter, *dst, 50, 200, 3);
}

void Hough_Transform(Mat source, vector<Vec2f> *lines, double rhoRes, double thetaRes, uchar treshold) // uhly v stupòoch
{
	uint w = source.cols;
	uint h = source.rows;
	uint uhlopriecka = (uint)sqrt(pow(w, 2) + pow(h, 2));
	printf("uhlopriecka=%d\n", uhlopriecka);
	Mat akumulator = Mat(Size((int)(180 / thetaRes), (int)(uhlopriecka / rhoRes)), CV_8U, Scalar(0));
	for (uint x = 0; x < w; x++)
	{
		for (uint y = 0; y < h; y++)
		{
			uchar pixel = source.at<uchar>(y, x);
			if (pixel == 255)
			{
				for (uint theta = 0; theta < (uint)akumulator.cols; theta++)
				{
					double rho = x*cos(theta*thetaRes*CV_PI / 180.0) + y*sin(theta*thetaRes*CV_PI / 180.0);
					rho /= rhoRes;
					if ((uint)abs(rho) < (uint)akumulator.rows)
					{
						uchar *accPix = &(akumulator.at<uchar>((uint)abs(rho), theta));
						if (*accPix < 255) (*accPix)++;
					}
				}
			}
		}
	}

	for (uint theta = 0; theta < (uint)akumulator.cols; theta++)
	{
		for (uint rho = 0; rho < (uint)akumulator.rows; rho++)
		{
			if (theta > 90) rhoRes *= -1;
			uchar accPix = akumulator.at<uchar>(rho, theta);
			if (accPix > treshold) (*lines).push_back(Vec2f((float)(rho*rhoRes), (float)((theta*thetaRes)*CV_PI / 180)));
		}
	}
	imshow("Akumulátor", akumulator);
}