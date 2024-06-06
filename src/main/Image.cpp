#include "Image.h"
#include <filesystem> 

using namespace std;
using namespace cv;

// Define the constructors
Image::Image(const string& imagePath)
{
	m_imageSource = imread(imagePath, IMREAD_COLOR);
	if (m_imageSource.empty()) {
    	cout << "Could not open or find the image" << endl;
    }
}

Image::Image(Mat& imageMat)
{
	m_imageSource = imageMat;
}



// Define the getters
Mat Image::getImage(){return m_imageSource;}



// Define the methods
/**
 * @brief Lighten/Darken : plays on the image's brightness
 * @param brightnessFactor = int from -250 to 250 - default 0
 **/ 
Image Image::Brightness(int brightnessFactor)
{
    Mat adjustedImage;
    m_imageSource.convertTo(adjustedImage, m_imageSource.type(), 1, brightnessFactor);
    return Image(adjustedImage);
}

// Might have to be modified !!
int Image::cols()
{
	return m_imageSource.cols;
}

int Image::rows()
{
	return m_imageSource.rows;
}
/**
 * @brief Rotating : rotates the image around a center point (a,b)
 * @param rotationAngle = btw 0 and 360 : - default 0
 * @param centerPoints = vector of two int {a,b} - default {width/2, height/2}
 **/ 
Image Image::Rotate(double rotationAngle, std::vector<int> centerPoints)
{
    // Obtenir la couleur du premier pixel de l'image
    Scalar borderValue = m_imageSource.at<Vec3b>(0,0);

	Mat tmp;
	Point2f center(centerPoints.at(0), centerPoints.at(1));
	Mat RotationMatrix = getRotationMatrix2D(center, rotationAngle, 1);
	warpAffine(m_imageSource, tmp, RotationMatrix, m_imageSource.size(), INTER_LINEAR, BORDER_CONSTANT, borderValue);
	return Image(tmp);
}


/**
 * @brief Resizing  : plays on the size of the image
 * @param scalingFactor = float from 0.1(inexistant) to maxWindow (~2) step by 0.1 - default 1
 **/ 
Image Image::Resize(double scalingFactor)
{
    Mat tmp;
    if(scalingFactor!=0)
        resize(m_imageSource, tmp, Size(round(scalingFactor*m_imageSource.cols),round(scalingFactor*m_imageSource.rows)), scalingFactor, scalingFactor, INTER_LINEAR);
    return Image(tmp);
}


/**
 * @brief Cropping : cuts the image in a smaller rectangle form
 * @param startRow = int vertical min value - default 0
 * @param endRow = int vertical max value - default height
 * @param startCol = int horizontal min value - default 0
 * @param endCol = int horizontal max value - default width
 **/ 
Image Image::Crop(int startRow, int endRow, int startCol, int endCol)
{
    if (startRow > endRow)
    {
        int tmpEndRow = endRow;
        endRow =  startRow;
        startRow = tmpEndRow;
    }
    if (startCol > endRow)
    {
        int tmpEndCol = endCol;
        endCol =  startCol;
        startCol = tmpEndCol;
    }
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
 * @brief Drawing Edges : draws the apparent edges of the image
 * @param blurredValue = float btw 0 and 1
 * @param lowThreshold = int btw 0 and 255
 * @param highThreshold = int btw 0 and 255
 **/ 
Image Image::CannyEdge(float blurredValue, int lowThreshold, int highThreshold)
{
    if (highThreshold < lowThreshold)
    {
        highThreshold = lowThreshold;
    }
    Mat imageEdges, blurredImage;
    cv::cvtColor(m_imageSource, blurredImage, cv::COLOR_BGR2GRAY);
 	GaussianBlur(blurredImage, blurredImage,Size(5, 5), blurredValue);
    Canny(blurredImage, imageEdges,lowThreshold,highThreshold);
    cv::cvtColor(imageEdges, imageEdges, cv::COLOR_GRAY2BGR);
    return Image(imageEdges);
}

/**
 * @brief Dilate : increase the volume of the white structures
 * @param SEsize = int btw 1 and 99 (usually closer to 13) - default 1
 **/ 
Image Image::Dilatation(int SEsize)
{
    Mat tmp;
    Mat structElement = getStructuringElement(MORPH_RECT, Size(SEsize, SEsize), Point(-1, -1));
    dilate(m_imageSource, tmp, structElement, Point(-1, -1), 1);
    return Image(tmp);
}

/**
 * @brief Erode : decrease the volume of the white structures
 * @param SEsize = int btw 1 and 99 (usually closer to 13) - default 1
 **/ 
Image Image::Erosion(int SEsize)
{
    Mat tmp;
    Mat structElement = getStructuringElement(MORPH_RECT, Size(SEsize, SEsize), Point(-1, -1));
    erode(m_imageSource, tmp, structElement, Point(-1, -1), 1);
    return Image(tmp);
}


void Image::Display()
{
	imshow("Image", m_imageSource);
  	waitKey(0);
}