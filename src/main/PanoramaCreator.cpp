#include "PanoramaCreator.h"

using namespace std;
using namespace cv;

// Define the constructor
PanoramaCreator::PanoramaCreator(const string& folderPath)
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


// Define getter
vector<string> PanoramaCreator::getListImages()
{
    return m_listImages;
}

 /**
  * The purpose of this function is to take as parameters the name of several images, and try to stitch them together,
  * ie detecting the common part.
  *
  * \nhttps://docs.opencv.org/2.4/modules/stitching/doc/stitching.html
  * \nhttps://docs.opencv.org/2.4/modules/stitching/doc/introduction.html
  * \nhttps://www.geeksforgeeks.org/stitching-input-images-panorama-using-opencv-c/
  * \nhttps://pyimagesearch.com/2018/12/17/image-stitching-with-opencv-and-python/
  *
  * @param m_listImages: list of the names of the images
  * @return Image
  */
Image PanoramaCreator::CreatePanorama(const vector<string>& listImages) {
    // Define object to store the stitched image
    Mat pano;
    // Define mode for stitching as panorama
    cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
    // Array for pictures
    vector<Mat> imgs;
    // import all images
    for (const std::string &i : listImages) {
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
