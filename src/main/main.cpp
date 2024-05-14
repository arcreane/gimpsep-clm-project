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
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


#include "ImageTreatment_Charles.h"



using namespace std;
using namespace cv;



int main1();
int main2();
int main3();
int working_test_openCV();


int main(int args, char** argv) {
    // Remove openCV output message
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    // Change the value to execute the right function
    std::string i = "2";
    if(args>1) {i = argv[1];}

    if      (i=="1") {return main1();}
    else if (i=="2") {return main2();}
    else if (i=="3") {return main3();}
    else {return working_test_openCV();}
}

int main1() {
    std::cout << "Hello world 1" << std::endl;

    // create image
    ImageTreatment_Charles img = ImageTreatment_Charles("../src/ressources/HappyFish.jpg");
    if (!img.loadImage()) {return -1;}
    std::cout << img.getImageName() << std::endl;

    // treatment

    /* https://www.youtube.com/watch?v=Qv16W361Uu0
     * Les fonctions dilatation et erosion sont complémentaire
     * Si on applique ces deux fonctions avec les meme parametres, on est sensé retrouver l'image d'origine
     * erosion : accentu les traits fin noir (et inversement pour les traits blanc)
     * dilatation : diminue les traits fin noir (et inversement pour les traits blanc)
     * erosion + dilatation : fait rien sinon pas grand chose (ou supprime les traits fin blanc)
     * dilatiation + erosion : supprime les traits fin noir (ou fait rien sinon pas grand chose)
     * */
    img.dilatation(5);
    img.erosion(5);
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
    std::cout << "Hello world 2" << std::endl;

    std::string directorypath("../src/ressources/stitching");
    ImageTreatment_Charles img = ImageTreatment_Charles::stitching_and_panorama(directorypath);
    if (img.isEmpty()) {return -1;}

    cv::imshow("Result", img.getImage());
    cv::waitKey(0);
    return 0;
}

int main3() {
    cout << "Mes tests" << endl;
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

