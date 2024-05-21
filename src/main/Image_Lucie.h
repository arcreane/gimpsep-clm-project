#pragma once
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Image
{
	cv::Mat m_imageSource;

public:

	Image(const std::string& imagePath);
	Image(cv::Mat& imageMat);
	
	Image Resize(double scalingFactor);
	Image Crop(int startRow, int endRow, int startCol, int endCol);
	Image Rotate(double rotationAngle, std::vector<int> centerPoints);
	Image CannyEdge(float blurredValue, int lowThreshold, int highThreshold);

	void Display();

};