//
// Created by charm on 07/05/2024.
//

#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <filesystem>
#include <set>
#include <algorithm>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/stitching.hpp"
#include <opencv2/highgui.hpp>


class ImageTreatment_Charles {

private:
    std::string imageName;
    cv::Mat imageSource;
    cv::Mat image;

public:
    //ImageTreatment_Charles() = default; // exist only for stitching_and_panorama()
    explicit ImageTreatment_Charles(std::string imageName);
    explicit ImageTreatment_Charles(cv::Mat &image);

    bool loadImage();
    void resetImage();
    void saveImage(const std::string& outputName);
    bool isEmpty();

    std::string getImageName();
    cv::Mat getImageSource();
    cv::Mat getImage();

    void dilatation(int size);
    void erosion(int size);

    static ImageTreatment_Charles stitching_and_panorama(std::vector<std::string> &imagesNameList);
    static ImageTreatment_Charles stitching_and_panorama(std::string folderPath);

};




