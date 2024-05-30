#include "PanoramaCreator.h"
#include "Image.h"

using namespace std;
using namespace cv;


PanoramaCreator::PanoramaCreator(const std::string& folderPath)
{
    set<string> valid_extensions = {".jpg", ".webp", ".png", ".jpeg"};
    for (const auto &file : std::filesystem::directory_iterator(folderPath))
    {
        string extension = file.path().extension().string();
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        if (valid_extensions.count(extension)) 
        {
            m_listImages.emplace_back(file.path().string());
        }
    }
}


Image PanoramaCreator::CreatePanorama(vector<string> & m_listImages) {
    // Define object to store the stitched image
    Mat pano;
    // Define mode for stitching as panorama
    cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
    // Array for pictures
    vector<Mat> imgs;
    // import all images
    for (const std::string &i : m_listImages) {
        Mat img = imread(i);
        if (img.empty()) {
            return Image(pano);
        }
        imgs.push_back(img);
    }
    // Create a Stitcher class object with mode panoroma
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
    // Command to stitch all the images present in the image array
    cv::Stitcher::Status status = stitcher->stitch(imgs, pano);
    // Check if images could not be stitched
    if (status != cv::Stitcher::OK) {
        std::cout << "Can't stitch images" << std::endl;
        return Image(pano);
    }
    // save the image
    return Image(pano);
}
