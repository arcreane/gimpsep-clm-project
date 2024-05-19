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

#define CVUI_IMPLEMENTATION
#include "../lib/cvui.h"

#include "ImageApp.h"
#include "ImageTreatment_Charles.h"


using namespace std;
using namespace cv;



int main1();
int main2();
int main3();
int working_test_openCV();
int test_cvui();
int test_app();



int main(int args, char** argv) {
    // Remove openCV output message
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    // Change the value to execute the right function
    std::string i = "app";
    if(args>1) {i = argv[1];}

    if      (i=="1") {return main1();}
    else if (i=="2") {return main2();}
    else if (i=="3") {return main3();}
    else if (i=="cvui") {return test_cvui();}
    else if (i=="app") {return test_app();}
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


// https://github.com/Dovyski/cvui
// https://fernandobevilacqua.com/cvui/components/image/
#define WINDOW_NAME "CVUI Hello World!"
enum {DILATATION_ERROSION, RESIZING, STITCHING_PANORAMA, CANNY_DETECTION, PIVOT};
cv::Mat imageSource, imageTemp, image;


/* Dans ImageApp mettre en place une method updateImage() qui sera utilisé dans tous les panel buttons
 * Cette fonction prend l'image source et lui applique l'ensemble des méthodes de traitement d'image
 * avec if (valueMethod != default value) pour n'appliquer reelement que les fonctions demandé par l'user
 * Problem : chaque fonction doit avoir un ordre precis ce qui peut avoir des effets non voulu
 * (ex: dilatation avant erosion ou erosion avant dilatation ?)
 * */
/*
 * Solution suivante : avoir Mat imageSource, imageTemp, imageView
 * imageSource est l'image à l'import
 * imageView est l'image que l'on affiche sur l'ecran
 * imageTemp est une save de l'image modifié à chaque fois qu'on change de panel
 * On aura un bouton reset all pour mettre  imageView = imageSource
 * et un bouton reset au niveau du panel pour annuler les modifs en cours
 *
 * Pour mettre en place le ctrl+Z, on instancie une liste de Mat
 * Le premier ctrl+Z annuler les modifs en cours et les suivants récupères l'etat initial au precedant panel
 * Qui dit ctrl+Z dit aussi ctrl+Y pour annuler les retours arrières

 */


void dilatationErrosionPanel(cv::Mat& frame, double& valueDilatation, double& valueErosion) {
    cvui::window(frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(frame, 0, 220, 300, 200, 10);
    cvui::text("dilatation / erosion panel");
    cvui::space(5);
    if (cvui::trackbar(300, &valueDilatation, (double)1.0, (double)100.0)) {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(valueDilatation, valueDilatation), cv::Point(-1, -1));
        cv::dilate(imageTemp, image,element, cv::Point(-1, -1), 1);
    }
    if (cvui::trackbar(300, &valueErosion, (double)1.0, (double)100.0)) {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(valueErosion, valueErosion), cv::Point(-1, -1));
        cv::erode(imageTemp, image,element, cv::Point(-1, -1), 1);
    }

    if (cvui::button("Default dilate")) {valueDilatation = 1;}
    if (cvui::button("Default erode")) {valueErosion = 1;}
    if (cvui::button("Default")) {
        valueDilatation = 1;
        valueErosion = 1;
        image = imageTemp.clone();
    }

    cvui::endColumn();
}

void resizingPanel(cv::Mat& frame) {
    cvui::window(frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(frame, 0, 220, 300, 200, 10);
    cvui::text("resizing");
    cvui::endColumn();
}

void cannyPanel(cv::Mat& frame, bool& isCanny, double& blurredValue, double& lowThreshold, double& highThreshold) {
    cvui::window(frame, 0, 200, 300, 300, "Actions");
    cvui::beginColumn(frame, 0, 220, 300, 300, 10);
    cvui::text("pivot panel");
    cvui::space(5);
    if (cvui::trackbar(300, &blurredValue, (double)0.0, (double)10.0)) {}
    cvui::trackbar(300, &lowThreshold, (double)0, (double)200, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1.0);
    cvui::trackbar(300, &highThreshold, (double)0, (double)200, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1.0);
    cvui::endColumn();
}

void pivotPanel(cv::Mat& frame, double& rotationAngle) {
    cvui::window(frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(frame, 0, 220, 300, 200, 10);
    cvui::text("pivot panel");
    cvui::space(5);
    if (cvui::trackbar(300, &rotationAngle, (double)-180, (double)180)) {
        // action sur image
        Point2f center(imageTemp.cols/2, imageTemp.rows/2);
        Mat RotationMatrix = getRotationMatrix2D(center, rotationAngle, 1);
        warpAffine(imageTemp, image, RotationMatrix, imageTemp.size());
    }
    cvui::endColumn();
}





// One (and only one) of your C++ files must define CVUI_IMPLEMENTATION
// before the inclusion of cvui.h to ensure its implementaiton is compiled.

#include <cstdlib>

int test_app() {  // TODO: option
    imageSource = cv::imread("../src/ressources/HappyFish.jpg");
    image = imageSource.clone();
    imageTemp = imageSource.clone();

    if (imageSource.empty()) {std::cout << "ERROR : can't open image" << std::endl;return -1;}

    double valueDilatation =0; double valueErosion = 0;
    double valuePivot = 0;
    bool valueCanny = false; double blurredValue=0; double lowThreshold=0; double highThreshold=0;
    int optionImage = 0;

    cv::Mat frame = cv::Mat(520, 900, CV_8UC3);
    cvui::init(WINDOW_NAME);
    while (true) {
        frame = cv::Scalar(49, 52, 49);
        //---------------------------------------------------//
        cv::Point cursor = cvui::mouse();



        cvui::window(frame, 0, 0, 300, 200, "Options");
        // faire une loop sur une liste contrnant les buttonName pour avoir une matrice
        cvui::beginColumn(frame, 0, 20, 300, 200);
        if (cvui::button("dilatation / erosion")) {optionImage=DILATATION_ERROSION;}
        if (cvui::button("resizing")) {optionImage=RESIZING;}
        if (cvui::button("panorama / stitching")) {optionImage=STITCHING_PANORAMA;}
        if (cvui::button("Canny edge detection")) {optionImage=CANNY_DETECTION;}
        if (cvui::button("Pivot")) {optionImage=PIVOT;}
        cvui::endColumn();

        // mettre un switch case pour chaque button de la liste precedante

        switch (optionImage) {
            case DILATATION_ERROSION: dilatationErrosionPanel(frame, valueDilatation, valueErosion); break;
            case RESIZING: resizingPanel(frame); break;
            case CANNY_DETECTION: cannyPanel(frame, valueCanny, blurredValue, lowThreshold, highThreshold); break;
            case PIVOT: pivotPanel(frame, valuePivot); break;
        }


        // resize image pour que sa taille soit fixe sur l'ecran si taille trop grande
        // centrer image
       // cvui::window(frame, 300, 0, 300, 400, "Image");
        cvui::image(frame, 325, 100, image);

        // cv::Mat, x, y, width, height, padding
        cvui::window(frame, 600, 0, 300, 400, "Parameters");
        cvui::beginColumn(frame, 600, 20, 300, 400);
        cvui::space(5);
        cvui::button("save image");
        if (cvui::button("change image")) {
            system("explorer");
        }
        cvui::endColumn();

        // bottom
        cvui::beginRow(frame, 0, 500);
        cvui::printf(0.4, 0x00ff00, "cursor: x = %d y = %d", cursor.x, cursor.y);
        cvui::endRow();




        //---------------------------------------------------//
        cvui::update();
        cvui::imshow(WINDOW_NAME, frame);
        char key = (char)cv::waitKey(20);
        if (key  == 27) {break;}
    }
    return 0;
}

int test_cvui() {
    // Create a frame where components will be rendered to.
    cv::Mat frame = cv::Mat(900, 800, CV_8UC3);
    image = cv::imread("../src/ressources/HappyFish.jpg");
    if (image.empty()) {std::cout << "ERROR : can't open image" << std::endl;return -1;}

    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);

    bool checked = false;
    double value = 3.14;
    int count = 2;

    while (true) {
        // Fill the frame with a nice color
        frame = cv::Scalar(49, 52, 49);

        // Render UI components to the frame
        cvui::text(frame, 110, 210, "Hello, world!");
        cvui::text(frame, 110, 230, "cvui is awesome!");

        // Affichez l'image à la position (50, 50)
       // cv::Rect roi = cv::Rect(110, 10, image.cols, image.rows);
        //image.copyTo(frame(roi));

        cvui::image(frame, 10, 10, image);


        // Créez un bouton à la position (50, 400)
        if (cvui::button(frame, 110, 250, "Cliquez-moi")) {
            std::cout << "Le bouton a été cliqué!" << std::endl;
        }

        cvui::checkbox(frame, 110, 300, "Checkbox label", &checked);

        cvui::printf(frame, 110, 320, 0.8, 0x00ff00, "value = %.2f", value);

        cvui::counter(frame, 110, 350, &count);

        cvui::trackbar(frame, 55, 370, 200, &value, (double)10.0, (double)15.0);

        cvui::window(frame, 110, 420, 130, 90, "Title");

        cvui::rect(frame, 10, 250, 70, 90, 0xff0000);
        cvui::rect(frame, 280, 250, 70, 90, 0xff0000, 0x00ff00);

        std::vector<double> values;
        for (std::vector<double>::size_type i = 0; i < 30; i++) {
            values.push_back(rand() + 0.);
        }
        cvui::sparkline(frame, values, 10, 500, 300, 100);



        // beginRow(cv::Mat, x, y)
        cvui::beginRow(frame, 10, 600);
        cvui::text("This is another row");
        cvui::checkbox("checkbox", &checked);
        cvui::window(80, 80, "window");
        cvui::button(100, 30, "Fixed");
        cvui::text("with text.");
        cvui::endRow();

        // cv::Mat, x, y, width, height, padding
        cvui::beginColumn(frame, 50, 680, 100, 200);
        cvui::text("Column 1 (no padding)");
        cvui::button("button1");
        cvui::button("button2");
        cvui::text("End of column 1");
        cvui::endColumn();
        // cv::Mat, x, y, width, height, padding
        cvui::beginColumn(frame, 300, 680, 100, 200, 10);
        cvui::text("Column 2 (padding = 10)");
        cvui::button("button1");
        cvui::button("button2");
        cvui::trackbar(150, &value, 0., 5.);
        cvui::text("End of column 2");
        cvui::endColumn();
        // cv::Mat, x, y, width, height, padding
        cvui::beginColumn(frame, 550, 680, 100, 200);
        cvui::text("Column 3 (use space)");
        cvui::space(5);                       // Add 5 pixels of (vertical) space.
        cvui::button("button1 5px below");
        cvui::space(50);                      // Add 50 pixels of (vertical) space.
        cvui::text("Text 50px below");
        cvui::space(20);                      // Add 20 pixels of (vertical) space.
        cvui::button("Button 20px below");
        cvui::space(40);                      // Add 40 pixels of (vertical) space.
        cvui::text("End of column 2 (40px below)");
        cvui::endColumn();


        cv::Point cursor = cvui::mouse();
        cvui::printf(frame, 300, 10, 0.8, 0x00ff00, "cursor: x = %d y = %d", cursor.x, cursor.y);


        // Update cvui stuff and show everything on the screen
        cvui::update();
        cvui::imshow(WINDOW_NAME, frame);

        char key = (char)cv::waitKey(20);
        if (key  == 27) {
            break;
        }
    }

    return 0;
}