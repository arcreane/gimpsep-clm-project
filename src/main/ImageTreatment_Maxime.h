//
// Created by Maxime on 14/05/2024.
//

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class ImageTreatment_Maxime {
public:

    cv::Mat imageSource;
    cv::Mat image;
    std::string imagePath;

    ImageTreatment_Maxime(const std::string& imagePath);
    void Brightness();

    void Display();

};

