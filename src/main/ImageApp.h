//
// Created by charm on 19/05/2024.
//

#pragma once

#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// https://github.com/Dovyski/cvui
// https://fernandobevilacqua.com/cvui/components/image/

#define WINDOW_NAME "CVUI IMAGE TREATEMENT ISEP"
enum {BRIGHTNESS, ROTATE, RESIZE, CROP, DILATATION, EROSION, CANNY_EDGE, PANORAMA};


class ImageApp {

private:
    std::string iconFolder = "../src/ressources/icon/";
    cv::Mat frame = cv::Mat(520, 900, CV_8UC3); // windows size here
    int option = BRIGHTNESS;

    // For checkpoint image saving
    std::string imagePathName;
    cv::Mat imageSource, imageSave, imageView;

    // trackbar values
    double valueDilatation =0;
    double valueErosion = 0;
    double valuePivot = 0;
    double blurredValue=0; double lowThreshold=0; double highThreshold=0;


public:
    // constructor
    explicit ImageApp();
    bool openStarterImage(const std::string& imagePathName);

    // getter
    cv::Mat& getFrame();
    cv::Mat getImageSource();
    cv::Mat getImageSave();
    cv::Mat getImageView();

    // setter
    void setOption(int newOption);

    // Block
    void topLeftBlock();
    void bottomLeftBlock();
    void centerBlock();
    void topRightBlock();
    void bottomBlock(cv::Point cursor);

    // Panel
    void brightnessPanel();
    void rotatePanel();
    void resizePanel();
    void cropPanel();
    void dilatationPanel();
    void erosionPanel();
    void cannyEdgePanel();
    void panoramaPanel();
};



