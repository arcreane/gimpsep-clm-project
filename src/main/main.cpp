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

#include "ImageApp.h"


using namespace std;
using namespace cv;


int run_application(const std::string& imagePathName);



int main(int args, char** argv) {
    // Remove openCV output message
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    // if imagePath in parameter
    std::string imagePath = "../src/ressources/HappyFish.jpg";
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
    if (!myApp.openStarterImage(imagePathName)){
        std::cout << "Error : cannot open your image" << std::endl;
        return -1; // if error open image then popup error [close or choose new image]
    }

    cvui::init(WINDOW_NAME);
    while (true) {
        myApp.getFrame() = cv::Scalar(49, 52, 49);
        cv::Point cursor = cvui::mouse();
        //---------------------------------------------------//
        myApp.topLeftBlock();
        myApp.bottomLeftBlock();
        myApp.centerBlock();
        myApp.topRightBlock();
        myApp.bottomBlock(cursor);
        //---------------------------------------------------//
        cvui::update();
        cvui::imshow(WINDOW_NAME, myApp.getFrame());
        char key = (char)cv::waitKey(20);
        if (key  == 27 || cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) < 1) {break;}
    }
    return 0;
}


/* idées de plugin
cvui::text(frame, 110, 230, "cvui is awesome!");
cvui::image(frame, 10, 10, image);
cvui::checkbox(frame, 110, 300, "Checkbox label", &checked);
cvui::printf(frame, 110, 320, 0.8, 0x00ff00, "value = %.2f", value);
cvui::counter(frame, 110, 350, &count);
cvui::window(frame, 110, 420, 130, 90, "Title");
cvui::rect(frame, 280, 250, 70, 90, 0xff0000, 0x00ff00);
cvui::sparkline(frame, values, 10, 500, 300, 100);
*/

