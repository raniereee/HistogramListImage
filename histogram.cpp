#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
using namespace cv;

int read_imgList(const string& filename, vector<Mat>& images)
{
	string line;

	std::ifstream file(filename.c_str(), ifstream::in);

	if(!file)
	{
		printf("No valid input file was given, please check the given filename.\n");
		return -1;
	}

	while (getline(file, line))
		images.push_back(imread(line, CV_LOAD_IMAGE_COLOR));

	return 0;
}

int main( int argc, char** argv )
{
	Mat src, dst;

	if (argc != 2)
	{
        	cout << "usage: " << argv[0] << " <image2histogram.txt>" << endl;
        	exit(1);
    	}

	string imgList     = string(argv[1]);

	vector<Mat> images;

	if(read_imgList(imgList,     images) == -1)
	{
		printf("Fail in load images. Exit");
		exit(1);
	}

	printf("Load %d images\n", images.size());


	vector<Mat> bgr_planes;
	int i;
	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	for(i = 0; i < images.size(); i++)
	{
		split(images[i], bgr_planes);

		/// Compute the histograms:
		calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
		calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
		calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

		Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

		/// Normalize the result to [ 0, histImage.rows ]
		normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

		/// Draw for each channel
		for( int i = 1; i < histSize; i++ )
		{
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
				       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
				       Scalar( 255, 0, 0), 2, 8, 0  );
			
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
				       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
				       Scalar( 0, 255, 0), 2, 8, 0  );
		
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
				       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
				       Scalar( 0, 0, 255), 2, 8, 0  );
		}

		/// Display
		namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
		imshow("calcHist Demo", histImage );
		imshow("Image", images[i]);

		waitKey(0);
	}


	return 0;
}
