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
    m_imageHistory.clear(); // Initialize history
    m_historyIndex = 0;
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
    SaveHistory(adjustedImage);
    cout << "Size: " << m_imageHistory.size() << endl;
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
    SaveHistory(tmp);
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
    SaveHistory(tmp);
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
    SaveHistory(tmp);
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
    SaveHistory(imageEdges);
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
    SaveHistory(tmp);
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
    SaveHistory(tmp);
    return Image(tmp);
}


void Image::Display(string name)
{
	imshow(name, m_imageSource);
  	waitKey(0);
  	destroyWindow(name);
}

/** Save history : keeps the 10 previous images as clones 
 * @param modifiedImage = new modiifed image to save

void Image::SaveHistory(const cv::Mat& modifiedImage)
{
    m_imageHistory.insert(m_imageHistory.begin(), modifiedImage.clone());
    if (m_imageHistory.size() > 10)
    {
        m_imageHistory.pop_back();
    }
}
 **/

void Image::SaveHistory(const cv::Mat& modifiedImage)
{
    // Store the modified image in history
    m_imageHistory.push_back(modifiedImage.clone());

    // Limit the history size to 10
    if (m_imageHistory.size() > 10)
    {
        m_imageHistory.erase(m_imageHistory.begin());
    }

    // Update the history index to point to the latest image
    m_historyIndex = m_imageHistory.size() - 1;
    
    // Debug output to verify
    cout << "Size: " << m_imageHistory.size() << endl;
    cout << "Index number: " << m_historyIndex << endl;
}


void Image::DisplayHistory()
{
    for (size_t i = 0; i < m_imageHistory.size(); ++i)
    {
        string windowName = "History Image " + std::to_string(i + 1);
        imshow(windowName, m_imageHistory[i]);
        waitKey(0);  // Wait for a key press
        destroyWindow(windowName);  // Close the window
    }
}


Image Image::ControlZ() {
    if (m_historyIndex > 0) 
    {
        // Move one step back in history
        m_historyIndex--;
        m_imageSource = m_imageHistory[m_historyIndex];
        cout << "Undo successful." << endl;
    } 
    else 
    {
        cout << "No more history to revert to." << endl;
    }
    return Image(m_imageSource);
}

Image Image::ControlY() {
    if (m_historyIndex < m_imageHistory.size() - 1) 
    {
        // Move one step forward in history
        m_historyIndex++;
        m_imageSource = m_imageHistory[m_historyIndex];
        cout << "Redo successful." << endl;
    }
    else
    {
        cout << "Already at the latest image." << endl;
    }
    return Image(m_imageSource);
}