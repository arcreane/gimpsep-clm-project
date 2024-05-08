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

#include "ImageTreatment_Charles.h"

using namespace std;
using namespace cv;


int main1();
int main2();
int working_test_openCV();


int main(int args, char** argv) {
    // Change the value to execute the right function
    std::string i = "1";
    if(args>1) {
        i = argv[1];
    }

    ImageTreatment_Charles img = ImageTreatment_Charles("HappyFish.jpg");
    if      (i=="1") {return main1();}
    else if (i=="2") {return main2();}
    else {return working_test_openCV();}
}

int main1() {
    std::cout << "Hello world" << std::endl;

    // create image
    ImageTreatment_Charles img = ImageTreatment_Charles("../src/ressources/HappyFish.jpg");
    if (!img.loadImage()) {return -1;}
    std::cout << img.getImageName() << std::endl;

    // treatment
    img.erosion(10);
    img.dilatation(10);
    img.saveImage("../src/ressources/output.jpg");

    // view windows
    string winName("Display window");
    Mat view;
    namedWindow(winName, WINDOW_AUTOSIZE);
    hconcat(img.getImageSource(), img.getImage(), view);
    imshow(winName, view);
    waitKey(0);

    return 0;
}

int main2() {
    cout << "Mes tests" << endl;
    return 0;}

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

