#include "Image.h"
#include <filesystem> 

using namespace std;
using namespace cv;

// Define the constructors
Image::Image(const string& imagePath)
{
	m_imageSource = imread(imagePath, IMREAD_COLOR);
	if (m_imageSource.empty()) {
    	cout << "Could not open or find the image" << endl;
    	// return -1;
    }
}

Image::Image(Mat& imageMat)
{
	m_imageSource = imageMat;
}




void on_bright(int pos, void* userdata) {
    // pointeur récupérant l'objet Image, pour modification en direct
    Image* img_treatment = reinterpret_cast<Image*>(userdata);
    double brightness = pos - 250; // brightness varie entre -250 et 250

    img_treatment->imageSource.convertTo(img_treatment->image, -1, 1, brightness);
    imshow("Brightness Control", img_treatment->image);
}

/**
 * 
**/
Image Image::Brightness() {
    namedWindow("Brightness Control", WINDOW_AUTOSIZE);
    int trackbarValue = 250; // Valeur initiale de la trackbar
    createTrackbar("Brightness", "Brightness Control", &trackbarValue, 500, on_bright, this);
    waitKey(0);
    destroyAllWindows();
    return 

}








Image Image::Resize(double scalingFactor)
{
	Mat tmp;
	if(scalingFactor!=0)
		resize(m_imageSource, tmp, Size(round(scalingFactor*m_imageSource.cols),round(scalingFactor*m_imageSource.rows)), scalingFactor, scalingFactor, INTER_LINEAR);
	return Image(tmp);
}


Image Image::Crop(int startRow, int endRow, int startCol, int endCol)
{
	if (endRow > m_imageSource.rows || endRow < startRow)
		endRow = m_imageSource.rows;
	if (endCol > m_imageSource.cols || endCol < startCol)
		endCol = m_imageSource.cols;
	if (startRow < 0)
		startRow = 0;
	if (startCol < 0)
		startCol = 0;
	Mat tmp = m_imageSource(Range(startRow, endRow), Range(startCol, endCol));
	return Image(tmp);
}

Image Image::Rotate(double rotationAngle)
{
	Mat tmp;
	Point2f center(m_imageSource.cols/2, m_imageSource.rows/2);
	Mat RotationMatrix = getRotationMatrix2D(center, rotationAngle, 1);
	warpAffine(m_imageSource, tmp, RotationMatrix, m_imageSource.size());
	return Image(tmp);
}

Image Image::CannyEdge(float blurredValue, int lowThreshold, int highThreshold)
{
    Mat imageEdges, blurredImage;
 	GaussianBlur(m_imageSource, blurredImage,Size(5, 5), blurredValue); 
    Canny(blurredImage, imageEdges,lowThreshold,highThreshold);
    return Image(imageEdges);
}

void Image::Display()
{
	imshow("Original Image", m_imageSource);
  	waitKey(0);
}