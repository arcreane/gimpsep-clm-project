#pragma once

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
	std::vector<Image> m_listImages;

public:
	PanoramaCreator(const std::string& folderPath);
    static Image CreatePanorama(const std::vector<Image>& listImages);
};