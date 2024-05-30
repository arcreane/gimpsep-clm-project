#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <filesystem>
#include <set>
#include <algorithm>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>


class Image
{
	cv::Mat m_imageSource;

public:

	Image(const std::string& imagePath);
	Image(cv::Mat& imageMat);

    cv::Mat getImageSource();
    cv::Mat getImage();
	
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