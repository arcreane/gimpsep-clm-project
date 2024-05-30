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
 * Lighten/Darken : plays on the image's brightness
 * @param brightnessFactor = int from -250 to 250 
 **/ 
Image Image::Brightness(int brightnessFactor)
{
    Mat adjustedImage;
    m_imageSource.convertTo(adjustedImage, m_imageSource.type(), 1, brightnessFactor);
    return Image(adjustedImage);
}


/**
 * Resizing  : plays on the size of the image
 * @param scalingFactor = float from 0(inexistant) to 5 (5 times normal size) with 1 being the original image size
 **/ 
Image Image::Resize(double scalingFactor)
{
	Mat tmp;
	if(scalingFactor!=0)
		resize(m_imageSource, tmp, Size(round(scalingFactor*m_imageSource.cols),round(scalingFactor*m_imageSource.rows)), scalingFactor, scalingFactor, INTER_LINEAR);
	return Image(tmp);
}


/**
 * Cropping : cuts the image in a smaller rectangle form
 * @param startRow = int vertical min value 
 * @param endRow = int vertical max value
 * @param startCol = int horizontal min value
 * @param endCol = int horizontal max value
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


int Image::cols()
{
	return m_imageSource.cols;
}

int Image::rows()
{
	return m_imageSource.rows;
}
/**
 * Rotating : rotates the image around a center point (a,b)
 * @param rotationAngle = btw 0 and 360
 * @param centerPoints = vector of two int {a,b}
 **/ 
Image Image::Rotate(double rotationAngle, std::vector<int> centerPoints)
{
	Mat tmp;
	Point2f center(centerPoints.at(0), centerPoints.at(1));
	Mat RotationMatrix = getRotationMatrix2D(center, rotationAngle, 1);
	warpAffine(m_imageSource, tmp, RotationMatrix, m_imageSource.size());
	return Image(tmp);
}


/**
 * Drawing Edges : draws the apparent edges of the image
 * @param blurredValue = float btw 0 and 1
 * @param lowThreshold = int btw 0 and 255
 * @param highThreshold = int btw 0 and 255
 **/ 
Image Image::CannyEdge(float blurredValue, int lowThreshold, int highThreshold)
{
    Mat imageEdges, blurredImage;
 	GaussianBlur(m_imageSource, blurredImage,Size(5, 5), blurredValue); 
    Canny(blurredImage, imageEdges,lowThreshold,highThreshold);
    return Image(imageEdges);
}

/**
 * Dilate : increase the volume of the white structures
 * @param SEsize = int btw 1 and 99 (usually closer to 13)
 **/ 
Image Image::Dilatation(int SEsize)
{
    Mat tmp;
    Mat structElement = getStructuringElement(MORPH_RECT, Size(SEsize, SEsize), Point(-1, -1));
    dilate(m_imageSource, tmp, structElement, Point(-1, -1), 1);
    return Image(tmp);
}

/**
 * Erode : decrease the volume of the white structures
 * @param SEsize = int btw 1 and 99 (usually closer to 13)
 **/ 
Image Image::Erosion(int SEsize)
{
    Mat tmp;
    Mat structElement = getStructuringElement(MORPH_RECT, Size(SEsize, SEsize), Point(-1, -1));
    erode(m_imageSource, tmp, structElement, Point(-1, -1), 1);
    return Image(tmp);
}


void Image::Display(string name)
{
	imshow(name, m_imageSource);
  	waitKey(0);
  	destroyWindow(name);
}