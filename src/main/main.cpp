#include "Image.h"
#include <string>

int main()
{
	std::string imageName = "Fish";
	std::string imagePath = "../../src/ressources/HappyFish.jpg";
	Image myImage(imageName, imagePath);
	
	Image scaledImage = myImage.Resize(2);
	Image croppedImage = myImage.Crop(5, 120, 6, 300);
	Image rotatedImage = myImage.Rotate(180);

	myImage.Display();
	scaledImage.Display();
	croppedImage.Display();
	rotatedImage.Display();
	return 0;
}
