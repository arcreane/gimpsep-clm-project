#include "Image_Lucie.h"
#include <string>

int main()
{
	std::string imagePath = "../src/ressources/HappyFish.jpg";
	Image myImage(imagePath);
	
	Image scaledImage = myImage.Resize(1);
	Image croppedImage = myImage.Crop(5, 120, 6, 300);
	Image rotatedImage = myImage.Rotate(75, {myImage.cols()/5, myImage.rows()/8});
	Image edges = myImage.CannyEdge(1.5, 150, 200);

	myImage.Display();
	scaledImage.Display();
	croppedImage.Display();
	rotatedImage.Display();
	edges.Display();

	return 0;
}
