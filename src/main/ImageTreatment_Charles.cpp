//
// Created by charm on 07/05/2024.
//

#include "ImageTreatment_Charles.h"

ImageTreatment_Charles::ImageTreatment_Charles(std::string imageName) {
    this->imageName = std::move(imageName);
}

ImageTreatment_Charles::ImageTreatment_Charles(cv::Mat &image) {
    this->image = image.clone();
    this->imageSource = image.clone();
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

bool ImageTreatment_Charles::isEmpty() {
    if (this->imageSource.empty()) {
        std::cout << "Error : cv::Mat imageSource is empty" << std::endl;
        return true;
    }
    return false;
}

void ImageTreatment_Charles::dilatation(int size) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
    cv::dilate(this->getImage(), this->getImage(),element, cv::Point(-1, -1), 1);
}


void ImageTreatment_Charles::erosion(int size) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
    cv::erode(this->getImage(), this->getImage(),element, cv::Point(-1, -1), 1);
}



 /**
  * The purpose of this function is to take as parameters the name of several images, and try to stitch them together,
  * ie detecting the common part.
  *
  * \nhttps://docs.opencv.org/2.4/modules/stitching/doc/stitching.html
  * \nhttps://docs.opencv.org/2.4/modules/stitching/doc/introduction.html
  * \nhttps://www.geeksforgeeks.org/stitching-input-images-panorama-using-opencv-c/
  * \nhttps://pyimagesearch.com/2018/12/17/image-stitching-with-opencv-and-python/
  *
  * @param imagesNameList: list of image path
  * @return ImageTreatment_Charles
  */
ImageTreatment_Charles ImageTreatment_Charles::stitching_and_panorama(std::vector<std::string> &imagesNameList) {
    // Define object to store the stitched image
    cv::Mat pano;
    // Define mode for stitching as panorama
    cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
    // Array for pictures
    std::vector<cv::Mat> imgs;
    // import all images
    for (const std::string &i : imagesNameList) {
        cv::Mat img = cv::imread(i);
        if (img.empty()) {
            return ImageTreatment_Charles(pano);
        }
        imgs.push_back(img);
    }
    // Create a Stitcher class object with mode panoroma
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
    // Command to stitch all the images present in the image array
    cv::Stitcher::Status status = stitcher->stitch(imgs, pano);
    // Check if images could not be stitched
    if (status != cv::Stitcher::OK) {
        std::cout << "Can't stitch images" << std::endl;
        return ImageTreatment_Charles(pano);
    }
    // save the image
    return ImageTreatment_Charles(pano);
}

/**
 * The purpose of this function is to take as parameters the path of a directory, and try to stitch them together,
 * ie detecting the common part.
 * @param directorypath
 * @return ImageTreatment_Charles
 */
ImageTreatment_Charles ImageTreatment_Charles::stitching_and_panorama(std::string directorypath) {
    std::vector<std::string> imagesNameList;
    std::set<std::string> valid_extensions = {".jpg", ".webp", ".png", ".jpeg"};
    for (const auto &file : std::filesystem::directory_iterator(directorypath)) {
        std::string extension = file.path().extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        if (valid_extensions.count(extension)) {
            imagesNameList.emplace_back(file.path().string());
        }
    }
    /* Same as
        std::vector<std::string> imagesNameList;
        imagesNameList.emplace_back("../src/ressources/image_stitching_1.webp");
        imagesNameList.emplace_back("../src/ressources/image_stitching_2.webp");
        imagesNameList.emplace_back("../src/ressources/image_stitching_3.webp");
    */
    return ImageTreatment_Charles::stitching_and_panorama(imagesNameList);
}



