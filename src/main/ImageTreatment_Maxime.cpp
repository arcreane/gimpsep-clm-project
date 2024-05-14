//
// Created by Maxime on 14/05/2024.
//

#include "ImageTreatment_Maxime.h"
#include <opencv2\opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

ImageTreatment_Maxime::ImageTreatment_Maxime(const std::string& path) : imagePath(path) {
    imagePath = path;
    imageSource = imread(imagePath, IMREAD_COLOR);
    if (imageSource.empty()) {
        cout << "Could not open or find the image" << endl;
        // return -1;
    }
}


void on_bright(int pos, void* userdata) {
    // pointeur récupérant l'objet ImageTreatment_Maxime, pour modification en direct
    ImageTreatment_Maxime* img_treatment = reinterpret_cast<ImageTreatment_Maxime*>(userdata);
    double brightness = pos - 250; // brightness varie entre -250 et 250

    img_treatment->imageSource.convertTo(img_treatment->image, -1, 1, brightness);
    imshow("Brightness Control", img_treatment->image);
}

void ImageTreatment_Maxime::Brightness() {
    namedWindow("Brightness Control", WINDOW_AUTOSIZE);
    int trackbarValue = 250; // Valeur initiale de la trackbar
    createTrackbar("Brightness", "Brightness Control", &trackbarValue, 500, on_bright, this);
    waitKey(0);
    destroyAllWindows();
}


void ImageTreatment_Maxime::Display() {
    imshow("Edited Image",image);
    waitKey(0);

}
