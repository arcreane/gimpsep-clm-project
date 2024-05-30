#include "Image.h"
#include "PanoramaCreator.h"
#include <string>

int main()
{
	std::string imagePath = "../src/ressources/HappyFish.jpg";
	std::string folderPath = "../src/ressources/stitching/";

	Image myImage(imagePath);
	PanoramaCreator myPano(folderPath);
	
	Image brighterImage = myImage.Brightness(80);
	Image scaledImage = myImage.Resize(1);
	Image croppedImage = myImage.Crop(5, 120, 6, 300);
	Image rotatedImage = myImage.Rotate(75, {myImage.cols()/2, myImage.rows()/2});
	Image edges = myImage.CannyEdge(1.5, 150, 200);
	Image dilateImage = myImage.Dilatation(9);
	Image erodeImage = myImage.Erosion(9);
	Image panorama = myPano.CreatePanorama(myPano.getListImages());

	myImage.Display("Input Image");
	brighterImage.Display("Brightened Image");
	scaledImage.Display("Resized Image");
	croppedImage.Display("Cropped Image");
	rotatedImage.Display("Rotated Image ");
	edges.Display("Canny Edges");
	dilateImage.Display("Dilated Image");
	erodeImage.Display("Eroded Image");
	panorama.Display("Reconstructed Panorama");

	return 0;
}
