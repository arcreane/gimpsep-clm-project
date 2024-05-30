#include "Image.h"
#include "PanoramaCreator.h"
#include <string>

int main()
{
	std::string imagePath = "../src/ressources/HappyFish.jpg";
	std::string folderPath = "../src/ressources/stitching/";

	Image myImage(imagePath);
	myImage.Display("Input Image");
	
    myImage = myImage.Rotate(75, {myImage.cols() / 2, myImage.rows() / 2});
    myImage.Display("Rotated Image");
    myImage = myImage.Brightness(80);
	myImage.Display("Brightened Image");
    myImage = myImage.Resize(3);
    myImage.Display("Resized Image");
    myImage = myImage.Crop(5, 320, 6, 400);
    myImage.Display("Cropped Image");
    myImage = myImage.CannyEdge(1.5, 150, 200);
    myImage.Display("Canny Edges");
    myImage = myImage.Dilatation(9);
    myImage.Display("Dilated Image");
    myImage = myImage.Erosion(9);
    myImage.Display("Eroded Image");

    myImage = myImage.ControlZ();
    myImage.Display("After ControlZ");

    myImage = myImage.ControlZ();
    myImage.Display("After ControlZ");

    myImage = myImage.ControlY();
    myImage.Display("After ControlY");

	PanoramaCreator myPano(folderPath);
	Image panorama = myPano.CreatePanorama(myPano.getListImages());
	panorama.Display("Reconstructed Panorama");

	return 0;
}
