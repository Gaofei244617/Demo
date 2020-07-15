#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) 
{
	Mat src = imread("G:\\01.jpg");
	if (src.empty()) 
	{
		std::cout << "could not load image..." << std::endl;
		return -1;
	}

	namedWindow("input image", WINDOW_AUTOSIZE);
	imshow("input image", src);

	waitKey(0);
	return 0;
}