#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

const String PATH_LEFT = "C:\\Projects\\Stereo\\BlockMatcher\\img\\Aloe\\view1.png";
const String PATH_RIGHT = "C:\\Projects\\Stereo\\BlockMatcher\\img\\Aloe\\view5.png";

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
	

	int block_size = 11;
	uint max_offset = 80;
	Mat result = Mat(img_left.rows, img_left.cols, CV_8S);
	Mat best_fit = Mat(img_left.rows, img_left.cols, CV_8U);
	best_fit.setTo(255);
	Mat intermediate;
	cvtColor(img_left, img_left, CV_BGR2GRAY);
	cvtColor(img_right, img_right, CV_BGR2GRAY);

	std::cout << img_left.type();
	 
	for (int ioff = 0; ioff <= max_offset*2; ++ioff)
	{
		int i = ioff - max_offset;
		int left_start =	i < 0 ? -i : 0;
		int left_end =		i < 0 ? img_left.cols : img_left.cols - i;
		int right_start =	i < 0 ? 0 : i;
		int right_end =		i < 0 ? img_right.cols + i : img_right.cols;
		absdiff(img_right(Range(0, img_left.rows), Range(right_start, right_end)), img_left(Range(0, img_left.rows), Range(left_start, left_end)), intermediate);

		imshow(window_left, intermediate);
		blur(intermediate, intermediate, Size(block_size, block_size));

		for (size_t y = 0; y < img_left.rows; y++)
		{
			for (size_t x = 0; x < right_end && x < left_end; x++)
			{
				if (intermediate.at<uchar>(y, x) < best_fit.at<uchar>(y, left_start + x)) {
					best_fit.at<uchar>(y, left_start + x) = intermediate.at<uchar>(y, x);
					result.at<char>(y, left_start + x) = i;
				}
			}
		}

		imshow(window_right, img_right(Range(0, img_left.rows), Range(right_start, right_end)));
		imshow(window_result, result);
 		waitKey(5);
	}
	
	
	imshow(window_left, img_left);
	imshow(window_right, img_right);
	imshow(window_result, result);

	waitKey(0);

	return 0;
}