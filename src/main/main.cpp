// ISEP A2 - Multimedia Application
// Final Project : gimpsep-clm-project
// For the 09/06/2024.
// https://github.com/arcreane/gimpsep-clm-project
//
// The project is to develop a small GIMP-like image editor, with basic functions.
// The advantages of this project are:
//     • the modularity
//     • using image manipulation
//     • adaptability

#include <iostream>
#include <string>
#include <cstdlib>

// installation Clion: https://www.youtube.com/watch?v=fjq8eTuHnMM
#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// One (and only one) of your C++ files must define CVUI_IMPLEMENTATION
// before the inclusion of cvui.h to ensure its implementaiton is compiled.
#define CVUI_IMPLEMENTATION
#include "../lib/cvui.h"
#include "../lib/EnhancedWindow.h" // exemple : https://github.com/Dovyski/cvui/blob/master/example/src/ui-enhanced-window-component/main.cpp

#include "ImageApp.h"
#include "Image.h"
#include "PanoramaCreator.h"
#include "ImageHandler.h"

using namespace std;
using namespace cv;

int run_application(const std::string& imagePathName);



int main(int args, char** argv) {
    // Remove openCV output message
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    // if imagePath in parameter
    //std::string imagePath = "../src/ressources/HappyFish.jpg";
    std::string imagePath = R"(..\\src\\ressources\\chaplin.mp4)";
    if(args>1) {imagePath = argv[1];}

    // run the gui application
    return run_application(imagePath);
}



int run_application(const std::string& imagePathName) {
    // Create the GUI application
    ImageApp myApp = ImageApp();

    // if no parameter then popup select image
    if (imagePathName.empty()) {
        // popup
    }
    // load the image in the application
    if (!myApp.isVideoFile(imagePathName) && !myApp.openStarterImage(imagePathName)){
        std::cout << "Error : cannot open your image" << std::endl;
        return -1; // if error open image then popup error [close or choose new image]
    }
    else if (myApp.isVideoFile(imagePathName) && !myApp.openVideo(imagePathName)){
        std::cout << "Error : cannot open your video" << std::endl;
        return -1; // if error open image then popup error [close or choose new image]
    }

    cvui::init(WINDOW_NAME);
    while (true) {
        myApp.getFrame() = cv::Scalar(49, 52, 49);
        cv::Point cursor = cvui::mouse();
        //---------------------------------------------------//
        if (myApp.getIsVideo() && myApp.getIsVideoRunning()) {
            myApp.showVideo();
        }
        myApp.centerBlock();
        if (!myApp.getIsVideoRunning()) {
            myApp.topLeftBlock();
            myApp.bottomLeftBlock();
        }
        myApp.topRightBlock();
        myApp.bottomBlock(cursor);

        //---------------------------------------------------//
        cvui::update();
        cvui::imshow(WINDOW_NAME, myApp.getFrame());
        char key = (char)cv::waitKey(20);
        if (key  == 27 || cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) < 1) {break;}
        if (key  == 26) {myApp.ControlZ();}
        if (key  == 25) {myApp.ControlY();}
    }
    return 0;
}

