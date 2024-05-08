//
// Created by charm on 07/05/2024.
//

#include "ImageTreatment_Charles.h"

#include <utility>

ImageTreatment_Charles::ImageTreatment_Charles(std::string imageName) {
    this->imageName = std::move(imageName);
}

std::string ImageTreatment_Charles::getImageName(){return this->imageName;}
cv::Mat ImageTreatment_Charles::getImageSource(){return this->imageSource;}
cv::Mat ImageTreatment_Charles::getImage(){return this->image;}

bool ImageTreatment_Charles::loadImage() {
    this->imageSource = imread(this->imageName, cv::IMREAD_COLOR);
    if (this->imageSource.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return false;
    }
    resetImage();
    return true;
}

void ImageTreatment_Charles::resetImage() {
    this->image = this->imageSource.clone();
}

void ImageTreatment_Charles::saveImage(const std::string& outputName) {
    imwrite (outputName, this->image);
}


void ImageTreatment_Charles::dilatation(int size) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size));
    cv::dilate(this->getImage(), this->getImage(),element);
}


void ImageTreatment_Charles::erosion(int size) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size));
    cv::erode(this->getImage(), this->getImage(),element);
}

void ImageTreatment_Charles::panorama() {
    return;
}

void ImageTreatment_Charles::stitching() {
    return;
}
