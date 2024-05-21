#include "Image_functions.h"
#include <filesystem> 

using namespace std;
using namespace cv;

// Define the constructors
Image::Image(const string& imagePath)
{
	m_imageSource = imread(imagePath, IMREAD_COLOR);
	if (m_imageSource.empty()) {
    	cout << "Could not open or find the image" << endl;
    	// return -1;
    }
}

Image::Image(Mat& imageMat)
{
	m_imageSource = imageMat;
}


/**
 * Lighten/Darken
 * plays on the image's brightness
 * brightnessFactor = Int from -250 to 250 
 **/ 
Image Image::Brightness(int brightnessFactor)
{
    Mat adjustedImage;
    m_imageSource.convertTo(adjustedImage, m_imageSource.type(), 1, brightnessFactor);
    return Image(adjustedImage);
}

/**
 * Resizing
 * plays on the size of the image
 * scalingFactor = float from 0(inexistant) to 5(5 times normal size) with 1 being the usual image size
 **/ 
Image Image::Resize(double scalingFactor)
{
	Mat tmp;
	if(scalingFactor!=0)
		resize(m_imageSource, tmp, Size(round(scalingFactor*m_imageSource.cols),round(scalingFactor*m_imageSource.rows)), scalingFactor, scalingFactor, INTER_LINEAR);
	return Image(tmp);
}

/**
 * Croping
 * cuts the image in a smaller rectangle form
 * startRow = int vertical min value 
 * endRow = int vertical max value
 * startCol = int horizontal min value
 * endCol = int horizontal max value
 **/ 
Image Image::Crop(int startRow, int endRow, int startCol, int endCol)
{
	if (endRow > m_imageSource.rows || endRow < startRow)
		endRow = m_imageSource.rows;
	if (endCol > m_imageSource.cols || endCol < startCol)
		endCol = m_imageSource.cols;
	if (startRow < 0)
		startRow = 0;
	if (startCol < 0)
		startCol = 0;
	Mat tmp = m_imageSource(Range(startRow, endRow), Range(startCol, endCol));
	return Image(tmp);
}

/**
 * Rotating
 * Rotates the image and replaces 
 * startRow = int vertical min value 
 * endRow = int vertical max value
 * startCol = int horizontal min value
 * endCol = int horizontal max value
 **/ 
Image Image::Rotate(double rotationAngle)
{
	Mat tmp;
	Point2f center(m_imageSource.cols/2, m_imageSource.rows/2);
	Mat RotationMatrix = getRotationMatrix2D(center, rotationAngle, 1);
	warpAffine(m_imageSource, tmp, RotationMatrix, m_imageSource.size());
	return Image(tmp);
}

Image Image::CannyEdge(float blurredValue, int lowThreshold, int highThreshold)
{
    Mat imageEdges, blurredImage;
 	GaussianBlur(m_imageSource, blurredImage,Size(5, 5), blurredValue); 
    Canny(blurredImage, imageEdges,lowThreshold,highThreshold);
    return Image(imageEdges);
}

void Image::Display()
{
	imshow("Original Image", m_imageSource);
  	waitKey(0);
}