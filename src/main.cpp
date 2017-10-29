#include <stdio.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;

//Copied and altered from Libelas to convert output into PFM format

int littleendian()
{
	int intval = 1;
	uchar *uval = (uchar *)&intval;
	return uval[0] == 1;
}

void WriteFilePFM(float *data, int width, int height, const char* filename, float scalefactor = 1 / 255.0)
{
	// Open the file
	FILE *stream = fopen(filename, "wb");
	if (stream == 0) {
		fprintf(stderr, "WriteFilePFM: could not open %s\n", filename);
		exit(1);
	}

	// sign of scalefact indicates endianness, see pfms specs
	if (littleendian())
		scalefactor = -scalefactor;

	// write the header: 3 lines: Pf, dimensions, scale factor (negative val == little endian)
	fprintf(stream, "Pf\n%d %d\n%f\n", width, height, scalefactor);

	int n = width;
	// write rows -- pfm stores rows in inverse order!
	for (int y = height - 1; y >= 0; y--) {
		float* ptr = data + y * width;
		// change invalid pixels (which seem to be represented as -10) to INF
		for (int x = 0; x < width; x++) {
			if (ptr[x] < 0)
				ptr[x] = INFINITY;
		}
		if ((int)fwrite(ptr, sizeof(float), n, stream) != n) {
			fprintf(stderr, "WriteFilePFM: problem writing data\n");
			exit(1);
		}
	}

	// close file
	fclose(stream);
}


int main(int argc, char** argv)
{
	//TODO Outdir
	Mat img_left, img_right;
	String PATH_LEFT = "Q:\\Libraries\\Stereo\\BlockMatcher\\img\\Aloe\\view1.png";
	String PATH_RIGHT = "Q:\\Libraries\\Stereo\\BlockMatcher\\img\\Aloe\\view5.png";
	String outdir = "out.pfm";
	int block_size = 13;
	uint max_offset = 90;

	if (argc >= 2)
	{
		PATH_LEFT = argv[1];
		PATH_RIGHT = argv[2];
	}
	else {
		printf("No Image provided, using debugging images...\n");
	}
	if (argc >= 3) {
		outdir = argv[3];
	}
	if (argc == 5) {
		block_size = std::stoi(argv[4]);
		max_offset = std::stoi(argv[5]);
	}
	img_left = imread(PATH_LEFT, 1);
	img_right = imread(PATH_RIGHT, 1);


	if (!img_left.data || !img_right.data)
	{
		printf("No image data \n");
		return -1;
	}
	

	
	Mat result = Mat(img_left.rows, img_left.cols, CV_32F);
	Mat best_fit = Mat(img_left.rows, img_left.cols, CV_32F);
	best_fit.setTo(INFINITY);
	Mat intermediate;

	 
	for (int ioff = 0; ioff <= max_offset*2; ++ioff)
	{
		int i = ioff - max_offset;
		int left_start =	i < 0 ? -i : 0;
		int left_end =		i < 0 ? img_left.cols : img_left.cols - i;
		int right_start =	i < 0 ? 0 : i;
		int right_end =		i < 0 ? img_right.cols + i : img_right.cols;
		absdiff(img_right(Range(0, img_left.rows), Range(right_start, right_end)), img_left(Range(0, img_left.rows), Range(left_start, left_end)), intermediate);
		blur(intermediate, intermediate, Size(block_size, block_size));

		for (size_t y = 0; y < img_left.rows; y++)
		{
			for (size_t x = 0; x < right_end && x < left_end; x++)
			{
				int val = norm(intermediate.at<Vec3b>(y, x));
				if (val < best_fit.at<float>(y, left_start + x)) {
					best_fit.at<float>(y, left_start + x) = val;
					result.at<float>(y, left_start + x) = abs(i);
				}
			}
		}
	}
	WriteFilePFM((float*)(result.data), result.cols, result.rows, outdir.c_str(), 1.0 / max_offset);
	return 0;
}