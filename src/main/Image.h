#pragma once
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Image
{
	//std::string m_imagePath;
	std::string m_imageName;
	cv::Mat m_imageSource;

public:

	Image(const std::string& imageName, const std::string& imagePath);
	Image(const std::string& imageName, cv::Mat& imageMat);
	
	Image Resize(double scalingFactor);
	Image Crop(int startRow, int endRow, int startCol, int endCol);
	Image Rotate(double rotationAngle);

	void Display();

};