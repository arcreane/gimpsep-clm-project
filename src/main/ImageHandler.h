#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>

#include "Image.h"

class ImageHandler
{
private:
	std::vector<Image> imageList;
	Image imageSource;
	std::vector<Image>::iterator current;
    int maxIterator = 30;

public:
	// Constructors
	ImageHandler(Image image);

	// Methods
	void Brightness(int brightnessFactor);
	void Rotate(double rotationAngle, std::vector<int> centerPoints);
	void Resize(double scalingFactor);
	void Crop(int startRow, int endRow, int startCol, int endCol);
	void CannyEdge(float blurredValue, int lowThreshold, int highThreshold);
	void Dilatation(int SEsize);
	void Erosion(int SEsize);

	void Save(const Image& imageToSave);
	void Display(); // not used
	void ControlZ();
	void ControlY();

    Image getCurrentImage();
    Image getSourceImage();

	
};