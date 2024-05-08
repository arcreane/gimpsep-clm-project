#include "Image.h"
#include <filesystem> 

namespace fs = std::filesystem;
using namespace std;
using namespace cv;

// Define the constructors
Image::Image(const string& imageName, const string& imagePath)
{
	//m_imagePath = imagePath;
	m_imageName = imageName;
	m_imageSource = imread(imagePath, IMREAD_COLOR);
	if (m_imageSource.empty()) {
    	cout << "Could not open or find the image" << endl;
    	// return -1;
    }
}

Image::Image(const string& imageName, Mat& imageMat)
{
	m_imageName = imageName;
	m_imageSource = imageMat;
}


Image Image::Resize(double scalingFactor)
{
	Mat tmp;
	if(scalingFactor!=0)
		resize(m_imageSource, tmp, Size(round(scalingFactor*m_imageSource.cols),round(scalingFactor*m_imageSource.rows)), scalingFactor, scalingFactor, INTER_LINEAR);
	return Image(m_imageName, tmp);
}


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
	return Image(m_imageName, tmp);
}

Image Image::Rotate(double rotationAngle)
{
	Mat tmp;
	Point2f center(m_imageSource.cols/2, m_imageSource.rows/2);
	Mat RotationMatrix = getRotationMatrix2D(center, rotationAngle, 1);
	warpAffine(m_imageSource, tmp, RotationMatrix, m_imageSource.size());
	return Image(m_imageName, tmp);
}


void Image::Display()
{
	imshow("Original Image", m_imageSource);
  	waitKey(0);
}