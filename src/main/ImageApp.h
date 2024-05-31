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


#define WINDOW_NAME "CVUI IMAGE TREATEMENT ISEP"
enum {BRIGHTNESS, ROTATE, RESIZE, CROP, DILATATION, EROSION, CANNY_EDGE, PANORAMA}; // options
enum {IDLE, OVER, DOWN}; // imageStates
enum {SAVE, NEW, RESET}; // imageParameters


class ImageApp {

private:
    std::string iconFolder = "../src/ressources/icon/";
    int windowWidth = 910; // in px
    int windowHeight = 520; // in px
    int blocksWidth = 22; // in % [1, 35]
    int topBlocksHeight = 40; // in % [1, 50]
    int bottomBlockHeight = 22; // in px
    cv::Mat frame = cv::Mat(this->windowHeight, this->windowWidth, CV_8UC3); // windows size here
    int widthBlocks = windowWidth * blocksWidth / 100;

    /* **** For checkpoint image saving **** */
    std::string imagePathName;
    std::string imageOutputPath = "../src/ressources/output.png";
    cv::Mat imageSource, imageSave, imageView;

    /* **** Block Variables **** */
    int option = BRIGHTNESS;
    // Button & grid parameters & info-bulles list
    int buttonWidth = 40;
    int buttonHeight = 40;
    int buttonsPerRow = widthBlocks / (topBlocksHeight+ 10);
    // Icon import
    std::vector<std::string> messagesBlock = {"Brightness", "Rotate", "Resize", "Crop", "Dilatation", "Erosion", "Canny edge", "Panorama"};
    std::vector<std::string> iconNameFileBlock = {"brightness", "rotate", "scissors", "crop", "dilatation", "erosion", "cannyEdge", "panorama"};
    std::vector<std::vector<cv::Mat>> iconListBlock;

    /* **** Parameters Variables **** */
    std::vector<std::string> messagesParameters = {"Save image", "New image", "Reset modifications"};
    std::vector<std::string> iconNameFileParameters = {"save", "new", "reset"};
    std::vector<std::vector<cv::Mat>> iconListParameters;

    /* **** Trackbar Panel Values **** */
    double valueDilatation =0;
    double valueErosion = 0;
    double valuePivot = 0;
    double blurredValue=0; double lowThreshold=0; double highThreshold=0;

    // to remove after import Image class
    double count = 0; bool checked{};

    // add parameters pour resize facilement la window

public:
    // constructors
    ImageApp();
    bool openStarterImage(const std::string& imagePathName);

    // getters
    cv::Mat& getFrame();
    cv::Mat getImageSource();
    cv::Mat getImageSave();
    cv::Mat getImageView();

    // setters
    void setOption(int newOption);
    void applyParameter(int parameter);


    // Blocks
    void topLeftBlock();
    void bottomLeftBlock();
    void centerBlock();
    void topRightBlock();
    void bottomBlock(cv::Point cursor);

    // Panels
    void createPanelWindow(const std::string& title);
    void brightnessPanel();
    void rotatePanel();
    void resizePanel();
    void cropPanel();
    void dilatationPanel();
    void erosionPanel();
    void cannyEdgePanel();
    void panoramaPanel();

    // Parameters
    void saveImage();
    void newImage();
    void resetImage();
    void defaultValueBlock(double& trackbarVariable);
};



