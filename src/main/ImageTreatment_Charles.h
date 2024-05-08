//
// Created by charm on 07/05/2024.
//

#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class ImageTreatment_Charles {

private:
    std::string imageName;
    cv::Mat imageSource;
    cv::Mat image;

public:
    ImageTreatment_Charles(std::string imageName);

    bool loadImage();
    void resetImage();
    void saveImage(const std::string& outputName);

    std::string getImageName();
    cv::Mat getImageSource();
    cv::Mat getImage();

    void dilatation(int size);
    void erosion(int size);

    void panorama();
    void stitching();
};




