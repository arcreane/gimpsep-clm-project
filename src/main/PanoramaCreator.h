#pragma once

#include "Image.h"

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


class PanoramaCreator
{	

public:
	// Parameters
	std::vector<std::string> m_listImages;

	// Constructeur
	PanoramaCreator();
	explicit PanoramaCreator(const std::string& folderPath);

	// Getter
	std::vector<std::string> getListImages();

	// Methods
    static Image CreatePanorama(const std::vector<std::string>& listImages);
};