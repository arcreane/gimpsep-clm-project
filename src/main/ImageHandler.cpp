#include "Image.h"
#include "ImageHandler.h"
#include <filesystem> 

using namespace std;
using namespace cv;

// Define the constructors
ImageHandler::ImageHandler(Image image) : imageSource(image) {
		imageList.push_back(imageSource);
		current = imageList.begin();
}


// Define the methods
void ImageHandler::Brightness(int brightnessFactor) {
	Save(current->Brightness(brightnessFactor));
}
void ImageHandler::Rotate(double rotationAngle, std::vector<int> centerPoints) {
	Save(current->Rotate(rotationAngle, centerPoints));
}
void ImageHandler::Resize(double scalingFactor) {
	Save(current->Resize(scalingFactor));
}
void ImageHandler::Crop(int startRow, int endRow, int startCol, int endCol) {
	Save(current->Crop(startRow, endRow, startCol, endCol));
}
void ImageHandler::CannyEdge(float blurredValue, int lowThreshold, int highThreshold) {
	Save(current->CannyEdge(blurredValue, lowThreshold, highThreshold));
}
void ImageHandler::Dilatation(int SEsize) {
	Save(current->Dilatation(SEsize));
}
void ImageHandler::Erosion(int SEsize) {
	Save(current->Erosion(SEsize));
}



void ImageHandler::Display() {
	current->Display();
}


/** 
 * @brief Save : keeps the 10 previous images as clones 
 * @param imageToSave = new modified image to save
 **/
void ImageHandler::Save(const Image& imageToSave)
{
	if (current != --imageList.end())
	{
    	imageList.erase(++current, imageList.end());
	}

	imageList.push_back(imageToSave);

	// Limit the history size to 10
	if (imageList.size() > maxIterator)
	{
    	imageList.erase(imageList.begin());
	}
	current = imageList.end() -1;
}

/** 
 * @brief ControlZ : allows to undo a change
 **/
void ImageHandler::ControlZ()
{
	if (current != imageList.begin())
	{
		current--;
        std::cout << "ctrl+z apply" << std::endl;
	}
    else
    {
        std::cout << "No more data saved" << std::endl;
    }
}


/** 
 * @brief ControlY : allows to redo a change
 **/
void ImageHandler::ControlY()
{
	if (current != imageList.end() - 1)
	{
		current++;
        std::cout << "ctrl+y apply" << std::endl;
    }
    else
    {
        std::cout << "No more data saved" << std::endl;
    }
}


Image ImageHandler::getCurrentImage() {
    return static_cast<Image &>(*current);
}


Image ImageHandler::getSourceImage() {
    return imageSource;
}



