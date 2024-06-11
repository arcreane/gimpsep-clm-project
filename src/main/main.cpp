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

#include <string>
#include <opencv2/core/utils/logger.hpp> // installation Clion: https://www.youtube.com/watch?v=fjq8eTuHnMM
#include "ImageApp.h"


int main(int args, char** argv) {
    // Remove openCV output message
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    // if imagePath in parameter
    std::string imagePath;
    //imagePath = "../src/ressources/HappyFish.jpg";
    //imagePath = R"(..\\src\\ressources\\chaplin.mp4)";
    if(args>1) {imagePath = argv[1];}

    // run the gui application
    return ImageApp::run_application(imagePath);  // optional parameters
}

