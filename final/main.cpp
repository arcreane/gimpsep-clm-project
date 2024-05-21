#include "Image.h"
#include <string>

int main()
{
	std::string imagePath = "../src/ressources/HappyFish.jpg";
	Image myImage(imagePath);
	
	Image scaledImage = myImage.Resize(2);
	Image croppedImage = myImage.Crop(5, 120, 6, 300);
	Image rotatedImage = myImage.Rotate(180);
	Image edges = myImage.CannyEdge(1.5, 200, 150);

	myImage.Display();
	scaledImage.Display();
	croppedImage.Display();
	rotatedImage.Display();
	edges.Display();

	return 0;
}
