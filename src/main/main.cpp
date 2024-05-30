#include "Image.h"
#include <string>

int main()
{
	std::string imagePath = "../src/ressources/HappyFish.jpg";
	Image myImage(imagePath);
	
	Image brighterImage = myImage.Brightness(80);
	Image scaledImage = myImage.Resize(1);
	Image croppedImage = myImage.Crop(5, 120, 6, 300);
	Image rotatedImage = myImage.Rotate(75, {myImage.cols()/2, myImage.rows()/2});
	Image edges = myImage.CannyEdge(1.5, 150, 200);
	Image dilateImage = myImage.Dilatation(9);
	Image erodeImage = myImage.Erosion(9);

	myImage.Display();
	brighterImage.Display();
	scaledImage.Display();
	croppedImage.Display();
	rotatedImage.Display();
	edges.Display();
	dilateImage.Display();
	erodeImage.Display();

	return 0;
}
