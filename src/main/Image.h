#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>


class Image
{
	cv::Mat m_imageSource;

public:
	// Constructors
	Image(const std::string& imagePath);
	Image(cv::Mat& imageMat);

	// Getter
    cv::Mat getImage();
	
	// Methods
	Image Brightness(int brightnessFactor);
	int cols();
	int rows();
	Image Rotate(double rotationAngle, std::vector<int> centerPoints);
	Image Resize(double scalingFactor);
	Image Crop(int startRow, int endRow, int startCol, int endCol);
	Image CannyEdge(float blurredValue, int lowThreshold, int highThreshold);
    Image Dilatation(int SEsize);
    Image Erosion(int SEsize);

	void Display();
};