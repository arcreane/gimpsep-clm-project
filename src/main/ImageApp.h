//
// Created by charm on 19/05/2024.
//

#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_set>


#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "Image.h"
#include "ImageHandler.h"
#include "PanoramaCreator.h"

#define WINDOW_NAME "CVUI IMAGE TREATEMENT ISEP"
enum {BRIGHTNESS, ROTATE, RESIZE, CROP, DILATATION, EROSION, CANNY_EDGE}; // options
enum {IDLE, OVER, DOWN}; // imageStates
enum {RESET, SAVE, NEW, PANORAMA, CAPTUREVIDEO}; // imageParameters


class ImageApp {

private:
    std::string iconFolder = "../src/ressources/icon/";
    int windowWidth = 1210; // in px
    int windowHeight = 620; // in px
    int blocksWidth = 22; // in % [1, 35]
    int topBlocksHeight = 30; // in % [1, 50]
    int bottomBlockHeight = 22; // in px
    cv::Mat frame = cv::Mat(this->windowHeight, this->windowWidth, CV_8UC3);
    int widthBlocks = windowWidth * blocksWidth / 100;

    /* **** For checkpoint image saving **** */
    std::string imagePathName;
    std::string fileExtension;
    std::string imageOutputPath = "../src/ressources/output";
    ImageHandler* image = nullptr;
    int option = BRIGHTNESS;

    /* **** Block Variables **** */
    // Button & grid parameters & info-bulles list
    int buttonWidth = 40;
    int buttonHeight = 40;
    int buttonsPerRow = widthBlocks / (buttonWidth+ 5);
    // Icon import
    std::vector<std::string> messagesBlock = {"Brightness", "Rotate", "Resize", "Crop", "Dilatation", "Erosion", "Canny edge"};
    std::vector<std::string> iconNameFileBlock = {"brightness", "rotate", "resize", "crop", "dilatation", "erosion", "cannyEdge"};
    std::vector<std::vector<cv::Mat>> iconListBlock;

    /* **** Parameters Variables **** */
    std::vector<std::string> messagesParameters = {"Reset modifications", "Save image", "New media (image, video)", "Panorama", "Video Capture"};
    std::vector<std::string> iconNameFileParameters = {"reset", "save", "new", "panorama", "captureVideo"};
    std::vector<std::vector<cv::Mat>> iconListParameters;

    /* **** Trackbar Panel Values **** */
    int lastAction = -1;
    int valuebrightness = 0;
    int valuePivot = 0; int centerPivotX = 0; int centerPivotY = 0;
    double valueResize = 0; double maxValue; // revoir tous les valeurs par defaut
    int startRowCrop = 0; int endRowCrop; int startColCrop = 0; int endColCrop;
    int valueDilatation = 0;
    int valueErosion = 0;
    double blurredValueCanny = 0; int lowThresholdCanny = 0; int highThresholdCanny = 0;


    /* **** Video **** */
    const std::unordered_set<std::string> videoExtensions = {".mp4", ".avi"};
    bool isVideo = false;
    bool isVideoRunning = false;
    cv::VideoCapture cap;
    int positionVideo = 0;
    int frameCount  = 0;
    int fps  = 0;
    int videoLength;
    bool isVideoCapture;



public:
    // constructors
    ImageApp();
    bool openStarterImage(const std::string& imagePathName);
    bool openVideo(const std::string& imagePathName);
    bool openVideo();

    // Deconstructors
    ~ImageApp();

    // getters
    cv::Mat& getFrame();

    // setters
    void setOption(int newOption);
    void applyParameter(int parameter);
    void ControlZ();
    void ControlY();


    // Blocks
    void topLeftBlock();
    void bottomLeftBlock();
    void centerBlock();
    void topRightBlock();
    void bottomBlock(cv::Point cursor);

    // Panels
    void createPanelWindow(const std::string& title);
    void setLastAction(int newLastAction);
    void saveChanges();
    void defaultValuesBtn(bool applyCtrlZ, double value, double valueInit);
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
    void defaultValues();

    // Video
    bool isVideoFile(std::string filename);
    void showVideo();
    void videoPanel();
    bool getIsVideo();
    bool getIsVideoRunning();
    void captureVideoPanel();
};



