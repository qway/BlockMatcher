#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

const String PATH_LEFT = "Q:\\Libraries\\Stereo\\BlockMatcher\\img\\Aloe\\view1.png";
const String PATH_RIGHT = "Q:\\Libraries\\Stereo\\BlockMatcher\\img\\Aloe\\view5.png";

int main(int argc, char** argv)
{

	Mat img_left, img_right;
	if (argc != 2)
	{
		printf("No Image provided, using debugging images...\n");
		img_left = imread(PATH_LEFT, 1);
		img_right = imread(PATH_RIGHT, 1);
	}
	else {
		img_left = imread(argv[1], 1);
		img_right = imread(argv[2], 1);
	}


	if (!img_left.data || !img_right.data)
	{
		printf("No image data \n");
		return -1;
	}
    String window_left = "Left";
	String window_right = "Right";
	String window_result = "Result";
	namedWindow(window_left, WINDOW_AUTOSIZE);
	namedWindow(window_right, WINDOW_AUTOSIZE);
	namedWindow(window_result, WINDOW_AUTOSIZE);


	uint block_size = 25;
	uint max_offset = 100;
	Mat result = Mat(img_left.rows, img_left.cols, CV_8U);
	Mat intermediate;
	cvtColor(img_left, img_left, CV_BGR2GRAY);
	cvtColor(img_right, img_right, CV_BGR2GRAY);
	for (int i = 0; i <= max_offset; i++)
	{
		int offset = max_offset - i;
		subtract(img_left(Range(0, img_left.rows), Range(offset, img_left.cols)),
					img_right(Range(0, img_left.rows), Range(0, img_right.cols - offset)),
					intermediate, noArray(), CV_8S);
		imshow(window_result, intermediate);
		waitKey(0);
		std::cout << intermediate.at<uchar>(0,0) << " ";
		abs(intermediate);
		std::cout << intermediate.at<uchar>(0, 0) << "\n";
		imshow(window_result, intermediate);
		waitKey(0);
		//blur(intermediate, intermediate, Size(block_size, block_size), Point(-1,-1), BORDER_DEFAULT);
		imshow(window_result, intermediate);
		waitKey(0);
	}
	
	
	imshow(window_left, img_left);
	imshow(window_right, img_right);
	imshow(window_result, result);

	waitKey(0);

	return 0;
}