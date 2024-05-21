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
#include "ImageTreatment_Maxime.h"
#include "../lib/tinyfiledialogs.h"

using namespace std;
using namespace cv;


int main1();
int main2();
int working_test_openCV();
int brightness();
int test();



int main(int args, char** argv) {

    // Change the value to execute the right function
    std::string i = "0";
    if(args>1) {
        i = argv[1];
    }

    if      (i=="1") {return main1();}
    else {return test();}
}

int main1() {
    printf("hello world");
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

int brightness() {
    try {
        ImageTreatment_Maxime image("../src/ressources/HappyFish.jpg");
        // Régler la luminosité avec curseur
        image.Brightness();
        // Afficher le résultat final
        image.Display();
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int test() {
    char const * lFilterPatterns[4] = {  "*.jpg", "*.png", "*.jpeg", "*.jpe" };
    char const * selection = tinyfd_openFileDialog( // there is also a wchar_t version
            "Select file", // title
            "C:\\", // optional initial directory
            2, // number of filter patterns
            lFilterPatterns, // char const * lFilterPatterns[2] = { "*.txt", "*.jpg" };
            NULL, // optional filter description
            0 // forbid multiple selections
    );

    cout << selection << endl;
    return 1;

}