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

// installation Clion: https://www.youtube.com/watch?v=fjq8eTuHnMM
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int main1();
int main2();
int working_test_openCV();


int main(int args, char** argv) {

    // Change the value to execute the right function
    std::string i = "0";
    if(args>1) {
        i = argv[1];
    }

    if      (i=="1") {return main1();}
    else if (i=="2") {return main2();}
    else {return working_test_openCV();}
}

int main1() {
    printf("hello world");
    return 0;
}

int main2() {
    printf("hello you");
    return 0;
}

int working_test_openCV() {
    string winName("Display window");
    string imageName("../src/ressources/HappyFish.jpg");
    Mat image;
    image = imread(imageName, IMREAD_COLOR);
    if (image.empty()) {std::cout << "Could not open or find the image" << std::endl;return -1;}
    namedWindow(winName, WINDOW_AUTOSIZE);
    imshow(winName, image);
    waitKey(0);
    return 0;
}

